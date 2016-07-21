/*
 * Menu.c
 *
 *  Created on: Oct 21, 2014
 *      Author: Ce Guo
 */

#include "system.h"
#include "..\inc\DisDriver.h"
#include "..\inc\keyVal.h"
#include "..\inc\delay.h"
#include "..\inc\config.h"
#include "..\inc\loadWriteData.h"
#include "..\inc\comInterface.h"
#include "..\inc\editNum2.h"
#include "..\inc\errorCheck.h"
#include "..\inc\menu.h"
#include "..\inc\KPLED.h"



void entryParameterMenu(unsigned char number)
{
	KPLED_All_Off();
	KPLED_Enter(3);
	KPLED_Enable(1);
	KPLED_RightArrow(3);
	//KPLED_LeftArrow(3);

	DisClear();
	DisShowCur(0);
	DisCurPos(1,1);
	DisWrStr("       MENU         ");
	DisCurPos(2,1);
	DisWrStr(Num2toStr2(number));
	DisWrStr(".Parameters");

	//Error Check
	if(chkAllTiming())
	{
		DisCurPos(2,13);
		DisWrStr("*");
	}

	DisCurPos(4,1);
	DisWrStr("        Enter  Next>");


}

void refreshParameterMenuEntry(unsigned char curr_state)
{

	if(curr_state == 1)
	{
		entryParameterRepRate();
	}
	else if(curr_state == 2)
	{
		entryParameterNumOfShots();
	}
	else
	{
		entryParameterTiming(curr_state-2);
	}
}

void editParameter(unsigned char curr_state)
{
	if(curr_state == 1)
		editParameterRepRate();
	else if(curr_state == 2)
		editParameterNumOfShot();
	else
		editParamterTiming(curr_state-2);
}

void enterParameterMenu()
{

	//unsigned int blinkCtr = 0;

	//initMenu();

	//showCoverPage();

	unsigned char curr_state = 1;
	refreshParameterMenuEntry(curr_state);
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
						refreshParameterMenuEntry(curr_state);
					}
				}
				else if(keyVal_latch == KEY_RIGHT)
				{
					if(curr_state < 2+CHNL_NUM)
					{
						curr_state++;
						refreshParameterMenuEntry(curr_state);
					}
				}
				else if(keyVal_latch == KEY_EDIT)
				{
					while(keyFlag);
					editParameter(curr_state);
				}
				else if(keyVal_latch == KEY_MENU)
				{
					while(keyFlag);
					return;
				}
				else if(keyVal_latch == KEY_ENA)
				{
					while(keyFlag);
					enterFireMenu();
					refreshParameterMenuEntry(curr_state);
				}
			}
			while(keyFlag);
		}


	}

}


