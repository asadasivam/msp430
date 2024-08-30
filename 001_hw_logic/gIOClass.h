/*
 * IODriver.h
 *
 *  Created on: 30-Aug-2024
 *      Author: sadash
 */

#ifndef GIOCLASS_H_
#define GIOCLASS_H_

typedef enum errors{

}gpioErrors;



class gIOClass
{
public:
    gIOClass();
    virtual ~gIOClass();
    bool SchmiddTrgger_Configure();
    bool Comparator_Configure();
    gpioErrors gpio_Configure();
};

#endif /* GIOCLASS_H_ */
