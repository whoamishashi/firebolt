/*
 * actuatorServo.h
 *
 *  Created on: 05.12.2017
 *      Author: I8FL-PC01-G01
 */

#ifndef LIB_ACTUATORS_ACTUATORSERVO_H_
#define LIB_ACTUATORS_ACTUATORSERVO_H_

#include "rodos.h"
#include "../structs.h"
#include <string>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <iterator>

#include "../topics.h"
using namespace std;

class ActuatorServo: public Thread {

private:
	int servo_pwm;
public:
	ActuatorServo();
	void init();
	void run();
	vector<string> split(string str, char delimiter);

};

extern ActuatorServo actuatorServo;

#endif /* LIB_ACTUATORS_ACTUATORSERVO_H_ */
