/*
 * TopMenu.c
 *
 *  Created on: Nov 18, 2014
 *      Author: Ce Guo
 */


/*
 * Go into each header file to see description of its content.
 */
#include "system.h"
#include "menu.h"
#include "DisDriver.h"
#include "Counter.h"
#include "keyVal.h"
#include "delay.h"
#include "config.h"
#include "loadWriteData.h"
#include "comInterface.h"
#include "errorCheck.h"
#include "KPLED.h"
#include "editNum2.h"
#include "powerFailure.h"
#include <stdio.h>

#define clrTimer 		*(unsigned char*)CLRTIMER_BASE			//Clear Password/Heater Timer. Active High
#define remoteEn 		*(unsigned char*)REMOTECTRL_BASE		//Enable remote control. (Switch UART connection from Nios core to external serial port). Active High
#define clrInterlock 	*(unsigned char*)RST_INTERLOCK_BASE		//Clear Interlock status. Active High
#define InterlockIn 	   *(unsigned char*)INTERLOCK_IN_BASE		//Clear Interlock Port. Active High
#define InterockSplitted 	*(unsigned char*)INTERLOCK_SPLITED_BASE		//Clear Interlock Splitted. Active High
#define pauseSeq		*(unsigned char*)PAUSE_SEQ_BASE			//Pause Firing Sequence. (Block trigger signal for timing engine) Active High
#define clrPreionFault	*(unsigned char*)RST_PREION_FAULT_BASE	//Clear Preion Fault handler. Active High

void initController()
{

	pauseSeq = 1;								//In case of the Timing Engine is in "Firing" on powering up, this setting can prevent it from actually firing

	KPLED_All_Bright();							//Turn on all keypad LEDs (Bright)

	DisInit();									//Initialize Display
	DisCurPos(1,1);
	DisWrStr("Initializing...");				//Show message "Initializing..."

	UartInit();									//Initialize Nios UART


	remoteEn = 0;								//Disable Remote Control (make sure Nios core has the right of the UART)

	Stop();										//Send a Stop instruction to the Timing Engine. Make sure it's not firing on initial state.


	clrTimer = 1; 								//Reset Password&Heater Timer
	clrTimer = 0;

	clrPreionFault = 1;							//Clear Preion Fault Handler
	clrPreionFault = 0;

	clrInterlock=1;								//Clear Interlock Handler
	clrInterlock=0;

	//InterlockIn=1;
	//InterlockIn=0;

	//InterockSplitted=1;
	//InterockSplitted=0;

	loadData();									//Load Data From EEPROM to Main RAM
	delay(5000);								//There is no signal that indicates if the loading is done so use delay() to wait.

	KPLED_All_Dim();							//Turn on all keypad LEDs (Dim)
	delay(5000);

	reloadRegisters();							//Reload all Timing Engine registers with current data in Main RAM. (Load data from EEPROM does not refresh those registers)


	refreshErrFlags();							//Data Error Check

	CounterInit();								//Load Counters data to FPGA Counter module (from FRAM)

	PowerFailure_Init();						//Power Failure Handler Initialization (enable interrupt and register ISR)

	pauseSeq = 0;								//Clear Pause

	KPLED_All_Off();							//Turn off all keypad LEDs

	printf("Done!\n");							//Send massage through JTAG, for debugging.
}



/*
 * Each sub-menu has an function called "entry". It's a "cover page" of the sub-menu.
 * It should be called when an entry screen is needed at a upper level menu.
 * Usually this type of function takes one parameter which indicates the sequence number of it.
 * CAUTION: Availability of keys, KPLED & Navigation Bar (the forth row on the screen), is hard coded inside each "entry" function.
 * 			Remember to check and update them when the order they appear in a menu changes.
 */


void refreshTopMenuEntry(unsigned char curr_state)
{
	if(curr_state==1)		entryParameterMenu(curr_state);
	else if(curr_state==2)	entryStartupMenu(curr_state);
	else if(curr_state==3)	entryShutdownMenu(curr_state);
	else if(curr_state==4)	entryFireMenu(curr_state);
	else if(curr_state==5)	entryRemoteMenu(curr_state);
	else if(curr_state==6)	entrySaveLoadMenu(curr_state);
	else if(curr_state==7)	entryShotCountersMenu(curr_state);
	//else if(curr_state==8)	entryExternalTriggerMenu(curr_state);
}

/*
 * Like function "entry", "enter" is another function of each sub-menu.
 * It should be called when need to enter an sub-menu, such as ENTER button is pressed.
 * It has no returns.
 */

void enterTopMenuPage(unsigned char curr_state)
{
	if(curr_state==1)		enterParameterMenu();
	else if(curr_state==2)	enterStartupMenu();
	else if(curr_state==3)	enterShutdownMenu();
	else if(curr_state==4)	enterFireMenu();
	else if(curr_state==5)	enterRemoteMenu();
	else if(curr_state==6)	enterSaveLoadMenu();
	else if(curr_state==7)	enterShotCountersMenu();
	//else if(curr_state==8)	entryExternalTriggerMenu(curr_state);
}

/*
 * The structure of a multi-page menu is a Finite State Machine (FSM).
 * Key press is pulled in a infinity loop.
 * Variable "curr_state" holds current "state" (page ID).
 * Initially, curr_state = 1.
 *
 * Pressing "Right Arrow" will increase curr_state by 1 if it's less than the maximum page ID.
 * Pressing "Left Arrow" will decrease curr_state by 1 if it's greater than 1.
 * Call "refreshTopMenuEntry(curr_state)" to refresh the display each time after curr_state changes.
 * Call "enterTopMenuPage(curr_state)" to enter a sub-menu when ENTER is pressed.
 * CAUTION: Remember to call "refreshTopMenuEntry(curr_state)" when it returns from a sub-menu, because the display might be manipulated by a sub-menu.
 *
 * CAUTION: Put "while(keyFlag);" before entering a sub-menu to make sure key-press is released. If not, the key-press may also be captured by the sub-menu.
 */

void TopMenu()
{
	initController();												//Initialize the controller (go into this function to see details)
	printf("Menu Initialized!\n");									//Send massage through JTAG, for debugging.
	int MinPageID = 1;
	int MaxPageID = 7;

	showCoverPage();												//Display the "welcome screen".

	enterStartupMenu();												//Enter startup instruction menu to guide through the startup sequence

	unsigned char curr_state = 1;									//Make sure the menu starts from its first page

	refreshTopMenuEntry(curr_state);								//Call "refreshTopMenuEntry(curr_state)" to refresh the display each time after curr_state changes.

	unsigned char keyVal_latch;										//Once a key-press is detected, latch the key value in keyVal_latch.
	while(1)														//Infinity loop to pull key-press and update the state of the FSM.
	{
		if(keyFlag)													//If a key press is detected.
		{
			delay(100);												//De-bouncing
			if(keyFlag)												//De-bouncing
			{
				keyVal_latch = keyVal;								//Latch key value
				if(keyVal_latch == KEY_LEFT)						//If left arrow is pressed
				{
					if(curr_state > MinPageID)								//If curr_state is greater than 1
					{
						curr_state--;								//Decrease curr_state by 1
						refreshTopMenuEntry(curr_state);			//Refresh display
					}
				}
				else if(keyVal_latch == KEY_RIGHT)					//If right arrow is pressed
				{
					if(curr_state < MaxPageID)								//If curr_state is less than the total page number, 7(May updated to 8)
					{
						curr_state++;								//Increase curr_state by 1
						refreshTopMenuEntry(curr_state);			//Refresh display
					}
				}
				else if(keyVal_latch == KEY_ENTER)					//If ENTER key is pressed
				{
					while(keyFlag);									//Wait until key is released
					enterTopMenuPage(curr_state);					//Enter a sub-menu
					refreshTopMenuEntry(curr_state);				//Remember to call "refreshTopMenuEntry(curr_state)" when it returns from a sub-menu, because the display might be manipulated by a sub-menu.
				}
				else if(keyVal_latch == KEY_ENA)					//Enter Fire menu if ENABLE is pressed.
				{
					while(keyFlag);
					enterFireMenu();
					refreshTopMenuEntry(curr_state);
				}
			}
			while(keyFlag);											//Wait until key is released
		}
	}
}
