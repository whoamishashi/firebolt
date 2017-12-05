/*
 * telecommand.h
 *
 *  Created on: 03.12.2017
 *      Author: I8FL-PC01-G01
 */

#ifndef TELECOMMAND_H_
#define TELECOMMAND_H_

#include "rodos.h"
#include "topics.h"
#include "structs.h"
#include "murmur.h"

class Telecommand: public Thread {
private:
	static int counter;
public:
	Telecommand();
	void run();
	int decodeCommand(DpCommand &comPack);
};

#endif /* TELECOMMAND_H_ */
