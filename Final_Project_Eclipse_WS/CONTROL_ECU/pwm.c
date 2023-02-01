/***************************************************************************
 *
 * Module Name: PWM
 * 	 
 * File Name: pwm.c
 *
 * Description: Source file for the ATmega16 PWM Timer0 Driver
 *
 * Created on: Oct 6, 2022
 *
 * Author: Omar EL-Sheikh
 * 
 **************************************************************************/

/***************************************************************************
 *								Inclusions
 ***************************************************************************/
#include "pwm.h"
#include "common_macros.h"
#include"gpio.h"
#include <avr/io.h>

/***************************************************************************
 *								Functions Definitions
 ***************************************************************************/

void PWM_Timer0_Start(uint8 duty_cycle){
	TCNT0 = 0; /* Setting timer register initial value to 0 */

	OCR0  = duty_cycle * DUTY_CYCLE_TO_COMPARE_VALUE_CONVERSION; /* Setting Compare value to duty cycle */

	GPIO_setupPinDirection(PORTB_ID, OC0, PIN_OUTPUT); /* Setting the direction of OC0 pin in PortB to output for PWM signal output*/

	/* Configure timer control register
	 * 1. Fast PWM mode FOC0 = 0
	 * 2. Fast PWM Mode WGM01 = 1 & WGM00 = 1
	 * 3. Clear OC0 when match occurs (non inverted mode) COM00 = 0 & COM01 = 1
	 * 4. clock = F_CPU/8 CS00 = 0 CS01 = 1 CS02 = 0
	 */
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS01);
}
