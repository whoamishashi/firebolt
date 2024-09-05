/*
 * sensorCurrent.h
 *
 *  Created on: 22 Dec 2017
 *      Author: Felix Hessinger
 */

#ifndef LIB_SENSORS_SENSORCURRENT_H_
#define LIB_SENSORS_SENSORCURRENT_H_

#include "rodos.h"
#include "HAL.h"
#include "../topics.h"
#include "../structs.h"



class Surveillance: public Thread {
private:

public:
	SurveillanceData surveillanceData;
	Surveillance();
	void init();
	void run();
};

extern Surveillance sensorCurrent;


#endif /* LIB_SENSORS_SENSORCURRENT_H_ */
