/*
 * globals.cpp
 *
 *  Created on: 05.12.2017
 *      Author: I8FL-PC01-G01
 */


#include "globals.h"

HAL_GPIO GreenLED(LED_GREEN);



Globals globals;

Globals::Globals() {
}

void Globals::init() {
	GreenLED.init(true, 1, 0);

	//BT2UART.init(921600);
}

void Globals::run() {
	char string[50];
	while (1) {

        suspendCallerUntil(NOW()+500*MILLISECONDS);
		GreenLED.setPins(~GreenLED.readPins());

//		if (BlueButton.readPins()){
//			BlueLED.setPins(~BlueLED.readPins());
//			suspendCallerUntil(NOW()+1000*MILLISECONDS);
//		}
	}
}
