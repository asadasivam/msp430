/*
 * swDebug.h
 *
 *  Created on: 30-Aug-2024
 *      Author: sadash
 */

#ifndef SWDEBUG_H_
#define SWDEBUG_H_

#include <gIOClass.h>
#include "gPreScaler.h"
#include "gIOClass.h"

typedef enum setDirection{
    input   = 0,
    output  = 1
}setDIR;

class swDebug
{
public:
    swDebug();
    virtual ~swDebug();
    GPIO_Direction(gPreScaler setPrescaler, setDIR outDIR);
};

#endif /* SWDEBUG_H_ */
