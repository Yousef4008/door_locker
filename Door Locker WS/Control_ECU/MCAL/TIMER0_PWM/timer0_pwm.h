/*	______________________________________________________________________________
   |																			  |
   | Driver: TIMER0 PWM															  |
   |																			  |
   | File Name: timer0_pwm.h													  |
   |																			  |
   | Description: Header file for the ATmega32 timer0 PWM driver				  |
   |																			  |
   | Author: Youssif Hossam														  |
   |______________________________________________________________________________|
*/

#ifndef MCAL_PWM_PWM_H_
#define MCAL_PWM_PWM_H_

#include "../../LIB/std_types.h"


/*______________________________________________________________________________
 |                               Functions Definitions                          |
 |______________________________________________________________________________|
 */

/*
 * Description :
 -> Function responsible for initialize Timer0 on PWM mode
 */
void Timer0_PWM_Init(uint8 channel, unsigned char set_duty_cycle);

#endif /* MCAL_PWM_PWM_H_ */
