/*
 * modes.h
 *
 *  Created on: 27 Jan 2018
 *      Author: felix
 */

#ifndef LIB_CONTROL_MODES_H_
#define LIB_CONTROL_MODES_H_


#include "rodos.h"
#include "../structs.h"
#include "../topics.h"



class ModesClass: public Thread {
private:
	int mode;

public:
	ModesClass();
	void init();
	void run();
};

extern Modes modes;


#endif /* LIB_CONTROL_MODES_H_ */
