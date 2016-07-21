/*
 * StartupMenu.c
 *
 *  Created on: Feb 18, 2015
 *      Author: Ce Guo
 */


#include "system.h"
#include "..\inc\DisDriver.h"
#include "..\inc\keyVal.h"
#include "..\inc\delay.h"
#include "..\inc\config.h"
#include "..\inc\KPLED.h"
#include "..\inc\editNum2.h"


#define STARTUP_STEP	10

#define rstHeaterTimer *(unsigned char*)RSTHEATERTIMER_BASE

char* getStartupInst(unsigned char step, unsigned char line)
{
	switch(step)
	{
		case 1:		return line==1	?	"Check"					:	"Oil & Hoses";
		case 2:		return line==1	?	"Turn on thyratron"		:	"heater (10 minutes)";
		case 3:		return line==1	?	"Pump using"			:	"roughing pumps";
		case 4:		return line==1	?	"Run cooling water"		:	" ";
		case 5:		return line==1	?	"Turn on turbo pump"	:	"stabilize pressure";
		case 6:		return line==1	?	"Turn on oil pump"		:	" ";
		case 7:		return line==1	?	"Open Argon tank"		:	"for lasing pressure";
		case 8:		return line==1	?	"Turn on RF unit"		:	"Check for faults";
		case 9:		return line==1	?	"Turn on power units"	:	"DO NOT enable HV";
		case 10:	return line==1	?	"Start to fire"			:	"Then enable HV";
	}
	return "";
}

void startupAction(unsigned char step)
{
	switch(step)
	{
		case 1:	break;
		case 2: {rstHeaterTimer=1; rstHeaterTimer=0; break;}
		case 3: break;
		case 4: break;
		case 5: break;
		case 6: break;
		case 7: break;
		case 8: break;
		case 9: break;
		case 10: break;
	}
}

void entryStartupMenu(unsigned char number)
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
	DisWrStr(".Startup Sequence");

	DisCurPos(4,1);
	DisWrStr("<Prev   Enter  Next>");
}

void refreshStartupMenuPage(unsigned char curr_state)
{
	//static unsigned char max_state_reached;
	KPLED_All_Off();
	KPLED_Enter(3);
	if(curr_state != 1) KPLED_LeftArrow(3);

	DisClear();
	DisShowCur(0);

	DisCurPos(1,1);
	DisWrStr("Startup Seq.   ");
	DisWrStr(Num2toStr2(curr_state));
	DisWrStr("/");
	DisWrStr(Num2toStr2(STARTUP_STEP));

	DisCurPos(2,1);
	DisWrStr(getStartupInst(curr_state,1));
	DisCurPos(3,1);
	DisWrStr(getStartupInst(curr_state,2));

	DisCurPos(4,1);
	if(curr_state != 1)
		DisWrStr("<Prev  ENTER=OK     ");
	else
		DisWrStr("       ENTER=OK     ");


}

void enterStartupMenu()
{
	unsigned char curr_state = 1;
	refreshStartupMenuPage(curr_state);
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
						refreshStartupMenuPage(curr_state);
					}
				}
				else if(keyVal_latch == KEY_ENTER)
				{
					startupAction(curr_state);
					if(curr_state < STARTUP_STEP)
					{
						curr_state++;
						refreshStartupMenuPage(curr_state);
					}
					else if(curr_state == STARTUP_STEP)
					{
						DisClear();
						DisCurPos(2,8);
						DisWrStr("Done!");
						delay(MSG_DELAY);
						return;
					}
				}
			}
			while(keyFlag);
		}
	}
}
