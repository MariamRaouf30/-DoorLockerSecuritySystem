/******************************************************************************
 *
 * Module: Buzzer driver
 *
 * File Name: buzzer.h
 *
 * Description: Buzzer driver header file
 *
 * Author: Mariam Raouf
 *
 *******************************************************************************/

#ifndef BUZZER_H_
#define BUZZER_H_

#include "std_types.h"
#include "gpio.h"

/*-------------------------------------------------------------------------------
 *                       Definitions                         					 *
--------------------------------------------------------------------------------*/
#define BUZZER_PORT_ID 		PORTA_ID
#define BUZZER_PIN_ID		PIN0_ID

/*-------------------------------------------------------------------------------
 *                       Functions Prototypes                          			*
--------------------------------------------------------------------------------*/

/*
 * Description:
 * Function responsible for initializing Buzzer.
 */
void Buzzer_init(void);

/*
 * Description:
 * Function responsible for turning on the Buzzer
 */
void Buzzer_on(void);

/*
 * Description:
 * Function responsible for turning off the Buzzer
 */
void Buzzer_off(void);

#endif /* BUZZER_H_ */
