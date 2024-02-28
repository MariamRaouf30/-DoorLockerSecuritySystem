 /******************************************************************************
 *
 * Module: Timer1
 *
 * File Name: timer1.h
 *
 * Description: Header file for the Timer1 driver
 *
 * Author: Mariam Raouf
 *
 *******************************************************************************/

#ifndef TIMER1_H_
#define TIMER1_H_

#include "std_types.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum{
	F_CPU_,F_CPU_1,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024,EXTERNAL_FALLING_EDGE,EXTERNAL_RISING_EDGE
}Timer1_Prescaler;

typedef enum{
	NORMAL_MODE,CTC_MODE
}Timer1_Mode;

/*-------------------------------------------------------------------------------
 * [Structure Name]: Timer1_ConfigType
 *
 * [Description]: This structure is responsible for maintaining information about the Timer1 configuration
 ----------------------------------------------------------------------------------*/
typedef struct {
 uint16 initial_value;
 uint16 compare_value; // it will be used in compare mode only.
 Timer1_Prescaler prescaler;
 Timer1_Mode mode;
} Timer1_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description:
 * Function responsible for initializing Timer1 Module.
 */
void Timer1_init(const Timer1_ConfigType * Config_Ptr);

/*
 * Description:
 * Function responsible for turning off (De-initalizing) Timer1
 */
void Timer1_deInit(void);

/*
 * Description:
 * Function responsible for setting the CallBack Function of Timer1
 */
void Timer1_setCallBack(void(*a_ptr)(void));


#endif /* TIMER1_H_ */
