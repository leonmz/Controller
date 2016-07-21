/*
 * KPLED.c
 *
 *  Created on: Jan 14, 2015
 *      Author: Ce Guo
 *
 *  Each key has two setting bits.
 *  For all the other buttons except FIRE,
 *  	00		Off
 *  	01		Dim
 *  	10		Bright
 *  	11		Very Bright
 *
 *  For FIRE Button,
 *  	00		Off
 *  	01		Bright***
 *  	10		Dim***
 *  	11		Flash once during each shot
 *
 *	***CAUTION:
 *		The reason that the FIRE button has different bright and dim configuration is because a resistor is added on the controller mother board.
 *		Without the additional resistor, the FIRE button was just too dim due to the nature of purple LED.
 *		Next time, when build a new one, please use a different type of LED, and then there is no need for that extra resistor.
 */

/*
	No.			Name			Key			GPIO	PIN
	0			0-9_D			-			128		M10
	1			0-9_B			-			129		L13
	2			R1C4A			->D			118		R16
	3			R1C4B			->B			120		P15
	4			R2C4A			<-D			122		R14
	5			R2C4B			<-B			124		N15
	6			R1C5A			MENU D		121		P16
	7			R1C5B			MENU B		123		N16
	8			R2C5A			EDIT D		119		L15
	9			R2C5B			EDIT B		116		L16
	10			R3C5A			ENT D		117		K16
	11			R3C5B			ENT B		114		N9
	12			R2C6A			ENA D		111		R10
	13			R2C6B			ENA B		112		N12
	14			R3C6A			FIRE D		115		N11
	15			R3C6B			FIRE B		113		P9
*/

#include "system.h"
#include "..\inc\delay.h"
#include "..\inc\config.h"
#include "..\inc\KPLED.h"

#define KPLED_Status	*(unsigned short*)KPLED_STATUS_BASE
#define KPLED_Load	*(unsigned char*)KPLED_LOAD_BASE //Load status on rising edge

static unsigned short KPLED_Status_Buff;		//Stores current status.



unsigned short KPLED_getStatus()
{
	return KPLED_Status_Buff;
}

void KPLED_setStatus(unsigned short status)
{
	KPLED_Status_Buff = status;
	KPLED_Status = KPLED_Status_Buff;
	KPLED_Load = 0;
	KPLED_Load = 1;
	KPLED_Load = 0;
}

void KPLED_All_Bright()
{
	KPLED_Status_Buff = 0x7fff;
	KPLED_Status = 0x7fff;
	KPLED_Load = 0;
	KPLED_Load = 1;
	KPLED_Load = 0;
}
void KPLED_All_Dim()
{
	KPLED_Status_Buff = 0x9555;
	KPLED_Status = 0x9555;
	KPLED_Load = 0;
	KPLED_Load = 1;
	KPLED_Load = 0;
}
void KPLED_All_Off()
{
	KPLED_Status_Buff = 0xc000;
	KPLED_Status = 0xc000;
	KPLED_Load = 0;
	KPLED_Load = 1;
	KPLED_Load = 0;
}

void KPLED_Numbers(unsigned short b)
{
	KPLED_Status_Buff = KPLED_Status_Buff & 0xfffc;
	KPLED_Status_Buff = KPLED_Status_Buff | (b & 0x0003);
	KPLED_Status = KPLED_Status_Buff;
	KPLED_Load = 0;
	KPLED_Load = 1;
	KPLED_Load = 0;
}

void KPLED_RightArrow(unsigned short b)
{
	KPLED_Status_Buff = KPLED_Status_Buff & 0xfff3;
	KPLED_Status_Buff = KPLED_Status_Buff | ((b & 0x0003)<<2);
	KPLED_Status = KPLED_Status_Buff;
	KPLED_Load = 0;
	KPLED_Load = 1;
	KPLED_Load = 0;
}

void KPLED_LeftArrow(unsigned short b)
{
	KPLED_Status_Buff = KPLED_Status_Buff & 0xffcf;
	KPLED_Status_Buff = KPLED_Status_Buff | ((b & 0x0003)<<4);
	KPLED_Status = KPLED_Status_Buff;
	KPLED_Load = 0;
	KPLED_Load = 1;
	KPLED_Load = 0;
}

void KPLED_Menu(unsigned short b)
{
	KPLED_Status_Buff = KPLED_Status_Buff & 0xff3f;
	KPLED_Status_Buff = KPLED_Status_Buff | ((b & 0x0003)<<6);
	KPLED_Status = KPLED_Status_Buff;
	KPLED_Load = 0;
	KPLED_Load = 1;
	KPLED_Load = 0;
}

void KPLED_Edit(unsigned short b)
{
	KPLED_Status_Buff = KPLED_Status_Buff & 0xfcff;
	KPLED_Status_Buff = KPLED_Status_Buff | ((b & 0x0003)<<8);
	KPLED_Status = KPLED_Status_Buff;
	KPLED_Load = 0;
	KPLED_Load = 1;
	KPLED_Load = 0;
}

void KPLED_Enter(unsigned short b)
{
	KPLED_Status_Buff = KPLED_Status_Buff & 0xf3ff;
	KPLED_Status_Buff = KPLED_Status_Buff | ((b & 0x0003)<<10);
	KPLED_Status = KPLED_Status_Buff;
	KPLED_Load = 0;
	KPLED_Load = 1;
	KPLED_Load = 0;
}

void KPLED_Enable(unsigned short b)
{
	KPLED_Status_Buff = KPLED_Status_Buff & 0xcfff;
	KPLED_Status_Buff = KPLED_Status_Buff | ((b & 0x0003)<<12);
	KPLED_Status = KPLED_Status_Buff;
	KPLED_Load = 0;
	KPLED_Load = 1;
	KPLED_Load = 0;
}

void KPLED_Fire(unsigned short b)
{
	KPLED_Status_Buff = KPLED_Status_Buff & 0x3fff;
	KPLED_Status_Buff = KPLED_Status_Buff | ((b & 0x0003)<<14);
	KPLED_Status = KPLED_Status_Buff;
	KPLED_Load = 0;
	KPLED_Load = 1;
	KPLED_Load = 0;
}

