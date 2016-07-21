/*
 * ShutdownInstruction.c
 *
 *  Created on: Mar 11, 2015
 *      Author: Ce Guo
 */




#include "system.h"
#include "..\inc\DisDriver.h"
#include "..\inc\keyVal.h"
#include "..\inc\delay.h"
#include "..\inc\config.h"
#include "..\inc\KPLED.h"
#include "..\inc\editNum2.h"
#include "..\inc\menu.h"

#define clrTimer *(unsigned char*)CLRTIMER_BASE

#define SHUTDOWN_STEP	7

char* getShutdownInst(unsigned char step, unsigned char line)
{
	switch(step)
	{
		case 1:		return line==1	?	"Disable & turn off"	:	"main HV supply";
		case 2:		return line==1	?	"Turn off all"			:	"power units";
		case 3:		return line==1	?	"Turn off turbo pump"	:	" ";
		case 4:		return line==1	?	"Turn off thyratron"	:	"heater";
		case 5:		return line==1	?	"Turn off cooling"		:	"water";
		case 6:		return line==1	?	"Turn off oil pump"		:	" ";
		case 7:		return line==1	?	"Close Argon tank"		:	" ";
	}
	return "";
}


void entryShutdownMenu(unsigned char number)
{
	KPLED_All_Off();
	KPLED_Enter(3);
	KPLED_Enable(1);
	KPLED_RightArrow(3);
	KPLED_LeftArrow(3);

	DisClear();
	DisShowCur(0);
	DisCurPos(1,1);
	DisWrStr("       MENU         ");
	DisCurPos(2,1);
	DisWrStr(Num2toStr2(number));
	DisWrStr(".Shutdown");

	DisCurPos(4,1);
	DisWrStr("<Prev   Enter  Next>");
}

//1:Shutdown	0:Exit
unsigned char confirmShutdown()
{
	KPLED_All_Off();
	KPLED_Enter(3);
	KPLED_Menu(3);

	DisClear();
	DisShowCur(0);
	DisCurPos(1,1);
	DisWrStr("Confirm to Shutdown?");
	DisCurPos(2,4);
	DisWrStr("Yes: Press ENTER");
	DisCurPos(3,4);
	DisWrStr("No:  Press MENU");

	unsigned char keyVal_latch;
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
				else if(keyVal == KEY_ENTER)
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

void refreshShutdownMenuPage(unsigned char curr_state)
{
	KPLED_All_Off();
	KPLED_Enter(3);
	if(curr_state != 1) KPLED_LeftArrow(3);

	DisClear();
	DisShowCur(0);

	DisCurPos(1,1);
	DisWrStr("Shutdown Seq.  ");
	DisWrStr(Num2toStr2(curr_state));
	DisWrStr("/");
	DisWrStr(Num2toStr2(SHUTDOWN_STEP));

	DisCurPos(2,1);
	DisWrStr(getShutdownInst(curr_state,1));
	DisCurPos(3,1);
	DisWrStr(getShutdownInst(curr_state,2));

	DisCurPos(4,1);
	if(curr_state != 1)
		DisWrStr("<Prev  ENTER=OK     ");
	else
		DisWrStr("       ENTER=OK     ");


}

void enterShutdownMenu()
{
	if(!confirmShutdown())
		return;

	unsigned char curr_state = 1;
	refreshShutdownMenuPage(curr_state);
	unsigned char keyVal_latch;
	while(1)
	{
		if(keyFlag)
		{
			delay(100);
			if(keyFlag)
			{
				keyVal_latch = keyVal;
				if(keyVal_latch == KEY_LEFT)
				{
					if(curr_state > 1)
					{
						curr_state--;
						refreshShutdownMenuPage(curr_state);
					}
				}
				else if(keyVal_latch == KEY_ENTER)
				{
					if(curr_state < SHUTDOWN_STEP)
					{
						curr_state++;
						refreshShutdownMenuPage(curr_state);
					}
					else if(curr_state == SHUTDOWN_STEP)
					{
						DisClear();
						DisCurPos(2,8);
						DisWrStr("Done!");
						delay(MSG_DELAY);
						break;
					}
				}
			}
			while(keyFlag);
		}
	}

	clrTimer=1;
	clrTimer=0;

	KPLED_All_Off();
	KPLED_Menu(3);

	DisClear();
	DisCurPos(1,1);
	DisWrStr(" System is Shutdown ");
	DisCurPos(2,1);
	DisWrStr("Power Off Controller");
	DisCurPos(3,1);
	DisWrStr("         OR         ");
	DisCurPos(4,1);
	DisWrStr("Press MENU to Start ");

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
					enterStartupMenu();
					break;
				}
			}
		}
	}
}
