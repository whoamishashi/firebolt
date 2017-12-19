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

using std::string;

//#define USB_UART UART_IDX3

//#define IMU_I2C I2C_IDX2

//#define DEBUG_SEND_NO_TELEMETRY

Telemetry telemetry;

CommBuffer<SensorData> SensorDataBuffer;
Subscriber SensorDataSubscriber(SensorDataTopic, SensorDataBuffer);

CommBuffer<GlobalsData> GlobalsDataBuffer;
Subscriber GlobalsDataSubscriber(GlobalsDataTopic, GlobalsDataBuffer);

Telemetry::Telemetry() {
}

void Telemetry::init() {
}

void Telemetry::run() {
	char string[50];

	while (1) {
		PRINTF(" ");
//		PRINTF("\ninside run telemetry");

		GlobalsData globalsData;
		GlobalsDataBuffer.get(globalsData);

		SensorData sensorData;
		SensorDataBuffer.get(sensorData);

//		BT2UART.write(string, strlen(string));

		PRINTF("\nTM: IMU: sensorData: gyro: %d, %d, %d", sensorData.gyroX,
				sensorData.gyroY, sensorData.gyroZ);
		PRINTF("\nTM: IMU: sensorData: acce: %d, %d, %d", sensorData.accX,
				sensorData.accY, sensorData.accZ);
		PRINTF("\nTM: IMU: sensorData: magn: %d, %d, %d", sensorData.magX,
				sensorData.magY, sensorData.magZ);
		PRINTF("\nTM: IMU: sensorData: cmps: %d", sensorData.angleZ);
		PRINTF("\nTM: IMU: sensorData: temp: %d", sensorData.temperature);

//		PRINTF("\nTM: %s", globalsData.status);

		suspendCallerUntil(NOW()+1000*MILLISECONDS);
	}
}

class TelecommandDataSubscriber1: public SubscriberReceiver<TelecommandData> {
public:
	TelecommandDataSubscriber1() :
			SubscriberReceiver<TelecommandData>(TelecommandDataTopic,
					"TestDataSubscriber1") {
	}
	void put(TelecommandData &data) {
		PRINTF("\nTC: %s", data.telecommand.c_str());
	}
} telecommandDataSubscriber1;
