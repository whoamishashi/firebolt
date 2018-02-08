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
#define BT2UART uart_stdout

//Defines for the H-BRIDGES
//H-Bridge A
#define HBRIDGEAA				GPIO_036
#define HBRIDGEAB				GPIO_017
#define HBRIDGE_TIMER_A			PWM_IDX12
//H-Bridge B
#define HBRIDGEBA				GPIO_016
#define HBRIDGEBB				GPIO_071
#define HBRIDGE_TIMER_B			PWM_IDX13
//H-Bridge C
#define HBRIDGECA				GPIO_072
#define HBRIDGECB				GPIO_074
#define HBRIDGE_TIMER_C			PWM_IDX14
//H-Bridge D
#define HBRIDGEDA				GPIO_076
#define HBRIDGEDB				GPIO_079
#define HBRIDGE_TIMER_D			PWM_IDX15

//Maximum Motor Speed of the ReactionWheel possible with particular board
#define MAX_RPM_RW_POSSIBLE		7800


namespace RODOS {
extern HAL_UART uart_stdout;
}

extern HAL_I2C i2c2_bus;
extern int WAITINGTIME_UNTIL_START;
extern HAL_UART RSPUART;

class Globals: public Thread {
private:
//	uint8_t DATA[12];

public:
	Globals();
	void init();
	void run();
};

extern Globals globals;

#endif /* LIB_GLOBALS_H_ */
