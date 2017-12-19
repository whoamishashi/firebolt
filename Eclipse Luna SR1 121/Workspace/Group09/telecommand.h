/*
 * telecommand.h
 *
 *  Created on: 03.12.2017
 *      Author: I8FL-PC01-G01
 */

#ifndef TELECOMMAND_H_
#define TELECOMMAND_H_

#include "globals.h"
#include "rodos.h"
#include "topics.h"
#include "structs.h"

#include <string>
#include <sstream>
#include <vector>
#include <iterator>

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

#endif /* TELECOMMAND_H_ */
