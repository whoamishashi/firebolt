/*
 * globals.h
 *
 *  Created on: 05.12.2017
 *      Author: I8FL-PC01-G01
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include "rodos.h"
#include "math.h"

//static Application module01("Template", 2001);

// UART
#define BT_UART UART_IDX2
#define USB_UART UART_IDX3

//computations
#define RAD2DEG(x) 				(x*180/M_PI);
#define DEG2RAD(x)				(x*M_PI/180);
//#define TEST(x,y) (((x) << 8) | y)

// GPIO pins
//#define LED_GREEN GPIO_060
#define LED_ORANGE GPIO_061
#define LED_RED GPIO_062
#define LED_BLUE GPIO_063


namespace RODOS{
extern HAL_UART uart_stdout;
}
#define TeleUART uart_stdout

class Globals : public Thread{
private:
//	uint8_t DATA[12];

public:
	Globals();
	void init();
	void run();
};

extern Globals globals;


#endif /* GLOBALS_H_ */
