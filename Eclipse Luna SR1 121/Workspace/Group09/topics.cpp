/*
 * topics.cpp
 *
 *  Created on: Dec 15, 2017
 *      Author: shashi
 */

#include"topics.h"

Topic<TelecommandData> TelecommandDataTopic(-1, "Telecommand Data");
Topic<GlobalsData> GlobalsDataTopic(-1, "Globals Data");
Topic<SensorData> SensorDataTopic(-1, "Sensor Data");
Topic<ActuatorData> ActuatorDataTopic(-1, "Actuator Data");
