/***************************************************************************
 *
 * Module Name: CONTROL ECU
 *
 * File Name: Control_Application.c
 *
 * Description: Source file for CONTROL ECU Application
 *
 * Created on: Oct 30, 2022
 *
 * Author: Omar EL-Sheikh
 *
 **************************************************************************/

/**************************************************************************
 *								Inclusions
 *************************************************************************/

#include "std_types.h"
#include "dc_motor.h"
#include "gpio.h"
#include "twi.h"
#include "external_eeprom.h"
#include "uart.h"
#include "timer1.h"
#include "buzzer.h"
#include "lcd.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/**************************************************************************
 *								 Definitions
 *************************************************************************/

#define CONTROL_ECU_ADDRESS 0X01
#define PASSWORD_SIZE 5
#define PASSWORD_ENTER_KEY 13
#define CONTROL_READY_TO_RECEIVE 0XAA
#define HMI_READY_TO_RECEIVE 0XBB
#define PASSWORD_CONFIRMED TRUE
#define PASSWORD_UNCONFIRMED FALSE
#define EEPROM_PASSWORD_START_BYTE 0X0001

/**************************************************************************
 *								 Global Variables
 *************************************************************************/

/* Global variable to store system password */
uint8 g_password[PASSWORD_SIZE];

/* Global variable to store system password confirmation */
uint8 g_passwordConfirm[PASSWORD_SIZE];

/* Global variable to store confirmation of password status */
boolean g_passwordConfirmStats = PASSWORD_UNCONFIRMED;

/* Global variable to store interrupts count for unlocking door */
uint8 g_unlockDoorInt;

/* Global variable to store interrupts count for holding door */
uint8 g_holdDoorInt;

/* Global variable to store interrupts count for locking door */
uint8 g_lockDoorInt;

/**************************************************************************
 *								Functions Prototypes
 *************************************************************************/

/*
 * Description:
 * Function to initiate all required Drivers
 */
void Drivers_Init(void);

/*
 * Description:
 * Function to check password and return state
 */
boolean checkPassword(void);

/* Description:
 * Function to receive password twice from HMI ECU
 * Confirm password
 * Send Confirmation to the HMI ECU
 */
void createPassword (void);

/*
 * Description:
 * Function to receive password by UART
 */
void receivePassword (uint8 * password_Ptr);

/* Description:
 * Function to write the received password in the EEPROM
 */
void savePassword(void);

/* Description:
 * Function to activate buzzer and freeze system for 1 minute
 */
void lockSystemAction(void);

/*
 * Description:
 * Call-back function to process interrupt by the timer for unlocking door
 */
void processUnlockDoor(void);

/* Description:
 * Function to unlock door for 15 seconds
 */
void unlockDoor(void);

/*
 * Description:
 * Function to process interrupt by timer in case of holding door
 */
void processHoldDoor(void);

/*
 * Description:
 * Function to hold the door for 3 seconds
 */
void holdDoor(void);

/*
 * Description:
 * Call-back function to process interrupt by the timer for locking door
 */
void processLockDoor(void);

/* Description:
 * Function to unlock door for 15 seconds
 */
void lockDoor(void);

/* Description:
 * Function to verify password in the EEPROM
 */
void verifyPassword(void);

/* Description:
 * Function to take action in case user choose to open door
 */
void openDoorAction(void);

/*
 * Description:
 * Function to change system password
 */
void changePasswordProcess(void);

/* Description:
 * Function to receive option from HMI ECU and take an action based on it
 */
void processOption(void);

/**************************************************************************
 *								Main Program Execution
 *************************************************************************/

int main (){
	sei(); /* Enabling Global Interrupt */
	Drivers_Init();

	while (!g_passwordConfirmStats){
		createPassword();
	}

	/* Saving the password in EEPROM*/
	savePassword();

	while (1){
		processOption();
	}
}

/**************************************************************************
 *								Functions Definitions
 *************************************************************************/

/*
 * Description:
 * Function to initiate all required Drivers
 */
void Drivers_Init(void){
	/* Variable to store UART Configurations */
	UART_ConfigType UART_Configs = {BITS_8, NO_PARITY, BIT_1, BAUD_RATE_9600};
	/* Variable to store TWI Configurations */
	TWI_ConfigType TWI_Configs = {CONTROL_ECU_ADDRESS, BIT_RATE_400_KBS};

	UART_init(&UART_Configs);
	TWI_init(&TWI_Configs);
	DcMotor_Init();
	Buzzer_init();
}

/*
 * Description:
 * Function to check password and return state
 */
boolean checkPassword(void){
	uint8 counter, temp;

	/* Checking password from EEPROM */
	for (counter = 0; counter < PASSWORD_SIZE; counter++){
		EEPROM_readByte(EEPROM_PASSWORD_START_BYTE + counter, &temp);
		_delay_ms(10);
		if (temp != g_password[counter]){
			return PASSWORD_UNCONFIRMED;
		}
	}

	return PASSWORD_CONFIRMED;
}

/* Description:
 * Function to receive password twice from HMI ECU
 * Confirm password
 * Send Confirmation to the HMI ECU
 */
void createPassword (void){
	uint8 i ;

	/* Receive password */
	receivePassword(&g_password);
	/* Receive password again to be confirmed */
	receivePassword(&g_passwordConfirm);

	/* Confirm Password */
	for (i = 0; i < PASSWORD_SIZE; i++){
		if (g_password [i] != g_passwordConfirm[i]){
			/* Sending unconfirmation to indicate a mismatch */
			while (UART_recieveByte() != HMI_READY_TO_RECEIVE){}
			UART_sendByte(PASSWORD_UNCONFIRMED);
			return;
		}
	}

	/* Sending confirmation to indicate matching */
	while (UART_recieveByte() != HMI_READY_TO_RECEIVE){}
	UART_sendByte(PASSWORD_CONFIRMED);
	g_passwordConfirmStats = PASSWORD_CONFIRMED;
}

/*
 * Description:
 * Function to receive password by UART
 */
void receivePassword (uint8 * password_Ptr){
	uint8 counter ;

	/* Sending an indicator that the Control ECU is ready to receive */
	UART_sendByte(CONTROL_READY_TO_RECEIVE);

	/* Receiving password by UART */
	for (counter = 0; counter < PASSWORD_SIZE; counter ++){
		password_Ptr [counter] = UART_recieveByte();
	}
}

/* Description:
 * Function to write the received password in the EEPROM
 */
void savePassword(void){
	uint8 counter;

	/* Writing the password in 5 consecutive bytes in EEPROM  */
	for (counter = 0; counter < PASSWORD_SIZE; counter++){
		EEPROM_writeByte(EEPROM_PASSWORD_START_BYTE + counter, g_password[counter]);
		_delay_ms(10);
	}
}

/* Description:
 * Function to activate buzzer and freeze system for 1 minute
 */
void lockSystemAction(void){
	uint8 counter;
	Buzzer_on();
	for (counter = 0; counter < 60; counter++){
		_delay_ms(1000);
	}
	Buzzer_off();
}

/*
 * Description:
 * Call-back function to process interrupt by the timer for unlocking door
 */
void processUnlockDoor(void){
	g_unlockDoorInt++;
}


/* Description:
 * Function to unlock door for 15 seconds
 */
void unlockDoor(void){
	g_unlockDoorInt = 0;

	/* Configurations for Timer1 to generate an interrupt every 7.5 seconds */
	Timer1_ConfigType Timer1_UnlockDoorConfigs = {0, 58594, F_CPU_1024, COMPARE_MODE};

	/* Sending the specified call back function to unlock door to Timer1 ISR */
	Timer1_setCallBack(processUnlockDoor);

	/* Timer1 Initializing */
	Timer1_init(&Timer1_UnlockDoorConfigs);

	/* Rotating the motor CW */
	DcMotor_Rotate(CW, 50);

	/* Wait until Timer1 counts 15 seconds*/
	while (g_unlockDoorInt != 2){}

	/* Stopping the Timer */
	Timer1_deInit();

	/* Stopping the Motor */
	DcMotor_Rotate(STOP, 0);

}

/*
 * Description:
 * Function to process interrupt by timer in case of holding door
 */
void processHoldDoor(void){
	g_holdDoorInt++;
}

/*
 * Description:
 * Function to hold the door for 3 seconds
 */
void holdDoor(void){
	g_holdDoorInt = 0;

	/* Configurations for Timer1 to generate an interrupt every 3 seconds */
	Timer1_ConfigType Timer1_HoldDoorConfigs = {0, 23438, F_CPU_1024, COMPARE_MODE};

	/* Sending the specified call back function to door door to Timer1 ISR */
	Timer1_setCallBack(processHoldDoor);

	/* Stopping the Motor */
	DcMotor_Rotate(STOP, 0);

	/* Timer1 Initializing */
	Timer1_init(&Timer1_HoldDoorConfigs);

	/* Wait until Timer1 counts 3 seconds */
	while (g_holdDoorInt != 1){}

	/* Stopping the Timer */
	Timer1_deInit();
}


/*
 * Description:
 * Call-back function to process interrupt by the timer for locking door
 */
void processLockDoor(void){
	g_lockDoorInt++;
}


/* Description:
 * Function to unlock door for 15 seconds
 */
void lockDoor(void){
	g_lockDoorInt = 0;

	/* Configurations for Timer1 to generate an interrupt every 7.5 seconds */
	Timer1_ConfigType Timer1_lockDoorConfigs = {0, 58594, F_CPU_1024, COMPARE_MODE};

	/* Sending the specified call back function to lock door to Timer1 ISR */
	Timer1_setCallBack(processLockDoor);

	/* Timer1 Initializing */
	Timer1_init(&Timer1_lockDoorConfigs);

	/* Rotating the motor A_CW */
	DcMotor_Rotate(A_CW, 50);

	/* Wait until Timer1 counts 15 seconds*/
	while (g_lockDoorInt != 2){}

	/* Stopping the Timer */
	Timer1_deInit();

	/* Stopping the Motor */
	DcMotor_Rotate(STOP, 0);
}

/* Description:
 * Function to verify password
 */
void verifyPassword(void){
	g_passwordConfirmStats = PASSWORD_UNCONFIRMED;
	uint8 passwordErrorCount = 0;

	while (!g_passwordConfirmStats){
		/* Locking the system if user entered 3 unmatched password */
		if (passwordErrorCount == 3){
			lockSystemAction();
			return;
		}

		/* Receive password */
		receivePassword(&g_password);
		g_passwordConfirmStats = checkPassword();

		/* Activating the alarm if the password is wrong */
		if (!g_passwordConfirmStats){
			passwordErrorCount++;
			Buzzer_on();
			_delay_ms(1000);
			Buzzer_off();
		}

		while (UART_recieveByte() != HMI_READY_TO_RECEIVE){}
		UART_sendByte(g_passwordConfirmStats);
	}
}


/* Description:
 * Function to take action in case user choose to open door
 */
void openDoorAction(void){

	verifyPassword();

	if (g_passwordConfirmStats){
		unlockDoor();
		holdDoor();
		lockDoor();
	}
}

/*
 * Description:
 * Function to change system password
 */
void changePasswordProcess(void){

	verifyPassword();

	if (g_passwordConfirmStats){
		g_passwordConfirmStats = PASSWORD_UNCONFIRMED;

		while (!g_passwordConfirmStats){
			createPassword();
		}
		savePassword();
	}
}


/* Description:
 * Function to receive option from HMI ECU and take an action based on it
 */
void processOption(void){
	uint8 option;
	/* Sending an indicator that the Control ECU is ready to receive */
	UART_sendByte(CONTROL_READY_TO_RECEIVE);
	/* Receiving option from HMI ECU */
	option = UART_recieveByte();

	switch (option){
	case '+' :
		openDoorAction();
		break;

	case '-' :
		changePasswordProcess();
		break;
	}
}

