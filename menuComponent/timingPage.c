/*
 * timingPage.c
 *
 *  Created on: Oct 21, 2014
 *      Author: Ce Guo
 */
#include "system.h"
#include "..\inc\DisDriver.h"
#include "..\inc\keyVal.h"
#include "..\inc\delay.h"
#include "..\inc\config.h"
#include "..\inc\editNum10.h"
#include "..\inc\loadWriteData.h"
#include "..\inc\errorCheck.h"

#include "..\inc\KPLED.h"


char* convertChnlTitle(unsigned char num)
{
	/*
	char* num_str = "00";

	num = num % 100;		//Ignore higher digits
	num_str[0] = 0x30 + num / 10;
	num = num % 10;
	num_str[1] = 0x30 + num;
	return num_str;
	*/

	switch(num)
	{
		case 1: return "01.LHV";
		case 2: return "02.HV";
		case 3: return "03.RF Preion";
		case 4: return "04.DC Preion Trig.";
		case 5: return "05.Thy Trig.";
		case 6: return "06.Aux1";
		case 7: return "07.Aux2";
		case 8: return "08.Aux3";
		case 9: return "09.Aux4";
	}
	return "";
}


void entryParameterTiming(unsigned char chnl)
{
	KPLED_All_Off();
	KPLED_Edit(3);
	KPLED_Menu(3);
	KPLED_Enable(1);
	if(chnl == CHNL_NUM)
		KPLED_LeftArrow(3);
	else
	{
		KPLED_RightArrow(3);
		KPLED_LeftArrow(3);
	}

	/*Print Title*/
	DisClear();
	DisShowCur(0);
	DisCurPos(1,1);
	//DisWrStr("Channel ");
	DisWrStr(convertChnlTitle(chnl));
	//DisWrStr(" Timing");

	DisCurPos(2,1);
	DisWrStr("START:");
	DisCurPos(3,1);
	DisWrStr("STOP:");

	/*Print Data*/
	unsigned int start = LoadRAMStartTime(chnl);
	unsigned int end = LoadRAMEndTime(chnl);

	DisCurPos(2,8);
	DisWrStr(Str10toDis10(Num10toStr10(start)));
	DisCurPos(3,8);
	DisWrStr(Str10toDis10(Num10toStr10(end)));



	/*Available Button*/
	DisCurPos(4,1);
	if(chnl==CHNL_NUM)
		DisWrStr("<Prev   Edit        ");
	else
		DisWrStr("<Prev   Edit   Next>");

	/*Error Check*/
	//Current Channel
	if(getErrFlag(chnl, 0))
	{
		DisCurPos(2,7);
		DisWrStr("*");
	}
	if(getErrFlag(chnl, 1))
	{
		DisCurPos(3,7);
		DisWrStr("*");
	}

	/*Timing Error Navigation*/
	//See also ParameterMenu.c --> refreshParameterMenuPage
	if(chkLeftTiming(chnl))
	{
		DisCurPos(4,6);
		DisWrStr("*");
	}
	if(chkRightTiming(chnl))
	{
		DisCurPos(4,15);
		DisWrStr("*");
	}

}



void editParamterTiming(unsigned char chnl)
{
	if(enterPWDPage())
	{
		entryParameterTiming(chnl);
		unsigned int start = LoadRAMStartTime(chnl);
		unsigned int end = LoadRAMEndTime(chnl);

		/*Edit Start Time*/
		WriteRAMStartTime(chnl,editNum10(chnl,0,start,2,8));
		refreshErrFlags();
		entryParameterTiming(chnl);  //Don't remove, the warning message may clear the screen

		/*Edit End Time*/
		WriteRAMEndTime(chnl,editNum10(chnl,1,end,3,8));
		refreshErrFlags();
	}
	entryParameterTiming(chnl);


}




