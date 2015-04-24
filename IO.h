/*
 * IO.h
 *
 * Created: 15/05/2013 17:20:27
 *  Author: nick
 */ 


#ifndef IO_H_
#define IO_H_

void IO_Init(void);
void IO_LED(uint8_t LED, bool status);
void IO_LED_PWM(bool status);
void IO_PWM7(bool status);

#define LED_INT		PE7
#define LED_EXT		PE6

#endif /* IO_H_ */