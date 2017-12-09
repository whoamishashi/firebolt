/*
 * actuatorThruster.cpp
 *
 *  Created on: 9 Dec 2017
 *      Author: felix
 */

#include "actuatorThruster.h"

#include "rodos.h"
#include <stdio.h>
#include "hal.h"
#include "math.h"
#include <string>

HAL_GPIO Thruster1(GPIO_072); /* declare HAL_GPIO for GPIO_072 = PE8 (HBRIDGE-C INA pin) */
HAL_GPIO Thruster2(GPIO_074); /* declare HAL_GPIO for GPIO_074 = PE10 (HBRIDGE-C INB pin) */
HAL_GPIO Thruster3(GPIO_076); /* declare HAL_GPIO for GPIO_076 = P12 (HBRIDGE-D INA pin) */
//HAL_GPIO HBRIDGE_EN(GPIO_066); /* declare HAL_GPIO for GPIO_066 = PE2 (HBRIDGE Power Enable pin) */

//HAL_GPIO Servo1(GPIO_073); /* declare HAL_GPIO for GPIO_036 = PC4 (HBRIDGE-A INA pin) */

ActuatorThruster actuatorThruster;

ActuatorThruster::ActuatorThruster() {
}

void ActuatorThruster::init() {
//	HBRIDGE_EN.init(true, 1, 1); /* initialization of the HAL object should be called one time only in the project*/
	Thruster1.init(true, 1, 0);  /* initialization of the HAL object should be called one time only in the project*/
	Thruster2.init(true, 1, 0);  /* initialization of the HAL object should be called one time only in the project*/
	Thruster3.init(true, 1, 0);  /* initialization of the HAL object should be called one time only in the project*/
}

void ActuatorThruster::run() {
	while (1) {
		//activate thruster1
		Thruster1.init(true, 1, 1); /* initialization of the HAL object should be called one time only in the project*/
		suspendCallerUntil(NOW()+1000*MILLISECONDS);
		//deactivate thruster1
		Thruster1.init(true, 1, 0); /* initialization of the HAL object should be called one time only in the project*/
		suspendCallerUntil(NOW()+1000*MILLISECONDS);

	}
}

