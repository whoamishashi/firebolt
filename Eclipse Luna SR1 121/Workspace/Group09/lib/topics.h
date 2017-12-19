/*
 * Topics.h
 *
 *  Created on: 03.12.2017
 *      Author: I8FL-PC01-G01
 */

#ifndef LIB_TOPICS_H_
#define LIB_TOPICS_H_

#include "rodos.h"

#include "structs.h"

extern Topic<TelecommandData> TelecommandDataTopic;
extern Topic<GlobalsData> GlobalsDataTopic;
extern Topic<SensorData> SensorDataTopic;
extern Topic<ActuatorData> ActuatorDataTopic;

#endif /* LIB_TOPICS_H_ICS_H_ */
