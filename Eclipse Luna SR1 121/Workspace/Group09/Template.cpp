
#include "rodos.h"
#include <stdio.h>
#include "hal.h"
#include "math.h"
#include <string>

static Application module01("Template", 2001);

// GPIO pins
#define LED_GREEN GPIO_060
#define LED_ORANGE GPIO_061
#define LED_RED GPIO_062
#define LED_BLUE GPIO_063

// UART
#define BT_UART UART_IDX2
#define USB_UART UART_IDX3

#define IMU_I2C I2C_IDX2

// Register for LSM9DS0
#define LSM9DS0_G 0x6B
#define LSM9DS0_XM 0x1D

// Start of Output Register for LSM9DS0
#define OUT_X_L_G 0x28 	// GYRO read 6 register for total output in x, y and z
#define OUT_X_L_A 0x28	// ACC has same 6 register since combined
#define OUT_X_L_M 0x08	// MAG read 6 register for total output in x, y and z
#define OUT_TEMP_L_XM			0x05

// MSB for setups
#define MSB						0x80


//Maximum Range of IMU Measurements
#define GYRO_MAX 	2000	// degree per second //should be optimized
#define ACC_MAX		2		// 2G
#define MAG_MAX		2		// 2Gauss
// Calibration Values
#define CALI_G		70		// Gyro [milliDegPerSec/digit] 	in DataSheet = "G_So"
#define CALI_M		0.08	// Mag  [milliGauss/LSB] 		in DataSheet = "M_GN"

//***************************************************************************************
// Useful FUNCTIONS
//***************************************************************************************
#define GYRO_DOUBLE(x)			(x) //((double)x*GYRO_MAX/3INT16_MAX)
#define ACC_DOUBLE(x)			((double)x*ACC_MAX/INT16_MAX)
#define MAG_DOUBLE(x)			(x) //((double)x*MAG_MAX/INT16_MAX)
//#define GYRO_8to16(low,high)	((((int16_t)high<<8) | low)*CALI_G)
//Temperature is missing at the moment

#define RAD2DEG(x) 				(x*180/M_PI);
#define DEG2RAD(x)				(x*M_PI/180);
//***************************************************************************************
namespace RODOS{
extern HAL_UART uart_stdout;
}
#define TeleUART uart_stdout

//GPIO
//LED and Buttons
HAL_GPIO GreenLED(LED_GREEN);
HAL_GPIO BlueLED(LED_BLUE);
HAL_GPIO BlueButton(GPIO_000);

//Servos, ReactionWheels and ReactionWheels
HAL_GPIO Servo1(GPIO_073); /* declare HAL_GPIO for GPIO_036 = PC4 (HBRIDGE-A INA pin) */
HAL_PWM ReactionWheel(PWM_IDX01); /* declare HAL_PWM for PWM_IDX12 = TIM4-CH1 (HBRIDGE-A), please refer to hal_pwm.h for correct PWM mapping*/


//HAL_UART BT2UART(BT_UART);

// for IMU
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



uint8_t IMU_G_DATA[1] = {OUT_X_L_G | MSB | 0x40};
uint8_t IMU_A_DATA[1] = {OUT_X_L_A | MSB | 0x40};
uint8_t IMU_M_DATA[1] = {OUT_X_L_M | MSB | 0x40};
uint8_t LSM9DS0_T_DATA[1] = {OUT_TEMP_L_XM | MSB | 0x40};

struct SensorData{
	//safe messages for other threats
	char i[100];
	char e[100];
	//Offsets
	float offGyroX,offGyroY,offGyroZ;
	float offAccX,offAccY,offAccZ;
	float offMagX,offMagY,offMagZ;
	//Actual values
	float gyroX,gyroY,gyroZ;
	float accX,accY,accZ;
	float magX,magY,magZ;

};


Topic<SensorData> SensorDataTopic(-1, "Sensor Data");
CommBuffer<SensorData> SensorDataBuffer;
Subscriber SensorDataSubscriber(SensorDataTopic, SensorDataBuffer);


class Telemetry: public Thread {

public:

	Telemetry(const char* name) : Thread(name) {
	}

	void init() {
		GreenLED.init(true, 1, 0);
		BlueLED.init(true, 1, 0);
		BlueButton.init(false,1,0);
		//BT2UART.init(921600);
	}

	void run() {
		SensorData Teledata;
		char string[50];
		while (1) {

			if (BlueButton.readPins()){
				BlueLED.setPins(~BlueLED.readPins());
				sprintf(string, "PENIS");
				//BT2UART.write(string, strlen(string));
				suspendCallerUntil(NOW()+1000*MILLISECONDS);
			}

			SensorDataBuffer.get(Teledata);

			GreenLED.setPins(~GreenLED.readPins());
			//PRINTF("Hello Rodos %f",SECONDS_NOW());
//			suspendCallerUntil(NOW()+1000*MILLISECONDS);

			sprintf(string, "IMU_STATUS %s \n",Teledata.i);
			//BT2UART.write(string, strlen(string));
			PRINTF(string);
			//sprintf(string, "%s \n",Teledata.e);
			//BT2UART.write(string, strlen(string));
            suspendCallerUntil(NOW()+500*MILLISECONDS);
		}
	}
};
Telemetry Telemetry("Telemetry");

/**********************************************************************/

//class Counter: public Thread {
//
//public:
//
//	Counter(const char* name) : Thread(name) {
//	}
//
//	void init() {
//		GreenLED.init(true, 1, 0);
//	}
//
//	void run() {
//		SensorData CounterData;
//		int j=0;
//		while (1) {
//			j++;
//			CounterData.i=j;
//			SensorDataTopic.publish(CounterData);
//		    suspendCallerUntil(NOW()+1000*MILLISECONDS);
//
//		}
//	}
//};
//Counter Counter("Counter");

/**********************************************************************/

//class Telecommand: public Thread {
//
//public:
//
//	Telecommand(const char* name) : Thread(name) {
//	}
//
//	void init() {
//		GreenLED.init(true, 1, 0);
//	}
//
//	void run() {
//		SensorData CounterData;
//		float j;
//		while (1) {
//			char buffer[50];
//			BT2UART.suspendUntilDataReady();
//			BT2UART.read(buffer,1);
//		}
//	}
//};
//Telecommand Telecommand("Telecommand");

/***********************************************************************/

#define TEST(x,y) (((x) << 8) | y)


class SensorIMU: public Thread {

private:

	int16_t DATA_G[3];
	int16_t DATA_M[3];
	SensorData imuData;
	uint8_t LSM9DS0_WHO_AM_I_G[1] = {0x0F};
	uint8_t LSM9DS0_CTRL_REG1_G[2] = {0x20, 0x4F};
	//uint8_t LSM9DS0_OUT_X_H_G[1] = {0x28};


public:

	SensorIMU(const char* name) : Thread(name) {
	}

	void init() {
		/* initialization of the HAL objects should be called one time only in the project*/
		CS_G.init(true, 1, 1);
		CS_XM.init(true, 1, 1);
		IMU_EN.init(true, 1, 1);
		IMU.init(1000000);
		BlueLED.init(true,1,0);
		BlueButton.init(false,1,0);
		//BT2UART.init(921600);
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

	void ImuRegSetup()
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

	void SensorCalibration(){
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
//DON'T GET DATA YET PLEASE ADD & GYRO_DOUBLE NOT FINISHED YET
			//Get Data from Sensor (Gyro)

			//Add Values
			valueX+= GYRO_DOUBLE(data[0]);
			valueY+= GYRO_DOUBLE(data[1]);
			valueZ+= GYRO_DOUBLE(data[2]);
		}
		SData.offGyroX =valueX/1000;
		SData.offGyroY =valueY/1000;
		SData.offGyroZ =valueZ/1000;


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

	void run() {
		uint8_t tempData_G[6], tempData_M[6];
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
			DATA_G[0] = (((int16_t)tempData_G[1]<<8) | tempData_G[0])*CALI_G;
			DATA_G[1] = (((int16_t)tempData_G[3]<<8) | tempData_G[2])*CALI_G;
			DATA_G[2] = (((int16_t)tempData_G[5]<<8) | tempData_G[4])*CALI_G;

			/*read LSM9DS0 (IMU) Gyro*/
			CS_XM.setPins(0);
			uint8_t LSM9DS0_OUT_X_L_M[1] = {0xC9};	//Address but with 11 in the front for read and continuous read
			IMU.write(LSM9DS0_OUT_X_L_M, 1);
			IMU.read((uint8_t*)tempData_M,6);			//read LSM9DS0_OUT_X_L_G = pitch-velocity
			CS_XM.setPins(1);



			/*publish*/
			//sprintf(imuData.e, "%f, %f, %f", GYRO_DOUBLE(TEST(DATA[1],DATA[2])),GYRO_DOUBLE(TEST(DATA[0],DATA[1])),GYRO_DOUBLE(TEST(DATA[5],DATA[6])));
			//sprintf(imuData.i, "%x ,%x ,%x ,%x ,%x ,%x ,%x ,%x ,%x ,%x ,%x ,%x", GYRO_DOUBLE(DATA[0]),GYRO_DOUBLE(DATA[1]),GYRO_DOUBLE(DATA[2]),GYRO_DOUBLE(DATA[3]),GYRO_DOUBLE(DATA[4]),GYRO_DOUBLE(DATA[5]),GYRO_DOUBLE(DATA[6]),GYRO_DOUBLE(DATA[7]),GYRO_DOUBLE(DATA[8]),GYRO_DOUBLE(DATA[9]),GYRO_DOUBLE(DATA[10]),GYRO_DOUBLE(DATA[11]));
			//sprintf(imuData.e,"X: %d    Y: %d    Z:%d",MAG_DOUBLE(DATA_M[0]),MAG_DOUBLE(DATA_M[1]),MAG_DOUBLE(DATA_M[2]));
			sprintf(imuData.i, "X':%d    Y':%d    Z':%d", GYRO_DOUBLE(DATA_G[0]),GYRO_DOUBLE(DATA_G[1]),GYRO_DOUBLE(DATA_G[2]));
			SensorDataTopic.publish(imuData);
		    suspendCallerUntil(NOW()+200*MILLISECONDS);
		}
	}
};

SensorIMU SensorIMU("SensorIMU");
/***********************************************************************/



class Motors: public Thread {

private:
	//GPIO_073
	uint8_t DATA[12];



public:

	Motors(const char* name) : Thread(name) {
	}

	void init() {
		/* initialization of the HAL objects should be called one time only in the project*/
		//CS_G.init(true, 1, 1);
		//CS_XM.init(true, 1, 1);
		//IMU_EN.init(true, 1, 1);
		//IMU.init(1000000);
		Servo1.init(true, 1, 1); /* initialization of the HAL object should be called one time only in the project*/
		ReactionWheel.init(5000, 1000); /* initialization of the HAL object should be called one time only in the project*/

	}

	void run() {
		while (1) {

			ReactionWheel.write(250); /* Set the Duty Cycle to 25% */



		}
	}
};

Motors Motors("Motors");
/***********************************************************************/


/***********************************************************************/
/* Telemetrie (standard input) and then with data conversion/usage     */
/***********************************************************************/

//class Telemetrie: public Thread {
//
//public:
//
//	Telemetrie(const char* name) : Thread(name) {
//	}
//
//	void init() {
//
//	}
//
//	void run() {
//		int counter=0;
//		char stream;
//		char tele_buf;
//		while (1) {
//			/* How to get Data from UART (in this case it is bluetooth (the standard output) */
//			TeleUART.suspendUntilDataReady();
//			TeleUART.read(&tele_buf,1);
//		      switch ( tele_buf )
//		      {
//
//		         case '#':
//		        	 counter = 0;
//		            break;
//		         case '$':
//		        	 stream = 0;
//		            break;
//		         default:
//		        	 switch(tele_buf)
//		        	 case ''
//		        	 stream[counter]=tele_buf;
//		        	 counter++;
//
//		        	 PRINTF("DEFAULT!!!");
//		      }
//			}
//			//PRINTF(&tele_buf);
//		}
//	}
//};
//
//Telemetrie Telemetrie("Telemetrie");
///***********************************************************************/




