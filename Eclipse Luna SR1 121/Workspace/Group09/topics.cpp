/*
 * Topics.cpp
 *
 *  Created on: 03.12.2017
 *      Author: I8FL-PC01-G01
 */

#include "topics.h"

Topic<TestData> TestDataTopic(-1, "Test Data");
CommBuffer<TestData> TestDataBuffer;
Subscriber TestDataSubscriber(TestDataTopic, TestDataBuffer);

Topic<SensorData> SensorDataTopic(-1, "Sensor Data");
CommBuffer<SensorData> SensorDataBuffer;
Subscriber SensorDataSubscriber(SensorDataTopic, SensorDataBuffer);
