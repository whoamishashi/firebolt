/*
 * controlRW.h
 *
 *  Created on: 20 Dec 2017
 *      Author: felix
 */

#ifndef LIB_CONTROL_CONTROLRW_H_
#define LIB_CONTROL_CONTROLRW_H_


#include "../rodos.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "../hal.h"
#include <stdio.h>
//#include <string>
#include "math.h"
#include "../topics.h"
#include "../structs.h"


//Motor channels
#define ENCLA_PIN 			GPIO_Pin_0				//PA0
#define ENCLA_GPIO_PORT 	GPIOA
#define ENCLA_GPIO_CLK		RCC_AHB1Periph_GPIOA
#define ENCLA_SOURCE		GPIO_PinSource0
#define ENCLA_AF			GPIO_AF_TIM2

#define ENCLB_PIN 			GPIO_Pin_1				//PA1
#define ENCLB_GPIO_PORT 	GPIOA
#define ENCLB_GPIO_CLK		RCC_AHB1Periph_GPIOA
#define ENCLB_SOURCE		GPIO_PinSource1
#define ENCLB_AF			GPIO_AF_TIM2


//Timer
#define ENCL_TIMER			TIM2
#define ENCL_TIMER_CLK		RCC_APB1Periph_TIM2

#define COUNT()				ENCL_TIMER->CNT

#define INCREMENT 1000



class ControlRW: public Thread{

private:
	//available to the rest of the code
	//speeds
	volatile int32_t leftCount;
	volatile int32_t fwdCount;
	volatile int32_t rotCount;
	//distances
	volatile int32_t leftTotal;
	volatile int32_t fwdTotal;
	volatile int32_t rotTotal;

	// local variables
	volatile int32_t oldLeftEncoder;
	volatile int32_t leftEncoder;
	static volatile int32_t encoderSum;
	static volatile int32_t encoderDiff;

	 double input, RWC_out, setpoint;
	 double sum_err , last_err,err;

	 int64_t now, last_time, time_change;
	 double diff_err, error, last_error;

	//Timing
	float time, dt;
	void encoderInit();
	void encoderReset();
	void encoderRead();

public:
	void init();
	void run();
	MotorData motorData;
	ControlData controlData;
	void compute();
	void set_PID_values(double Kp, double Ki,double Kd);
	void set_Sample_Time(double NewSampleTime);
	void set_cmd_Limits(double Min,double Max);
	ControlRW();
	int16_t getRPM();
	void setDutyCycle(float d); //between +1 and -1

};
extern ControlRW controlRW;

#endif /* LIB_CONTROL_CONTROLRW_H_ */
