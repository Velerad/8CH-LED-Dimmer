/*
 * Uart.cpp
 *
 * Created: 15/05/2013 16:13:55
 *  Author: nick
 */ 

#include "Main.h"
#include "UART.h"

extern Mode Modes;
extern uint8_t PWM_IN[9];
volatile uint8_t cmdh_state = 0;
volatile uint8_t buffer[10];
volatile uint8_t dbg_counter = 0;
bool test;

#define read 0
#define scan 1

#define FRAMING_ERROR0 (1<<FE)
#define PARITY_ERROR0 (1<<UPE)
#define DATA_OVERRUN0 (1<<OVR)
#define DATA_REGISTER_EMPTY0 (1<<UDRE)
#define RX_COMPLETE0 (1<<RXC)

volatile char status,data;

// This flag is set on USART0 Receiver buffer overflow
bool rx_buffer_overflow0;

// USART0 Receiver buffer
char rx_buffer0[RX_BUFFER_SIZE0];
uint8_t rx_wr_index0;
uint8_t rx_rd_index0;
uint8_t rx_counter0;

// USART0 Transmitter buffer
char tx_buffer0[TX_BUFFER_SIZE0];
uint8_t tx_wr_index0;
uint8_t tx_rd_index0;
uint8_t tx_counter0;

//DMX variables
volatile uint8_t DMX_Break = 0;
volatile uint8_t DMX_Start = 0;
volatile uint16_t counter = 0;
extern volatile uint16_t  DMX_Address;
volatile uint8_t dmx_counter;

void USART0_Init(void)
{	
	// Set baud rate
	UBRR0H = (USART0_BAUD_PRESCALE >> 8);
	UBRR0L = USART0_BAUD_PRESCALE;

	// Enable receiver and transmitter
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0)|(1<<TXCIE0);
	// Set frame format: 8data, 2stop bit
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

void USART0_Disable(void)
{
	// Disable receiver and transmitter
	UCSR0B = (0<<RXEN0)|(0<<TXEN0)|(0<<RXCIE0)|(0<<TXCIE0);
}

void DMX_Init(void)
{
	// Set DMX baud rate (250000baud)
	UBRR0H = (DMX_BAUD_PRESCALE >> 8);
	UBRR0L = DMX_BAUD_PRESCALE;
	
	//Enable receiver and interupt
	UCSR0B = (1<<RXEN0)|(0<<TXEN0)|(1<<RXCIE0)|(0<<TXCIE0);
	// Set frame format: 8data, 2stop bit
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);

}

void USART0_Commandhandler(void)
{
	uint8_t checksum = 0x16;
	
	switch (cmdh_state)
	{
		case read:
		if(rx_counter0 >= 10)
		{
			for(int i = 0; i < 10; i++)
			{
				buffer[i] = USART0_GetChar();
				if(i < 9)
				checksum += buffer[i];
			}
			if(checksum == buffer[9])
			{
				if(DMX_Address == buffer[0])
				{
					PWM_IN[0] = buffer[1];
					PWM_IN[1] = buffer[2];
					PWM_IN[2] = buffer[3];
					PWM_IN[3] = buffer[4];
					PWM_IN[4] = buffer[5];
					PWM_IN[5] = buffer[6];
					PWM_IN[6] = buffer[7];
					PWM_IN[7] = buffer[8];
				}
			}
			else
			cmdh_state = scan;
		}
		break;
		
		case scan:
		if(rx_counter0 >= 1)
		{
			for(int i = 0; i < 9; i++)
			{
				buffer[i] = buffer[i+1];
				checksum += buffer[i];
			}
			buffer[9] = USART0_GetChar();
			if(checksum == buffer[9])
			{
				cmdh_state = read;
				if(DMX_Address == buffer[0])
				{
					PWM_IN[0] = buffer[1];
					PWM_IN[1] = buffer[2];
					PWM_IN[2] = buffer[3];
					PWM_IN[3] = buffer[4];
					PWM_IN[4] = buffer[5];
					PWM_IN[5] = buffer[6];
					PWM_IN[6] = buffer[7];
					PWM_IN[7] = buffer[8];
				}
			}
			else
			cmdh_state = scan;
		}
		break;
	}
}

void USART0_PutString(char* buffer)
{
	// start sending characters over the serial port until we reach the end of the string
	while (*buffer != '\0') {	// are we at the end of the string yet?
	USART0_PutChar(*buffer);	// send the contents
	buffer++;				// advance to next char in buffer
	}	
}

char USART0_Flush(void)
{
	char data = 0;
	while ((UCSR0A & (1<<RXC0)) )
	{
		data = UDR0;
	}
	return data;
}

bool USART0_DataReceived(void)
{
	return (rx_counter0 != 0);
}

char USART0_GetChar(void)
{
	char data;
	
	//while (rx_counter0 == 0);                  // TBD : add time-out
	
	data = rx_buffer0[rx_rd_index0];
	if (++rx_rd_index0 == RX_BUFFER_SIZE0)
	rx_rd_index0 = 0;

	cli();
	--rx_counter0;
	sei();

	return data;
}

void USART0_PutChar(char c)
{
	while (tx_counter0 == TX_BUFFER_SIZE0);
	
	cli();
	if (tx_counter0 || ((UCSR0A & DATA_REGISTER_EMPTY0)==0))
	{
		tx_buffer0[tx_wr_index0]=c;
		if (++tx_wr_index0 == TX_BUFFER_SIZE0)
		tx_wr_index0=0;
		++tx_counter0;
	}
	else
	UDR0=c;
	sei();
}

ISR(USART0_TX_vect)
{
	if (tx_counter0)
	{
		--tx_counter0;
		UDR0=tx_buffer0[tx_rd_index0];
		if (++tx_rd_index0 == TX_BUFFER_SIZE0)
		tx_rd_index0=0;
	};
}

ISR(USART0_RX_vect)
{
	PORTE |= (1 << PE7);
	status=UCSR0A;
	data=UDR0;
	
	if(Modes.Serial_Command)
	{
		if ((status & (FRAMING_ERROR0 | PARITY_ERROR0 | DATA_OVERRUN0))==0)
		{
			rx_buffer0[rx_wr_index0]=data;
			if (++rx_wr_index0 == RX_BUFFER_SIZE0)
			rx_wr_index0=0;
			if (++rx_counter0 == RX_BUFFER_SIZE0)
			{
				rx_counter0=0;
				rx_buffer_overflow0=1;
			};
		};
		USART0_Commandhandler();
	}
	else/* if(Modes.Serial_DMX)*/
	{
		
		if (status & FRAMING_ERROR0)	//If we detect a frame error=> RX_line low for longer then 8 bits (Break condition)
		{
			DMX_Break = 1;	//Reset the counter
			DMX_Start = 1;
			counter = 0;
			dmx_counter = 0;
		}

		else if(DMX_Break == 1)	//If break was detected
		{
			if(DMX_Start == 1)
			{
				if((counter++ == (DMX_Address + dmx_counter)) && (dmx_counter < 8))
				{
					PWM_IN[dmx_counter++] = data;
					//dmx_counter++;
				}
				//counter++;
			}
				
			if(counter == 0)
			{
				if(data == 0x00) //Lighting start byte
				DMX_Start = 1;
				else
				DMX_Start = 0;
			}
		}
	}
	PORTE &= ~(1 << PE7);
}