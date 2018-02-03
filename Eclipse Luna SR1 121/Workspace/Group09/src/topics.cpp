/*
 * topics.cpp
 *
 *  Created on: Dec 15, 2017
 *      Author: shashi
 */

#include "../lib/topics.h"

Topic<GlobalsData> GlobalsDataTopic(2, "Globals Data");
Topic<SensorData> SensorDataTopic(3, "Sensor Data");
Topic<ActuatorData> ActuatorDataTopic(4, "Actuator Data");
Topic<SurveillanceData> SurveillanceDataTopic(5,"Surveillance Data");
Topic<MotorData> MotorDataTopic(6,"Motor Data");
Topic<Modes> ModesTopic(7,"Modes");
Topic<ControlData> ControlDataTopic(8,"Control Data");
Topic<FusedData> FusedDataTopic(9,"Fused Data");
Topic<TelecommandMeasurements> TelecommandMeasurementsDataTopic(10,"TelecommandMeasurements Data");
Topic<TelecommandData> TelecommandDataTopic(1, "Telecommand Data");
Topic<StarTrackerData> StarTrackerDataTopic(11, "StarTracker Data");
Topic<RadioData> RadioDataTopic(12, "Radio Data");
Topic<IRData> itInfraredDataTopic(-1, "itInfrared Data");
Topic<ObjectRecognitionData> ObjectRecognitionDataTopic(-1, "ObjectRecognition Data");
