/*
 * buffer.c
 *
 *  Created on: Mar 21, 2021
 *      Author: sadasivam
 */


#include "buffer.h"


int buffer[100] = {0}; //empty buffer
int bufferTop = -1;

void pushBuffer(int getChar)
{
        bufferTop++;
        buffer[bufferTop] = getChar;
}
void popBuffer()
{
    bufferTop--;
}

int getBufferState()
{
    if(bufferTop == -1)
    {
        return buffer_empty;
    }
    else
    {
        return buffer_full;
    }
}
int getBufferVal(int bufferTop)
{
    return buffer[bufferTop];
}

