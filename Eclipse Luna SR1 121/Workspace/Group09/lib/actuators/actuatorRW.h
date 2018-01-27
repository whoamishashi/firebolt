/*
 * ActuatorRW.h
 *
 *  Created on: 15 Dec 2017
 *      Author: felix
 */

#ifndef LIB_ACTUATORS_ACTUATORRW_H_
#define LIB_ACTUATORS_ACTUATORRW_H_

#include "rodos.h"
#include "math.h"
#include "../topics.h"
#include <cstdlib>
#include <sstream>
#include <vector>
#include <iterator>

#include "../globals.h"
#include "../structs.h"
#include "../topics.h"

using namespace std;

class ActuatorRW: public Thread { //public thread was not there initially
private:
	bool dir;
	int m_speed;
//	uint8_t DATA[12];
	//GPIO_073

public:
	ActuatorRW();
	SurveillanceData surveillanceData;
	void init();
	void run();
	vector<string> split(string str, char delimiter);
};

extern ActuatorRW actuatorRW;

#endif /* LIB_ACTUATORS_ACTUATORRW_H_ */
