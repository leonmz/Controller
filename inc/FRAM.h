/*
 * FRAM.h
 *
 *  Created on: Jan 26, 2015
 *      Author: Ce Guo
 */

#ifndef FRAM_H_
#define FRAM_H_

#define FRAM_MOSI	*(char*)FRAM_MOSI_BASE
#define FRAM_MISO	*(char*)FRAM_MISO_BASE
#define FRAM_SCLK	*(char*)FRAM_SCLK_BASE
#define FRAM_CS_n	*(char*)FRAM_CS_N_BASE


//FM25CL64B Opcodes
#define FRAM_OP_WREN	0x06
#define FRAM_OP_WRDI	0x04
#define FRAM_OP_RDSR	0x05
#define FRAM_OP_WRSR	0x01
#define FRAM_OP_READ	0x03
#define FRAM_OP_WRITE	0x02

void FRAM_Write32Bits(unsigned short addr, unsigned int data);
unsigned int FRAM_Read32Bits(unsigned short addr);


#endif /* FRAM_H_ */
