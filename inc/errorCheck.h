/*
 * errorCheck.h
 *
 *  Created on: Jan 5, 2015
 *      Author: Ce Guo
 */

#ifndef ERRORCHECK_H_
#define ERRORCHECK_H_

unsigned char chkSingleTiming(unsigned char chnl, unsigned char SorE, unsigned int data);
unsigned char chkEditedParameter(unsigned char chnl, unsigned char SorE, char* data);
unsigned char chkAllTiming();
void refreshErrFlags();
unsigned char getErrFlag(unsigned char chnl, unsigned char SorE);
unsigned char chkLeftTiming(char chnl);
unsigned char chkRightTiming(char chnl);

#endif /* ERRORCHECK_H_ */
