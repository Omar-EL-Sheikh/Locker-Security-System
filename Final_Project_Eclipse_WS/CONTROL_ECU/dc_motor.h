/***************************************************************************
 *
 * Module Name: DC_MOTOR
 * 	 
 * File Name: dc_motor.h
 *
 * Description: Header file for DC Motor Driver
 *
 * Created on: Oct 6, 2022
 *
 * Author: Omar EL-Sheikh
 * 
 **************************************************************************/
#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

/***************************************************************************
*								Inclusions
***************************************************************************/
#include "std_types.h"
#include <avr/io.h>

/***************************************************************************
*								Definitions
***************************************************************************/
#define MOTOR_PIN_IN1 PB0
#define MOTOR_PIN_IN2 PB1

/***************************************************************************
*								Types Declaration
***************************************************************************/
typedef enum DcMotor_State {
	STOP, A_CW, CW
} DcMotor_State;

/***************************************************************************
*								Functions Prototypes
***************************************************************************/

/*
 * Description:
 * Function to setup the direction for the two motor pins
 * The Motor is stopped at the beginning
 * */
void DcMotor_Init(void);

/*
 * Description:
 * Function to choose the direction and state of the DC Motor (CW, ACW, Stop)
 * Sets the speed of the motor based on the required PWM signal sent
 * */
void DcMotor_Rotate(DcMotor_State state,uint8 speed);

#endif /* DC_MOTOR_H_ */
