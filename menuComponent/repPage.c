/*
 * repPage.c
 *
 *  Created on: Oct 21, 2014
 *      Author: Charlie
 */

#include "system.h"
#include "..\inc\DisDriver.h"
#include "..\inc\keyVal.h"
#include "..\inc\delay.h"
#include "..\inc\config.h"
#include "..\inc\editNum2.h"
#include "..\inc\loadWriteData.h"
#include "..\inc\errorCheck.h"
#include "..\inc\KPLED.h"

void entryParameterRepRate()
{
	KPLED_All_Off();
	KPLED_Edit(3);
	KPLED_Menu(3);
	KPLED_Enable(1);
	KPLED_RightArrow(3);

	DisClear();
	DisShowCur(0);
	DisCurPos(1,1);
	DisWrStr("Repetition Rate:");

	unsigned int rep=LoadRAMRepRate();
	DisCurPos(2,16);
	DisWrStr(Num2toStr2(rep));
	DisWrStr(" Hz");

	DisCurPos(4,1);
	DisWrStr("        Edit   Next>");

	/*Timing Error Navigation*/
	if(chkRightTiming(-1))
	{
		DisCurPos(4,15);
		DisWrStr("*");
	}
}

void editParameterRepRate()
{
	entryParameterRepRate();
	unsigned int rep=LoadRAMRepRate();

	unsigned int rep_new;
	while(1)
	{
		rep_new = editNum2(rep,2,16);
		if(rep_new>=REP_RATE_MIN && rep_new<=REP_RATE_MAX)
			break;
		else
		{
			DisShowCur(0);
			DisCurPos(2,1);
			DisWrStr("        ERR:OutOfRng");
			delay(MSG_DELAY);
			DisCurPos(2,1);
			DisWrStr("                  Hz");
		}
	}

	WriteRAMRepRate(rep_new);
	entryParameterRepRate();
	DisShowCur(0);
	DisCurPos(2,1);
	DisWrStr("         Checking...");
	refreshErrFlags();
	entryParameterRepRate();
}
