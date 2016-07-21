/*
 * FRAM.c
 *
 *  Created on: Jan 26, 2015
 *      Author: Ce Guo
 */

#include <system.h>
#include "..\inc\config.h"

#include "..\inc\FRAM.h"
#include "..\inc\Counter.h"
#include <stdio.h>


void FRAM_SendByte(unsigned char data)
{
	//Bit 7
	FRAM_MOSI = (data & 0x80)?1:0;
	FRAM_SCLK = 1;
	data = (data<<1);
	FRAM_SCLK = 0;

	//Bit 6
	FRAM_MOSI = (data & 0x80)?1:0;
	FRAM_SCLK = 1;
	data = (data<<1);
	FRAM_SCLK = 0;

	//Bit 5
	FRAM_MOSI = (data & 0x80)?1:0;
	FRAM_SCLK = 1;
	data = (data<<1);
	FRAM_SCLK = 0;

	//Bit 4
	FRAM_MOSI = (data & 0x80)?1:0;
	FRAM_SCLK = 1;
	data = (data<<1);
	FRAM_SCLK = 0;

	//Bit 3
	FRAM_MOSI = (data & 0x80)?1:0;
	FRAM_SCLK = 1;
	data = (data<<1);
	FRAM_SCLK = 0;

	//Bit 2
	FRAM_MOSI = (data & 0x80)?1:0;
	FRAM_SCLK = 1;
	data = (data<<1);
	FRAM_SCLK = 0;

	//Bit 1
	FRAM_MOSI = (data & 0x80)?1:0;
	FRAM_SCLK = 1;
	data = (data<<1);
	FRAM_SCLK = 0;

	//Bit 0
	FRAM_MOSI = (data & 0x80)?1:0;
	FRAM_SCLK = 1;
	FRAM_SCLK = 0;
}

unsigned char FRAM_ReadByte()
{
	char data = 0x00;
	FRAM_SCLK = 0;

	//Bit 7
	data = data | FRAM_MISO;
	FRAM_SCLK = 1;
	data = (data<<1);
	FRAM_SCLK = 0;

	//Bit 6
	data = data | FRAM_MISO;
	FRAM_SCLK = 1;
	data = (data<<1);
	FRAM_SCLK = 0;

	//Bit 5
	data = data | FRAM_MISO;
	FRAM_SCLK = 1;
	data = (data<<1);
	FRAM_SCLK = 0;

	//Bit 4
	data = data | FRAM_MISO;
	FRAM_SCLK = 1;
	data = (data<<1);
	FRAM_SCLK = 0;

	//Bit 3
	data = data | FRAM_MISO;
	FRAM_SCLK = 1;
	data = (data<<1);
	FRAM_SCLK = 0;

	//Bit 2
	data = data | FRAM_MISO;
	FRAM_SCLK = 1;
	data = (data<<1);
	FRAM_SCLK = 0;

	//Bit 1
	data = data | FRAM_MISO;
	FRAM_SCLK = 1;
	data = (data<<1);
	FRAM_SCLK = 0;

	//Bit 0
	data = data | FRAM_MISO;
	FRAM_SCLK = 1;
	FRAM_SCLK = 0;
	return data;
}


void FRAM_Write32Bits(unsigned short addr, unsigned int data)
{
	unsigned short addr_new = addr * 4;

	unsigned char addr0 = addr_new & 0xFF;
	unsigned char addr1 = addr_new>>8 & 0xFF;

	unsigned char byte0 = data & 0xFF;
	unsigned char byte1 = data>>8 & 0xFF;
	unsigned char byte2 = data>>16 & 0xFF;
	unsigned char byte3 = data>>24 & 0xFF;

	FRAM_CS_n = 1;
	FRAM_SCLK = 0;
	FRAM_CS_n = 0;
	FRAM_SendByte(FRAM_OP_WREN);
	FRAM_CS_n = 1;

	FRAM_CS_n = 0;
	FRAM_SendByte(FRAM_OP_WRITE);
	FRAM_SendByte(addr1);
	FRAM_SendByte(addr0);
	FRAM_SendByte(byte3);
	FRAM_SendByte(byte2);
	FRAM_SendByte(byte1);
	FRAM_SendByte(byte0);
	FRAM_CS_n = 1;
}

unsigned int FRAM_Read32Bits(unsigned short addr)
{
	unsigned short addr_new = addr * 4;

	unsigned char addr0 = addr_new & 0xFF;
	unsigned char addr1 = addr_new>>8 & 0xFF;

	FRAM_CS_n = 1;
	FRAM_SCLK = 0;
	FRAM_CS_n = 0;
	FRAM_SendByte(FRAM_OP_READ);
	FRAM_SendByte(addr1);
	FRAM_SendByte(addr0);

	unsigned char input3 = FRAM_ReadByte();
	unsigned char input2 = FRAM_ReadByte();
	unsigned char input1 = FRAM_ReadByte();
	unsigned char input0 = FRAM_ReadByte();


	unsigned int byte0 = input0;
	unsigned int byte1 = input1 << 8;
	unsigned int byte2 = input2 << 16;
	unsigned int byte3 = input3 << 24;

	unsigned int data = byte0 + byte1 + byte2 + byte3;
	return data;
}



void FRAM_test()
{
	FRAM_Write32Bits(COUNTER_1_ADDR,218999);
	printf("%d\n",FRAM_Read32Bits(COUNTER_1_ADDR));
	FRAM_Write32Bits(COUNTER_0_ADDR,218999);
	printf("%d\n",FRAM_Read32Bits(COUNTER_0_ADDR));

}


