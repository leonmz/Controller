/*
 * pwdPage.c
 *
 *  Created on: Oct 23, 2014
 *      Author: Charlie
 */


#include "system.h"
#include "..\inc\DisDriver.h"
#include "..\inc\keyVal.h"
#include "..\inc\delay.h"
#include "..\inc\config.h"


#define rstPwdTimer *(unsigned char*)RSTPWDTIMER_BASE
#define pwdTimerExp *(unsigned char*)PWDTIMEREXP_BASE


unsigned char enterPWDPage()
{
	if(!pwdTimerExp)
		return 1;
	DisClear();
	DisShowCur(0);
	DisCurPos(1,1);
	//        12345678901234567890
	DisWrStr("Restricted Parameter");
	DisCurPos(2,1);
	DisWrStr("Enter Access Code:  ");

	if(enterPWD(3,8))
	{
		rstPwdTimer = 1;
		rstPwdTimer = 0;
		while(keyFlag);
		return 1;
	}
	else
		return 0;
}
