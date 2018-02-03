/*
 * globals.cpp
 *
 *  Created on: 05.12.2017
 *      Author: I8FL-PC01-G01
 */

//static Application module01("Template", 2001);
#include "../lib/globals.h"

#include <stdio.h>
#include <string>

//HAL_UART BT2UART(BT_UART);
HAL_UART RSPUART(UART_IDX5);

//HAL_GPIO GreenLED(LED_GREEN);
HAL_GPIO RedLED(LED_RED);

Globals globals;

//CommBuffer<TestData> TestDataBuffer;
//Subscriber TestDataSubscriber(TestDataTopic, TestDataBuffer);

//fifo<TestData, 8> TestDataBuffer;
//Subscriber TestDataSubscriber(TestDataTopic, TestDataBuffer);

Globals::Globals() {
}

void Globals::init() {
	RedLED.init(true, 1, 0);
	//BT2UART.init(921600);
	RSPUART.init(115200);
}

void Globals::run() {
	char string[50];
//	while (1) {
////		globals.redLEDtoggle();
//
////		if (BlueButton.readPins()){
////			BlueLED.setPins(~BlueLED.readPins());
////			suspendCallerUntil(NOW()+1000*MILLISECONDS);
////		}
////		suspendCallerUntil(NOW()+1000*MILLISECONDS);
//	}
}

uint32_t Globals::redLEDtoggle(uint32_t buffer) { //uint32_t buffer

//	RedLED.setPins(~RedLED.readPins());
//	PRINTF("in redLEDtoggle");
	RedLED.setPins(buffer);
	return RedLED.readPins();
}

//class TelecommandDataSubscriber: public SubscriberReceiver<TelecommandData> {
//public:
//	TelecommandDataSubscriber() :
//			SubscriberReceiver<TelecommandData>(TelecommandDataTopic,
//					"TelecommandDataSubscriber") {
//	}
//	void put(TelecommandData &data) {
//		GlobalsData globalsData;
//		sprintf(globalsData.status,
//				globals.redLEDtoggle((uint32_t) data.telecommand.c_str()) ?
//						"RedLED on" : "RedLED off");
//		GlobalsDataTopic.publish(globalsData);
//	}
//}telecommandDataSubscriber;
