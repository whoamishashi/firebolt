/*
 * topics.cpp
 *
 *  Created on: Dec 15, 2017
 *      Author: shashi
 */

#include "../lib/topics.h"

Topic<TelecommandData> TelecommandDataTopic(1, "Telecommand Data");
Topic<GlobalsData> GlobalsDataTopic(2, "Globals Data");
Topic<SensorData> SensorDataTopic(3, "Sensor Data");
Topic<ActuatorData> ActuatorDataTopic(4, "Actuator Data");
Topic<SurveillanceData> SurveillanceDataTopic(5,"Surveillance Data");
Topic<MotorData> MotorDataTopic(6,"Motor Data");
Topic<Modes> ModesTopic(7,"Modes");
Topic<ControlData> ControlDataTopic(8,"Control Data");
Topic<FusedData> FusedDataTopic(9,"Fused Data");
