/*
 * sensorIMU.h
 *
 *  Created on: 03.12.2017
 *      Author: I8FL-PC01-G01
 */

#ifndef LIB_SENSORS_SENSORIMU_H_
#define LIB_SENSORS_SENSORIMU_H_

#include "rodos.h"
#include "../structs.h"
#include <stdio.h>
#include "hal.h"
#include "math.h"
#include <string>
#include "../globals.h"
#include "../topics.h"

#define IMU_I2C I2C_IDX2

// Register for LSM9DS0
#define LSM9DS0_G 0x6B
#define LSM9DS0_XM 0x1D

// Start of Output Register for LSM9DS0
#define OUT_X_L_G	 			0x28 	// GYRO read 6 register for total output in x, y and z
#define OUT_X_L_A 				0x28	// ACC has same 6 register since combined
#define OUT_X_L_M 				0x08	// MAG read 6 register for total output in x, y and z
#define OUT_TEMP_L_XM			0x05

// MSB for setups
#define MSB						0x80

//Maximum Range of IMU Measurements
#define GYRO_MAX 	2000	// degree per second //should be optimized
#define ACC_MAX		2		// 2G
#define MAG_MAX		2		// 2Gauss
// Calibration Values
#define CALI_G		7		// Gyro [(1/100)*DegPerSec/digit]		//70		// Gyro [milliDegPerSec/digit] 	in DataSheet = "G_So"
#define CALI_A		0.061	// Acc  [mG/LSB]				in DataSheet = "LA_So"
#define CALI_M		0.08	// Mag  [milliGauss/LSB] 		in DataSheet = "M_GN"
#define CALI_T		8		// Temp [LSB/°C]				from Slides

//***************************************************************************************
// Useful FUNCTIONS
//***************************************************************************************
#define GYRO_DOUBLE(x)			(x*CALI_G) //((double)x*GYRO_MAX/3INT16_MAX)
#define ACC_DOUBLE(x)			(x*CALI_A)
#define MAG_DOUBLE(x)			(x*CALI_M) //((double)x*MAG_MAX/INT16_MAX)
//#define GYRO_8to16(low,high)	((((int16_t)high<<8) | low)*CALI_G)
//Temperature is missing at the moment

class SensorIMU: public Thread {
private:
	int16_t offGyroX, offGyroY, offGyroZ;
	int16_t offAccX, offAccY, offAccZ;
	int16_t offMagX, offMagY, offMagZ;
	int32_t _angleZ;

//	uint8_t DATA[2];
//	TestData imuData;
//	uint8_t LSM9DS0_CTRL_REG1_G[2] = { 0x20, 0b01001111 };
//	uint8_t LSM9DS0_OUT_X_H_G[1] = { 0x29 };

//uint8_t LSM9DS0_OUT_X_H_G[1] = {0x28};

public:
	SensorData imuData;
	SensorIMU();
	void init();
	void run();
//	int16_t* read2value(HAL_GPIO &PinSelect,uint8_t ReadRegister , int16_t *data , int numberOfBits );
	void ImuRegSetup();
	void SensorCalibration();
	void GyroCalibration();
	void AccCalibration();
	void MagCalibration();
	void readout(HAL_GPIO &pin, uint8_t *targetregister, int16_t *dataarray,
			int numberOfBits);
	void AngleGyro();
};

extern SensorIMU sensorIMU;

#endif /* LIB_SENSORS_SENSORIMU_H_ */
