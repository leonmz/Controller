/*
 * loadWriteData.h
 *
 *  Created on: Nov 13, 2014
 *      Author: Ce Guo
 */

#ifndef LOADWRITEDATA_H_
#define LOADWRITEDATA_H_

void loadData();

unsigned int LoadRAMRepRate();							//Load and return Repetition Rate from the main memory of timing engine
unsigned int LoadRAMShots();							//Load and return Number of Shots from the main memory of timing engine
unsigned int LoadRAMStartTime(unsigned char chnl);		//Load and return start time of a certain channel
unsigned int LoadRAMEndTime(unsigned char chnl);		//Load and return end time of a certain channel
unsigned int LoadCounter0Init();						//Load and return current system counter's value
unsigned int LoadCounter1Init();						//Load and return current capillary counter's value


void WriteRAMRepRate(unsigned int data);						//Write new Repetition Rate to the main memory of timing engine
void WriteRAMShots(unsigned int data);							//Write new Number of Shots to the main memory of timing engine
void WriteRAMStartTime(unsigned char chnl, unsigned int data);	//Write new start time of a certain channel to the main memory of timing engine
void WriteRAMEndTime(unsigned char chnl, unsigned int data);	//Write new end time of a certain channel to the main memory of timing engine

void reloadRegisters();											//Refresh all timing engine registers

#endif /* LOADWRITEDATA_H_ */
