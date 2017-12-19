/*
 * telecommand.h
 *
 *  Created on: 03.12.2017
 *      Author: I8FL-PC01-G01
 */

#ifndef LIB_TELECOMMAND_H_
#define LIB_TELECOMMAND_H_

#include "rodos.h"
#include "topics.h"
#include <sstream>
#include <vector>
#include <iterator>
#include "globals.h"
#include "structs.h"
#include "topics.h"


using namespace std;

class Telecommand: public Thread {
private:
//	static int counter;

public:
	Telecommand();
	void init();
	void run();
	vector<string> split(string str, char delimiter);
};

#endif /* LIB_TELECOMMAND_H_ */
