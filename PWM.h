/*
 * PWM.h
 *
 * Created: 22/12/2014 12:05:14
 *  Author: nick
 */ 


#ifndef PWM_H_
#define PWM_H_

void PWM_Init(void);

#define PWM0	OCR1CL
#define PWM1	OCR1BL
#define PWM2	OCR1AL
#define PWM3	OCR0
#define PWM4	OCR3CL
#define PWM5	OCR3BL
#define PWM6	OCR3AL
#define PWM7	OCR2

#endif /* PWM_H_ */