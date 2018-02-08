/*
 * actuatorThruster.cpp
 *
 *  Created on: 9 Dec 2017
 *      Author: felix
 */

#include "../../lib/actuators/actuatorThruster.h"
#include "rodos.h"
#include <stdio.h>
#include "hal.h"
#include "math.h"
#include <string>


using namespace std;

using std::string;
// all 3 HAL_GPIO for Thrusters
HAL_GPIO Thruster1(HBRIDGEDA); /* declare HAL_GPIO for GPIO_072 = PE8 (HBRIDGE-C INA pin) */
HAL_GPIO Thruster2(HBRIDGEDB); /* declare HAL_GPIO for GPIO_074 = PE10 (HBRIDGE-C INB pin) */
HAL_GPIO Thruster3(HBRIDGEAA); /* declare HAL_GPIO for GPIO_076 = P12 (HBRIDGE-D INA pin) */

////HAL_GPIO HBRIDGE_EN(GPIO_066); /* declare HAL_GPIO for GPIO_066 = PE2 (HBRIDGE Power Enable pin) */

////HAL_GPIO Servo1(GPIO_073); /* declare HAL_GPIO for GPIO_036 = PC4 (HBRIDGE-A INA pin) */

ActuatorThruster actuatorThruster;

CommBuffer<TelecommandData> ActuatorThruster_TelecommandDataBuffer3;
Subscriber ActuatorThruster_TelecommandDataSubscriber3(TelecommandDataTopic,
		ActuatorThruster_TelecommandDataBuffer3);

CommBuffer<ControlData> ControlDataBuffer3;
Subscriber ControlDataSubscriber3(ControlDataTopic, ControlDataBuffer3);

double thrust;
int x;

ActuatorThruster::ActuatorThruster() {
	t1 = t2 = t3 = 0;
}

void ActuatorThruster::init() {
//	HBRIDGE_EN.init(true, 1, 1); /* initialization of the HAL object should be called one time only in the project*/
	Thruster1.init(true, 1, 0); /* initialization of the HAL object should be called one time only in the project*/
	Thruster2.init(true, 1, 0); /* initialization of the HAL object should be called one time only in the project*/
	Thruster3.init(true, 1, 0); /* initialization of the HAL object should be called one time only in the project*/
}

void ActuatorThruster::run() {
//	init();
	while (1) {

		ControlData controlData;
		ControlDataBuffer3.get(controlData);

		TelecommandData telecommandData;
		ActuatorThruster_TelecommandDataBuffer3.get(telecommandData);


		t1 = controlData.controlledT1;
		t2 = controlData.controlledT2;
		t3 = controlData.controlledT3;
		Thruster1.setPins(t1);
		Thruster2.setPins(t2);
		Thruster3.setPins(t3);
//		PRINTF("Thrusters:%d%d%d",t1,t2,t3);
		suspendCallerUntil(NOW()+50*MILLISECONDS);


	}
}


