/*
 * telemetry.cpp
 *
 *  Created on: 03.12.2017
 *      Author: I8FL-PC01-G01
 */

#include "telemetry.h"
#include "rodos.h"
#include <stdio.h>
#include "hal.h"
#include "math.h"
#include <string>

//static Application module01("Template", 2001);

#define BT_UART UART_IDX2
#define USB_UART UART_IDX3

#define IMU_I2C I2C_IDX2

HAL_UART BT2UART(BT_UART);

//#define DEBUG_SEND_NO_TELEMETRY

Telemetry telemetry;

Telemetry::Telemetry() {
}

void Telemetry::init() {

}

void Telemetry::run() {
	TestData testData;
	char string[50];

	while (1) {
		suspendCallerUntil(NOW()+1000*MILLISECONDS);

		SensorData Teledata;
		SensorDataBuffer.get(Teledata);

		char buffer[50];
		TestDataBuffer.get(testData);

		//PRINTF("Hello Rodos, the time now is %f \r\n",SECONDS_NOW());

		sprintf(string, "IMU_STATUS %s \n",testData.i);

		//sprintf(string, "IMU_STATUS %s \n",GreenLED.readPins() ? "true" : "false");
		BT2UART.write(string, strlen(string));

		//BT2UART.read(string, strlen(string));
		//GreenLED.setPins(string);

		//BT2UART.suspendUntilDataReady();
//		BT2UART.read(buffer, 1);
//		PRINTF("Hello Rodos, the buffer now is %s \r\n",buffer);
		PRINTF("G: %s , %s , %s \n",Teledata.gyroX, Teledata.gyroY, Teledata.gyroZ);


	}
}

uint32_t Telemetry::generateChecksum(char *buffer, int size)
{
    return Murmur::mm_hash_32((uint8_t*)buffer, size);
}
