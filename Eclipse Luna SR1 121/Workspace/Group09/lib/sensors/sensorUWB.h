/*
 * sensorUWB.h
 *
 *  Created on: Jan 2, 2018
 *      Author: shashi
 */

#ifndef LIB_SENSORS_SENSORUWB_H_
#define LIB_SENSORS_SENSORUWB_H_

#include "globals.h"
#include "structs.h"
#include "topics.h"

using namespace std;

class SensorUWB: public Thread {
private:

public:
	SensorUWB();
	void init();
	void run();
};

extern SensorUWB sensorUWB;


#endif /* LIB_SENSORS_SENSORUWB_H_ */
