/*	______________________________________________________________________________
   |																			  |
   | APP: Control ECU Application												  |
   |																			  |
   | File Name: APP.h															  |
   |																			  |
   | Description: Source file for the Control ECU Application					  |
   |																			  |
   | Author: Youssif Hossam														  |
   |______________________________________________________________________________|
 */

#include "LIB/std_types.h"
#include "MCAL/USART/usart.h"
#include "MCAL/GPIO/gpio.h"
#include "MCAL/TIMER1/timer1.h"
#include "MCAL/I2C/i2c.h"
#include "HAL/BUZZER/buzzer.h"
#include "HAL/EXT_EEPROM/ext_eeprom.h"
#include "HAL/L293D/l293d.h"
#include "util/delay.h"
#include "avr/io.h"
#include "commands.h"

/*______________________________________________________________________________
 |                               Functions Prototypes                           |
 |______________________________________________________________________________|
 */

/*
 * Description :
 -> Function responsible for receiving the password from HMI ECU
 */
void receive_password(uint8 *PasswordArr);

/*
 * Description :
 -> Function responsible for increment timer1 ticks
 */
void timer1_tick(void);

uint8 Timer1Ticks = 0;
Timer1_ConfigType Timer1Config = {0 , 31250 , COMPARE_MODE , CLK_256_PRESCALER} ;

/*______________________________________________________________________________
 |                       		  Main Function                       		    |
 |______________________________________________________________________________|
 */

int main (void){


	uint8 Password_Arr1[5] , Password_Arr2[5] ;		/* Password buffer arrays */
	uint8 matching_flag ;	/* This flag is true if the two passwords are identical while creating new password */
	uint8 PasswordTempVar;	/* Variable to hold the data of EEPROM before using it */
	uint8 PasswordSetFlag;	/* This flag is true when the password is successfully created */
	uint8 MultiUsedCounter;

	/* The configuration structure of the UART protocol */
	UART_ConfigType USART_MyConfig = {EIGHT_BIT_MODE , DISABLED_PARITY , ONE_STOP_BIT , 9600};
	/* The configuration structure of the I2c protocol */
	I2C_ConfigType	I2C_MyConfig = {0x01 , BAUDRATE_400KHZ};

	SREG |= 1<<7;		/* Enabling Global Interrupts Service */

	UART_init(&USART_MyConfig);
	I2C_init(&I2C_MyConfig);
	L293D_Init(CHANNEL1_MODE);
	BUZZER_init();
	Timer1_setCallBack(timer1_tick);

	EEPROM_writeByte(PASSWORD_SET_FLAG_LOCATION , 0);
	_delay_ms(15);

	while(1){

		switch(UART_recieveByte()){

		case IS_PASSWORD_SET:

			PasswordSetFlag = 0;

			/* reading password set flag from EEPROM */
			EEPROM_readByte(PASSWORD_SET_FLAG_LOCATION , &PasswordSetFlag);

			if(PasswordSetFlag)				/* If the password was created */
				UART_sendByte(SETTED);		/* Informing HMI ECU */

			else							/* If the password wasn't created yet */
				UART_sendByte(NOT_SETTED);	/* Informing HMI ECU */

			break;

		case TAKE_THE_FIRST_PASSWORD :		/* Getting ready to receive the first password */

			receive_password(Password_Arr1);

			break;

		case TAKE_THE_SECOND_PASSWORD :		/* Getting ready to receive the second password */

			receive_password(Password_Arr2);

			break;

		case ARE_THEM_MATCHED :				/* Checking if the two passwords are identical or not */

			matching_flag = 1;

			for(MultiUsedCounter = 0 ; MultiUsedCounter < 5 ; MultiUsedCounter++){
				if(Password_Arr1[MultiUsedCounter] != Password_Arr2[MultiUsedCounter]){
					matching_flag = 0;
					break;
				}

			}

			if(matching_flag){		/* If the two password are identical */

				UART_sendByte(MATCHED);		/* Informing HMI ECU that they are identical */

				/* Store the password set flag into EEPROM */
				EEPROM_writeByte(PASSWORD_SET_FLAG_LOCATION , matching_flag);
				_delay_ms(15);

				for(MultiUsedCounter = 0 ; MultiUsedCounter < 5 ; MultiUsedCounter++){
					EEPROM_writeByte(MultiUsedCounter , Password_Arr1[MultiUsedCounter]);
					_delay_ms(10);
				}

			}

			else {
				UART_sendByte(UNMATCHED);
			}

			_delay_ms(10);
			break;

		case TAKE_MAIN_PASSWORD :		/* Checking if the password is correct or not */

			matching_flag = 1;

			receive_password(Password_Arr1);	/* Receiving the password from HMI ECU */

			for(MultiUsedCounter = 0 ; MultiUsedCounter < 5 ; MultiUsedCounter++){
				EEPROM_readByte(MultiUsedCounter , &PasswordTempVar);
				if(Password_Arr1[MultiUsedCounter] != PasswordTempVar)
					matching_flag = 0;
				_delay_ms(10);
			}

			if(matching_flag)
				UART_sendByte(CORRECT_PASSWORD);	/* Informing HMI ECU that the password is correct */

			else
				UART_sendByte(WRONG_PASSWORD);		/* Informing HMI ECU that the password is wrong */

			break;

		case OPEN_DOOR	:							/* Unlocking the door */

			Timer1_init(&Timer1Config);

			L293D_Rotate(1 , CW , 100);
			while(Timer1Ticks != 15);		/* waiting 15 seconds*/
			Timer1Ticks = 0;
			Timer1_deInit();
			Timer1_init(&Timer1Config);

			L293D_Rotate(1 , OFF , 0);
			while(Timer1Ticks != 3);		/* waiting 3 seconds*/
			Timer1Ticks = 0;
			Timer1_deInit();
			Timer1_init(&Timer1Config);

			L293D_Rotate(1 , A_CW , 100);
			while(Timer1Ticks != 15);		/* waiting 15 seconds*/
			Timer1Ticks = 0;
			Timer1_deInit();

			L293D_Rotate(1 , OFF , 0);

			break;

		case ALARM_MODE :				/* Alarm mode */

			Timer1_init(&Timer1Config);
			BUZZER_on();
			while(Timer1Ticks != 60);	/* waiting 60 seconds*/
			Timer1Ticks = 0;
			Timer1_deInit();

			BUZZER_off();

			break;
		}
	}
	return 0;
}

void receive_password(uint8 *PasswordArr){
	for(uint8 i = 0 ; i < 5 ; i++){
		PasswordArr[i] = UART_recieveByte();
		_delay_ms(15);
	}
}
void timer1_tick(void){
	Timer1Ticks++;
}