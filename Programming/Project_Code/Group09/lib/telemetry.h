/*
 * telemetry.h
 *
 *  Created on: 03.12.2017
 *      Author: Felix Hessinger & shashi
 */

#ifndef LIB_TELEMETRY_H_
#define LIB_TELEMETRY_H_

#include "globals.h"
#include "structs.h"
#include "topics.h"

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

#endif /* LIB_TELEMETRY_H_LEMETRY_H_ */
