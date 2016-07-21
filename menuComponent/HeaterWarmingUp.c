/*
 * HeaterWarmingUp.c
 *
 *  Created on: Mar 18, 2015
 *      Author: Ce Guo
 */

#include "system.h"
#include "..\inc\editNum2.h"
#include "..\inc\DisDriver.h"
#include "..\inc\config.h"
#include "..\inc\keyVal.h"
#include "..\inc\delay.h"
#include "..\inc\KPLED.h"

#define heaterCounter	*(unsigned int*) HEATERCOUNTER_BASE		//Input, 10bit, A count down timer for heater warming up
#define heaterDone		*(unsigned char*) HEATERDONE_BASE		//Input, Flag, set when heater is ready (warming up finished)

/*
 * Convert time format from seconds to MM:SS
 */
void printTime(unsigned short seconds)
{
	unsigned char min = seconds / 60;
	unsigned char sec = seconds % 60;

	DisWrStr(Num2toStr2(min));
	DisWrStr(":");
	DisWrStr(Num2toStr2(sec));
}



/*
 * Check heater status
 * If the heater is warming up, show a count down on the screen
 * If the heater is ready, return 0, which indicates OK
 * Return:
 * 		0: Ready(Done)	1: Not Ready(Warming Up)
 */
unsigned char checkHeater()
{
	if((!heaterDone) && !ENGR_MODE)				//If heater is still warming up
	{
		KPLED_All_Off();
		KPLED_Menu(3);

		DisClear();								//Display the count down timer for the heater
		DisCurPos(1,2);
		DisWrStr("Heater Warming Up");
		DisCurPos(2,5);
		DisWrStr("Please Wait...");
		DisCurPos(4,1);
		DisWrStr("     MENU=Exit      ");

		unsigned char keyVal_latch;				//Detect key press, MENU to return
		while(!heaterDone)
		{
			DisCurPos(3,8);
			printTime(heaterCounter);
			if(keyFlag)
			{
				delay(100);
				if(keyFlag)
				{
					keyVal_latch = keyVal;
					if(keyVal_latch == KEY_MENU)
					{
						while(keyFlag);
						return 1;				//Return 1, indicates that the heater is not ready
					}
				}
			}
		}
	}
	return 0;									//Return 0, indicates that the heater is ready
}
