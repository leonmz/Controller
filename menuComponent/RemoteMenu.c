/*
 * topRemotePage.c
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
#include "..\inc\KPLED.h"
#include "..\inc\editNum2.h"

#define remoteEn *(unsigned char*)REMOTECTRL_BASE    //If remoteEn = 1, then goes into remote control mode; if remoteEn = 0, then goes into domestic control mode.

void StartRemoteCtrl()
{
	unsigned char keyVal_latch;

	remoteEn = 1;

	KPLED_All_Off();
	KPLED_Menu(3);

	DisClear();
	DisShowCur(0);
	DisCurPos(2,1);
	DisWrStr("Controlled Remotely");
	DisCurPos(3,1);
	DisWrStr(" Press MENU to Exit");

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
					remoteEn = 0;
					KPLED_All_Off();
					DisClear();
					DisCurPos(2,2);
					DisWrStr("Remote Terminated");
					delay(MSG_DELAY);
					return;
				}
			}
			while(keyFlag);
		}
	}
}

void entryRemoteMenu(unsigned char number)
{
	KPLED_All_Off();
	KPLED_Enter(3);
	KPLED_Enable(1);
	KPLED_LeftArrow(3);
	KPLED_RightArrow(3);

	DisClear();
	DisShowCur(0);
	DisCurPos(1,1);
	DisWrStr("       MENU         ");
	DisCurPos(2,1);
	DisWrStr(Num2toStr2(number));
	DisWrStr(".Remote Control");

	DisCurPos(4,1);
	DisWrStr("<Prev   Enter  Next>");
}

void enterRemoteMenu()
{
	KPLED_All_Off();
	KPLED_Enter(3);
	KPLED_Menu(3);

	DisClear();
	DisShowCur(0);
	DisCurPos(1,1);
	DisWrStr("Enable Remote Ctrl?");
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
					StartRemoteCtrl();
					return;
				}
			}
			while(keyFlag);
		}
	}
}





