/*
 * globals.h
 *
 *  Created on: 05.12.2017
 *      Author: I8FL-PC01-G01
 */

#ifndef LIB_GLOBALS_H_
#define LIB_GLOBALS_H_

#include "rodos.h"
#include "topics.h"
#include "hal.h"
#include <cstdlib>
#include <sstream>
#include <vector>
#include <iterator>
#include "structs.h"
#include "topics.h"
#include <hal.h>

using namespace std;

// UART
//#define BT_UART UART_IDX2
//#define USB_UART UART_IDX3
//extern HAL_UART BT2UART;
//extern HAL_UART USB2UART;

#define IMU_I2C I2C_IDX2

//computations
#define RAD2DEG(x) 				(x*180/M_PI);
#define DEG2RAD(x)				(x*M_PI/180);
//#define TEST(x,y) (((x) << 8) | y)

// GPIO pins
//#define LED_GREEN GPIO_060
#define LED_ORANGE GPIO_061
#define LED_RED GPIO_062
#define LED_BLUE GPIO_063

extern HAL_I2C i2c2_bus;

namespace RODOS {
extern HAL_UART uart_stdout;
}
#define BT2UART uart_stdout

extern HAL_UART RSPUART;
extern HAL_I2C i2c2_bus;

class Globals: public Thread {
private:
//	uint8_t DATA[12];

public:
	Globals();
	void init();
	void run();
	uint32_t redLEDtoggle(uint32_t buffer);
};

extern Globals globals;

#endif /* LIB_GLOBALS_H_ */
