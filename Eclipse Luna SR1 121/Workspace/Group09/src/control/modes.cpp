/*
 * modes.cpp
 *
 *  Created on: 27 Jan 2018
 *      Author: felix
 */

#include "../../lib/control/modes.h"

#include <stdio.h>
#include <string>

Modes modes1;



//CommBuffer<Modes> ModesBuffer1;
//Subscriber ModesSubscriber1(ModesTopic, ModesBuffer1);

//ModesClass::ModesClass() {
//}

void ModesClass::init() {

}

void ModesClass::run() {

	while (1) {
		//Get modes from groundstation communication.
		mode=2;


		//publishing the Mode-Topic
		modes1.mode = mode;
		ModesTopic.publish(modes1);

	}
}



