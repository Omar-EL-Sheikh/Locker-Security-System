/***************************************************************************
 *
 * Module Name: Timer1
 * 	 
 * File Name: timer1.c
 *
 * Description: Source file for ATmega32 Timer1 Driver
 *
 * Created on: Oct 27, 2022
 *
 * Author: Omar EL-Sheikh
 * 
 **************************************************************************/

/*******************************************************************************
 * 								 Inclusions
 *******************************************************************************/
#include"timer1.h"
#include "common_macros.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/*******************************************************************************
 * 								 Global Variables
 *******************************************************************************/
static volatile void (*g_Timer1_Call_Back) (void) = NULL_PTR;

/*******************************************************************************
 * 								 Functions Definitions
 *******************************************************************************/
void Timer1_init(const Timer1_ConfigType * Config_Ptr){
	/* Setting Timer1 counter register to its specified initial value from Configurations */
	TCNT1 = Config_Ptr -> initial_value;

	/* Choosing between CTC and Normal mode */
	if (Config_Ptr -> mode == COMPARE_MODE){
		TCCR1B |= (1 << WGM12); /* Setting Timer1 in CTC Mode with (Compare A) OCR1A as Compare Register */
		OCR1A = Config_Ptr -> compare_value; /* Setting Compare Register to its specified value from Configurations*/
		TIMSK = (1 << OCIE1A); /* Output Compare A Match Interrupt Enable */
	}
	else if (Config_Ptr -> mode == NORMAL_MODE){
		/*For Normal Mode: WGM10:13 = 0 */
		TIMSK |= (1 << TOIE1); /* Overflow Interrupt Enable */
	}

	/* Setting the specified Pre-scaler from Configurations */
	TCCR1B = ((TCCR1B & (0XF8)) | (Config_Ptr -> prescaler));
}

void Timer1_setCallBack(void(*a_ptr)(void)){
	/* Saving the address of the call back function in a global pointer to function*/
	g_Timer1_Call_Back = a_ptr;
}

void Timer1_deInit(void){
	TCCR1B = 0; /* Clearing Control Register to disable Timer1*/
	TCNT1 = 0; /* Clearing Counter Register*/
	OCR1A = 0; /* Clearing Compare Register*/
	TIMSK &= (0XEB); /* Disabling Interrupt Bits */
}

ISR (TIMER1_COMPA_vect){
	if(g_Timer1_Call_Back != NULL_PTR)
	{
		/* Calling the Call Back function in the application after Compare Match */
		(*g_Timer1_Call_Back)();
	}
}

ISR (TIMER1_OVF_vect){
	if(g_Timer1_Call_Back != NULL_PTR)
	{
		/* Calling the Call Back function in the application after Compare Match */
		(*g_Timer1_Call_Back)();
	}
}
