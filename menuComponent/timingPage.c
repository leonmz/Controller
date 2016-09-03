/*
 * timingPage.c
 *
 *  Created on: Oct 21, 2014
 *      Author: Ce Guo
 *  Modified on: Aug 27th, 2016 (Lock parameter LHV and HV)
 *      Leon
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
		case 8: return "08.RF CW";
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
        if(chnl==2||chnl==8||chnl==1||chnl==5){
        		DisClear();
        		DisCurPos(1,1);
        	    DisWrStr("Locked Parameter");
        	    DisCurPos(2,1);
        	    if(chnl==2){
        	    	DisWrStr("You can adjust by ad");
        	    	DisCurPos(3,1);
		            DisWrStr("justing Thy Trig");
		            KPLED_All_Off();
        	    }
        	    else if(chnl==1){
        	    	DisWrStr("You can adjust by ad");
        	    	DisCurPos(3,1);
        	    	DisWrStr("justing DC Preion");
        	    	KPLED_All_Off();
        	    }
        	    else if(chnl==8){
        	        DisWrStr("You can adjust by ad");
        	        DisCurPos(3,1);
        	        DisWrStr("justing RF Preion");
        	        KPLED_All_Off();
        	    }
        	    else if(chnl==5){
        	        DisWrStr("You can adjust by ad");
        	        DisCurPos(3,1);
        	        DisWrStr("justing RF Preion");
        	        KPLED_All_Off();
        	    }
        	    delay(100000);
        	    entryParameterTiming(chnl);
        	    return;
        }
		/*Edit Start Time*/
        if(chnl==3||chnl==4){
        	if (chnl==3){
        		DisClear();
        		DisCurPos(1,1);
        		DisWrStr("Starttime of RF must");                      //Display instruction for user when they want to make change
        		DisCurPos(2,1);                                        // to RF preion time or DC preion time.
        		DisWrStr("be greater than ");
        		DisCurPos(3,1);
        		DisWrStr("39009 us.");
        		KPLED_All_Off();
        	}
        	if (chnl==4){
        		unsigned int RF_Start=LoadRAMEndTime(3);
        		char upperlimit[6];
        		char lowerlimit[6];
        		RF_Start/=100;
        	    sprintf(upperlimit,"%d",RF_Start-9);
        	    sprintf(lowerlimit,"%d",RF_Start-1);
        	    DisClear();
        	    DisCurPos(1,1);
        	    DisWrStr("Starttime of DC is ");
        	    DisCurPos(2,1);
        	    DisWrStr("in range of ");
        	    DisCurPos(2,13);
        	    DisWrStr(upperlimit);
        	    DisCurPos(3,1);
        	    DisWrStr(" us");
        	    DisCurPos(3,5);
        	    DisWrStr("to ");
        	    DisCurPos(3,7);
        	    DisWrStr(lowerlimit);
        	    DisCurPos(3,12);
        	    DisWrStr(" us.");
        	    KPLED_All_Off();
        	        	}
        	delay(100000);
        	entryParameterTiming(chnl);
        	unsigned int StartTime=editNum10(chnl,0,start,2,8);
        	if(chnl==3){
        		WriteRAMStartTime(8,StartTime-900000);             //adjust Chnl8(RF CW) automatically when RF preion changes
        		WriteRAMEndTime(8,StartTime+100000);               //It will triggered 9ms before RF preion boost signal  and end 10ms after.
        		WriteRAMStartTime(chnl,StartTime);
        		WriteRAMEndTime(chnl,StartTime+100000);            //Endtime of RF Boost will be adjusted as 1ms after starttime.
        		WriteRAMStartTime(5,StartTime+100100);             //adjust Chnl5(Main Trigger) automatically when RF preion changes
        		WriteRAMEndTime(5,StartTime+100200);
        	    WriteRAMStartTime(2,StartTime-1700900);            //adjust HV automatically  when RF preion changes
        	    WriteRAMEndTime(2,StartTime+99100);
        	    refreshErrFlags();
        	    entryParameterTiming(chnl);
        		return;
        	}
        	if(chnl==4){
        		WriteRAMStartTime(1,StartTime-4000000);             //adjust LHV automatically when DC preion changes
        		WriteRAMEndTime(1,StartTime-100000);
        		WriteRAMStartTime(chnl,StartTime);
        	}
    	    refreshErrFlags();
    	    entryParameterTiming(chnl);
    		WriteRAMEndTime(chnl,editNum10(chnl,1,end,3,8));
    		refreshErrFlags();
    		entryParameterTiming(chnl);
    		return;
        }
		WriteRAMStartTime(chnl,editNum10(chnl,0,start,2,8));
		refreshErrFlags();
		entryParameterTiming(chnl);  //Don't remove, the warning message may clear the screen

		/*Edit End Time*/
		WriteRAMEndTime(chnl,editNum10(chnl,1,end,3,8));
		refreshErrFlags();
	}
	entryParameterTiming(chnl);


}




