/******************************************************************************
 *
 * Module: Buzzer driver
 *
 * File Name: buzzer.c
 *
 * Description: Buzzer driver source file
 *
 * Author: Mariam Raouf
 *
 *******************************************************************************/

#include "buzzer.h"

/*-------------------------------------------------------------------------------
 *                       Functions Definitions                           		 *
--------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------
 * [Function Name]: Buzzer_init
 *
 * [Description]:  Function to initialize the Buzzer
 *
 * [Args]:        void
 *
 * [Returns]:      Void
 *
 ----------------------------------------------------------------------------------*/
void Buzzer_init(void){
	GPIO_setupPinDirection(BUZZER_PORT_ID, BUZZER_PIN_ID, PIN_OUTPUT);
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_LOW);
}

/*-------------------------------------------------------------------------------
 * [Function Name]: Buzzer_on
 *
 * [Description]:  Function to turn the Buzzer on for alert
 *
 * [Args]:        void
 *
 * [Returns]:      void
 *
 ----------------------------------------------------------------------------------*/
void Buzzer_on(void){
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_HIGH);
}

/*-------------------------------------------------------------------------------
 * [Function Name]: Buzzer_OFF
 *
 * [Description]:  Function to turn the Buzzer off
 *
 * [Args]:        void
 *
 * [Returns]:      void
 *
 ----------------------------------------------------------------------------------*/
void Buzzer_off(void){
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_LOW);
}

