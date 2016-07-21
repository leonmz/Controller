/*
 * powerFailure.c
 *
 *  Created on: Jan 16, 2015
 *      Author: Ce Guo
 */

#include "altera_avalon_pio_regs.h"
#include "system.h"
#include "sys/alt_irq.h"
#include "..\inc\Counter.h"
#include "..\inc\DisDriver.h"
#include "..\inc\KPLED.h"
#include "..\inc\config.h"
#include "..\inc\Counter.h"
#include <stdlib.h>
#include <stdio.h>

#define powerOk			*(unsigned char*)POWER_OK_BASE
#define pauseSeq		*(unsigned char*)PAUSE_SEQ_BASE

static void PowerFailure_ISR(void *context)
{
	//De-bouncing
	delay(0);
	if(powerOk)
	{
		IOWR_ALTERA_AVALON_PIO_EDGE_CAP(POWER_OK_BASE, 0x0);
		return;
	}

	delay(0);
	if(powerOk)
	{
		IOWR_ALTERA_AVALON_PIO_EDGE_CAP(POWER_OK_BASE, 0x0);
		return;
	}

	delay(0);
	if(powerOk)
	{
		IOWR_ALTERA_AVALON_PIO_EDGE_CAP(POWER_OK_BASE, 0x0);
		return;
	}

	delay(0);
	if(powerOk)
	{
		IOWR_ALTERA_AVALON_PIO_EDGE_CAP(POWER_OK_BASE, 0x0);
		return;
	}


	pauseSeq = 1;
	DisOff();
	KPLED_All_Off();

	//Counter Protection
	SaveCounterToFRAM();

	//In case of fake power failure
	DisInit();
	DisCurPos(2,1);
	DisWrStr("Controller Pwr Fails");
	DisCurPos(3,1);
	DisWrStr(" Counter Protected  ");

	while(1);

}


void PowerFailure_Init()
{
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(POWER_OK_BASE, 0xF);
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(POWER_OK_BASE, 0x0);
	alt_ic_isr_register(POWER_OK_IRQ_INTERRUPT_CONTROLLER_ID, POWER_OK_IRQ, PowerFailure_ISR, NULL, 0x0);
}
