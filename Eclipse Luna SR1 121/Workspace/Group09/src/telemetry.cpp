/*
 * telemetry.cpp
 *
 *  Created on: 03.12.2017
 *      Author: I8FL-PC01-G01
 */

#include "../lib/telemetry.h"

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

//CommBuffer<GlobalsData> GlobalsDataBuffer;
//Subscriber GlobalsDataSubscriber(GlobalsDataTopic, GlobalsDataBuffer);

CommBuffer<IRData> IRDataBuffer;
Subscriber IRDataSubscriber(IRDataTopic, IRDataBuffer);

CommBuffer<ObjectRecognitionData> ObjectRecognitionDataBuffer;
Subscriber ObjectRecognitionDataSubscriber(ObjectRecognitionDataTopic, ObjectRecognitionDataBuffer);

CommBuffer<StarTrackerData> StarTrackerDataBuffer;
Subscriber StarTrackerDataSubscriber(StarTrackerDataTopic, StarTrackerDataBuffer);

CommBuffer<RadioData> RadioDataBuffer;
Subscriber RadioDataSubscriber(RadioDataTopic, RadioDataBuffer);

CommBuffer<MotorData> MotorDataBuffer;
Subscriber MotorDataSubscriber(MotorDataTopic, MotorDataBuffer);

CommBuffer<SurveillanceData> SurveillanceDataBuffer;
Subscriber SurveillanceDataSubscriber(SurveillanceDataTopic, SurveillanceDataBuffer);

CommBuffer<SensorData> SensorDataBuffer;
Subscriber SensorDataSubscriber(SensorDataTopic, SensorDataBuffer);

CommBuffer<FusedData> FusedDataBuffer;
Subscriber FusedDataSubscriber(FusedDataTopic, FusedDataBuffer);

CommBuffer<ControlData> ControlDataBuffer;
Subscriber ControlDataSubscriber(ControlDataTopic, ControlDataBuffer);

Telemetry::Telemetry() {
}

void Telemetry::init() {
}

void Telemetry::run() {
	char string[50];

	while (1) {
		PRINTF("_____________________________");

		IRData irData;
		IRDataBuffer.get(irData);

		ObjectRecognitionData objectRecognitionData;
		ObjectRecognitionDataBuffer.get(objectRecognitionData);

		StarTrackerData starTrackerData;
		StarTrackerDataBuffer.get(starTrackerData);

		RadioData radioData;
		RadioDataBuffer.get(radioData);

		MotorData motorData;
		MotorDataBuffer.get(motorData);

		SurveillanceData surveillanceData;
		SurveillanceDataBuffer.get(surveillanceData);

		SensorData sensorData;
		SensorDataBuffer.get(sensorData);

		FusedData fusedData;
		FusedDataBuffer.get(fusedData);

		ControlData controlData;
		ControlDataBuffer.get(controlData);

//		BT2UART.write(string, strlen(string));

		PRINTF("\nTM:IRData:range1:%f", irData.range1);
		PRINTF("\nTM:IRData:range2:%f", irData.range2);
		PRINTF("\nTM:IRData:distance:%f", irData.distance);
		PRINTF("\nTM:IRData:angle:%f", irData.angle);

		PRINTF("\nTM:ObjectRecognitionData:alpha:%f", objectRecognitionData.alpha);
		PRINTF("\nTM:ObjectRecognitionData:G0:%f", objectRecognitionData.G0);
		PRINTF("\nTM:ObjectRecognitionData:g0:%f", objectRecognitionData.g0);
		PRINTF("\nTM:ObjectRecognitionData:trusted:%d", objectRecognitionData.trusted);

		PRINTF("\nTM:StarTrackerData:x:%f", starTrackerData.x);
		PRINTF("\nTM:StarTrackerData:y:%f", starTrackerData.y);
		PRINTF("\nTM:StarTrackerData:angle:%f", starTrackerData.angle);

		PRINTF("\nTM:RadioData:x:%f", radioData.x);
		PRINTF("\nTM:RadioData:y:%f", radioData.y);

		PRINTF("\nTM:MotorData:sensorMotorSpeed:%ld", motorData.sensorMotorSpeed);

		PRINTF("\nTM:SurveillanceData:batteryVoltage:%f", surveillanceData.batteryVoltage);
		PRINTF("\nTM:SurveillanceData:batteryCurrent:%f", surveillanceData.batteryCurrent);

		PRINTF("\nTM:SensorData:gyroX:%d", sensorData.gyroX);
		PRINTF("\nTM:SensorData:gyroY:%d", sensorData.gyroY);
		PRINTF("\nTM:SensorData:gyroZ:%d", sensorData.gyroZ);
		PRINTF("\nTM:SensorData:accX:%d", sensorData.accX);
		PRINTF("\nTM:SensorData:accY:%d", sensorData.accY);
		PRINTF("\nTM:SensorData:accZ:%d", sensorData.accZ);
		PRINTF("\nTM:SensorData:magX:%d", sensorData.magX);
		PRINTF("\nTM:SensorData:magY:%d", sensorData.magY);
		PRINTF("\nTM:SensorData:magZ:%d", sensorData.magZ);
		PRINTF("\nTM:SensorData:temperature:%d", sensorData.temperature);
		PRINTF("\nTM:SensorData:angleZ:%d", sensorData.angleZ);

		PRINTF("\nTM:FusedData:x:%f", fusedData.x);
		PRINTF("\nTM:FusedData:y:%f", fusedData.y);
		PRINTF("\nTM:FusedData:angle:%ld", fusedData.angle);

		suspendCallerUntil(NOW()+500*MILLISECONDS);
	}
}

class TelecommandDataSubscriber1: public SubscriberReceiver<TelecommandData> {
public:
	TelecommandDataSubscriber1() :
			SubscriberReceiver<TelecommandData>(TelecommandDataTopic,
					"TestDataSubscriber1") {
	}
	void put(TelecommandData &data) {
		//PRINTF("\nTC: %s", data.telecommand.c_str());
	}
} telecommandDataSubscriber1;
