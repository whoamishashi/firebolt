/*
 * Template.cpp
 *
 * Created on: 09.11.2017
 * Author: Shashikant Gupta
 * Last Modified: Shashikant Gupta
 *
 */

#include "sensorIMU.h"

#include "rodos.h"
#include "globals.h"
#include <stdio.h>
#include "hal.h"
#include "math.h"
#include <string>

HAL_GPIO BlueLED(LED_BLUE);

HAL_GPIO BlueButton(GPIO_000);

HAL_GPIO CS_G(GPIO_018); /* declare HAL_GPIO for GPIO_018 = PB2 (IMU Chip Select pin for the Gyro) */
HAL_GPIO CS_XM(GPIO_032); /* declare HAL_GPIO for GPIO_032 = PC0 (IMU Chip Select pin for the Accelerometer + Magnetometer)*/
HAL_GPIO IMU_EN(GPIO_055); /* declare HAL_GPIO for GPIO_055 = PD7 (IMU Power Enable pin) */
HAL_SPI IMU(SPI_IDX1); /* declare HAL_SPI for SPI_IDX1 = SPI1 (IMU), please refer to hal_spi.h for correct SPI mapping */

SensorIMU sensorIMU;

SensorIMU::SensorIMU() {
}

void SensorIMU::init() {

	BlueLED.init(true,1,0);
	BlueButton.init(false,1,0);

	// initialization of the HAL objects should be called one time only in the project
	CS_G.init(true, 1, 1);
	CS_XM.init(true, 1, 1);
	IMU_EN.init(true, 1, 1);
	IMU.init(1000000);

	uint8_t IMU_G_DATA[1] = {OUT_X_L_G | MSB | 0x40};
	uint8_t IMU_A_DATA[1] = {OUT_X_L_A | MSB | 0x40};
	uint8_t IMU_M_DATA[1] = {OUT_X_L_M | MSB | 0x40};
	uint8_t LSM9DS0_T_DATA[1] = {OUT_TEMP_L_XM | MSB | 0x40};
}

void SensorIMU::run() {
	uint8_t tempData_G[6], tempData_A[6], tempData_M[6], tempData_T[2];
	SensorData SData;
	ImuRegSetup();

	while (1) {
		if (BlueButton.readPins()){
			SensorCalibration();
		}

		/*read LSM9DS0 (IMU) Gyro*/
		uint8_t WHO_AM_I_G[1]={0x8F};
		uint8_t LSM9DS0_OUT_X_L_G[1] = {0xE8}; // first two bits should be 1 for contiguous read
		CS_G.setPins(0);
		IMU.write(LSM9DS0_OUT_X_L_G, 1);
		IMU.read((uint8_t*)tempData_G,6);			//read LSM9DS0_OUT_X_L_G = pitch-velocity
		CS_G.setPins(1);
		DATA_G[0] = (((int16_t)tempData_G[1]<<8) | tempData_G[0]);
		DATA_G[1] = (((int16_t)tempData_G[3]<<8) | tempData_G[2]);
		DATA_G[2] = (((int16_t)tempData_G[5]<<8) | tempData_G[4]);
		for (int i=0;i<3;i++){
			DATA_G[i]*=CALI_G;
		}
		sprintf(imuData.gyroX,"%d",DATA_G[0]);
		sprintf(imuData.gyroY,"%d",DATA_G[1]);
		sprintf(imuData.gyroZ,"%d",DATA_G[2]);


		/*read LSM9DS0 (IMU) Accelerometer*/
		uint8_t LSM9DS0_OUT_X_L_A[1] = {0xE8}; // first two bits should be 1 for contiguous read
		CS_XM.setPins(0);
		IMU.write(LSM9DS0_OUT_X_L_A, 1);
		IMU.read((uint8_t*)tempData_A,6);			//read LSM9DS0_OUT_X_L_G = pitch-velocity
		CS_XM.setPins(1);
		DATA_A[0] = (((int16_t)tempData_A[1]<<8) | tempData_A[0]);
		DATA_A[1] = (((int16_t)tempData_A[3]<<8) | tempData_A[2]);
		DATA_A[2] = (((int16_t)tempData_A[5]<<8) | tempData_A[4]);

		for (int i=0;i<3;i++){
			DATA_A[i]*=CALI_A;
		}
		sprintf(imuData.accX,"%d",DATA_A[0]);
		sprintf(imuData.accY,"%d",DATA_A[1]);
		sprintf(imuData.accZ,"%d",DATA_A[2]);


		/*read LSM9DS0 (IMU) Magnetometer*/
		CS_XM.setPins(0);
		uint8_t LSM9DS0_OUT_X_L_M[1] = {0xC9};	//Address but with 11 in the front for read and continuous read
		IMU.write(LSM9DS0_OUT_X_L_M, 1);
		IMU.read((uint8_t*)tempData_M,6);			//read LSM9DS0_OUT_X_L_G = pitch-velocity
		CS_XM.setPins(1);
		DATA_M[0] = (((int16_t)tempData_M[1]<<8) | tempData_M[0]);
		DATA_M[1] = (((int16_t)tempData_M[3]<<8) | tempData_M[2]);
		DATA_M[2] = (((int16_t)tempData_M[5]<<8) | tempData_M[4]);
		for (int i=0;i<3;i++){
			DATA_M[i]*=CALI_M;
		}
		sprintf(imuData.magX,"%d",DATA_M[0]);
		sprintf(imuData.magY,"%d",DATA_M[1]);
		sprintf(imuData.magZ,"%d",DATA_M[2]);

		/*read LSM9DS0 (IMU) Temperature */
		tempData_T[0]=0b00000000;
		CS_XM.setPins(0);
		uint8_t LSM9DS0_OUT_TEMP_L_XM[1] = {0b11000101};	//Address but with 11 in the front for read and continuous read
		IMU.write(LSM9DS0_OUT_TEMP_L_XM, 1);
		IMU.read((uint8_t*)tempData_T,2);			//read LSM9DS0_OUT_TEMP_L_XM = temperature
		CS_XM.setPins(1);
		DATA_T[0] = tempData_T[0];
		DATA_T[0] = (DATA_T[0] & 0x800) ? (-1 ^ 0xFFF) | DATA_T[0] : DATA_T[0];		//Copy pasted -> why is it exactly like this?!
		//DATA_T[0]= (((int16_t)tempData_T[1]<<8) | tempData_G[0]);
		sprintf(imuData.temperature,"%d",DATA_M[2]);

		/*publish*/
		//sprintf(imuData.e, "%f, %f, %f", GYRO_DOUBLE(TEST(DATA[1],DATA[2])),GYRO_DOUBLE(TEST(DATA[0],DATA[1])),GYRO_DOUBLE(TEST(DATA[5],DATA[6])));
		sprintf(imuData.i, "%d ,%d ,%d", DATA_A[0],DATA_A[1],DATA_A[2]);
		//sprintf(imuData.e,"X: %d    Y: %d    Z:%d",DATA_M[0],DATA_M[1],DATA_M[2]);
		//sprintf(imuData.i, "X':%d    Y':%d    Z':%d", DATA_G[0],DATA_G[1],DATA_G[2]);
		sprintf(imuData.e,"%d",DATA_T[0]);
		SensorDataTopic.publish(imuData);

	    suspendCallerUntil(NOW()+200*MILLISECONDS);
	}
}

//    //first try to make a function to get data plus converting them

//	int16_t* read2value(HAL_GPIO &PinSelect,uint8_t ReadRegister , int16_t *data , int numberOfBits ){
//		uint8_t tempData;
//		PinSelect.setPins(0);
//		ReadRegister = ReadRegister | 0b11000000;	//Address but with 11 in the front for read and continuous read
//		IMU.write(&ReadRegister, 1);
//		IMU.read((uint8_t*)tempData,numberOfBits);			//read LSM9DS0_OUT_X_L_G = pitch-velocity
//		PinSelect.setPins(1);
//		*data=tempData;
//		return data;
//
//	}



void SensorIMU::ImuRegSetup()
{// for LSM9DS0

	uint8_t xff=0xFF;

	// Gyroscope and Accelerometer
	CS_G.setPins(0);
	IMU.write(CTRL_REG1_G, 2);
	CS_G.setPins(1);
	IMU.write(&xff,1);

	CS_G.setPins(0);
	IMU.write(CTRL_REG4_G, 2);
	CS_G.setPins(1);

	// Magnetometer
	CS_XM.setPins(0);
	IMU.write(CTRL_REG1_XM, 2);
	CS_XM.setPins(1);
	IMU.write(&xff,1);

	CS_XM.setPins(0);
	IMU.write(CTRL_REG2_XM, 2);
	CS_XM.setPins(1);
	IMU.write(&xff,1);

	CS_XM.setPins(0);
	IMU.write(CTRL_REG5_XM, 2);
	CS_XM.setPins(1);
	IMU.write(&xff,1);

	CS_XM.setPins(0);
	IMU.write(CTRL_REG6_XM, 2);
	CS_XM.setPins(1);
	IMU.write(&xff,1);

	CS_XM.setPins(0);
	IMU.write(CTRL_REG7_XM, 2);
	CS_XM.setPins(1);

}

void SensorIMU::SensorCalibration(){

	SensorData SData;
	char string[120];
	char data[3];
	float valueX,valueY,valueZ,valueTemperature;

	//Gyroscope
	PRINTF("Gyro Calibration:\nPlease put it in horizontal position and make sure it doesn't move.\nIf ready press the blue button.");
	//BT2UART.write(string, strlen(string));
	suspendCallerUntil(NOW()+500*MILLISECONDS);
	while(BlueButton.readPins()!=1){
		suspendCallerUntil(NOW()+10*MILLISECONDS);
// BLUELED DOESN'T WORK YET PLEASE EDIT
		BlueLED.write(~BlueLED.readPins());
	}
	suspendCallerUntil(NOW()+100*MILLISECONDS);
	sprintf(string,"Calibrating...");
	//BT2UART.write(string, strlen(string));
	suspendCallerUntil(NOW()+100*MILLISECONDS);
	for(int i = 0; i>=1000;i++){
//DON'T GET DATA YET PLEASE ADD & GYRO_DOUBLE NOT FINISHED YET.
		//Get Data from Sensor (Gyro)

		//Add Values
		valueX+= GYRO_DOUBLE(data[0]);
		valueY+= GYRO_DOUBLE(data[1]);
		valueZ+= GYRO_DOUBLE(data[2]);
	}
//		SData.offGyroX =valueX/1000;
//		SData.offGyroY =valueY/1000;
//		SData.offGyroZ =valueZ/1000;


	//ACCELEROMETER
	PRINTF("Acc Calibration:\nPlease put floatsat in horizontal position and make sure it doesn't move.\nIf ready press the blue button.");
	//BT2UART.write(string, strlen(string));
	suspendCallerUntil(NOW()+500*MILLISECONDS);
	while(BlueButton.readPins()!=1){
		suspendCallerUntil(NOW()+10*MILLISECONDS);
// BLUELED DOESN'T WORK YET PLEASE EDIT
		BlueLED.write(~BlueLED.readPins());
	}
	suspendCallerUntil(NOW()+100*MILLISECONDS);
	PRINTF("Calibrating...");
	//BT2UART.write(string, strlen(string));
	suspendCallerUntil(NOW()+100*MILLISECONDS);


}

