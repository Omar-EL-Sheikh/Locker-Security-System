/***************************************************************************
 *
 * Module Name: UART
 * 	 
 * File Name: uart.h
 *
 * Description: Header file for ATmega16 UART Driver
 *
 * Created on: Oct 24, 2022
 *
 * Author: Omar EL-Sheikh
 * 
 **************************************************************************/
#ifndef UART_H_
#define UART_H_

/*******************************************************************************
 *                                Inclusions                                  *
 *******************************************************************************/
#include "std_types.h"
/*******************************************************************************
 *                                Types Declarations                          *
 *******************************************************************************/
/* Enumeration Constants for Data Bits Number */
typedef enum {
	BITS_5, BITS_6, BITS_7, BITS_8, BITS_9 = 7
} UART_BitData;

/* Enumeration Constants for Parity Mode */
typedef enum {
	NO_PARITY, EVEN_PARITY = 2, ODD_PARITY
} UART_Parity;

/*Enumeration Constants for Number of Stop Bits */
typedef enum {
	BIT_1, BIT_2
} UART_StopBit;

/* Enumeration Constants for Baud Rate */
typedef enum {
	BAUD_RATE_10 = 10, BAUD_RATE_300 = 300, BAUD_RATE_600 = 600,
	BAUD_RATE_1200 = 1200, BAUD_RATE_2400 = 2400, BAUD_RATE_4800 = 4800,
	BAUD_RATE_9600 = 9600, BAUD_RATE_14400 = 14400, BAUD_RATE_19200 = 19200,
	BAUD_RATE_38400 = 38400, BAUD_RATE_57600 = 57600, BAUD_RATE_115200 = 115200,
	BAUD_RATE_128000 = 128000, BAUD_RATE_256000 = 256000
} UART_BaudRate;

/* Structure to define UART Configurations */
typedef struct{
 UART_BitData bit_Data;
 UART_Parity parity;
 UART_StopBit stop_Bit;
 UART_BaudRate baud_Rate;
} UART_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_ConfigType * UART_Configs_Ptr);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
