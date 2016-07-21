/*
 * topFirePage.c
 *
 *  Created on: Nov 18, 2014
 *      Author: Ce Guo
 */


#include "system.h"
#include "..\inc\DisDriver.h"
#include "..\inc\keyVal.h"
#include "..\inc\delay.h"
#include "..\inc\config.h"
#include "..\inc\loadWriteData.h"
#include "..\inc\comInterface.h"
#include "..\inc\errorCheck.h"
#include "..\inc\editNum2.h"
#include "..\inc\editNum5.h"
#include "..\inc\editNum10.h"
#include "..\inc\KPLED.h"
#include "..\inc\Counter.h"
#include "..\inc\HeaterWarmingUp.h"

#define isFiring	*(volatile unsigned char*)ISFIRING_BASE				//Input, Flag, indicates firing sequence is in progress
#define seqCount	*(volatile unsigned int*)SEQ_COUNT_BASE				//Input, Sequence Counter, 32bit, Counter for current firing sequences.
#define interlock	*(volatile unsigned char*)INTERLOCK_IN_BASE			//Input, Flag, this bit is set when an interlock is detected.
#define interlock_flags  *(volatile unsigned char*)INTERLOCK_IN_BASE
#define HV_Inhibit	*(volatile unsigned char*)HV_INHIBIT_IN_BASE		//Input, Flag, this bit is set when HV is inhibited.
#define clrInterlock	*(unsigned char*)RST_INTERLOCK_BASE				//Output, reset interlock latch (which is programmed in FPGA)
#define pauseSeq	*(unsigned char*)PAUSE_SEQ_BASE						//Output, pause firing sequence. When this bit is set, trigger to time engine will be blocked
#define preionFault	*(unsigned char*)PREION_FAULT_IN_BASE				//Input, Flag, indicates preion fault
#define clrPreionFault	*(unsigned char*)RST_PREION_FAULT_BASE			//Output, reset preion fault latch (which is programmed in FPGA)
#define interlock_code  *(volatile unsigned char*)INTERLOCK_SPLITED_BASE //Input,a sequence of binary which determines which interlock channel has an error.



void entryFireMenu(unsigned char number)
{
	//Configure backlit LEDs
	KPLED_All_Off();
	KPLED_Enter(3);
	KPLED_Enable(1);
	KPLED_RightArrow(3);
	KPLED_LeftArrow(3);

	//Construct display screen
	DisClear();
	DisShowCur(0);
	DisCurPos(1,1);
	DisWrStr("       MENU         ");
	DisCurPos(2,1);
	DisWrStr(Num2toStr2(number));
	DisWrStr(".Fire");

	DisCurPos(4,1);
	DisWrStr("<Prev   Enter  Next>");

}

/*
 * Pre-Firing Error Check
 * Check all parameters, interlock, and heater.
 * Returns:
 * 		non-zero: Error(Do NOT Fire)	0:No Error
 */

unsigned char PreFiringErrChk()
{
	unsigned char keyVal_latch;
	unsigned char err = chkAllTiming();			//Check all timing parameters. Returns "1" if ny parameter error is detected.
	unsigned char interlock_latch;

	clrInterlock = 1;
	clrInterlock = 0;
	interlock_latch = interlock;

	KPLED_All_Off();
	if(err)										//If any parameter error is detected.
	{

		KPLED_Menu(3);							//Display "Range check error" message
		DisClear();
		DisShowCur(0);
		DisCurPos(1,8);
		DisWrStr("ERROR");
		DisCurPos(2,1);
		DisWrStr("Range Check Error");
		DisCurPos(3,1);
		DisWrStr(convertChnlTitle(err));
		DisCurPos(4,1);
		DisWrStr("Press MENU to Return");


		while(1)								//Detect key press, MENU to return.
		{
			if(keyFlag)
			{
				delay(100);
				if(keyFlag)
				{
					keyVal_latch = keyVal;
					if(keyVal_latch == KEY_MENU)
					{
						while(keyFlag);
						return err;
					}
				}
				while(keyFlag);
			}
		}
	}
	else if(interlock_latch)					//If interlock is detected
	{
		KPLED_Menu(3);							//Display Interlock message
		DisClear();
		DisShowCur(0);
		DisCurPos(1,8);
		DisWrStr("ERROR");
		DisCurPos(2,1);
		DisWrStr("Interlock!");
		DisCurPos(4,1);
		DisWrStr("Press MENU to Return");

		while(1)								//Detect key press, MENU to return
		{
			if(keyFlag)
			{
				delay(100);
				if(keyFlag)
				{
					keyVal_latch = keyVal;
					if(keyVal_latch == KEY_MENU)
					{
						while(keyFlag);
						return 255;
					}
				}
				while(keyFlag);
			}
		}
	}
	return checkHeater();						//Return heater check status. If the heater is already warming up for 10 minutes, then it is ready.
}

void showFiringInformation()
{
	KPLED_All_Off();
	KPLED_Enable(3);
	KPLED_Menu(3);
	KPLED_Fire(3);								//Let the Fire button flash once during each shot

	DisClear();
	DisCurPos(1,5);
	DisWrStr(" Firing...");
	DisCurPos(2,1);
	DisWrStr("SeqCount:     /");
	DisWrStr(Num5toStr5(LoadRAMShots()));
	DisCurPos(3,1);
	DisWrStr("CapCount:");
	DisCurPos(4,1);
	DisWrStr("ENA=Pause  MENU=Stop");
}



/*
 * Display fault massage when DC preion fault is detected
 */
void PreionFaultHandler()
{
	unsigned char preionFault_latch = preionFault;
	unsigned char HV_Inhibit_latch = HV_Inhibit;



	if(preionFault_latch || HV_Inhibit_latch)
	{
		DisCurPos(2,1);
		DisWrStr("  DC Preion Fault!  ");
	}

	if(HV_Inhibit_latch)
	{
		DisCurPos(3,1);
		DisWrStr("     HV Inhibit     ");
	}
}


/*
 * A firing sequence can be paused for two reasons:
 * 		1. PAUSE button is pressed while firing
 * 		2. An interlock is detected
 *
 * Once firing sequence is paused, the program keeps pulling interlock input.
 * 		-> No interlock at all: 			Normal Pause (RESUME is available)
 * 		-> Interlock is detected:			"Interlock" flashes (RESUME is unavailable)
 * 		-> Interlock was detected:			"Interlock Occurred" (RESUME is available)
 */

//Return 0: Resume
//Return 1: Stop
unsigned char PauseSequence(unsigned char isFromInterlock)
{
	pauseSeq = 1;

	unsigned char interlock_latch;
	unsigned int interlock_code_latch;
	unsigned short blink_ctr = 0;
	unsigned char keyVal_latch;
	unsigned char interlock_flag;	//This flag is set once an interlock is detected

	interlock_flag = isFromInterlock;		//Initialize Interlock Flag

	showFiringInformation();

	KPLED_All_Off();
	KPLED_Enable(1);
	KPLED_Menu(3);


	//Construct the Pause screen
	DisCurPos(1,1);
	DisWrStr(" Firing Seq. Paused");
	DisCurPos(4,1);
	DisWrStr("ENA=Resume MENU=Stop");
	unsigned int seqCount_latch = seqCount;
	DisCurPos(2,10);
	DisWrStr(Num5toStr5(seqCount_latch));
	DisCurPos(3,11);
	DisWrStr(Num10toStr10(getCounter1Val()));

	//Pull interlock
	while(1)
	{
		interlock_latch = interlock;
		interlock_code_latch = interlock_code;
		clrInterlock = 1;
		clrInterlock = 0;


		if(interlock_latch)
		{
			if(! interlock_flag)
			{
				KPLED_Enable(0);
				DisCurPos(4,1);
				DisWrStr("          ");
				interlock_flag = 1;
			}
			blink_ctr++;
			if(blink_ctr == 50000)
			{
				DisCurPos(3,6);
				ShowInterlockMessage(interlock_code);
				//DisWrStr("Interlock!");
				blink_ctr = 0;
			}
			else if(blink_ctr == 25000)
			{
				DisClearRow(3);
			}

			if(keyFlag)
			{
				delay(100);
				if(keyFlag)
				{
					keyVal_latch = keyVal;
					if(keyVal_latch==KEY_MENU)
					{
						while(keyFlag);
						return(1);
					}
				}
			}
		}
		else
		{
			if(interlock_flag)
			{
				KPLED_Enable(1);
				DisCurPos(3,1);
				DisWrStr(" Interlock Occurred ");
				DisCurPos(4,1);
				DisWrStr("ENA=Resume");
				interlock_flag = 0;
			}
			while(! interlock_latch)
			{
				clrInterlock = 1;
				clrInterlock = 0;
				interlock_latch = interlock;
				if(keyFlag)
				{
					delay(100);
					if(keyFlag)
					{
						keyVal_latch = keyVal;
						if(keyVal_latch==KEY_ENA)
						{
							while(keyFlag);
							showFiringInformation();
							clrInterlock = 1;
							clrInterlock = 0;
							pauseSeq = 0;
							return(0);
						}
						else if(keyVal_latch==KEY_MENU)
						{
							while(keyFlag);
							return(1);
						}
					}
				}
			}
		}
	}
	return(0);
}


void StopFiring()
{
	Stop();
	DisClear();
	DisCurPos(2,5);
	DisWrStr("Stopping...");

	//Refresh Timing Engine Registers *** Necessary!!
	reloadRegisters();

	//Clear Preion Fault
	clrPreionFault = 1;
	clrPreionFault = 0;
	//Clear Interlock
	clrInterlock = 1;
	clrInterlock = 0;
	//Clear Pause Lock
	pauseSeq = 0;

	//Save Counter to FRAM
	SaveCounterToFRAM();

	DisCurPos(2,4);
	DisWrStr("Firing Stopped");
	delay(MSG_DELAY);
	while(keyFlag);
	return;
}

/*
 * Start Firing
 * Before the new interlock handler system is implemented, the Preion Fault is handled with a separate logic block which is designed in Quartus II
 */
//1: Fire Again		0:Exit
unsigned char StartFiring()
{
	unsigned char keyVal_latch;
	unsigned char isFiring_latch;
	unsigned int  seqCount_latch;
	unsigned char interlock_latch;
	unsigned char preionFault_latch;
	unsigned char HV_Inhibit_latch;
	unsigned char preionFault_flag = 0;
	unsigned char stop_flag = 0;
	DisClear();
	DisShowCur(0);
	DisCurPos(2,5);
	DisWrStr("Starting...");

	//Clear Preion Fault
	clrPreionFault = 1;
	clrPreionFault = 0;
	//Clear Interlock
	clrInterlock = 1;
	clrInterlock = 0;
	//Clear Pause Lock
	pauseSeq = 0;

	Fire();
	delay(100);
	if(isFiring)
	{
		showFiringInformation();
		isFiring_latch = isFiring;
		while(isFiring_latch)
		{

			isFiring_latch = isFiring;
			if(! preionFault_flag)								//Check for Preion Fault
			{
				//If no preion fault, refresh counter on the screen
				seqCount_latch = seqCount;
				DisCurPos(2,10);
				DisWrStr(Num5toStr5(seqCount_latch));
				DisCurPos(3,11);
				DisWrStr(Num10toStr10(getCounter1Val()));
			}

			HV_Inhibit_latch = HV_Inhibit;
			preionFault_latch = preionFault;
			if(preionFault_latch || HV_Inhibit_latch)
			{
				preionFault_flag = 1;
				PreionFaultHandler();
			}
			else if(preionFault_flag)
			{
				preionFault_flag = 0;
				showFiringInformation();
			}
			interlock_latch = interlock;
			if(interlock_latch)
			{
				stop_flag = PauseSequence(1);
			}

			if(stop_flag)
			{
				StopFiring();
				return 0;
			}
			if(keyFlag)
			{
				delay(100);
				if(keyFlag)
				{
					keyVal_latch = keyVal;
					if(keyVal_latch == KEY_MENU)
					{
						while(keyFlag);
						StopFiring();
						return 0;
					}
					else if(keyVal_latch == KEY_ENA)
					{
						while(keyFlag);
						stop_flag = PauseSequence(0);
						if(stop_flag)
						{
							StopFiring();
							return 0;
						}
					}
				}
			}
		}
	}


	KPLED_All_Off();
	KPLED_Menu(3);
	KPLED_Fire(1);

	DisClear();
	DisCurPos(2,5);
	DisWrStr("Done Firing");

	DisCurPos(3,3);
	DisWrStr("MENU=Exit");

	DisCurPos(4,3);
	DisWrStr("FIRE=Fire Again");

	while(1)
	{
		if(keyFlag)
		{
			delay(100);
			if(keyFlag)
			{
				keyVal_latch = keyVal;
				if(keyVal_latch == KEY_MENU)
				{
					while(keyFlag);
					return 0;
				}
				else if(keyVal_latch == KEY_FIRE)
				{
					while(keyFlag);
					return 1;
				}
			}
			while(keyFlag);
		}
	}
	return 0;
}

void enterFireMenu()
{
	if(!PreFiringErrChk())		//If PreFiringErrChk() returns 0, then there is no error.
	{
		KPLED_All_Off();
		KPLED_Fire(1);
		KPLED_Menu(3);

		DisClear();
		DisShowCur(0);
		DisCurPos(1,1);
		DisWrStr("Confirm to Fire?");
		DisCurPos(2,1);
		DisWrStr("   Yes: Press FIRE");
		DisCurPos(3,1);
		DisWrStr("   No:  Press MENU");
		DisCurPos(4,1);

		unsigned char keyVal_latch;
		while(1)
		{
			if(PreFiringErrChk())	//If PreFiringErrChk() returns 1, then there is  error.
			{
				return;
			}
			KPLED_All_Off();
			KPLED_Fire(1);
			KPLED_Menu(3);
			if(keyFlag)
			{
				delay(100);
				if(keyFlag)
				{
					keyVal_latch = keyVal;
					if(keyVal_latch == KEY_MENU)
					{
						while(keyFlag);
						return;
					}
					else if(keyVal_latch == KEY_FIRE)
					{
						while(keyFlag);
						if(!PreFiringErrChk())
							while(1)
							{
								unsigned char fireAgain;
								fireAgain = StartFiring();	//If PreFiringErrChk() returns 1, then the fire sequence continues.
								if(fireAgain==0) break;
							}
						return;
					}
				}
				while(keyFlag);
			}
		}
	}
}

char *interlockNames[5] = {"LHV ", "Ht ", "RF ", "DC ", "Thy"};
void ShowInterlockMessage (unsigned int ILchnl)
{
	DisCurPos(3,1);
	DisWrStr("INT:");
	DisCurPos(3,5);
	unsigned int mask = 0x1;
	int i;
	for(i = 0; i < 5; i++) {
		if(ILchnl&mask) DisWrStr(interlockNames[i]);
		mask=mask<<1;
	}
}

/*
void ShowInterlockMessage (unsigned int ILchnl){
	unsigned int Mask =1;
		if((Mask&ILchnl))
			DisWrStr("FP Interlock!");
		Mask = Mask << 1;
		if((Mask&ILchnl))
			DisWrStr("Chnl1!");
		Mask = Mask << 1;
		if((Mask&ILchnl))
			DisWrStr("Chnl2!");
		Mask = Mask << 1;
		if((Mask&ILchnl))
			DisWrStr("Chnl3!");
		Mask = Mask << 1;
		if((Mask&ILchnl))
			DisWrStr("Chnl4!");
		Mask = Mask << 1;
		if((Mask&ILchnl))
			DisWrStr("Chnl5!");
		Mask = Mask << 1;
		if((Mask&ILchnl))
			DisWrStr("Chnl6!");
		Mask = Mask << 1;
		if((Mask&ILchnl))
			DisWrStr("Chnl7!");
		Mask = Mask << 1;
		if((Mask&ILchnl))
			DisWrStr("Chnl8!");
		Mask = Mask << 1;
		if((Mask&ILchnl))
			DisWrStr("Chnl9!");
		Mask = Mask << 1;

}
*/
