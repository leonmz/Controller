/*
 * Counter.c
 *
 *  Created on: Jan 16, 2015
 *      Author: Ce Guo
 */

#include "altera_avalon_pio_regs.h"
#include "system.h"
#include "sys/alt_irq.h"
#include "..\inc\Counter.h"
#include "..\inc\config.h"
#include "..\inc\loadWriteData.h"
#include "..\inc\FRAM.h"
#include "..\inc\delay.h"
#include <stdlib.h>
#include <stdio.h>

/*
 * The counter module loads initial values on the rising edge of Counter_load (COUNTER_LOAD_BASE)
 * Procedure of Counters Initialization
 * 		1. Set Counter_load (COUNTER_LOAD_BASE) to 0
 * 		2. Load saved counter values from FRAM.
 * 		3. Put the values to Counter0_init and Counter1_init (COUNTER0_INIT_BASE and COUNTER1_INIT_BASE) respectively.
 * 		4. Set Counter_load to 1.
 * 		5. Set Counter_load to 0.
 */

void CounterInit()
{
	Counter_load = 0;
	unsigned int init0 = LoadCounter0Init();
	unsigned int init1 = LoadCounter1Init();
	Counter0_init = init0;
	Counter1_init = init1;
	Counter_load = 1;
	Counter_load = 0;
}


/*
 * At all time, the two outputs of the counter module hold the current value of the two counters.
 * 		Counter0_val (COUNTER0_IN_BASE): System counter
 * 		Counter1_val (COUNTER1_IN_BASE): Capillary counter
 */
unsigned int getCounter0Val()
{
	unsigned int val = Counter0_val;
	return val;
}
unsigned int getCounter1Val()
{
	unsigned int val = Counter1_val;
	return val;
}

void SaveCounterToFRAM()
{
	unsigned int Counter0_val_latch = Counter0_val;
	unsigned int Counter1_val_latch = Counter1_val;
	FRAM_Write32Bits(COUNTER_0_ADDR,Counter0_val_latch);
	FRAM_Write32Bits(COUNTER_1_ADDR,Counter1_val_latch);
}


/*
 * The capillary counter can be reset by user.
 * When reset, simply load 0 to Counter1 and the old Counter0 value to Coutner0.
 */
void resetCounter1()
{

	delay(10000);
	Counter_load = 0;
	Counter0_init = Counter0_val;
	Counter1_init = 0;
	Counter_load = 1;
	Counter_load = 0;
}
