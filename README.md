# Locker-Security-System
Description:
A door locker security system to unlock door using registered password on two ATmega32 
microcontrollers.
The system uses Two ECUs connected using UART.
The first ECU (HMI) is responsible for interfacing with the Keypad for input and LCD for
displaying messages.
The second ECU (CONTROL) is responsible for all the processing and decisions in the system like 
password checking, opening the door and activating the system alarm.
The system is designed to take password from user for the first time and save it in the EEPROM.
The main menu has two options Open Door or Change Password which both require password 
confirmation from user, if the password is wrong for three consecutive times, the system makes 
an alert using the buzzer and locked for one minute.

Programming Language(s):
This project is written in C programming language.

Hardware Implementation:
This project is implemented on two ATmega32 MC with a frequency of 8Mhz connected to the 
following components:
  - Keypad
  - LM016L LCD
  - DC Motor
  - L293D Motor Driver
  - M24C16 EEPROM
  - Buzzer
Drivers:
  - GPIO
  - UART
  - I2C or TWI
  - DC Motor
  - LCD
  - Timer
  - EEPROM
  - Buzzer
