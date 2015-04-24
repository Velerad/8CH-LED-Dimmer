/*
 * ADC.cpp
 *
 * Created: 15/05/2013 16:14:54
 *  Author: nick
 */ 

#include "Main.h"

extern uint8_t PWM_IN[8];
uint8_t ADCcounter = 0;

extern Mode Modes;

void ADC_Init(void)
{
	ADMUX = (1<<REFS1)|(1<<REFS0)|(1<<ADLAR)|(0<<MUX4)|(0<<MUX3)|(0<<MUX2)|(0<<MUX1)|(0<<MUX0);
	ADCSRA = (1<<ADEN)|(0<<ADSC)|(0<<ADATE)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	ADCSRB = (0<<ADTS2)|(1<<ADTS1)|(0<<ADTS0);
	
	ADCSRA |= (1<<ADSC); //Start conversion
}

ISR(ADC_vect)
{
	switch(ADCcounter)
	{
		case 0:
			PWM_IN[7-ADCcounter] = ADCH;
			ADCcounter+=1;
			ADMUX = ((1<<REFS1)|(1<<REFS0)|(1<<ADLAR)) + ADCcounter;
			break;
		case 1:
			PWM_IN[7-ADCcounter] = ADCH;
			ADCcounter+=1;
			ADMUX = ((1<<REFS1)|(1<<REFS0)|(1<<ADLAR)) + ADCcounter;
			break;
		case 2:
			PWM_IN[7-ADCcounter] = ADCH;
			ADCcounter+=1;
			ADMUX = ((1<<REFS1)|(1<<REFS0)|(1<<ADLAR)) + ADCcounter;
			break;
		case 3:
			PWM_IN[7-ADCcounter] = ADCH;
			ADCcounter+=1;
			ADMUX = ((1<<REFS1)|(1<<REFS0)|(1<<ADLAR)) + ADCcounter;
			break;
		case 4:
			PWM_IN[7-ADCcounter] = ADCH;
			ADCcounter+=1;
			ADMUX = ((1<<REFS1)|(1<<REFS0)|(1<<ADLAR)) + ADCcounter;
			break;
		case 5:
			PWM_IN[7-ADCcounter] = ADCH;
			ADCcounter+=1;
			ADMUX = ((1<<REFS1)|(1<<REFS0)|(1<<ADLAR)) + ADCcounter;
			break;
		case 6:
			PWM_IN[7-ADCcounter] = ADCH;
			ADCcounter+=1;
			ADMUX = ((1<<REFS1)|(1<<REFS0)|(1<<ADLAR)) + ADCcounter;
			break;
		case 7:
			PWM_IN[7-ADCcounter] = ADCH;
			ADCcounter=0;
			ADMUX = ((1<<REFS1)|(1<<REFS0)|(1<<ADLAR)) + ADCcounter;
			break;
	}
	
	if(Modes.Analog)
	{
		ADCSRA |= (1<<ADSC); //Start conversion
	}
}