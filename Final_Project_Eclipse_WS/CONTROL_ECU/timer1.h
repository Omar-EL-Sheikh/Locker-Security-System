/***************************************************************************
 *
 * Module Name: Timer1
 * 	 
 * File Name: timer1.h
 *
 * Description: Header file for ATmega32 Timer1 Driver
 *
 * Created on: Oct 27, 2022
 *
 * Author: Omar EL-Sheikh
 * 
 **************************************************************************/
#ifndef TIMER1_H_
#define TIMER1_H_

/*******************************************************************************
 * 								 Inclusions
 *******************************************************************************/
#include "std_types.h"

/*******************************************************************************
 * 								 Definitions
 *******************************************************************************/


/*******************************************************************************
 * 								 User-Defined Data Types
 *******************************************************************************/

/* Enumeration Constants to define Timer1 Pre-scaler */
typedef enum {
	NO_CLOCK, F_CPU_1, F_CPU_8, F_CPU_64,F_CPU_256,
	F_CPU_1024, EXT_T1_FALLING, EXT_T1_RISING
} Timer1_Prescaler;

/* Enumeration Constants to define Timer1 Modes */
typedef enum {
	NORMAL_MODE, COMPARE_MODE
} Timer1_Mode;

/* Structure Data Type to define Timer1 Configurations */
typedef struct {
 uint16 initial_value;
 uint16 compare_value;
 Timer1_Prescaler prescaler;
 Timer1_Mode mode;
} Timer1_ConfigType;

/*******************************************************************************
 * 								 Functions Prototypes
 *******************************************************************************/

/*
 * Description:
 * Function to initiate and start Timer1 based on the given configurations
 */
void Timer1_init(const Timer1_ConfigType * Config_Ptr);

/*
 * Description:
 * Function to set call-back function address
 */
void Timer1_setCallBack(void(*a_ptr)(void));

/*
 * Description:
 * Function to disable Timer1
 */
void Timer1_deInit(void);


#endif /* TIMER1_H_ */
