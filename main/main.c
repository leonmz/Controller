/*
 * XUV Lasers
 * Program for the system controller of the Compact Capillary Discharge Soft X-Ray Laser.
 *
 * Author: Ce Guo (guoce.ecnu@gmail.com)
 */
#include "DisDriver.h"
#include <stdio.h>
#include <system.h>														//CPU configutation
#include "delay.h"
#define clrTimer 		*(unsigned char*)CLRTIMER_BASE
#define rstPwdTimer *(unsigned char*)RSTPWDTIMER_BASE
#define heaterCounter	*(unsigned int*) HEATERCOUNTER_BASE		//Input, 10bit, A count down timer for heater warming up
#define heaterDone		*(unsigned char*) HEATERDONE_BASE		//Input, Flag, set when heater is ready (warming up finished)
#define rstHeaterTimer *(unsigned char*)RSTHEATERTIMER_BASE

int main()
{
	//initController();
	/*clrTimer=1;
	clrTimer=0;
	rstHeaterTimer=1;
	rstHeaterTimer=0;
	rstPwdTimer=1;
	rstPwdTimer=0;
	//printf("Initial Value: %d\n",heaterCounter);
	int i=0;
	while(1){
		printf("%dth HeaterCounter: %d\n",i,heaterCounter);
		i++;
		delay(5000);
	}*/

	TopMenu();

  return 0;
}
