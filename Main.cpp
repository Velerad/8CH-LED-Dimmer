/*
 * Led_Dimmer.cpp
 *
 * Created: 22/12/2014 9:43:33
 *  Author: nick
 */ 

#include "Main.h"
#include "IO.h"
#include "ADC.h"
#include "PWM.h"
#include "UART.h"

extern uint8_t PWM[8];
uint8_t PWM_IN[8];
uint8_t PWM_FADE[8];
uint16_t DMX_Address;
uint8_t Analog_I, Serial_DMX_I, Serial_Command_I = 0;
Mode Modes;

uint8_t brightness_curve[256] = {
	0,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
	1,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
	2,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   4,   4,
	4,   4,   4,   5,   5,   5,   5,   6,   6,   6,   6,   7,   7,   7,   7,   8,
	8,   8,   9,   9,   9,  10,  10,  10,  11,  11,  12,  12,  12,  13,  13,  14,
	14,  15,  15,  15,  16,  16,  17,  17,  18,  18,  19,  19,  20,  20,  21,  22,
	22,  23,  23,  24,  25,  25,  26,  26,  27,  28,  28,  29,  30,  30,  31,  32,
	33,  33,  34,  35,  36,  36,  37,  38,  39,  40,  40,  41,  42,  43,  44,  45,
	46,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,
	61,  62,  63,  64,  65,  67,  68,  69,  70,  71,  72,  73,  75,  76,  77,  78,
	80,  81,  82,  83,  85,  86,  87,  89,  90,  91,  93,  94,  95,  97,  98,  99,
	101, 102, 104, 105, 107, 108, 110, 111, 113, 114, 116, 117, 119, 121, 122, 124,
	125, 127, 129, 130, 132, 134, 135, 137, 139, 141, 142, 144, 146, 148, 150, 151,
	153, 155, 157, 159, 161, 163, 165, 166, 168, 170, 172, 174, 176, 178, 180, 182,
	184, 186, 189, 191, 193, 195, 197, 199, 201, 204, 206, 208, 210, 212, 215, 217,
	219, 221, 224, 226, 228, 231, 233, 235, 238, 240, 243, 245, 248, 250, 253, 255
};

int main(void)
{	
	//Initialization
	IO_Init();
	_delay_ms(10);
	PWM_Init();
	//Enable interrupts
	sei();
	
    while(1)
    {
		Init_Selection();
		if(Modes.Fading)
		{
			PWM[0] = brightness_curve[PWM_FADE[0]];
			PWM[1] = brightness_curve[PWM_FADE[1]];
			PWM[2] = brightness_curve[PWM_FADE[2]];
			PWM[3] = brightness_curve[PWM_FADE[3]];
			PWM[4] = brightness_curve[PWM_FADE[4]];
			PWM[5] = brightness_curve[PWM_FADE[5]];
			PWM[6] = brightness_curve[PWM_FADE[6]];
			PWM[7] = brightness_curve[PWM_FADE[7]];	
		}
		else
		{
			PWM[0] = brightness_curve[PWM_IN[0]];
			PWM[1] = brightness_curve[PWM_IN[1]];
			PWM[2] = brightness_curve[PWM_IN[2]];
			PWM[3] = brightness_curve[PWM_IN[3]];
			PWM[4] = brightness_curve[PWM_IN[4]];
			PWM[5] = brightness_curve[PWM_IN[5]];
			PWM[6] = brightness_curve[PWM_IN[6]];
			PWM[7] = brightness_curve[PWM_IN[7]];
		}
    }
}

void DIP_Switch_Readout(void)
{
	DMX_Address = ((~PINA & 0x80) << 1) + (~PINC & 0xFF);
	
	if((~PINA & 0x40) == 0x40) //SET_0
	{
		Modes.Analog = 1;
		Modes.Serial = 0;
	}
	else
	{
		Modes.Analog = 0;
		Modes.Serial = 1;
	}
	
	if((~PINA & 0x20) == 0x20) //SET_1
	{
		Modes.Serial_DMX = 1;
		Modes.Serial_Command = 0;
	}
	else
	{
		Modes.Serial_DMX = 0;
		Modes.Serial_Command = 1;
	}
	
	if((~PINA & 0x10) == 0x10) //SET_2
	{
		Modes.Fading = 1;
	}
	else
	{
		Modes.Fading = 0;
	}
}

void Init_Selection(void)
{
	DIP_Switch_Readout();
	if(Modes.Analog && Analog_I == 0)
	{
		PWM_Zero();
		USART0_Disable();
		ADC_Init();
		Analog_I = 1;
		Serial_DMX_I = 0;
		Serial_Command_I = 0;
	}
	else if(Modes.Serial)
	{
		if(Modes.Serial_DMX && Serial_DMX_I == 0)
		{
			PWM_Zero();
			DMX_Init();
			Analog_I = 0;
			Serial_DMX_I = 1;
			Serial_Command_I = 0;
		}
		else if(Modes.Serial_Command && Serial_Command_I == 0)
		{
			PWM_Zero();
			USART0_Init();
			Analog_I = 0;
			Serial_DMX_I = 0;
			Serial_Command_I = 1;
		}
	}
}

void PWM_Zero(void)
{
	PWM_IN[0] = 0;
	PWM_IN[1] = 0;
	PWM_IN[2] = 0;
	PWM_IN[3] = 0;
	PWM_IN[4] = 0;
	PWM_IN[5] = 0;
	PWM_IN[6] = 0;
	PWM_IN[7] = 0;
}