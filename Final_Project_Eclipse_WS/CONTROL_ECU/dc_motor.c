/***************************************************************************
 *
 * Module Name: DC_MOTOR
 * 	 
 * File Name: dc_motor.c
 *
 * Description: Source file for DC Motor Driver
 *
 * Created on: Oct 7, 2022
 *
 * Author: Omar EL-Sheikh
 * 
 **************************************************************************/

/***************************************************************************
 *								Inclusions
 ***************************************************************************/
#include "dc_motor.h"
#include "common_macros.h"
#include "gpio.h"

/***************************************************************************
 *								Functions Definitions
 ***************************************************************************/

/*
 * Description:
 * Function to setup the direction for the two motor pins
 * The Motor is stopped at the beginning
 * */
void DcMotor_Init(void){
	/*
	 * Setting the direction of motor direction pins in PORTB to output
	 * */
	GPIO_setupPinDirection(PORTB_ID, MOTOR_PIN_IN1, PIN_OUTPUT);
	GPIO_setupPinDirection(PORTB_ID, MOTOR_PIN_IN2, PIN_OUTPUT);

	/*
	 * Stopping the motor at the beginning by writing logic zero to the two motor pins
	 * */
	GPIO_writePin(PORTB_ID, MOTOR_PIN_IN1, LOGIC_LOW);
	GPIO_writePin(PORTB_ID, MOTOR_PIN_IN2, LOGIC_LOW);
}

/*
 * Description:
 * Function to choose the direction and state of the DC Motor (CW, ACW, Stop)
 * Sets the speed of the motor based on the required PWM signal sent
 * */
void DcMotor_Rotate(DcMotor_State state,uint8 speed){
	/*
	 * Clearing the motor direction pins
	 * */
	GPIO_writePin(PORTB_ID, MOTOR_PIN_IN1, LOGIC_LOW);
	GPIO_writePin(PORTB_ID, MOTOR_PIN_IN2, LOGIC_LOW);

	/*Setting the motor direction pins in PORTB to the specified state*/
	PORTB |= (state);

	/*
	 * Sending the specified speed value to TIMER0 to generate a PWM signal
	 * on PC0 which will control the motor speed */
	PWM_Timer0_Start(speed);
}
