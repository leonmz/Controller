/*
 * comInterface.c
 *
 *  Created on: Nov 4, 2014
 *      Author: Ce Guo
 */


/*
 * See header file for description of each function.
 */

#include "..\inc\sopc.h"
#include "..\inc\uart.h"
#include "..\inc\delay.h"

void UartInit()
{
	uart.init();
}

void WriteRAM32Bits(unsigned char address, unsigned int data)
{

	unsigned char byte0 = data & 0xFF;
	unsigned char byte1 = data>>8 & 0xFF;
	unsigned char byte2 = data>>16 & 0xFF;
	unsigned char byte3 = data>>24 & 0xFF;

	/*Message Format*/
	/*
	[0] 0x02   			//Start
	[1] 'W'				//Instruction
	[2] Address			//Multiply word length of 4
	[3] x				//Unused
	[4] Byte3			//Most significant byte of data
	[5] Byte2
	[6] Byte1
	[7] Byte0			//Least significant byte of data
	[8] x				//Unused
	[9] 0x04			//End of transmission
	[10] 0x04
	*/

	unsigned char* message = (unsigned char*)"\x02Waxddddx\x04\x04";

	message[2] = address*4;
	message[4] = byte3;
	message[5] = byte2;
	message[6] = byte1;
	message[7] = byte0;

	uart.send_string(11,message);

}




unsigned int ReadRAM32Bits(unsigned int address)
{
	/*Message Format*/
	/*
	[0] 0x02   			//Start
	[1] 'R'				//Instruction
	[2] Address			//Multiply word length of 4
	[3] x				//Unused
	[4] x
	[5] x
	[6] x
	[7] x
	[8] x				//Unused
	[9] 0x04			//End of transmission
	[10] 0x04
	*/

	unsigned char* message = (unsigned char*)"\x02Raxxxxxx\x04\x04";
	unsigned char* input = (unsigned char*)"*******";
	message[2] = address*4;

	uart.receive_count = 0;
	uart.receive_flag = 0;

	uart.send_string(11,message);

	while(! uart.receive_flag);

	input[0] = uart.receive_buffer[0];
	input[1] = uart.receive_buffer[1];
	input[2] = uart.receive_buffer[2];
	input[3] = uart.receive_buffer[3];
	input[4] = uart.receive_buffer[4];
	input[5] = uart.receive_buffer[5];
	input[6] = uart.receive_buffer[6];
    //printf("%s",input);
	uart.receive_flag = 0;

	unsigned int byte0 = input[2];
	unsigned int byte1 = input[3] << 8;
	unsigned int byte2 = input[4] << 16;
	unsigned int byte3 = input[5] << 24;

	unsigned int data = byte0 + byte1 + byte2 + byte3;
	return data;
}


/*
def WriteEEPROM8Bits(port_name,address,message):
	char1=int('{0:08b}'.format(message),2)
	ser=serial.Serial(port_name)
	ser.write(chr(2)+'w'+chr(address)+'x'+chr(char1)+'xxxx'+chr(4)+chr(4))
	ser.close()
	time.sleep(0.1)
	return
 */

void WriteEEPROM8Bits(unsigned char address, unsigned char data)
{
	//unsigned char byte0 = data & 0xFF;

	unsigned char* message = (unsigned char*)"\x02waxdxxxx\x04\x04";

	message[2] = address;
	message[4] = data;

	uart.send_string(11,message);
	//delay(1000);
}



/*
 def WriteEEPROM32Bits(port_name,address,message):
	address_new=address*4
	char1=int('{0:032b}'.format(message)[0:8],2)
	char2=int('{0:032b}'.format(message)[8:16],2)
	char3=int('{0:032b}'.format(message)[16:24],2)
	char4=int('{0:032b}'.format(message)[24:32],2)
	WriteEEPROM8Bits(port_name,address,char1)
	WriteEEPROM8Bits(port_name,address+1,char2)
	WriteEEPROM8Bits(port_name,address+2,char3)
	WriteEEPROM8Bits(port_name,address+3,char4)
	return
 */
void WriteEEPROM32Bits(unsigned char address, unsigned int data)
{
	unsigned char address_new = address*4;

	unsigned char byte0 = data & 0xFF;
	unsigned char byte1 = data>>8 & 0xFF;
	unsigned char byte2 = data>>16 & 0xFF;
	unsigned char byte3 = data>>24 & 0xFF;

	WriteEEPROM8Bits(address_new,byte3);
	WriteEEPROM8Bits(address_new+1,byte2);
	WriteEEPROM8Bits(address_new+2,byte1);
	WriteEEPROM8Bits(address_new+3,byte0);

}

void Fire()
{
	unsigned char* message = (unsigned char*)"\x02\x46xxxxxxx\x04\x04";

	uart.send_string(11,message);
}

void Stop()
{
	unsigned char* message = (unsigned char*)"\x02Sxxxxxxx\x04\x04";
	uart.send_string(11,message);
}

void ResetRAMFromEEPROM()
{
	unsigned char* message = (unsigned char*)"\x02\x45xxxxxxx\x04\x04";
	uart.send_string(11,message);
	delay(40000);
}

