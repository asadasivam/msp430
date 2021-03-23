/*
 * volatile_mem.h
 *
 *  Created on: Mar 20, 2021
 *      Author: sadasivam
 */

#ifndef VOLATILE_MEM_H_
#define VOLATILE_MEM_H_

#include <msp430fr5969.h>
//port reading
//#define uint16_t unsigned int
//#define baseAddress __MSP430_BASEADDRESS_PORT1_R__
#define PORT1_IN (*(volatile unsigned int*)0x0200)
//#define PORT1_IN    (*((volatile uint16_t *)((uint16_t)baseAddress + (0x0000))))

//macros
#define RAMbase     (*(unsigned char*)0x1C00) //accessing RAM location
#define FRAMbase    (*(unsigned char*)0x4400) //accessing FRAM memory
#define RESETbase   (*(unsigned char*)0xFFFE) //accessing RESET vector

//globals
//extern unsigned int arr[100];
//extern unsigned char i;

void volatile_memCheck(void);


#endif /* VOLATILE_MEM_H_ */
