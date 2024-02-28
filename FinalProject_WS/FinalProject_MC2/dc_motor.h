/*
 * dc_motor.h
 *
 *  Created on: Oct 8, 2022
 *      Author: dell
 */

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "std_types.h"


#define DCMOTOR_PORT_ID			PORTB_ID
#define DCMOTOR_PIN1_ID			PIN0_ID
#define DCMOTOR_PIN2_ID			PIN1_ID

typedef enum{
	STOP,CW,A_CW
}DcMotor_State;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

void PWM_Timer0_Start(uint8 duty_cycle);

/* Description:
  * Function responsible of initialization of DC Motor Driver
  */
void DcMotor_Init(void);

/* Description:
  * Function that control DC Motor direction & speed using PWM from MC. connect the
  * PWM PIN of Timer0 OC0(PB3) to the enable of the Motor.
  */
void DcMotor_Rotate(DcMotor_State state,uint8 speed);


#endif /* DC_MOTOR_H_ */
