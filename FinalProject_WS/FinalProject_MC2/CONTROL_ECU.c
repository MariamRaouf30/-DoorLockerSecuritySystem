/******************************************************************************
 *
 * Module: Control ECU Main Function
 *
 * File Name: CONTROL_ECU.c
 *
 * Description: Source file Control ECU Main function
 *
 * Author: Mariam Raouf
 *
 *******************************************************************************/

#include "CONTROL_func.h"

uint8 pass[5];

int main(void){
	/*Initialize Control Unit*/
	controlECU_init();
	uint8 receive_byte ;
	while(1){
		/*receive byte from human unit to decide condition and current state actions*/
		receive_byte = UART_recieveByte();
		if (receive_byte == SEND_PASS){ /*human unit is sending password case*/
			receive_password(password); /*receive password from human unit*/
			while(UART_recieveByte() != AGAIN); /*wait for confirmation password*/
			receive_password(confirmed_password); /*receive confirmation password*/
			compare_passwords(password,confirmed_password); /*compare password and confirmation password*/
			if(flag == 0){
				UART_sendByte(MATCHED);
				save_password(password); /*if passwords match save password in memory*/
			}
			if(flag == 1 ){
				UART_sendByte(UNMATCHED);
			}
			/*send result of comparison based on flag*/
		}
		if (receive_byte == GSEND_PASS){ /*receiving saved password when user chooses to open door */
			receive_password(pass); /*receive password from human unit*/
			get_password(); /*read password saved in memory in a global variable*/
			compare_passwords(eeprom_password,pass); /*compare passwords sent with password saved in memory*/
			if(flag == 0){
				UART_sendByte(MATCHED);
			}
			if(flag == 1 ){
				UART_sendByte(UNMATCHED);
			}
			/*send result of comparison based on flag*/
		}
		if (receive_byte == OPEN){/* human unit sent open door byte condition*/
			open_door(); /*execute open door function*/
		}
		if (receive_byte == ALERT){/* human unit sent Alert byte condition*/
			Alert(); /*execute Alert function*/
		}
		if(receive_byte == ERROR1_CHECK){/* human unit sent ERROR1 byte condition*/
			uint8 v_pass[5];
			receive_password(v_pass);
			compare_passwords(eeprom_password,v_pass);/*compare passwords sent with password saved in memory*/
			if(flag == 0){
				UART_sendByte(MATCHED);
			}
			if(flag == 1){
				UART_sendByte(UNMATCHED);
			}
			/*send result of comparison based on flag*/
		}
		if(receive_byte == ERROR2_CHECK){/* human unit sent ERROR2 byte condition*/
			uint8 v_pass1[5];
			receive_password(v_pass1);
			compare_passwords(password,v_pass1);/*compare passwords sent with first sent password */
			if(flag == 0){
				UART_sendByte(MATCHED);/*if passwords match save password in memory and change password is done*/
				save_password(v_pass1);
			}
			if(flag == 1){
				UART_sendByte(UNMATCHED);
			}
			/*send result of comparison based on flag*/
		}
	}
}
