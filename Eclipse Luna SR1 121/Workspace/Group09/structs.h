/*
 * structs.h
 *
 *  Created on: 03.12.2017
 *      Author: I8FL-PC01-G01
 */

#ifndef STRUCTS_H_
#define STRUCTS_H_

#include "rodos.h"

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

//test topic
struct TestData {
	char i[100];
};

struct __attribute__((packed)) DpCommand {
	uint8_t sync;
	uint8_t id;
	uint32_t argument;
	uint32_t check;
};

struct SensorData{
	//safe messages for other threats
	char i[100];
	char e[100];
	//Offsets
	char offGyroX[12],offGyroY[12],offGyroZ[12];
	char offAccX[12],offAccY[12],offAccZ[12];
	char offMagX[12],offMagY[12],offMagZ[12];
	//Actual values
	char gyroX[12],gyroY[12],gyroZ[12];
	char accX[12],accY[12],accZ[12];
	char magX[12],magY[12],magZ[12];
	char temperature[12];
};

#endif /* STRUCTS_H_ */
