/*
 * PWM.cpp
 *
 * Created: 22/12/2014 12:05:04
 *  Author: nick
 */ 

#include "Main.h"
#include "PWM.h"
#include "IO.h"

extern uint8_t PWM_IN[8];
extern uint8_t PWM_FADE[8];
uint8_t PWM[8];
volatile uint8_t CNT[8];

void PWM_Init(void)
{
	////Timer0 Setup
	//
	//TCCR0 = (0 << FOC0)|(1 << WGM00)|(1 << COM01)|(0 << COM00)|(1 << WGM01)|(1 << CS02)|(1 << CS01)|(0 << CS00); //Phase correct PWM Mode, Non-inverting, CLK/256
	
	//Timer2 Setup (for software PWM)
	TCCR2 = (0 << FOC2)|(0 << WGM20)|(0 << COM21)|(0 << COM20)|(1 << WGM21)|(0 << CS22)|(1 << CS21)|(0 << CS20); //Phase correct PWM Mode, Non-inverting, CLK/64
	OCR2 = 30; //77 gives 100Hz
	
	//TCCR2 = (0 << FOC2)|(1 << WGM20)|(0 << COM21)|(0 << COM20)|(1 << WGM21)|(1 << CS22)|(0 << CS21)|(0 << CS20); //Phase correct PWM Mode, Non-inverting, CLK/64
	//TIMSK = (1 << OCIE2)|(1 << TOIE2);
	
	////Timer1 Setup
	//TCCR1A = (1 << COM1A1)|(0 << COM1A0)|(1 << COM1B1)|(0 << COM1B0)|(1 << COM1C1)|(0 << COM1C0)|(0 << WGM11)|(1 << WGM10); //Phase correct PWM Mode, Non-inverting, CLK/256
	//TCCR1B = (0 << ICNC1)|(0 << ICES1)|(0 << WGM13)|(1 << WGM12)|(1 << CS12)|(0 << CS11)|(0 << CS10);
	TCCR1A = (0 << COM1A1)|(0 << COM1A0)|(0 << COM1B1)|(0 << COM1B0)|(0 << COM1C1)|(0 << COM1C0)|(0 << WGM11)|(0 << WGM10); //Phase correct PWM Mode, Non-inverting, CLK/256
	TCCR1B = (0 << ICNC1)|(0 << ICES1)|(0 << WGM13)|(1 << WGM12)|(1 << CS12)|(0 << CS11)|(1 << CS10);
	OCR1A = 31; 
	//
	////Timer3 Setup
	//TCCR3A = (1 << COM3A1)|(0 << COM3A0)|(1 << COM3B1)|(0 << COM3B0)|(1 << COM3C1)|(0 << COM3C0)|(0 << WGM31)|(1 << WGM30); //Phase correct PWM Mode, Non-inverting, CLK/256
	//TCCR3B = (0 << ICNC3)|(0 << ICES3)|(0 << WGM33)|(1 << WGM32)|(1 << CS32)|(0 << CS31)|(0 << CS30);
	
	CNT[0] = 0;
	CNT[1] = 32;
	CNT[2] = 64;
	CNT[3] = 96;
	CNT[4] = 128;
	CNT[5] = 160;
	CNT[6] = 192;
	CNT[7] = 224;
	
	TIMSK = (1 << OCIE0)|(0 << TOIE0)|(1 << OCIE2)|(0 << TOIE2)|(1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect)
{
	for(uint8_t i = 0; i <= 7; i++)
	{
		if(PWM_IN[i] > PWM_FADE[i])
		PWM_FADE[i]++;
		if(PWM_IN[i] < PWM_FADE[i])
		PWM_FADE[i]--;
	}
}

ISR(TIMER2_COMP_vect)
{
	//PORTE |= (1 << PE6);
	
	if(PWM[0] != 0 && PWM[0] >= CNT[0])
		PORTB |= (1 << PB7);
	else
		PORTB &= ~(1 << PB7);
		
	if(PWM[1] != 0 && PWM[1] >= CNT[1])
		PORTB |= (1 << PB6);
	else
		PORTB &= ~(1 << PB6);
	
	if(PWM[2] != 0 && PWM[2] >= CNT[2])
		PORTB |= (1 << PB5);
	else
		PORTB &= ~(1 << PB5);
	
	if(PWM[3] != 0 && PWM[3] >= CNT[3])
		PORTB |= (1 << PB4);
	else
		PORTB &= ~(1 << PB4);
	
	if(PWM[4] != 0 && PWM[4] >= CNT[4])
		PORTE |= (1 << PE5);
	else
		PORTE &= ~(1 << PE5);
	
	if(PWM[5] != 0 && PWM[5] >= CNT[5])
		PORTE |= (1 << PE4);
	else
		PORTE &= ~(1 << PE4);
	
	if(PWM[6] != 0 && PWM[6] >= CNT[6])
		PORTE |= (1 << PE3);
	else
		PORTE &= ~(1 << PE3);
		
	if(PWM[7] != 0 && PWM[7] >= CNT[7])
		PORTG |= (1 << PG4);
	else
		PORTG &= ~(1 << PG4);
		
	CNT[0]++;
	CNT[1]++;
	CNT[2]++;
	CNT[3]++;
	CNT[4]++;
	CNT[5]++;
	CNT[6]++;
	CNT[7]++;
	
	//PORTE &= ~(1 << PE6);
}