/*
 * Topics.h
 *
 *  Created on: 03.12.2017
 *      Author: I8FL-PC01-G01
 */

#ifndef TOPICS_H_
#define TOPICS_H_

#include "rodos.h"
#include "structs.h"

extern Topic<TestData> TestDataTopic;
extern CommBuffer<TestData> TestDataBuffer;

extern Topic<SensorData> SensorDataTopic;
extern CommBuffer<SensorData> SensorDataBuffer;

#endif /* TOPICS_H_ */
