/*
*
*   adc.h
*/

#ifndef ADC_H_
#define ADC_H_

#include <inc/tm4c123gh6pm.h>
#include <stdint.h>

void ADC_init(void);
unsigned ADC_read(char channel);

#endif
