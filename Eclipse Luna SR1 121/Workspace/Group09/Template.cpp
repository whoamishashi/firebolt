/*
 * Template.cpp
 *
 * Created on: 25.06.2014
 * Author: Atheel Redah
 *
 */

#include "rodos.h"
#include <stdio.h>
#include "hal.h"
#include "math.h"
#include <string>

static Application module01("Template", 2001);

#define LED_GREEN GPIO_060
#define LED_ORANGE GPIO_061
#define LED_RED GPIO_062
#define LED_BLUE GPIO_063

#define BT_UART UART_IDX2
#define USB_UART UART_IDX3

#define IMU_I2C I2C_IDX2

HAL_GPIO GreenLED(LED_GREEN);
HAL_UART BT2UART(BT_UART);

struct SensorData{
	char i[100];
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
		BT2UART.init(921600);
	}

	void run() {
		SensorData Teledata;
		char string[50];
		while (1) {
			SensorDataBuffer.get(Teledata);

			GreenLED.setPins(~GreenLED.readPins());
			//PRINTF("Hello Rodos %f",SECONDS_NOW());
//			suspendCallerUntil(NOW()+1000*MILLISECONDS);

			sprintf(string, "IMU_STATUS %s \n",Teledata.i);
			BT2UART.write(string, strlen(string));
            suspendCallerUntil(NOW()+1000*MILLISECONDS);
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

HAL_GPIO CS_G(GPIO_018); /* declare HAL_GPIO for GPIO_018 = PB2 (IMU Chip Select pin for the Gyro) */
HAL_GPIO CS_XM(GPIO_032); /* declare HAL_GPIO for GPIO_032 = PC0 (IMU Chip Select pin for the Accelerometer + Magnetometer)*/
HAL_GPIO IMU_EN(GPIO_055); /* declare HAL_GPIO for GPIO_055 = PD7 (IMU Power Enable pin) */
HAL_SPI IMU(SPI_IDX1); /* declare HAL_SPI for SPI_IDX1 = SPI1 (IMU), please refer to hal_spi.h for correct SPI mapping */

class SensorIMU: public Thread {

private:

	uint8_t DATA[2];
	SensorData imuData;
	uint8_t LSM9DS0_CTRL_REG1_G[2] = {0x20, 0b01001111};
	uint8_t LSM9DS0_OUT_X_H_G[1] = {0x29};

public:

	SensorIMU(const char* name) : Thread(name) {
	}

	void init() {
		/* initialization of the HAL objects should be called one time only in the project*/
		CS_G.init(true, 1, 1);
		CS_XM.init(true, 1, 1);
		IMU_EN.init(true, 1, 1);
		IMU.init(1000000);
	}

	void run() {
		while (1) {
			/*initialize LSM9DS0_CTRL_REG1_G */
			LSM9DS0_CTRL_REG1_G[0] &= 0x3F; //If write, bit 0 of MSB should be 0; If single byte, bit 1 of MSB should be 0
			CS_G.setPins(0);
			IMU.write (LSM9DS0_CTRL_REG1_G, 2);
			CS_G.setPins(1);

			/*read LSM9DS0_OUT_X_H_G*/
			LSM9DS0_OUT_X_H_G[0] |= 0b11000000; // first two bits should be 1 for contiguous read
			CS_G.setPins(0);
			IMU.writeRead(LSM9DS0_OUT_X_H_G, 1, DATA, 2); //read LSM9DS0_OUT_X_L_G
			CS_G.setPins(1);

			/*publish*/
			sprintf(imuData.i, "LSM9DS0_OUT_X_H_G = %x", DATA[1]);
			SensorDataTopic.publish(imuData);
		    suspendCallerUntil(NOW()+500*MILLISECONDS);
		}
	}
};

SensorIMU SensorIMU("SensorIMU");
/***********************************************************************/
