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

class ActuatorServo: public Thread {

private:

public:
	ActuatorServo();
	void init();
	void run();
};

extern ActuatorServo actuatorServo;

#endif /* ACTUATORSERVO_H_ */
