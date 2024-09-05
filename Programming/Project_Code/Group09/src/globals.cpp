/*
 * globals.cpp
 *
 *  Created on: 05.12.2017
 *      Author: Felix Hessinger
 */

//static Application module01("Template", 2001);
#include "../lib/globals.h"

#include <stdio.h>
#include <string>

//HAL_UART BT2UART(BT_UART);
//HAL_UART USB2UART(USB_UART);
HAL_UART RSPUART(UART_IDX5);

//HAL_GPIO GreenLED(LED_GREEN);
HAL_GPIO RedLED(LED_RED);
HAL_I2C i2c2_bus(I2C_IDX2);

Globals globals;
int WAITINGTIME_UNTIL_START = 8;	//has to multipied by *SECONDS in suspendCallerUntil()

Globals::Globals() {
}

void Globals::init() {
	RedLED.init(true, 1, 0);
	i2c2_bus.init(400000);
	//BT2UART.init(921600);
	RSPUART.init(115200);
}

void Globals::run() {


}
