/*
 * actuatorServo.h
 *
 *  Created on: 05.12.2017
 *      Author: Felix Hessinger
 */

#ifndef LIB_ACTUATORS_ACTUATORSERVO_H_
#define LIB_ACTUATORS_ACTUATORSERVO_H_

#include "rodos.h"
#include "../structs.h"
#include "../topics.h"
using namespace std;

class ActuatorServo: public Thread {

private:
	int servo_pwm;
public:
	ActuatorServo();
	void init();
	void run();
};

extern ActuatorServo actuatorServo;

#endif /* LIB_ACTUATORS_ACTUATORSERVO_H_ */
