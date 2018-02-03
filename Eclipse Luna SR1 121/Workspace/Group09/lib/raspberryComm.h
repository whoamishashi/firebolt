/*
 * raspberryComm.h
 *
 *  Created on: Jan 31, 2018
 *      Author: shashi
 */

#ifndef LIB_RASPBERRYCOMM_H_
#define LIB_RASPBERRYCOMM_H_

#include "rodos.h"
#include "topics.h"
#include <sstream>
#include <vector>
#include <iterator>
#include "globals.h"
#include "structs.h"
#include "topics.h"

using namespace std;

class RaspberryComm: public Thread {
private:
//	static int counter;

public:
	RaspberryComm();
	void init();
	void run();
	vector<string> split(string str, char delimiter);
};

#endif /* LIB_RASPBERRYCOMM_H_ */
