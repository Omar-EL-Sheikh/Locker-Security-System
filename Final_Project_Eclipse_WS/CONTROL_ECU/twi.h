/***************************************************************************
 *
 * Module Name: TWI
 * 	 
 * File Name: twi.h
 *
 * Description: Header file for ATmega16 TWI/I2C Driver
 *
 * Created on: Oct 26, 2022
 *
 * Author: Omar EL-Sheikh
 * 
 **************************************************************************/
#ifndef TWI_H_
#define TWI_H_

/*******************************************************************************
 *                      Inclusions                                  *
 *******************************************************************************/
#include "std_types.h"

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

/* I2C Status Bits in the TWSR Register */
#define TWI_START         0x08 /* start has been sent */
#define TWI_REP_START     0x10 /* repeated start */
#define TWI_MT_SLA_W_ACK  0x18 /* Master transmit ( slave address + Write request ) to slave + ACK received from slave. */
#define TWI_MT_SLA_R_ACK  0x40 /* Master transmit ( slave address + Read request ) to slave + ACK received from slave. */
#define TWI_MT_DATA_ACK   0x28 /* Master transmit data and ACK has been received from Slave. */
#define TWI_MR_DATA_ACK   0x50 /* Master received data and send ACK to slave. */
#define TWI_MR_DATA_NACK  0x58 /* Master received data but doesn't send ACK to slave. */

/*******************************************************************************
 *                      User-Defined Data Types                                    *
 *******************************************************************************/

/* Defining Device Address as a 8-bit Variable*/
typedef uint8 TWI_Adress;

/* Enumeration Constants for TWI Bit Rate*/
typedef enum {
	BIT_RATE_100_KBS = 100000, BIT_RATE_400_KBS = 400000,
	BIT_RATE_1_MBS = 1000000, BIT_RATE_3_4_MBS = 3400000
} TWI_BaudRate;

/* Structure Data Type to define the configurations of TWI */
typedef struct{
 TWI_Adress address;
 TWI_BaudRate bit_rate;
} TWI_ConfigType;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void TWI_init(const TWI_ConfigType * Config_Ptr);
void TWI_start(void);
void TWI_stop(void);
void TWI_writeByte(uint8 data);
uint8 TWI_readByteWithACK(void);
uint8 TWI_readByteWithNACK(void);
uint8 TWI_getStatus(void);


#endif /* TWI_H_ */
