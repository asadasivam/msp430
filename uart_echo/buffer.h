/*
 * buffer.h
 *
 *  Created on: Mar 21, 2021
 *      Author: sadasivam
 */

#ifndef BUFFER_H_
#define BUFFER_H_

#define buffer_empty    0
#define buffer_full     1

extern int bufferTop;

void pushBuffer(int);
void popBuffer(void);
int getBufferState(void);
int getBufferVal(int bufferTop);

#endif /* BUFFER_H_ */
