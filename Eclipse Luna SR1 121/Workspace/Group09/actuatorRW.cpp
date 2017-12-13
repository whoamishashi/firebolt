/*
 * actuatorRW.cpp
 *
 *  Created on: 30.11.2017
 *      Author: Felix Hessinger
 */

#include <stdio.h>
#include "hal.h"
#include "math.h"
#include <string>
#include "actuatorRW.h"

HAL_GPIO HBRIDGE_A_INA(GPIO_036); /* declare HAL_GPIO for GPIO_036 = PC4 (HBRIDGE-A INA pin) */
HAL_GPIO HBRIDGE_A_INB(GPIO_017); /* declare HAL_GPIO for GPIO_017 = PB1 (HBRIDGE-B INA pin) */
HAL_GPIO HBRIDGE_EN(GPIO_066); /* declare HAL_GPIO for GPIO_066 = PE2 (HBRIDGE Power Enable pin) */
HAL_PWM ReactionWheel(PWM_IDX12); /* declare HAL_PWM for PWM_IDX12 = TIM4-CH1 (HBRIDGE-A), please refer to hal_pwm.h for correct PWM mapping*/

ActuatorRW actuatorRW;

ActuatorRW::ActuatorRW() {
}

void ActuatorRW::init() {

	HBRIDGE_EN.init(true, 1, 1); /* initialization of the HAL object should be called one time only in the project*/
	HBRIDGE_A_INA.init(true, 1, 1); /* initialization of the HAL object should be called one time only in the project*/
	HBRIDGE_A_INB.init(true, 1, 0); /* initialization of the HAL object should be called one time only in the project*/
	ReactionWheel.init(5000, 1000); /* initialization of the HAL object should be called one time only in the project*/
	//ReactionWheel.write(250); /* Set the Duty Cycle to 25% */

}

void ActuatorRW::run() {
	while (1) {
		ReactionWheel.write(0);
		HBRIDGE_A_INB.setPins(0); /* initialization of the HAL object should be called one time only in the project*/
		HBRIDGE_A_INA.setPins(1); /* initialization of the HAL object should be called one time only in the project*/
		//ReactionWheel.write(0);

		for (int var = 0; var < 10; ++var) {
			ReactionWheel.write(var*100);
			suspendCallerUntil(NOW()+800*MILLISECONDS);
		}
		ReactionWheel.write(0);
		HBRIDGE_A_INA.setPins(0); /* initialization of the HAL object should be called one time only in the project*/
		HBRIDGE_A_INB.setPins(1); /* initialization of the HAL object should be called one time only in the project*/
		//ReactionWheel.write(2);

		for (int var = 0; var <= 4; (++var)*2) {
			ReactionWheel.write(var*100);
			suspendCallerUntil(NOW()+800*MILLISECONDS);
		}
		//ReactionWheel.write(250); /* Set the Duty Cycle to 25% */
	}
}
