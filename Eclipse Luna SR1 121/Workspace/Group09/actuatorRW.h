/*
 * ActuatorRW.h
 *
 *  Created on: 15 Dec 2017
 *      Author: felix
 */

#ifndef ACTUATORRW_H_
#define ACTUATORRW_H_

#include "rodos.h"
#include "math.h"
#include "structs.h"
#include "topics.h"
#include "globals.h"

#include <string>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <iterator>
using namespace std;

class ActuatorRW: public Thread { //public thread was not there initially
private:
	bool dir;
	int m_speed;
//	uint8_t DATA[12];
	//GPIO_073

public:
	ActuatorRW();
	void init();
	void run();
	vector<string> split(string str, char delimiter);
};

extern ActuatorRW actuatorRW;

#endif /* ACTUATORRW_H_ */
