/*
 * IO.cpp
 *
 * Created: 15/05/2013 17:20:18
 *  Author: nick
 */ 

#include "Main.h"
#include "IO.h"

void IO_Init(void)
{
	//Set DDR Registers
	DDRA = 0b00000000;
	DDRB = 0b11110001; //SPI not yet incorporated
	DDRC = 0b00000000;
	DDRD = 0x00;
	DDRE = 0b11111100;
	DDRF = 0b00000000;
	DDRG = 0b00010000;
	
	PORTA = 0xF0; //Pullups enabled (DIP Switch)
	PORTC = 0xFF; //Pullups enabled (DIP Switch)
	
	PORTE &= ~(1 << PE2); //Set RS485 Tranceiver to receive
	//PORTD = 0xFF; //Pullups on USART01
	//PORTE = 0xC0; //Pullups on USART0

}

void IO_LED(uint8_t LED, bool status)
{
	if(status)
	PORTE |= (1 << LED);
	else
	PORTE &= ~(1 << LED);
}

void IO_PWM7(bool status)
{
	if(status)
	PORTG |= (1 << PG4);
	else
	PORTG &= ~(1 << PG4);
}