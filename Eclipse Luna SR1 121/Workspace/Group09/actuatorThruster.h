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

#include <string>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <iterator>
using namespace std;

class ActuatorThruster: public Thread {

private:
	bool t1, t2, t3;
public:
	ActuatorThruster();
	void init();
	void run();
	vector<string> split(string str, char delimiter);
};

extern ActuatorThruster actuatorThruster;

#endif /* ACTUATORTHRUSTER_H_ */
