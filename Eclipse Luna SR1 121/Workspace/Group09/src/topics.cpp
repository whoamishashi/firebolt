/*
 * topics.cpp
 *
 *  Created on: Dec 15, 2017
 *      Author: shashi
 */

#include "../lib/topics.h"

Topic<TelecommandData> TelecommandDataTopic(-1, "Telecommand Data");
Topic<GlobalsData> GlobalsDataTopic(-1, "Globals Data");
Topic<SensorData> SensorDataTopic(-1, "Sensor Data");
Topic<ActuatorData> ActuatorDataTopic(-1, "Actuator Data");
Topic<SurveillanceData> SurveillanceDataTopic(-1,"Surveillance Data");
Topic<MotorData> MotorDataTopic(-1,"Motor Data");
