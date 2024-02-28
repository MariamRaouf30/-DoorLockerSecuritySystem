/*
 * dc_motor.c
 *
 *  Created on: Oct 8, 2022
 *      Author: dell
 */


#include "dc_motor.h"
#include <avr/io.h>
#include "gpio.h"


/*
 * Description:
 * Generate a PWM signal with frequency
 * Timer0 will be used with pre-scaler F_CPU/8
 * F_PWM=(F_CPU)/(256*N) = (10^6)/(256*8) = 500 Hz
 * Duty Cycle can be changed by updating the value
 * in The Compare Register
 */
void PWM_Timer0_Start(uint8 duty_cycle){
	TCNT0 = 0;
	OCR0  = duty_cycle;
	DDRB |= (1 << PB3);
	/* Configure timer control register
	 * 1. Fast PWM mode FOC0=0
	 * 2. Fast PWM Mode WGM01=1 & WGM00=1
	 * 3. Clear OC0 when match occurs (non inverted mode) COM00=0 & COM01=1
	 * 4. clock = F_CPU/8 CS00=0 CS01=1 CS02=0
	 */
	TCCR0 = (1 << WGM01) | (1 << WGM00) | (1 << COM01) | (1 << CS01) ;
}

/* Description:
  * Initialize DC Motor Driver
  * Set PIN1 & PIN2 as OUTPUTS
  * Start with stop mode of motor
  */

void DcMotor_Init(void){
	GPIO_setupPinDirection(DCMOTOR_PORT_ID,DCMOTOR_PIN1_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(DCMOTOR_PORT_ID,DCMOTOR_PIN2_ID,PIN_OUTPUT);

	GPIO_writePin(DCMOTOR_PORT_ID,DCMOTOR_PIN1_ID,LOGIC_LOW);
	GPIO_writePin(DCMOTOR_PORT_ID,DCMOTOR_PIN1_ID,LOGIC_LOW);
}



/* Description:
  * Control the DC Motor direction using L293D H-bridge.
  * Control The DC Motor Speed using PWM from MC. connect the
  * PWM PIN of Timer0 OC0(PB3) to the enable of the Motor.
  * duty cycle 100% --> Maximum Speed
  * duty cycle 50%  --> Half Maximum Speed
  * duty cycle 0%   --> Motor Stop
  */

void DcMotor_Rotate(DcMotor_State state,uint8 speed){
	uint8 duty_cycle = ((float)speed/100)*255;
	/*
	* duty cycle variable to adjust percentage of pwm then call PWM with the adjusted value
	*/
	PWM_Timer0_Start(duty_cycle);

	if(state == STOP){
		GPIO_writePin(DCMOTOR_PORT_ID,DCMOTOR_PIN1_ID,LOGIC_LOW);
		GPIO_writePin(DCMOTOR_PORT_ID,DCMOTOR_PIN2_ID,LOGIC_LOW);
	}
	else if(state == CW){
		GPIO_writePin(DCMOTOR_PORT_ID,DCMOTOR_PIN1_ID,LOGIC_HIGH);
		GPIO_writePin(DCMOTOR_PORT_ID,DCMOTOR_PIN2_ID,LOGIC_LOW);
	}
	else if(state == A_CW){
		GPIO_writePin(DCMOTOR_PORT_ID,DCMOTOR_PIN1_ID,LOGIC_LOW);
		GPIO_writePin(DCMOTOR_PORT_ID,DCMOTOR_PIN2_ID,LOGIC_HIGH);
	}


}
