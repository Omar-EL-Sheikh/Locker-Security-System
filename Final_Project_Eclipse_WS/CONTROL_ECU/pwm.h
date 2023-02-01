/***************************************************************************
 *
 * Module Name: PWM
 * 	 
 * File Name: pwm.h
 *
 * Description: Header file for the ATmega16 PWM Timer0 Driver
 *
 * Created on: Oct 6, 2022
 *
 * Author: Omar EL-Sheikh
 * 
 **************************************************************************/
#ifndef PWM_H_
#define PWM_H_

/***************************************************************************
*								Inclusions
***************************************************************************/
#include "std_types.h"

/***************************************************************************
*								Definitions
***************************************************************************/
#define OC0 PB3
#define DUTY_CYCLE_TO_COMPARE_VALUE_CONVERSION 2.55
/***************************************************************************
*								Functions Declaration
***************************************************************************/
/**
 * Description:
 * Function to trigger Timer0 with PWM mode
 */
void PWM_Timer0_Start(uint8 duty_cycle);

#endif /* PWM_H_ */
