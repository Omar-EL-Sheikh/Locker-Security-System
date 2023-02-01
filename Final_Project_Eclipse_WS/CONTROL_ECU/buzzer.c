/***************************************************************************
 *
 * Module Name: Buzzer
 * 	 
 * File Name: buzzer.c
 *
 * Description: Source file for Buzzer Driver
 *
 * Created on: Oct 27, 2022
 *
 * Author: Omar EL-Sheikh
 * 
 **************************************************************************/

/*******************************************************************************
 *								Inclusions
 *******************************************************************************/
#include "buzzer.h"
#include "gpio.h"
/*******************************************************************************
 *								Functions Definitions
 *******************************************************************************/
void Buzzer_init(void){
	GPIO_setupPinDirection(BUZZER_PORT, BUZZER_PIN, PIN_OUTPUT);
}

void Buzzer_on(void){
	GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_HIGH);
}

void Buzzer_off(void){
	GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_LOW);
}
