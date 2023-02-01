/***************************************************************************
 *
 * Module Name: Buzzer
 * 	 
 * File Name: buzzer.h
 *
 * Description: Header file for Buuzer Driver
 *
 * Created on: Oct 27, 2022
 *
 * Author: Omar EL-Sheikh
 * 
 **************************************************************************/
#ifndef BUZZER_H_
#define BUZZER_H_

/*******************************************************************************
 *								Inclusions
 *******************************************************************************/
#include "std_types.h"
#include "gpio.h"

/*******************************************************************************
 *								Definitions
 *******************************************************************************/
#define BUZZER_PORT PORTA_ID
#define BUZZER_PIN PIN0_ID

/*******************************************************************************
 *								Functions Prototypes
 *******************************************************************************/
/*
 * Description:
 * Function to initialize Buzzer pin as output pin
 */
void Buzzer_init(void);

/*
 * Description:
 * Function to enable Buzzer
 */
void Buzzer_on(void);

/*
 * Description:
 * Function to disable Buzzer
 */
void Buzzer_off(void);
#endif /* BUZZER_H_ */
