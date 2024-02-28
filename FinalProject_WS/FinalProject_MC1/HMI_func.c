/******************************************************************************
 *
 * Module: Human ECU functions
 *
 * File Name: HMI_ECU.c
 *
 * Description: Functions Executed in Human ECU main function
 *
 * Author: Mariam Raouf
 *
 *******************************************************************************/

#include "HMI_func.h"

/*------------------------------------------------------------------------------
 *                              Global Variables                                *
--------------------------------------------------------------------------------*/
uint8 password[5];
uint8 confirmed_password[5];
uint8 g_ticks = 0;

/*---------------------------------------------------------------------------
 * [Function Name]: HMI_init
 *
 * [Description]:  Function used to initialize Human MCU in the start of the program
 *
 * [Args]:         void
 *
 * [Returns]:      void
 *
 ----------------------------------------------------------------------------------*/
void HMI_init(void){
	/*turn on interrupts*/
	SREG|=(1<<7);
	/* Uart Configuration & Initialization*/
	UART_ConfigType Config_type = {EIGHT_bIT, DISABLED, BIT_1,RATE_9600};
	UART_init(&Config_type);
	/* LCD Initialization*/
	LCD_init();
}

/*---------------------------------------------------------------------------
 * [Function Name]: take_pass
 *
 * [Description]:  Function used to receive password of 5 digits from user
 *
 * [Args]:         pass: array of uint8
 *
 * [Returns]:      void
 *
 ----------------------------------------------------------------------------------*/
void take_pass(uint8 pass[]){
	uint8 i=0;
	uint8 key = KEYPAD_getPressedKey(); /*store value of pressed character in key varible*/
	while(key != '='){ /* while enter '=' is not pressed */
		if(key>=0 && key <= 9){ /*password has to be a number from 0 to 9*/
			if(i<5){ /*length of password = 5 digits*/
				pass[i] = key ;
				LCD_displayCharacter('*'); /*password appears on screen as '*' */
				i++;
			}
		}
		key = KEYPAD_getPressedKey(); /*take value of next digit*/
		_delay_ms(500);
	}
}

/*---------------------------------------------------------------------------
 * [Function Name]: userPASSWORD
 *
 * [Description]:  Function used to take password and confirmation password from user
 * 				   and send both passwords to Control unit
 *
 * [Args]:         void
 *
 * [Returns]:      void
 *
 ----------------------------------------------------------------------------------*/
void userPASSWORD(void){
	LCD_clearScreen(); /*first clear Screen*/
	LCD_displayString("plz enter pass:"); /*take first password message on LCD*/
	LCD_moveCursor(1,0); /*user writes password in second row*/

	take_pass(password); /*take password from user*/
	LCD_clearScreen();
	UART_sendByte(SEND_PASS); /*for MCU1 & MCU2 synchronization to make sure password is sent*/
	send_password(password); /*send password to Control MCU*/

	LCD_displayString("plz reenter pass:"); /*take confirmation password message on LCD*/
	LCD_moveCursor(1,0);

	take_pass(confirmed_password); /*take confirmation password from user*/
	LCD_clearScreen();

	LCD_displayString("comparing pass..."); /*comparing both passwords message*/
	UART_sendByte(AGAIN);
	send_password(confirmed_password);/*send confirmation password to Control MCU*/
}

/*---------------------------------------------------------------------------
 * [Function Name]: send_password
 *
 * [Description]:  Function used to send password to Control unit
 *
 * [Args]:         pass: array of uint8
 *
 * [Returns]:      void
 *
 ----------------------------------------------------------------------------------*/
void send_password(uint8 pass[]){
	/*loop to send each byte of password to Control unit*/
	for(int i = 0; i < 5; i++){
		UART_sendByte(pass[i]);
		_delay_ms(50);
	}
}

/*---------------------------------------------------------------------------
 * [Function Name]: open
 *
 * [Description]:  Function used when user chooses open door function and enters correct password
 * 					to show on lCD steps of unlocking and locking door
 *
 * [Args]:         void
 *
 * [Returns]:      void
 *
 ----------------------------------------------------------------------------------*/
void open(void){
	LCD_clearScreen();
	/* Timer1 Configuration & Initialization*/
	Timer1_ConfigType Config_timer ={0, 8000, F_CPU_1024,CTC_MODE};
	Timer1_init(&Config_timer);
	Timer1_setCallBack(timer_setCallBack);
	/*
	 * make g_ticks zero to start calculation and comparison of time
	 */
	g_ticks = 0;
	LCD_displayString("Door is Unlocking"); /*Door unlocking message on LCD*/
	while(g_ticks < 15); /*Door unlocking for 15 seconds*/
	g_ticks = 0;
	LCD_clearScreen();
	while(g_ticks < 3);/*stop for 3 seconds*/
	g_ticks = 0;
	LCD_displayString("Door is locking"); /*Door Locking message on LCD*/
	while(g_ticks < 15);
	Timer1_deInit();
}

/*---------------------------------------------------------------------------
 * [Function Name]: send_password
 *
 * [Description]:  Function used to show ALERT message on LCD for 60 seconds
 *
 * [Args]:         void
 *
 * [Returns]:      void
 *
 ----------------------------------------------------------------------------------*/
void alert(void){
	LCD_clearScreen();
	/* Timer1 Configuration & Initialization*/
	Timer1_ConfigType Config_timer ={0, 8000, F_CPU_1024,CTC_MODE};
	Timer1_init(&Config_timer);
	Timer1_setCallBack(timer_setCallBack);
	/*
	 * make g_ticks zero to start calculation and comparison of time
	 */
	g_ticks = 0;
	LCD_displayString("ALERT!"); /*ALERT message on LCD*/
	while(g_ticks < 60); /* Alert for 1 minute*/
	Timer1_deInit();
}

/*---------------------------------------------------------------------------
 * [Function Name]: timer_setCallBack
 *
 * [Description]:  Call Back Function for timer1 increments g_ticks every second
 *
 * [Args]:         void
 *
 * [Returns]:      void
 *
 ----------------------------------------------------------------------------------*/
void timer_setCallBack(void){
	g_ticks++;
}


