/*
 * sensorCurrent.h
 *
 *  Created on: 22 Dec 2017
 *      Author: felix
 */

#ifndef LIB_SENSORS_SENSORCURRENT_H_
#define LIB_SENSORS_SENSORCURRENT_H_

#include "rodos.h"
#include "HAL.h"
#include "../topics.h"
#include "../structs.h"



class Surveillance: public Thread {
private:
//	uint8_t DATA[12];

public:
	SurveillanceData surveillanceData;
	Surveillance();
	void init();
	void run();

};

extern Surveillance sensorCurrent;


#endif /* LIB_SENSORS_SENSORCURRENT_H_ */
