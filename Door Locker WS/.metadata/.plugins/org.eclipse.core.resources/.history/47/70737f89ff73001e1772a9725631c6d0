/*	______________________________________________________________________________
   |																			  |
   | Driver: TIMER0 PWM															  |
   |																			  |
   | File Name: timer0_pwm.c													  |
   |																			  |
   | Description: Source file for the ATmega32 timer0 PWM driver				  |
   |																			  |
   | Author: Youssif Hossam														  |
   |______________________________________________________________________________|
*/

#include "timer0_pwm.h"

#include <avr/io.h>
#include "../../MCAL/GPIO/gpio.h"

/*
 * Description :
 -> Function responsible for initialize Timer0 on PWM mode
 */
void Timer0_PWM_Init(uint8 channel, unsigned char set_duty_cycle)
{
	set_duty_cycle =((uint16)(255*set_duty_cycle)) / 100;

	switch(channel){
	case 1:
		TCNT0 = 0;  // Set Timer0 Initial Value to 0
		OCR0  = set_duty_cycle; //Set Compare value
		GPIO_setupPinDirection(PORTB_ID , PIN3_ID , PIN_OUTPUT);
		TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS01);
		break;
	case 2:
		TCNT2 = 0;	// Set Timer2 Initial Value to 0
		OCR2  = set_duty_cycle; //Set Compare value
		GPIO_setupPinDirection(PORTD_ID , PIN7_ID , PIN_OUTPUT);
		TCCR2 = (1<<WGM20) | (1<<WGM21) | (1<<COM21) | (1<<CS21);
		break;
	}
	/* Configure timer control register
	 * 1. Fast PWM mode FOC0=0
	 * 2. Fast PWM Mode WGM01=1 & WGM00=1
	 * 3. Clear OC0 when match occurs (non inverted mode) COM00=0 & COM01=1
	 * 4. clock = F_CPU/8 CS00=0 CS01=1 CS02=0
	 */
}
