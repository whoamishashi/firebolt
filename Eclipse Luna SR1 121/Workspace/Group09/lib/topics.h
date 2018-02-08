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

extern Topic<GlobalsData> GlobalsDataTopic;
extern Topic<SensorData> SensorDataTopic;
extern Topic<ActuatorData> ActuatorDataTopic;
extern Topic<SurveillanceData> SurveillanceDataTopic;
extern Topic<MotorData> MotorDataTopic;
extern Topic<ControlData> ControlDataTopic;
extern Topic<FusedData> FusedDataTopic;
extern Topic<TelecommandData> TelecommandDataTopic;
extern Topic<StarTrackerData> StarTrackerDataTopic;				//Mission: star tracker
extern Topic<RadioData> RadioDataTopic;							//Mission: radio positioning
extern Topic<IRData> IRDataTopic;								//Mission: docking tracking
extern Topic<ObjectRecognitionData> ObjectRecognitionDataTopic;	//Mission: object recognition
#endif /* LIB_TOPICS_H_ICS_H_ */
