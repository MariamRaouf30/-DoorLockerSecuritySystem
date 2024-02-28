/******************************************************************************
 *
 * Module: Control ECU Functions
 *
 * File Name: HMI_ECU.c
 *
 * Description: Source file Human ECU Main Function
 *
 * Author: Mariam Raouf
 *
 *******************************************************************************/


#include "HMI_func.h"

uint8 count_error = 0;

uint8 g_password[5];
uint8 received_byte=0;


int main(void){
	/*Initialize Human Unit*/
	HMI_init();
	state = 0; /*begin with state = 0, start state*/
	while(1){
		/*switch cases for different cases of program*/
		switch(state){
		case start: /*first case*/
			userPASSWORD(); /*take password and confirmation password from user*/
			received_byte = UART_recieveByte(); /*receive response byte from control unit to compare passwords*/
			if (received_byte == MATCHED ){ /* if passwords match*/
				LCD_clearScreen();
				LCD_displayString("MATCHED"); /*display matched message for user*/
				_delay_ms(500);
				state = Main_menu; /*Go to main menu if passwords match to choose desired step*/
			}
			else if (received_byte == UNMATCHED ){ /*if passwords don't match*/
				LCD_clearScreen();
				LCD_displayString("PASSWORD UNMATCHED"); /*display passwords don't match message*/
				_delay_ms(500);
				state = start; /*stay in case 0 repeat until passwords match*/
			}
			break;
		case user_password :
			userPASSWORD(); /*user change password case*/
			state++;
			break;
		case confirmation : /*confirm password and confirmation password*/
			received_byte = UART_recieveByte(); /*receive confirmation byte from control*/
			if(received_byte == MATCHED){
				LCD_clearScreen();
				LCD_displayString("MATCHED");
				_delay_ms(500);
				state = Main_menu;
				/*if passwords matched display on LCD for user then go to main menu*/
			}
			else if(received_byte == UNMATCHED){
				state = change_error;
				count_error++;
				/*if passwords unmatched go to change password error state and increment error counter*/
			}
			break;
		case Main_menu:
			LCD_clearScreen();
			LCD_displayString("+: open door");
			LCD_moveCursor(1,0);
			LCD_displayString("-: change pass");
			/*Messages appearing on LCD giving choices to user*/
			uint8 key = KEYPAD_getPressedKey(); /*take input from user*/
			_delay_ms(500);
			if (key == '+'){ /*if user chooses to open door*/
				LCD_clearScreen();
				LCD_displayString("enter saved pass:");  /*ask for saved password*/
				LCD_moveCursor(1,0);
				take_pass(g_password);
				UART_sendByte(GSEND_PASS);
				send_password(g_password);
				/*receive password from user and send it to control unit to compare with stored password*/
				received_byte = UART_recieveByte(); /*receive whether passwords match or not from Control unit*/
				if (received_byte == MATCHED ){
					LCD_clearScreen();
					state = open_door;
					/*if password matched saved password go to open door state*/
				}
				else if (received_byte == UNMATCHED ){
					state = Unlock_error;
					count_error++;
					/*if passwords don't match go to unlock door error state and increment error counter*/
				}
			}
			else if (key == '-'){
				state = user_password;
				/*if user chooses to change password go to user password state*/
			}
			break;
		case Unlock_error:
			while(count_error<=3){
				if(count_error == 3){
					state = Alert;
					count_error = 0;
					break;
				}
				/*if user types incorrect password 3 consecutive times go to alert state*/
				LCD_clearScreen();
				LCD_displayString("PASSWORD UNMATCHED");/*passwords unmatched message*/
				_delay_ms(500);
				LCD_clearScreen();
				LCD_displayString("plz enter pass:");
				LCD_moveCursor(1,0);
				take_pass(password);
				/*take password one more time from user*/
				UART_sendByte(ERROR1_CHECK);/*open door error to control unit*/
				send_password(password);/*send password*/
				received_byte = UART_recieveByte();/*wait for comparison result*/
				/*if passwords match and count error is less than 3 go to next state*/
				if (received_byte == MATCHED ){
					state = open_door;
					count_error = 0;
					break;
				}
				if (received_byte == UNMATCHED ){
					count_error++;
				}
				/*if passwords unmatched one more time increment counter*/
			}
			break;

		case change_error:
			while(count_error<=3){ /* while error counter is less than or equals 3*/
				if(count_error == 3){
					state = Alert;
					count_error = 0;
					break;
				}
				/*if user types incorrect password 3 consecutive times go to alert state*/
				LCD_clearScreen();
				LCD_displayString("PASSWORD UNMATCHED"); /*passwords unmatched message*/
				_delay_ms(500);
				LCD_clearScreen();
				LCD_displayString("plz enter pass:");
				LCD_moveCursor(1,0);
				take_pass(password);
				/*take password one more time from user*/
				UART_sendByte(ERROR2_CHECK); /*change password error to control unit*/
				send_password(password); /*send password*/
				received_byte = UART_recieveByte(); /*wait for comparison result*/
				/*if passwords match and count error is less than 3 go to next state*/
				if (received_byte == MATCHED ){
					LCD_clearScreen();
					LCD_displayString("SAVED");
					_delay_ms(200);
					state = Main_menu;
					count_error = 0;
					break;
				}
				if (received_byte == UNMATCHED ){
					count_error++;
				}
				/*if passwords unmatched one more time increment counter*/
			}
			break;
		case open_door:
			UART_sendByte(OPEN); /*send control unit OPEN Byte to open door*/
			open(); /*open function controls messages displaying on LCD during door opening*/
			state = Main_menu; /*go to main menu state after opening door function returns/executes */
			break;
		case Alert:
			UART_sendByte(ALERT); /*send Alert Byte to control unit to turn on buzzer*/
			alert(); /* function Display alert message on LCD during alert state*/
			state = Main_menu; /*return to main menu after function executes*/
			break;
		}

	}
}




