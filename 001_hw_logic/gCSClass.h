/*
 * gCSClass.h
 *
 *  Created on: 30-Aug-2024
 *      Author: sadash
 */

#ifndef GCSCLASS_H_
#define GCSCLASS_H_

/*
 * Clock System (CS) Configuration
 * 1. Main Clock (MCLK)         <- Processor frequency - 24MHz
 * 2. Subsidiary Clock (SMCLK)  <- Medium frequency preferred for Peripherals
 * 3. Auxiliary Clock (ACLK)    <- Low clock frequency rates supported - 32KHz
 *
 * Pre-scaler - Clock Divider circuit after CS Configuration
 *
 */

class gCSClass
{
public:
    gCSClass();
    virtual ~gCSClass();
};

#endif /* GCSCLASS_H_ */
