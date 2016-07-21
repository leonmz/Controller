/*
 * topSavePage.c
 *
 *  Created on: Dec 29, 2014
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

void entrySaveLoadMenu(unsigned char number)
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
	DisWrStr(".Load/Save Setting");

	DisCurPos(4,1);
	DisWrStr("<Prev   Enter  Next>");
}

void SaveToEEPROM()
{
	KPLED_All_Off();

	DisClear();
	DisShowCur(0);
	DisCurPos(2,1);
	DisWrStr("      Saving...     ");
	WriteEEPROM32Bits(1,LoadRAMRepRate());
	WriteEEPROM32Bits(2,LoadRAMShots());
	unsigned char i;
	for(i=1;i<=CHNL_NUM;i++)
	{
		WriteEEPROM32Bits(2*i+1,LoadRAMStartTime(i));
		WriteEEPROM32Bits(2*i+2,LoadRAMEndTime(i));
	}
	ResetRAMFromEEPROM();
	DisCurPos(3,1);
	DisWrStr("        Done!       ");
	delay(MSG_DELAY);
}

void refreshSaveLoadMenuPage(unsigned char curr_state)
{
	if(curr_state == 1)
	{
		KPLED_All_Off();
		KPLED_Enter(3);
		KPLED_RightArrow(3);
		KPLED_Menu(3);
		KPLED_Enable(1);

		DisClear();
		DisShowCur(0);
		DisCurPos(1,1);
		DisWrStr("     Load/Save      ");
		DisCurPos(2,1);
		DisWrStr("Load Settings");

		DisCurPos(4,1);
		DisWrStr("        Enter  Next>");
	}
	else if(curr_state == 2)
	{
		KPLED_All_Off();
		KPLED_Enter(3);
		KPLED_LeftArrow(3);
		KPLED_Menu(3);
		KPLED_Enable(1);

		DisClear();
		DisShowCur(0);
		DisCurPos(1,1);
		DisWrStr("     Load/Save      ");
		DisCurPos(2,1);
		DisWrStr("Save Settings");

		DisCurPos(4,1);
		DisWrStr("<Prev   Enter       ");
	}
}

void enterSaveLoadMenuPage(unsigned char curr_state)
{
	if(curr_state == 1)
	{
		KPLED_All_Off();
		KPLED_Enter(3);
		KPLED_Menu(3);

		DisClear();
		DisShowCur(0);
		DisCurPos(1,1);
		DisWrStr("Confirm to Load?");
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
						DisClear();
						DisShowCur(0);
						DisCurPos(2,1);
						DisWrStr("      Loading...    ");
						ResetRAMFromEEPROM();
						refreshErrFlags();
						DisCurPos(3,1);
						DisWrStr("        Done!       ");
						delay(MSG_DELAY);
						return;
					}
				}
				while(keyFlag);
			}
		}
	}
	else if(curr_state == 2)
	{
		KPLED_All_Off();
		KPLED_Enter(3);
		KPLED_Menu(3);

		DisClear();
		DisShowCur(0);
		DisCurPos(1,1);
		DisWrStr("Confirm to Save?");
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
						SaveToEEPROM();

						return;
					}
				}
				while(keyFlag);
			}
		}
	}
}

void enterSaveLoadMenu()
{
	unsigned char curr_state = 1;

	refreshSaveLoadMenuPage(curr_state);
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
							refreshSaveLoadMenuPage(curr_state);
						}
					}
					else if(keyVal_latch == KEY_RIGHT)
					{
						if(curr_state < 2)
						{
							curr_state++;
							refreshSaveLoadMenuPage(curr_state);
						}
					}
					else if(keyVal_latch == KEY_ENTER)
					{
						while(keyFlag);
						enterSaveLoadMenuPage(curr_state);
						refreshSaveLoadMenuPage(curr_state);
					}
					else if(keyVal_latch == KEY_MENU)
					{
						return;
					}
					else if(keyVal_latch == KEY_ENA)
					{
						while(keyFlag);
						enterFireMenu();
						refreshSaveLoadMenuPage(curr_state);
					}
				}
				while(keyFlag);
			}
		}
}



