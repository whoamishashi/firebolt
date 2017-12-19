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

//typedef enum{
//        Aksan = 0,
//        Mandy = 1,
//        Sahriani = 2
//};

struct TelecommandData {
	std::string telecommand;
};

struct GlobalsData {
	char status[50];
};

struct __attribute__((packed)) DpCommand {
	uint8_t sync;
	uint8_t id;
	uint32_t argument;
	uint32_t check;
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

#endif /* LIB_STRUCTS_H_ */
