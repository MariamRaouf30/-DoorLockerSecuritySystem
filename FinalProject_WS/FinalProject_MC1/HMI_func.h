/******************************************************************************
 *
 * Module: Human ECU Functions
 *
 * File Name: CONTROL_func.h
 *
 * Description: Header file for Human ECU functions when Turning on Program
 *
 * Author: Mariam Raouf
 *
 *******************************************************************************/

#ifndef HMI_FUNC_H_
#define HMI_FUNC_H_

#include "lcd.h"
#include "keypad.h"
#include <util/delay.h>
#include "uart.h"
#include <avr/io.h>
#include "timer1.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* Common messages between Control MCU & Human MCU */
#define AGAIN 		0x10
#define MATCHED 	0x01
#define UNMATCHED   0x03
#define ALERT 		0x02
#define OPEN 		0x04
#define SEND_PASS	0x05
#define CHECK		0x06
#define GSEND_PASS  0x07
#define READY 		0X08
#define ERROR1_CHECK 0x09
#define ERROR2_CHECK 0x020

/* States of Human ECU*/
enum states{
	start,user_password, confirmation, Unlock_error,change_error , Main_menu , open_door, Alert
}state;

/*******************************************************************************
 *                              Global Variables                                *
 *******************************************************************************/
extern uint8 g_ticks;
extern uint8 password[5];
extern uint8 confirmed_password[5];

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description:
 * Function used to initialize Human MCU in the start of the program
 */
void HMI_init(void);

/*
 * Description:
 * Function used to receive password of 5 digits from user
 */
void take_pass(uint8 pass[]);

/*
 * Description:
 *Function used to take password and confirmation password from user
 * and send both passwords to Control unit
 */
void userPASSWORD(void);

/*
 * Description:
 * Function used to send password to Control unit
 */
void send_password(uint8 pass[]);

/*
 * Description:
 * Function used to show on lCD steps of unlocking and locking door
 */
void open(void);

/*
 * Description:
 * Function used to show ALERT message on LCD for 60 seconds
 */
void alert(void);

/*
 * Description:
 * Call Back Function for timer1 increments g_ticks every second
 */
void timer_setCallBack(void);

#endif /* HMI_FUNC_H_ */
