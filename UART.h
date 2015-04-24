/*
 * uart.h
 *
 * Created: 15/05/2013 16:14:04
 *  Author: nick
 */ 


#ifndef UART_H_
#define UART_H_

void USART0_Init(void);
void USART0_Disable(void);
void USART0_Commandhandler(void);
char USART0_Flush(void);
void USART0_PutChar(char c);
char USART0_GetChar(void);
void USART0_PutString(char *buffer);

void DMX_Init(void);

// Uart baud rate configuration
#define USART0_BAUDRATE 		9600
#define USART0_BAUD_PRESCALE	((F_CPU / (USART0_BAUDRATE * 16UL))-1)

// DMX configuration
#define DMX_BAUDRATE			250000
#define DMX_BAUD_PRESCALE		((F_CPU / (DMX_BAUDRATE * 16UL))-1)

// Uart related definitions
#define RXB8 1
#define TXB8 0
#define UPE 2
#define OVR 3
#define FE 4
#define UDRE 5
#define RXC 7

// UART0 Rx & Tx buffer sizes (must be <= 255)
#define RX_BUFFER_SIZE0     128
#define TX_BUFFER_SIZE0     128

#endif /* UART_H_ */