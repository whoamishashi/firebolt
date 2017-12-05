/*
 * actuatorServo.cpp
 *
 *  Created on: 05.12.2017
 *      Author: I8FL-PC01-G01
 */


#include "actuatorServo.h"

#include "rodos.h"
#include <stdio.h>
#include "hal.h"
#include "math.h"
#include <string>

//HAL_GPIO HBRIDGE_A_INA(GPIO_036); /* declare HAL_GPIO for GPIO_036 = PC4 (HBRIDGE-A INA pin) */
//HAL_GPIO HBRIDGE_A_INB(GPIO_017); /* declare HAL_GPIO for GPIO_017 = PB1 (HBRIDGE-B INA pin) */
//HAL_GPIO HBRIDGE_EN(GPIO_066); /* declare HAL_GPIO for GPIO_066 = PE2 (HBRIDGE Power Enable pin) */

HAL_GPIO Servo1(GPIO_073); /* declare HAL_GPIO for GPIO_036 = PC4 (HBRIDGE-A INA pin) */
HAL_PWM Servo01(PWM_IDX00);

ActuatorServo actuatorServo;

ActuatorServo::ActuatorServo() {
}

void ActuatorServo::init() {
	Servo1.init(true, 1, 1); /* initialization of the HAL object should be called one time only in the project*/
	Servo01.init(5000,1000);
}

void ActuatorServo::run() {
	while (1) {

		//Servo01.write(750);
		for (int var = 0; var <= 3; ++var) {
			Servo01.write(var*50);
			suspendCallerUntil(NOW()+800*MILLISECONDS);
		}
		suspendCallerUntil(NOW()+500*MILLISECONDS);

		for (int var = 0; var <= 3; ++var) {
			Servo01.write(var*50);
			suspendCallerUntil(NOW()+800*MILLISECONDS);
		}
		suspendCallerUntil(NOW()+500*MILLISECONDS);
	}
}

