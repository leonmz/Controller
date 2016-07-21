/*
 * comInterface.h
 *
 *  Created on: Nov 11, 2014
 *      Author: Ce Guo
 */

#ifndef COMINTERFACE_H_
#define COMINTERFACE_H_

void UartInit();														//Initialize Nios II UART Controller
void WriteRAM32Bits(unsigned char address, unsigned int data);			//Write a 32-bit data to the main memory of the timing engine.
unsigned int ReadRAM32Bits(unsigned int address);						//Read a 32-bit data from the main memory of the timing engine.
void WriteEEPROM8Bits(unsigned char address, unsigned char data);		//Write a 32-bit data to the EEPROM.
void WriteEEPROM32Bits(unsigned char address, unsigned int data);		//Read a 32-bit data from the EEPROM.
void Fire();															//Start to Fire
void Stop();															//Stop Firing
void ResetRAMFromEEPROM();												//Restore all data from EEPROM to the main memory of the timing engine.


#endif /* COMINTERFACE_H_ */
