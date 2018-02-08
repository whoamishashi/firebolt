/*
 * actuatorThruster.h
 *
 *  Created on: 9 Dec 2017
 *      Author: felix
 */

#ifndef LIB_ACTUATORS_ACTUATORTHRUSTER_H_
#define LIB_ACTUATORS_ACTUATORTHRUSTER_H_

#include "rodos.h"
#include "../structs.h"
#include "../globals.h"
#include "../topics.h"
using namespace std;

class ActuatorThruster: public Thread {

private:
	bool t1, t2, t3;
public:
	ControlData controlData;
	ActuatorThruster();
	void init();
	void run();

};

extern ActuatorThruster actuatorThruster;

#endif /* LIB_ACTUATORS_ACTUATORTHRUSTER_H_ */
