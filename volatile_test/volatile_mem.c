/*
 * volatile_mem.c
 *
 *  Created on: Mar 20, 2021
 *      Author: sadasivam
 */
#include "volatile_mem.h"


unsigned int arr[100] = {0};
unsigned char i = 0;

void volatile_memCheck()
{
    //arr[0] = 0x1234;
    //reading 8-bits
    unsigned int rm8_t = RAMbase;
    //reading 16-bits
    unsigned int rm16_t = RAMbase;
    //reading 32-bits
    unsigned int rm32_t = RAMbase;

    for(i = 0; i<100; i++)
    {
        arr[i] = PORT1_IN; //reading input buffer 100 times
    }
}
