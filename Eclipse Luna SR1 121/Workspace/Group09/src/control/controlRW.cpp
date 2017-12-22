/*
 * controlRW.cpp
 *
 *  Created on: 20 Dec 2017
 *      Author: felix
 */

#include "../../lib/control/controlRW.h"




ControlRW controlRW;

//	int16_t offGyroX, offGyroY, offGyroZ;
	// local variables
	static volatile int16_t oldLeftEncoder =0;
	static volatile int16_t oldRightEncoder=0;
	static volatile int16_t leftEncoder=0;
	static volatile int16_t rightEncoder=0;
	static volatile int16_t encoderSum=0;
	static volatile int16_t encoderDiff=0;
	//available to the rest of the code
	//speeds
	volatile int16_t leftCount;
	volatile int16_t rightCount;
	volatile int16_t fwdCount;
	volatile int16_t rotCount;
	//distances
	volatile int32_t leftTotal;
	volatile int32_t rightTotal;
	volatile int32_t fwdTotal;
	volatile int32_t rotTotal;



ControlRW::ControlRW() {

}
void ControlRW::init() {
	encoderInit();
}

int16_t rpm;


void ControlRW::run() {
	init();
	while(1){

		rpm = getRPM();
		PRINTF("%d\n",rpm);
		suspendCallerUntil(NOW()+100*MILLISECONDS);


	}
}

void ControlRW::encoderInit() {
	 GPIO_InitTypeDef GPIO_InitStructure;
	  // turn on the clocks for each of the ports needed
	  RCC_AHB1PeriphClockCmd (ENCLA_GPIO_CLK, ENABLE);
	  RCC_AHB1PeriphClockCmd (ENCLB_GPIO_CLK, ENABLE);

	  // now configure the pins themselves
	  // they are all going to be inputs with pullups
	  GPIO_StructInit (&GPIO_InitStructure);
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	  GPIO_InitStructure.GPIO_Pin = ENCLA_PIN;
	  GPIO_Init (ENCLA_GPIO_PORT, &GPIO_InitStructure);
	  GPIO_InitStructure.GPIO_Pin = ENCLB_PIN;
	  GPIO_Init (ENCLB_GPIO_PORT, &GPIO_InitStructure);

	  // Connect the pins to their Alternate Functions
	  GPIO_PinAFConfig (ENCLA_GPIO_PORT, ENCLA_SOURCE, ENCLA_AF);
	  GPIO_PinAFConfig (ENCLB_GPIO_PORT, ENCLB_SOURCE, ENCLB_AF);

	  // Timer peripheral clock enable
	  RCC_APB1PeriphClockCmd (ENCL_TIMER_CLK, ENABLE);

	  // set them up as encoder inputs
	  // set both inputs to rising polarity to let it use both edges
	  TIM_EncoderInterfaceConfig (ENCL_TIMER, TIM_EncoderMode_TI12,
	                              TIM_ICPolarity_Rising,
	                              TIM_ICPolarity_Rising);
	  TIM_SetAutoreload (ENCL_TIMER, 0xffffffff);

	  // turn on the timer/counters
	  TIM_Cmd (ENCL_TIMER, ENABLE);

	  encoderReset();
}

void ControlRW::encoderReset() {
	  __disable_irq();
	  oldLeftEncoder = 0;
	  leftTotal = 0;
	  fwdTotal = 0;
	  rotTotal = 0;
	  TIM_SetCounter (ENCL_TIMER, 0);
	  time=SECONDS_NOW();
	  encoderRead();
	  __enable_irq();
}


void ControlRW::encoderRead() {
	dt=SECONDS_NOW()-time;
	time=SECONDS_NOW();
	 oldLeftEncoder = leftEncoder;
	  leftEncoder = TIM_GetCounter (ENCL_TIMER) ;
	  leftCount = leftEncoder - oldLeftEncoder;
	  fwdTotal += fwdCount;
	  rotTotal += rotCount;
	  leftTotal += leftCount;
}


int16_t ControlRW::getRPM() {
	encoderRead();
	return leftCount*60/(16*dt)/4;
}

