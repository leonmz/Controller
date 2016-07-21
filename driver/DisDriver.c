
#include "system.h"
#include "..\inc\keyVal.h"
#include "..\inc\config.h"
#include "..\inc\delay.h"
#include <stdio.h>

#define DIS_RS *(unsigned char*)DIS_RS_BASE
#define DIS_RW *(unsigned char*)DIS_RW_BASE
#define DIS_EN *(unsigned char*)DIS_EN_BASE
#define DIS_DATA *(unsigned char*)DIS_DATA_BASE
#define DIS_BUFF_EN_n *(unsigned char*)DISBUFFEO_N_BASE
#define leds  *(unsigned char*)LED_BASE


/*
 * For the data writing, instruction writing, and initialization procedures,
 * please refer to the data sheet of the VFD (CU20049-UW2J)
 */

void DisWrData(unsigned char data)
{
	DIS_EN = 0;
	DIS_RS = 1;
	DIS_RW = 0;
	DIS_DATA = data;
	DIS_EN = 1;
	DIS_EN = 0;
}

void DisWrInst(unsigned char inst)
{
	DIS_EN = 0x00;
	DIS_RS = 0x00;
	DIS_RW = 0x00;
	DIS_DATA = inst;
	DIS_EN = 0x11;
	DIS_EN = 0;
}
void DisInit()
{
	DIS_BUFF_EN_n = 0;		//Set the translating transceiver (U42 74LVX4245) to output mode

	delay(1000);
	DisWrInst(0x38);
	DisWrData(VFD_BRT);		//Set Brightness
	DisWrInst(0x08);

	DisWrInst(0x01);
	DisWrInst(0x06);
	DisWrInst(0x02);
	DisWrInst(0x0c);
}

/*
 * Turn on or off cursor
 * 		0: On
 * 		1: Off
 */
void DisShowCur(int b)
{
	if(b)
		DisWrInst(0x0f);	//Show a blinking cursor
	else
		DisWrInst(0x0c);	//Turn off cursor
}


/*
 * Locate the cursor to position (row, col)
 */
void DisCurPos(unsigned char row,unsigned char col)
{
	unsigned char addr = 0;
	switch(row)
	{
		case 1: addr = 0x00; break;
		case 2: addr = 0x40; break;
		case 3: addr = 0x14; break;
		case 4: addr = 0x54; break;
	}

	addr += col-1;
	DisWrInst(0x80+addr);
}

/*
 * Display a string at current position.
 */
void DisWrStr(char* s)
{
	unsigned short i = 0;
	while(s[i]!=0x00)
	{
		DisWrData(s[i]);
		i++;
	}
}

/*
 * Clear the entire display.
 */
void DisClear()
{
	DisWrInst(0x01);
	delay(0);
}

/*
 * Clear a certain row.
 */
void DisClearRow(unsigned char row)
{
	DisCurPos(row,1);
	DisWrStr("                    ");
}


/*
 * Turn off the display.
 */
void DisOff()
{
	DisWrInst(0x08);
}
