/*
 * actuatorThruster.h
 *
 *  Created on: 9 Dec 2017
 *      Author: felix
 */

#ifndef ACTUATORTHRUSTER_H_
#define ACTUATORTHRUSTER_H_


#include "rodos.h"
#include "topics.h"
#include "structs.h"

class ActuatorThruster: public Thread {

private:

public:
	ActuatorThruster();
	void init();
	void run();
};

extern ActuatorThruster actuatorThruster;


#endif /* ACTUATORTHRUSTER_H_ */
