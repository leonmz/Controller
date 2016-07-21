/*
 * shotPage.c
 *
 *  Created on: Oct 21, 2014
 *      Author: Ce Guo
 */

#include "system.h"
#include "..\inc\DisDriver.h"
#include "..\inc\keyVal.h"
#include "..\inc\delay.h"
#include "..\inc\config.h"
#include "..\inc\editNum5.h"
#include "..\inc\loadWriteData.h"
#include "..\inc\KPLED.h"

void entryParameterNumOfShots()
{
	KPLED_All_Off();
	KPLED_Edit(3);
	KPLED_Menu(3);
	KPLED_Enable(1);
	KPLED_RightArrow(3);
	KPLED_LeftArrow(3);

	DisClear();
	DisShowCur(0);
	DisCurPos(1,1);
	DisWrStr("Number of Shots:");

	unsigned int shot=LoadRAMShots();
	DisCurPos(2,10);
	DisWrStr(Num5toStr5(shot));
	DisWrStr(" Shots");

	DisCurPos(4,1);
	DisWrStr("<Prev   Edit   Next>");

	/*Timing Error Navigation*/
	if(chkRightTiming(-1))
	{
		DisCurPos(4,15);
		DisWrStr("*");
	}
}

void editParameterNumOfShot()
{
	entryParameterNumOfShots();
	unsigned int shot=LoadRAMShots();

	unsigned int shot_new;
	while(1)
	{
		shot_new = editNum5(shot,2,10);
		if(shot_new>=SHOTS_MIN && shot_new<=SHOTS_MAX)
			break;
		else
		{
			DisShowCur(0);
			DisCurPos(2,1);
			DisWrStr("        ERR:OutOfRng");
			delay(MSG_DELAY);
			DisCurPos(2,1);
			DisWrStr("               Shots");
		}
	}
	WriteRAMShots(shot_new);
	entryParameterNumOfShots();
}
