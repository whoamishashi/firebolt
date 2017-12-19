/*
 * telemetry.h
 *
 *  Created on: 03.12.2017
 *      Author: I8FL-PC01-G01
 */

#ifndef TELEMETRY_H_
#define TELEMETRY_H_

#include "rodos.h"
#include "topics.h"
#include "structs.h"
#include "globals.h"

using namespace std;

//#define ACTIVATE_DEBUG_DL

class Telemetry: public Thread {
private:

public:
	Telemetry();
	void init();
	void run();
	uint32_t generateChecksum(char *buffer, int size);
};

extern Telemetry telemetry;

#endif /* TELEMETRY_H_ */
