/*
 * coverPage.c
 *
 *  Created on: Oct 21, 2014
 *      Author: Ce Guo
 */

#include "system.h"
#include "..\inc\DisDriver.h"
#include "..\inc\keyVal.h"
#include "..\inc\delay.h"
#include "..\inc\config.h"
#include "..\inc\KPLED.h"
#include <unistd.h>

/*
 * Display the cover page (welcome screen).
 */

void showCoverPage()
{
	DisClear();
	DisShowCur(0);
	DisCurPos(2,1);
	//        12345678901234567890
	DisWrStr("  Soft X-Ray Laser  ");
	DisCurPos(3,1);
	DisWrStr("Press MENU to Start");
	printf("Engr: %d",ENGR_MODE);
	if(ENGR_MODE)
	{
		DisCurPos(4,1);
		DisWrStr("  !!!ENGR_MODE!!!   ");
	}

	KPLED_All_Off();
	KPLED_Menu(3);

	unsigned char keyVal_latch;
	while(1)
	{
		if(keyFlag)
		{
			delay(100);
			if(keyFlag)
			{
				keyVal_latch = keyVal;
				if(keyVal_latch==KEY_MENU)
				{
					while(keyFlag);
					return;
				}
			}
		}
	}
}
