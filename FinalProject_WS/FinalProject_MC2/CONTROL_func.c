/******************************************************************************
 *
 * Module: Control ECU functions
 *
 * File Name: CONTROL_func.c
 *
 * Description: Functions Executed in Control ECU main function
 *
 * Author: Mariam Raouf
 *
 *******************************************************************************/
#include "CONTROL_func.h"

/*------------------------------------------------------------------------------
 *                              Global Variables                                *
--------------------------------------------------------------------------------*/
uint8 flag;
uint8 g_ticks ;
uint8 g_password[5];
uint8 password[5];
uint8 confirmed_password[5];
uint8 confirm_pass;
uint8 eeprom_password[5];


/*---------------------------------------------------------------------------
 * [Function Name]: controlECU_init
 *
 * [Description]:  Function used to initialize CONTROL UNIT in the start of the program
 *
 * [Args]:         void
 *
 * [Returns]:      void
 *
 ----------------------------------------------------------------------------------*/
void controlECU_init(void){
	/*turn on interrupts*/
	SREG |=(1<<7);
	/* Uart Configuration & Initialization*/
	UART_ConfigType Config_UART = {EIGHT_bIT, DISABLED, BIT_1,RATE_9600};
	UART_init(&Config_UART);
	/* DC motor Initialization*/
	DcMotor_Init();
	/* TWI Configuration & Initialization*/
	TWI_ConfigType Config_TWI ={ 0b00000010 , 0x02};
	TWI_init(&Config_TWI);
	/* Buzzer Initialization*/
	Buzzer_init();
}

/*---------------------------------------------------------------------------
 * [Function Name]: receive_password
 *
 * [Description]:  Function used to receive password of 5 digits from Human MCU
 *
 * [Args]:         pass: array of uint8
 *
 * [Returns]:      void
 *
 ----------------------------------------------------------------------------------*/
void receive_password(uint8 pass[5]){
	/* for loop to receive each one of 5 bytes of password */
	for (int i = 0; i < 5; i++){
		pass[i] = UART_recieveByte();
	}
}

/*---------------------------------------------------------------------------
 * [Function Name]: compare_passwords
 *
 * [Description]:  Function used to compare between 2 passwords and raise flag if not equal
 *
 * [Args]:         pass1, pass2: arrays of uint8
 *
 * [Returns]:      void
 *
 ----------------------------------------------------------------------------------*/
void compare_passwords(uint8 pass1[],uint8 pass2[]){
	flag = 0;
	for(int i = 0; i<5; i++){
		if(pass1[i] != pass2[i]){
			flag = 1; /* if any of the characters dont't match raise a flag and break*/
			break;
		}
	}
}

/*---------------------------------------------------------------------------
 * [Function Name]: save_password
 *
 * [Description]:  Function used to save password in eeprom
 *
 * [Args]:         pass: array of uint8
 *
 * [Returns]:      void
 *
 ----------------------------------------------------------------------------------*/
void save_password(uint8 pass[]){
	/*
		 * looping on every number in (array parameter)password and store this number in specific
		 * 		address in memory
		 */
	for(int i = 0 ; i<5; i++){
		EEPROM_writeByte( EEPROM_ADDREESS+i, pass[i]);
		_delay_ms(10);
	}
}

/*---------------------------------------------------------------------------
 * [Function Name]: get_password
 *
 * [Description]:  Function used to read password from eeprom in a global variable
 *
 * [Args]:         void
 *
 * [Returns]:      void
 *
 ----------------------------------------------------------------------------------*/
void get_password(void){
	/*
	 * looping on every number in (array parameter)password saved in eeprom and store this number
	 * in specific address
	 */
	for(int i=0;i<5;i++){
		EEPROM_readByte(EEPROM_ADDREESS+i, &eeprom_password[i]);
		_delay_ms(10);
	}

}

/*---------------------------------------------------------------------------
 * [Function Name]: open_door
 *
 * [Description]:  Function used to turn on motor/open door for 15 seconds wait for 3 seconds then re-locking the door
 * 				   turn on motor Anti-Clockwise for another 15 seconds
 *
 * [Args]:         void
 *
 * [Returns]:      void
 *
 ----------------------------------------------------------------------------------*/
void open_door(void){
	    /* Timer1 Configuration & Initialization*/
	    Timer1_ConfigType Config_timer ={0, 8000, F_CPU_1024,CTC_MODE};
		Timer1_init(&Config_timer);
		Timer1_setCallBack(timer_setCallBack);
		/*
		 * make g_ticks zero to start calculation and comparison of time
		 */
		g_ticks = 0;
		DcMotor_Rotate(CW,100);
		/*
		 * wait until door next state
		 */
		while (g_ticks < 15);

		g_ticks = 0;
		DcMotor_Rotate(STOP,0);
		/*
		 * wait until door next state
		 */
		while (g_ticks < 3);
		g_ticks = 0;
		DcMotor_Rotate(A_CW,100);
		/*
		 * wait until door next state
		 */
		while (g_ticks < 15);
		DcMotor_Rotate(STOP,0);
		Timer1_deInit();

}

/*---------------------------------------------------------------------------
 * [Function Name]: Alert
 *
 * [Description]:  Function used to turn on alert by turning on buzzer for 60 seconds
 *
 * [Args]:         void
 *
 * [Returns]:      void
 *
 ----------------------------------------------------------------------------------*/
void Alert(void){
	/* Timer1 Configuration & Initialization*/
	Timer1_ConfigType Config_timer ={0, 8000, F_CPU_1024,CTC_MODE};
	Timer1_init(&Config_timer);
	Timer1_setCallBack(timer_setCallBack);
	/*
	 * make g_ticks zero to start calculation and comparison of time
	 */
	g_ticks = 0;
	Buzzer_on();
	while(g_ticks < 60);
	Buzzer_off();
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



