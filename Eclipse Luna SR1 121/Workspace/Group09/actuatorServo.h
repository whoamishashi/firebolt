/*
 * actuatorServo.h
 *
 *  Created on: 05.12.2017
 *      Author: I8FL-PC01-G01
 */

#ifndef ACTUATORSERVO_H_
#define ACTUATORSERVO_H_

#include "rodos.h"
#include "topics.h"
#include "structs.h"
#include<string>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <iterator>
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

#endif /* ACTUATORSERVO_H_ */
