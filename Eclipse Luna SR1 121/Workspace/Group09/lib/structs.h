/*
 * structs.h
 *
 *  Created on: 03.12.2017
 *      Author: I8FL-PC01-G01
 */

#ifndef LIB_STRUCTS_H_
#define LIB_STRUCTS_H_

#include "rodos.h"
#include <string>
#include <cstdlib>
#include <sstream>

using namespace std;

#define SYNC_COMM   (uint8_t)0xA1 // 0b10100001

#define FORMAT_SD		0x00

enum Status {
	OK = 0,
	NO_INIT = 1,
	INIT_FAILED = 2,
	COMM_ERROR = 3,
	MINOR_ERROR = 4,
	CRITICAL_ERROR = 5,
	SD_DL_PROGRESS = 6
};

enum AttitudeControlMode
{
	ATTITUDE_CONTROL_MODE_AUTO = 0,
	ATTITUDE_CONTROL_MODE_MANUAL = 1,
	ATTITUDE_CONTROL_MODE_ANGULAR_SPEED = 2,
	ATTITUDE_CONTROL_MODE_DOCKING = 3,
	ATTITUDE_CONTROL_MODE_OBJECTRECOGNITION = 4,
	ATTITUDE_CONTROL_MODE_SEMIMANUAL = 5
};

struct TelecommandData {

	std::string telecommand;				//{CMD}

	int mode;								//{-1:standby,
											//	0:automatic (starTracker, radioPositioning),
											//	1:manual,
											//	2:angVelang ,
											//	3:dockingTracking,
											//	4:objectRecognition}

	//manual
	int t1;									//{0:off,1:on}
	int t2;									//{0:off,1:on}
	int t3;									//{0:off,1:on}
	int rotateDelta;  						//{-1:anticw,0:doNothing,1:cw}
	int rwRotationSpeed;						//wheel[-1000dps, 1000dps]
	int servoAngle;							//{940:down, 1900:straight}
	int activateMagnet;						//{0:inactive, 1:active}

	//automatic
	double targetPosition_x;				//{[0m,2m]:valid, 9999:invalid}
	double targetPosition_y;				//[0m,2m]
	int desiredAngle;						//[0deg, 360deg]
	int desiredAngularVel;					//satellite[-180dps, 180dps]
	int rotate180;  						//{0:doNothing,1:rotate}
};

struct GlobalsData {
	char status[50];
};

struct IRData {
	float range1;
	float range2;
	float distance;
	float angle;
};

struct ObjectRecognitionData {
};

struct StarTrackerData {
};

struct RadioData {
};

struct __attribute__((packed)) DpCommand {
	uint8_t sync;
	uint8_t id;
	uint32_t argument;
	uint32_t check;
};


struct MotorData{
	int32_t sensorMotorSpeed;
	double 	controlled_m_speed;

};

struct SurveillanceData {
	float 	batteryVoltage;
	float 	batteryCurrent;
	int 	asdf;
};



struct ActuatorData {
	//Actual values
	int mode;
	bool dir;   // {1: anticlock, 0:cw}
	float m_speed;
	bool t1, t2, t3;  // {0:off, 1:on}
};

struct SensorData {
	//Actual values
	int16_t gyroX, gyroY, gyroZ;
	int16_t accX, accY, accZ;
	int16_t magX, magY, magZ;
	int16_t temperature;
	int32_t angleZ;
};

struct Modes {
	int mode;
};

struct ControlData {	//this is for the upper Controllers to give the values to the sub-controller
	int desiredRWSpeed;
	int desiredThrusterValue;
	int controlledT1;
	int controlledT2;
	int controlledT3;
};

struct FusedData {
	double fusedPosition[2];
	double fusedAngle;
};
struct TelecommandMeasurements {  //to control topic
  float st_x;
  float st_y;
  float st_angle;
};


#endif /* LIB_STRUCTS_H_ */
