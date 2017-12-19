/*
 * actuatorServo.cpp
 *
 *  Created on: 05.12.2017
 *      Author: I8FL-PC01-G01
 */

#include "../../lib/actuators/actuatorServo.h"
#include "rodos.h"
#include <stdio.h>
#include "hal.h"
#include "math.h"
#include<string>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <iterator>
using namespace std;

//HAL_GPIO Servo1(GPIO_073); /* declare HAL_GPIO for GPIO_036 = PC4 (HBRIDGE-A INA pin) */
HAL_PWM Servo01(PWM_IDX00);

ActuatorServo actuatorServo;

CommBuffer<TelecommandData> ActuatorServo_TelecommandDataBuffer;
Subscriber ActuatorServo_TelecommandDataSubscriber(TelecommandDataTopic,
		ActuatorServo_TelecommandDataBuffer);

ActuatorServo::ActuatorServo() {
	servo_pwm=0;
}

void ActuatorServo::init() {
//	Servo1.init(true, 1, 1); /* initialization of the HAL object should be called one time only in the project*/
	Servo01.init(50, 20000);
}

void ActuatorServo::run() {
//	init();
	while (1) {
		TelecommandData telecommandData;
		ActuatorServo_TelecommandDataBuffer.get(telecommandData);

		vector<string> splitted = split(telecommandData.telecommand.c_str(),';');

		servo_pwm = atoi(splitted[7].c_str());

		PRINTF("\nservo_pwm %d", servo_pwm);

		Servo01.write(servo_pwm);

//		for (int var = 1000; var <= 2000; ++var) {
//			Servo01.write(var);
//			suspendCallerUntil(NOW()+2*MILLISECONDS);
//		}
//
//		for (int var = 2000; var >= 1000; --var) {
//			Servo01.write(var);
//			suspendCallerUntil(NOW()+2*MILLISECONDS);
//		}
		suspendCallerUntil(NOW()+500*MILLISECONDS);
	}
}

vector<string> ActuatorServo::split(string str, char delimiter) {
	vector<string> internal;
	stringstream ss(str); // Turn the string into a stream.
	string tok;

	while (getline(ss, tok, delimiter)) {
		internal.push_back(tok);
	}

	return internal;
}

