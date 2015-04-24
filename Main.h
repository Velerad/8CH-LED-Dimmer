/*
 * Main.h
 *
 * Created: 15/05/2013 16:13:43
 *  Author: nick
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/wdt.h> 

struct Mode
{
uint8_t Analog, Serial, Serial_DMX, Serial_Command, Fading, Linear;
};

void DIP_Switch_Readout(void);
void Init_Selection(void);
void PWM_Zero(void);
void Fast_PWM(uint8_t channel, uint8_t PWM);


#endif /* MAIN_H_ */