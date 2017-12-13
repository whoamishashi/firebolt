/*
 * sensorIMU.cpp
 *
 * Created on: 09.11.2017
 * Author: Felix Hessinger
 * Last Modified: Felix Hessinger
 *
 */

#include "sensorIMU.h"



// blue button and blue LED
HAL_GPIO BlueLED(LED_BLUE);
HAL_GPIO BlueButton(GPIO_000);

// GPIO pins for IMU
HAL_GPIO CS_G(GPIO_018); /* declare HAL_GPIO for GPIO_018 = PB2 (IMU Chip Select pin for the Gyro) */
HAL_GPIO CS_XM(GPIO_032); /* declare HAL_GPIO for GPIO_032 = PC0 (IMU Chip Select pin for the Accelerometer + Magnetometer)*/
HAL_GPIO IMU_EN(GPIO_055); /* declare HAL_GPIO for GPIO_055 = PD7 (IMU Power Enable pin) */
HAL_SPI IMU(SPI_IDX1); /* declare HAL_SPI for SPI_IDX1 = SPI1 (IMU), please refer to hal_spi.h for correct SPI mapping */


// Setup for registers as recommended in slide 34 of "FloatSat Rodos(Hals) with stm32f4 & ahrs"
uint8_t CTRL_REG1_G[2] = {0x20, 0x4F};
uint8_t CTRL_REG4_G[2] = {0x23, 0xB0};
uint8_t CTRL_REG1_XM[2] = {0x20, 0x7F};
uint8_t CTRL_REG2_XM[2] = {0x21, 0xC0};
uint8_t CTRL_REG5_XM[2] = {0x24, 0x94};
uint8_t CTRL_REG6_XM[2] = {0x25, 0x00};
uint8_t CTRL_REG7_XM[2] = {0x26, 0x80};

// Register for Readout
uint8_t IMU_G_DATA[1] = {OUT_X_L_G | MSB | 0x40};
uint8_t IMU_A_DATA[1] = {OUT_X_L_A | MSB | 0x40};
uint8_t IMU_M_DATA[1] = {OUT_X_L_M | MSB | 0x40};
uint8_t IMU_T_DATA[1] = {OUT_TEMP_L_XM | MSB | 0x40};  	// LSM9DS0_T_DATA



int16_t temp_GAM[3], temp_T[1];
int16_t DATA_G[3];
int16_t DATA_A[3];
int16_t DATA_M[3];
int16_t DATA_T[1];


SensorData SData;
SensorIMU sensorIMU;

SensorIMU::SensorIMU() {
}

void SensorIMU::GyroCalibration(){
	int32_t offset[3] = {0,0,0};
	suspendCallerUntil(NOW()+2000*MILLISECONDS);

	for (int j=0; j<1000;j++)
	{
		readout(CS_G,IMU_G_DATA,temp_GAM,6);
		for (int i=0;i<3;i++){
		temp_GAM[i]*=CALI_G; 			// now in [mDeg/sec]
		offset[i] = offset[i] + (int32_t)temp_GAM[i];
		PRINTF("%d    ", temp_GAM[i]);
		temp_GAM[i]=0;
		}
		PRINTF("\n");
	}
	PRINTF("Offset:\nXd:%d\nYd:%d\nZd:%d\n", offset[0], offset[1], offset[2]);
	for (int i=0;i<3;i++){
		offset[i]/=1000;
	}
	PRINTF("Offset:\nXd:%d\nYd:%d\nZd:%d\n", offset[0], offset[1], offset[2]);
}
void SensorIMU::MagCalibration(){

}


void SensorIMU::AccCalibration(){

}



void SensorIMU::readout(HAL_GPIO &pin, uint8_t *targetregister, int16_t *dataarray, int numberOfBits){
	pin.setPins(0);
	IMU.write(targetregister, 1);
	IMU.read((uint8_t*)dataarray , numberOfBits);
	pin.setPins(1);

}



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



void SensorIMU::init() {

	BlueLED.init(true,1,0);
	BlueButton.init(false,1,0);

	// initialization of the HAL objects should be called one time only in the project
	CS_G.init(true, 1, 1);
	CS_XM.init(true, 1, 1);
	IMU_EN.init(true, 1, 1);
	IMU.init(1000000);


}

void SensorIMU::run() {

	ImuRegSetup();




	while (1) {

		if (BlueButton.readPins()){
			SensorCalibration();
			GyroCalibration();
			MagCalibration();
			AccCalibration();
		}
/*
		//read LSM9DS0 (IMU) Gyro

		readout(CS_G,IMU_G_DATA,temp_GAM,6);
		for (int i=0;i<3;i++){
			temp_GAM[i]*=CALI_G; 			// now in [mDeg/sec]
		}

		PRINTF("%d,",temp_GAM[0]);
		PRINTF("%d,",temp_GAM[1]);
		PRINTF("%d\n",temp_GAM[2]);
		suspendCallerUntil(NOW()+500*MILLISECONDS);

		sprintf(imuData.gyroX,"%d",DATA_G[0]);
		sprintf(imuData.gyroY,"%d",DATA_G[1]);
		sprintf(imuData.gyroZ,"%d",DATA_G[2]);



		//read LSM9DS0 (IMU) Accelerometer
		readout(CS_XM,IMU_A_DATA,temp_GAM,6);

		for (int i=0;i<3;i++){
			temp_GAM[i]*=CALI_A;
		}

		PRINTF("%d,",temp_GAM[0]);
		PRINTF("%d,",temp_GAM[1]);
		PRINTF("%d\n",temp_GAM[2]);
		suspendCallerUntil(NOW()+500*MILLISECONDS);

		sprintf(imuData.accX,"%d",DATA_A[0]);
		sprintf(imuData.accY,"%d",DATA_A[1]);
		sprintf(imuData.accZ,"%d",DATA_A[2]);

*/
		//read LSM9DS0 (IMU) Magnetometer

		readout(CS_XM,IMU_M_DATA,temp_GAM,6);

//		CS_XM.setPins(0);
//		IMU.write(IMU_M_DATA, 1);
//		IMU.read((uint8_t*)temp_GAM,6);			//read LSM9DS0_OUT_X_L_G = pitch-velocity
//		CS_XM.setPins(1);
//		DATA_M[0] = (((int16_t)temp_GAM[1]<<8) | temp_GAM[0]);
//		DATA_M[1] = (((int16_t)temp_GAM[3]<<8) | temp_GAM[2]);
//		DATA_M[2] = (((int16_t)temp_GAM[5]<<8) | temp_GAM[4]);
		for (int i=0;i<3;i++){
			temp_GAM[i]*=CALI_M;
		}

		PRINTF("%d,",temp_GAM[0]);
		PRINTF("%d,",temp_GAM[1]);
		PRINTF("%d\n",temp_GAM[2]);
		suspendCallerUntil(NOW()+500*MILLISECONDS);


		imuData.magX = temp_GAM[0];
		imuData.magY = temp_GAM[1];
		imuData.magZ = temp_GAM[2];

/*
		// read LSM9DS0 (IMU) Temperature
		readout(CS_XM,IMU_T_DATA,temp_T,2);
		temp_T[0]=temp_T[0]/CALI_T +18;
		PRINTF("%d\n,",temp_T[0]);
		suspendCallerUntil(NOW()+500*MILLISECONDS);

*/

		//publish
		sprintf(imuData.i, "%d ,%d ,%d", DATA_A[0],DATA_A[1],DATA_A[2]);
		sprintf(imuData.e,"%d",DATA_T[0]);
		//SensorDataTopic.publish(imuData);

	    //suspendCallerUntil(NOW()+200*MILLISECONDS);

	}
}




void SensorIMU::SensorCalibration(){

//	SensorData SData;
//	char string[120];
//	char data[3];
//	float valueX,valueY,valueZ,valueTemperature;
//
//	//Gyroscope
//	PRINTF("Gyro Calibration:\nPlease put it in horizontal position and make sure it doesn't move.\nIf ready press the blue button.");
//	//BT2UART.write(string, strlen(string));
//	suspendCallerUntil(NOW()+500*MILLISECONDS);
//	while(BlueButton.readPins()!=1){
//		suspendCallerUntil(NOW()+10*MILLISECONDS);
//// BLUELED DOESN'T WORK YET PLEASE EDIT
//		BlueLED.write(~BlueLED.readPins());
//	}
//	suspendCallerUntil(NOW()+100*MILLISECONDS);
//	sprintf(string,"Calibrating...");
//	//BT2UART.write(string, strlen(string));
//	suspendCallerUntil(NOW()+100*MILLISECONDS);
//	for(int i = 0; i>=1000;i++){
////DON'T GET DATA YET PLEASE ADD & GYRO_DOUBLE NOT FINISHED YET.
//		//Get Data from Sensor (Gyro)
//
//		//Add Values
//		valueX+= GYRO_DOUBLE(data[0]);
//		valueY+= GYRO_DOUBLE(data[1]);
//		valueZ+= GYRO_DOUBLE(data[2]);
//	}
////		SData.offGyroX =valueX/1000;
////		SData.offGyroY =valueY/1000;
////		SData.offGyroZ =valueZ/1000;
//


}






