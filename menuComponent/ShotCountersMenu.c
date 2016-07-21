/*
 * topShotCounters.c
 *
 *  Created on: Jan 17, 2015
 *      Author: Ce Guo
 */




#include "system.h"
#include "..\inc\DisDriver.h"
#include "..\inc\keyVal.h"
#include "..\inc\delay.h"
#include "..\inc\config.h"
#include "..\inc\loadWriteData.h"
#include "..\inc\comInterface.h"
#include "..\inc\KPLED.h"
#include "..\inc\editNum10.h"
#include "..\inc\Counter.h"

void entryShotCountersMenu(unsigned char number)
{
	KPLED_All_Off();
	KPLED_Enter(3);
	KPLED_Enable(1);
	KPLED_LeftArrow(3);

	DisClear();
	DisShowCur(0);
	DisCurPos(1,1);
	DisWrStr("       MENU         ");
	DisCurPos(2,1);
	DisWrStr(Num2toStr2(number));
	DisWrStr(".Shot Counters");

	DisCurPos(4,1);
	DisWrStr("<Prev   Enter       ");
}

void refreshShotCountersMenuPage(unsigned char curr_state)
{
	if(curr_state == 1)
	{
		KPLED_All_Off();
		KPLED_RightArrow(3);
		KPLED_Menu(3);
		KPLED_Enable(1);

		DisClear();
		DisShowCur(0);
		DisCurPos(1,1);
		DisWrStr("   Shot Counters    ");
		DisCurPos(2,1);
		DisWrStr("System Counter:");
		DisCurPos(3,5);
		DisWrStr(Num10toStr10(getCounter0Val()));
		DisWrStr(" Shots");
		DisCurPos(4,1);
		DisWrStr("               Next>");
	}
	else if(curr_state == 2)
	{
		KPLED_All_Off();
		KPLED_Enter(3);
		KPLED_LeftArrow(3);
		KPLED_Menu(3);
		KPLED_Edit(3);
		KPLED_Enable(1);

		DisClear();
		DisShowCur(0);
		DisCurPos(1,1);
		DisWrStr("   Shot Counters    ");
		DisCurPos(2,1);
		DisWrStr("Capillary Counter:");
		DisCurPos(3,5);
		DisWrStr(Num10toStr10(getCounter1Val()));
		DisWrStr(" Shots");

		DisCurPos(4,1);
		DisWrStr("<Prev   Edit        ");
	}
}

void editShotCountersMenuPage(unsigned char curr_state)
{
	if(curr_state == 2)
	{
		KPLED_All_Off();
		KPLED_Enter(3);
		KPLED_Menu(3);

		DisClear();
		DisShowCur(0);
		DisCurPos(1,1);
		DisWrStr("Reset Cap. Counter?");
		DisCurPos(2,1);
		DisWrStr("   Yes: Press ENTER");
		DisCurPos(3,1);
		DisWrStr("   No:  Press MENU");

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
						return;
					}
					else if(keyVal_latch == KEY_ENTER)
					{
						while(keyFlag);
						if(enterPWDPage())
						{
							DisClear();
							DisCurPos(2,1);
							DisWrStr("      Resetting...  ");
							DisCurPos(3,1);
							DisWrStr(" Capillary Counter  ");
							resetCounter1();
							DisClear();
							DisCurPos(2,1);
							DisWrStr("      Reseted       ");
							delay(MSG_DELAY);
						}
						return;
					}
				}
				while(keyFlag);
			}
		}
	}
}


void enterShotCountersMenu()
{
	unsigned char curr_state = 1;

	refreshShotCountersMenuPage(curr_state);
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
						refreshShotCountersMenuPage(curr_state);
					}
				}
				else if(keyVal_latch == KEY_RIGHT)
				{
					if(curr_state < 2)
					{
						curr_state++;
						refreshShotCountersMenuPage(curr_state);
					}
				}
				else if(keyVal_latch == KEY_EDIT)
				{
					while(keyFlag);
					editShotCountersMenuPage(curr_state);
					refreshShotCountersMenuPage(curr_state);
				}
				else if(keyVal_latch == KEY_MENU)
				{
					return;
				}
				else if(keyVal_latch == KEY_ENA)
				{
					while(keyFlag);
					enterFireMenu();
					refreshShotCountersMenuPage(curr_state);
				}
			}
			while(keyFlag);
		}
	}
}
