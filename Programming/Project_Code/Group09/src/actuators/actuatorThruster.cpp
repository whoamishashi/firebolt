/*
 * actuatorThruster.cpp
 *
 *  Created on: 9 Dec 2017
 *      Author: Felix Hessinger
 */

#include "../../lib/actuators/actuatorThruster.h"
#include "rodos.h"
#include <stdio.h>
#include "hal.h"
#include "math.h"



using namespace std;

using std::string;
// all 3 HAL_GPIO for Thrusters
HAL_GPIO Thruster1(HBRIDGEDA);
HAL_GPIO Thruster2(HBRIDGEDB);
HAL_GPIO Thruster3(HBRIDGEAA);

ActuatorThruster actuatorThruster;

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
	while (1) {

		ControlData controlData;
		ControlDataBuffer3.get(controlData);

		// getting data from controlData Buffer published in MODES.cpp
		t1 = controlData.controlledT1;
		t2 = controlData.controlledT2;
		t3 = controlData.controlledT3;
		Thruster1.setPins(t1);
		Thruster2.setPins(t2);
		Thruster3.setPins(t3);
		suspendCallerUntil(NOW()+50*MILLISECONDS);
	}
}


