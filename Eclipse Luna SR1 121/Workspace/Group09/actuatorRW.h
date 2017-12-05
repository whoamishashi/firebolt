/*
 * actuatorMotor.h
 *
 *  Created on: 03.12.2017
 *      Author: I8FL-PC01-G01
 */

#ifndef ACTUATORRW_H_
#define ACTUATORRW_H_

#include "rodos.h"
#include "topics.h"
#include "structs.h"

class ActuatorRW: public Thread {
private:
//	uint8_t DATA[12];
	//GPIO_073

public:
	ActuatorRW();
	void init();
	void run();
};

extern ActuatorRW actuatorRW;

#endif /* ACTUATORRW_H_ */
