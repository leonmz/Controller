/*
 * Counter.h
 *
 *  Created on: Jan 16, 2015
 *      Author: Ce Guo
 *
 *  Drivers for system and capillary counters
 */


//System counter: Counter0
//Capillary counter: Counter1
#ifndef COUNTER_H_
#define COUNTER_H_

#define Counter0_val	*(unsigned int*)COUNTER0_IN_BASE				//Counter 0 current value 32-bit /Input
#define Counter1_val	*(unsigned int*)COUNTER1_IN_BASE				//Counter 1 current value 32-bit /Input
#define Counter0_init	*(unsigned int*)COUNTER0_INIT_BASE				//Counter 0 initial value 32-bit /Output
#define Counter1_init	*(unsigned int*)COUNTER1_INIT_BASE				//Counter 1 initial value 32-bit /Output
#define Counter_load	*(unsigned char*)COUNTER_LOAD_BASE				//Load signal for counter module

void CounterInit();														//Initialize counter: load counter values from non-volatile memory
unsigned int getCounter0Val();											//Get current counter 0 value
unsigned int getCounter1Val();											//Get current counter 1 value
void SaveCounterToFRAM();												//Save current counters' value to FRAM
void resetCounter1();													//Reset counter 1 (capillary counter)



#endif /* COUNTER_H_ */
