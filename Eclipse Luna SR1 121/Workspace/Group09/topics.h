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

extern Topic<TelecommandData> TelecommandDataTopic;
extern Topic<GlobalsData> GlobalsDataTopic;
extern Topic<SensorData> SensorDataTopic;
extern Topic<ActuatorData> ActuatorDataTopic;

#endif /* TOPICS_H_ */
