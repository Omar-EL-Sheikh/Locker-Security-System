/***************************************************************************
 *
 * Module Name: HMI ECU
 *
 * File Name: HMI_Application.c
 *
 * Description: Source file for HMI ECU Application
 *
 * Created on: Oct 30, 2022
 *
 * Author: Omar EL-Sheikh
 *
 **************************************************************************/

/**************************************************************************
 *								Inclusions
 *************************************************************************/
#include "gpio.h"
#include "std_types.h"
#include "lcd.h"
#include <util/delay.h>
#include "uart.h"
#include "timer1.h"
#include "keypad.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/**************************************************************************
 *								 Definitions
 *************************************************************************/

#define PASSWORD_SIZE 5
#define PASSWORD_ENTER_KEY 13
#define CONTROL_READY_TO_RECEIVE 0XAA
#define HMI_READY_TO_RECEIVE 0XBB
#define PASSWORD_CONFIRMED TRUE
#define PASSWORD_UNCONFIRMED FALSE

/**************************************************************************
 *								 Global Variables
 *************************************************************************/

/* Global variable to store system password */
uint8 g_password [PASSWORD_SIZE];

/*Boolean variable to confirm password */
boolean g_passwordConfirm = PASSWORD_UNCONFIRMED;

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
 * Function to take password from user
 */
void getPassword(void);

/*
 * Description:
 * Function to send system password to Control ECU by UART
 */
void sendPassword(void);

/*
 * Description:
 * Function to create or change system password
 */
void createPassword (void);

/*
 * Description:
 * Function to lock the system for 1 minute
 */
void lockSystem(void);

/*
 * Description:
 * Call-back function to process interrupt by the timer for unlocking door
 */
void processUnlockDoor(void);

/* Description:
 * Function to unlock door for 15 seconds
 */
void unlockDoorMessage(void);

/*
 * Description:
 * Call-back function to process interrupt by timer in case of holding door
 */
void processHoldDoor(void);

/*
 * Description:
 * Function to hold the door for 3 seconds
 */
void holdDoorMessage(void);

/*
 * Description:
 * Call-back function to process interrupt by the timer for locking door
 */
void processlockDoor(void);

/* Description:
 * Function to unlock door for 15 seconds
 */
void lockDoorMessage(void);

/* Description:
 * Function to verify password from user
 */
void verifyPassword(void);

/*
 * Description:
 * Function to process data in case of user choose to open door
 */
void openDoor(void);

/*
 * Description:
 * Function to take new password from user and send to Control ECU to change system password
 */
void changePassword(void);

/*
 * Description:
 * Function to display main system options
 * Take an option from user
 * Send the option to Control ECU
 */
void mainOptions(void);


/**************************************************************************
 *								Main Program Execution
 *************************************************************************/

int main(void){
	sei(); /* Enabling Global Interrupt */
	Drivers_Init(); /* Initializing all required Drivers */

	/* Asking user to create password and confirm until a confirmation occurs  */
	while (!g_passwordConfirm){
		createPassword();
	}

	/* Program Flow */
	while(1){
		mainOptions(); /* Displaying and processing main options all the time */
	}
}

/**************************************************************************
 *								Functions Definitions
 *************************************************************************/

/*
 * Description:
 * Function to initialize all required Drivers
 */
void Drivers_Init(void){
	/* Variable to store UART Configurations */
	UART_ConfigType UART_Configs = {BITS_8, NO_PARITY, BIT_1, BAUD_RATE_9600};
	UART_init(&UART_Configs);
	LCD_init();
}

/*
 * Description:
 * Function to take password from user by keypad
 */
void getPassword(void){
	uint8 temp = 0;
	uint8 counter = 0;

	/* Receiving exactly 5-digits password from user before user presses Enter */
	while (temp != PASSWORD_ENTER_KEY || counter < PASSWORD_SIZE){
		temp = KEYPAD_getPressedKey();

		/* Accepting only digits from 0 to 9 */
		if (temp <= 9 && counter < PASSWORD_SIZE){
			g_password[counter] = temp;
			LCD_displayCharacter('*');
			counter++;
		}

		_delay_ms(500);
	}
}

/*
 * Description:
 * Function to create or change system password
 */
void createPassword (void){
	g_passwordConfirm = PASSWORD_UNCONFIRMED;

	LCD_clearScreen();
	LCD_displayString("Please Enter");
	LCD_moveCursor(1,0);
	LCD_displayString("New Password: ");
	LCD_moveCursor(2,0);

	getPassword(); /* Receiving password from user */
	sendPassword(); /* Sending password to Control ECU */

	LCD_clearScreen();
	LCD_moveCursor(0,0);
	LCD_displayString("Please Re-enter");
	LCD_moveCursor(1,0);
	LCD_displayString("Password: ");

	getPassword(); /* Receiving password again from user for confirmation */
	sendPassword(); /* Sending password again to be confirmed */

	/* Sending an indicator that the HMI ECU is ready to receive confirmation from Control ECU */
	UART_sendByte(HMI_READY_TO_RECEIVE);
	/* Receiving confirmation from Control ECU */
	g_passwordConfirm = UART_recieveByte();
}

/*
 * Description:
 * Function to send system password to Control ECU by UART
 */
void sendPassword(void){
	uint8 counter;

	/* Waiting for Control ECU to be ready to receive data */
	while (UART_recieveByte() != CONTROL_READY_TO_RECEIVE);

	/* Sending password by UART */
	for (counter = 0; counter < PASSWORD_SIZE; counter++){
		UART_sendByte(g_password[counter]);
	}

}

/*
 * Description:
 * Function to lock the system for 1 minute
 */
void lockSystem(void){
	sint8 counter;

	LCD_clearScreen();
	LCD_displayString("System is Locked");
	LCD_moveCursor(1,0);
	LCD_displayString("Unlocking in ");

	/* Freezing the system for 60 seconds */
	for (counter = 59; counter >= 0; counter--){
		LCD_moveCursor(1,14);
		LCD_intgerToString(counter);
		if(counter < 10){
			LCD_displayCharacter(' ');
		}
		_delay_ms(1000);
	}
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
void unlockDoorMessage(void){
	g_unlockDoorInt = 0;

	/* Configurations for Timer1 to generate an interrupt every 7.5 seconds */
	Timer1_ConfigType Timer1_UnlockDoorConfigs = {0, 58594, F_CPU_1024, COMPARE_MODE};

	/* Sending the specified call back function to unlock door to Timer1 ISR */
	Timer1_setCallBack(processUnlockDoor);

	/* Initializing Timer1  */
	Timer1_init(&Timer1_UnlockDoorConfigs);

	LCD_clearScreen();
	LCD_displayString("Unlocking...");

	/* Wait until Timer1 counts 15 seconds*/
	while (g_unlockDoorInt != 2){}

	/* Stopping the Timer */
	Timer1_deInit();
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
void holdDoorMessage(void){
	g_holdDoorInt = 0;

	/* Configurations for Timer1 to generate an interrupt every 3 seconds */
	Timer1_ConfigType Timer1_HoldDoorConfigs = {0, 23438, F_CPU_1024, COMPARE_MODE};

	/* Sending the specified call back function to door door to Timer1 ISR */
	Timer1_setCallBack(processHoldDoor);

	/* Timer1 Initializing */
	Timer1_init(&Timer1_HoldDoorConfigs);

	LCD_clearScreen();
	LCD_displayString("Door is Open !");

	/* Wait until Timer1 counts 3 seconds */
	while (g_holdDoorInt != 1){}

	/* Stopping the Timer */
	Timer1_deInit();
}


/*
 * Description:
 * Call-back function to process interrupt by the timer for locking door
 */
void processlockDoor(void){
	g_lockDoorInt++;
}


/* Description:
 * Function to unlock door for 15 seconds
 */
void lockDoorMessage(void){
	g_lockDoorInt = 0;

	/* Configurations for Timer1 to generate an interrupt every 7.5 seconds */
	Timer1_ConfigType Timer1_lockDoorConfigs = {0, 58594, F_CPU_1024, COMPARE_MODE};

	/* Sending the specified call back function to lock door to Timer1 ISR */
	Timer1_setCallBack(processlockDoor);

	/* Timer1 Initializing */
	Timer1_init(&Timer1_lockDoorConfigs);

	LCD_clearScreen();
	LCD_displayString("Locking...");

	/* Wait until Timer1 counts 15 seconds*/
	while (g_lockDoorInt != 2){}

	/* Stopping the Timer */
	Timer1_deInit();
}

/* Description:
 * Function to verify password from user
 */
void verifyPassword(void){
	g_passwordConfirm = PASSWORD_UNCONFIRMED;
	uint8 passwordErrorCount = 0;

	while(!g_passwordConfirm){

		/* Locking the system if user entered 3 unmatched password */
		if (passwordErrorCount == 3){
			lockSystem();
			return;
		}

		LCD_clearScreen();
		LCD_displayString("Please Enter");
		LCD_moveCursor(1,0);
		LCD_displayString("Password: ");

		/* Receiving password from user to open the door */
		getPassword();
		sendPassword();

		/* Sending an indicator that the HMI ECU is ready to receive confirmation from Control ECU */
		UART_sendByte(HMI_READY_TO_RECEIVE);
		/* Receiving confirmation from Control ECU */
		g_passwordConfirm = UART_recieveByte();

		/* Displaying an error message if the password is wrong */
		if (!g_passwordConfirm){
			passwordErrorCount++;
			LCD_clearScreen();
			LCD_displayString("Wrong Password !");
			_delay_ms(1000);
		}
	}
}


/*
 * Description:
 * Function to process data in case of user choose to open door
 */
void openDoor(void){

	/* Verifying password before processing */
	verifyPassword();

	if (g_passwordConfirm){
		unlockDoorMessage();
		holdDoorMessage();
		lockDoorMessage();
	}
}

/*
 * Description:
 * Function to take new password from user and send to Control ECU to change system password
 */
void changePassword(void){

	/* Verifying password before processing */
	verifyPassword();

	/* Receiving new password and new password confirmation */
	if (g_passwordConfirm){
		g_passwordConfirm = PASSWORD_UNCONFIRMED;

		while (!g_passwordConfirm){
			createPassword();
		}

		LCD_clearScreen();
		LCD_displayString("Password Changed");
		LCD_moveCursor(1, 0);
		LCD_displayString("Successfully !");
		_delay_ms(1000);
	}
}

/*
 * Description:
 * Function to display main system options
 * Take an option from user
 * Send the option to Control ECU  */
void mainOptions(void){
	uint8 option = '\0';
	LCD_clearScreen();
	LCD_displayString(" + : Open Door ");
	LCD_moveCursor(1,0);
	LCD_displayString(" - : Change Pass ");

	/* Taking input from Keypad until user enters a valid button*/
	while (option != '+' && option != '-'){
		option = KEYPAD_getPressedKey();
		_delay_ms(500);
	}

	/* Waiting for Control ECU to be ready to receive data */
	while (UART_recieveByte() != CONTROL_READY_TO_RECEIVE);
	/* Sending Option to Control ECU */
	UART_sendByte(option);

	switch (option){
	case '+' :
		openDoor();
		break;

	case '-' :
		changePassword();
		break;
	}
}
