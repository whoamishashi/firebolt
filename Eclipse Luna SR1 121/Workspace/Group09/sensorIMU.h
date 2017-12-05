/*
 * sensorIMU.h
 *
 *  Created on: 03.12.2017
 *      Author: I8FL-PC01-G01
 */

#ifndef SENSORIMU_H_
#define SENSORIMU_H_

#include "rodos.h"
#include "topics.h"
#include "structs.h"

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
#define CALI_G		70		// Gyro [milliDegPerSec/digit] 	in DataSheet = "G_So"
#define CALI_A		0.061	// Acc  [mG/LSB]				in DataSheet = "LA_So"
#define CALI_M		0.08	// Mag  [milliGauss/LSB] 		in DataSheet = "M_GN"
#define CALI_TEMP	8		// Temp [LSB/°C]				from Slides

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
//	uint8_t DATA[2];
//	TestData imuData;
//	uint8_t LSM9DS0_CTRL_REG1_G[2] = { 0x20, 0b01001111 };
//	uint8_t LSM9DS0_OUT_X_H_G[1] = { 0x29 };

	// Setup for registers as recommended in slide 34 of "FloatSat Rodos(Hals) with stm32f4 & ahrs"
	uint8_t CTRL_REG1_G[2] = {0x20, 0x4F};
	uint8_t CTRL_REG4_G[2] = {0x23, 0xB0};
	uint8_t CTRL_REG1_XM[2] = {0x20, 0x7F};
	uint8_t CTRL_REG2_XM[2] = {0x21, 0xC0};
	uint8_t CTRL_REG5_XM[2] = {0x24, 0x94};
	uint8_t CTRL_REG6_XM[2] = {0x25, 0x00};
	uint8_t CTRL_REG7_XM[2] = {0x26, 0x80};

	int16_t DATA_G[3];
	int16_t DATA_A[3];
	int16_t DATA_M[3];
	int16_t DATA_T[1];
	SensorData imuData;
	uint8_t LSM9DS0_WHO_AM_I_G[1] = {0x0F};
	uint8_t LSM9DS0_CTRL_REG1_G[2] = {0x20, 0x4F};
	//uint8_t LSM9DS0_OUT_X_H_G[1] = {0x28};

public:
	SensorIMU();
	void init();
	void run();
//	int16_t* read2value(HAL_GPIO &PinSelect,uint8_t ReadRegister , int16_t *data , int numberOfBits );
	void ImuRegSetup();
	void SensorCalibration();
};

extern SensorIMU sensorIMU;

#endif /* SENSORIMU_H_ */
