/*
 * loadTimeData.c
 *
 *  Created on: Oct 21, 2014
 *      Author: Charlie
 */

/*
 * See header file for description of each function
 */

#include <string.h>
#include "..\inc\comInterface.h"
#include "..\inc\config.h"
#include "..\inc\FRAM.h"

void loadData()
{
	ResetRAMFromEEPROM();
	return;
}

unsigned int LoadRAMRepRate()
{
	return ReadRAM32Bits(1);
}

unsigned int LoadRAMShots()
{
	return ReadRAM32Bits(2);
}

unsigned int LoadRAMStartTime(unsigned char chnl)
{
	return ReadRAM32Bits(1+chnl*2);
}

unsigned int LoadRAMEndTime(unsigned char chnl)
{
	return ReadRAM32Bits(2+chnl*2);
}

unsigned int LoadCounter0Init()
{
	return FRAM_Read32Bits(COUNTER_0_ADDR);
}

unsigned int LoadCounter1Init()
{
	return FRAM_Read32Bits(COUNTER_1_ADDR);
}


void WriteRAMRepRate(unsigned int data)
{
	WriteRAM32Bits(1,data);
}

void WriteRAMShots(unsigned int data)
{
	WriteRAM32Bits(2,data);
}

void WriteRAMStartTime(unsigned char chnl, unsigned int data)
{
	WriteRAM32Bits(1+chnl*2,data);
}

void WriteRAMEndTime(unsigned char chnl, unsigned int data)
{
	WriteRAM32Bits(2+chnl*2,data);
}

void reloadRegisters()		//All registers are reset on StopFiring signal. Reload all registers by rewriting to RAM.
{
	WriteRAMRepRate(LoadRAMRepRate());
	WriteRAMShots(LoadRAMShots());
	unsigned char i;
	for(i=1;i<=CHNL_NUM;i++)
	{
		WriteRAMStartTime(i,LoadRAMStartTime(i));
		WriteRAMEndTime(i,LoadRAMEndTime(i));
	}
	refreshErrFlags();
}

