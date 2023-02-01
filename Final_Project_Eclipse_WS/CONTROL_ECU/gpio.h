/***************************************************************************
*
* Module Name: GPIO
*
* File Name: adc.h
*
* Description: Header file for Atmega16 GPIO Driver
*
* Created on: Oct 6, 2022
*
* Author: Omar EL-Sheikh
*
**************************************************************************/

#ifndef GPIO_H_
#define GPIO_H_

/***************************************************************************
*								Inclusions
* ************************************************************************/
#include "std_types.h"

/***************************************************************************
*								Definitions
* ************************************************************************/
#define NUM_OF_PORTS 4
#define NUM_OF_PINS 8

#define PORTA_ID 0
#define PORTB_ID 1
#define PORTC_ID 2
#define PORTD_ID 3

#define PIN0_ID 0
#define PIN1_ID 1
#define PIN2_ID 2
#define PIN3_ID 3
#define PIN4_ID 4
#define PIN5_ID 5
#define PIN6_ID 6
#define PIN7_ID 7

/***************************************************************************
*								Type Declarations
* ************************************************************************/
typedef enum {
PIN_INPUT, PIN_OUTPUT
}GPIO_PinDirection_Type;

typedef enum {
PORT_INPUT, PORT_OUTPUT = 0XFF
}GPIO_PortDirection_Type;

/***************************************************************************
*								Functions Prototypes
* ************************************************************************/

/*
* Description:
* Setup the direction of the specified pin to input/output
* If the input port number or pin number are not correct,
* The function will not handle the request.
*/
void GPIO_setupPinDirection(uint8 port_num, uint8 pin_num, GPIO_PinDirection_Type direction);

/*
* Description:
* Writes a logic high/low to the specified pin in port
* If the input port number or pin number are not correct,
* The function will not handle the request.
*/
void GPIO_writePin(uint8 port_num, uint8 pin_num, uint8 value);

/*
* Description:
* Reads the value of the specified pin in port and returns its value
* If the input port number or pin number are not correct,
* The function will not handle the request.
*/
uint8 GPIO_readPin(uint8 port_num, uint8 pin_num);

/*
* Description:
* Sets up the specified port direction to input/output
* If the input port number or pin number are not correct,
* The function will not handle the request.
*/
void GPIO_setup_PortDirection(uint8 port_num, GPIO_PortDirection_Type direction);

/*
* Description:
* Writes logic high/low to all the pins in the port
* If the input port number or pin number are not correct,
* The function will not handle the request.
*/
void GPIO_writePort(uint8 port_num, uint8 value);

/*
* Description:
* Reads the value of the specified port and returns its value
* If the input port number or pin number are not correct,
* The function will not handle the request.
*/
uint8 GPIO_readPort(uint8 port_num);

#endif /* GPIO_H_ */
