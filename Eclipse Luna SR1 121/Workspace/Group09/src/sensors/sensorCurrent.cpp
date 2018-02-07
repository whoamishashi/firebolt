/*
 * sensorCurrent.cpp
 *
 *  Created on: 22 Dec 2017
 *      Author: felix
 */

#include "../../lib/sensors/sensorCurrent.h"

#include <stdio.h>
//#include <string>


Surveillance sensorCurrent;

//registers
#define CONFIG_R		0x00	// configuration register
#define V_SHUNT_R		0x01	// differential shunt voltage
#define V_BUS_R			0x02	// bus voltage (wrt to system/chip GND)
#define P_BUS_R			0x03	// system power draw (= V_BUS * I_SHUNT)
#define I_SHUNT_R		0x04	// shunt current
#define CAL_R			0x05	// calibration register

#define INA219_REG_SHUNTVOLTAGE                (0x01)
#define INA219_REG_BUSVOLTAGE                  (0x02)
#define INA219_REG_POWER                       (0x03)
#define INA219_REG_CURRENT                     (0x04)
#define INA219_REG_CALIBRATION					(0x05)

#define INA219_CONFIG_GAIN_8_320MV             (0x1800)
#define INA219_CONFIG_BVOLTAGERANGE_32V        (0x2000)
#define INA219_CONFIG_BADCRES_12BIT            (0x0400)
#define INA219_CONFIG_SADCRES_12BIT_1S_532US   (0x0018)
#define INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS (0x0007)

//=======Battery stuff======
#define ADDRESS 0b01000000
HAL_I2C HAL_Voltage(I2C_IDX1);
uint8_t testbuffer[2];
uint16_t config = 0;
CommBuffer<SurveillanceData> SurveillanceDataBuffer1;
Subscriber SurveillanceDataSubscriber1(SurveillanceDataTopic, SurveillanceDataBuffer1);


Surveillance::Surveillance() : Thread("Surveillance"){
}

void Surveillance::init(){
	suspendCallerUntil(NOW()+(10*MILLISECONDS));
	//Battery init
	HAL_Voltage.init(400000);

	config = INA219_CONFIG_BVOLTAGERANGE_32V |
	INA219_CONFIG_GAIN_8_320MV |
	INA219_CONFIG_BADCRES_12BIT |
	INA219_CONFIG_SADCRES_12BIT_1S_532US |
	INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
	uint8_t txBuf[3];
	txBuf[0] = CONFIG_R;
	txBuf[1] = (config >> 8) & 0xFF;
	txBuf[2] = config & 0xFF;

	HAL_Voltage.write(0x40, txBuf, 3);

	HAL_Voltage.read(0x40,testbuffer,2);

}

void Surveillance::run(){
	init();
	surveillanceData.asdf =-1100;
	PRINTF("qewr %d", config);
	uint16_t testbuffer2 = (testbuffer[0]<<8 | testbuffer[1]);
	PRINTF("asdf %d", testbuffer2);

	while(1){
		//=========================Battery stuff===========================
		//Voltage
		uint8_t read[2];
		uint8_t txBuf[1];
		txBuf[0] = V_BUS_R;
		HAL_Voltage.write(0x40, txBuf, 1);
		HAL_Voltage.read(0x40, read, 2);
//		HAL_Voltage.writeRead(0x40, txBuf, 1, read, 2);
		uint16_t batteryVoltage = (uint16_t) ((read[0] << 8) | read[1]);
		batteryVoltage = (int16_t)((batteryVoltage >> 3) * 4);
		float batteryvoltageTemp = (float) batteryVoltage;
		surveillanceData.batteryVoltage = (float) batteryvoltageTemp;
		//Current
		uint16_t value = 0x1000;				// 1A
		uint8_t tx[3];
		tx[0] = INA219_REG_CALIBRATION;
		tx[1] = (value >> 8) & 0xFF;
		tx[2] = value & 0xFF;
		HAL_Voltage.write(0x40, tx, 3);
		memset(txBuf, 0, sizeof(txBuf));
		txBuf[0] = I_SHUNT_R;
		HAL_Voltage.writeRead(0x40,txBuf,1,read,2);
		uint16_t batteryCurrent = (uint16_t) ((read[1] << 8) | read[0]);
		surveillanceData.batteryCurrent = batteryCurrent / 100.0;


		if (surveillanceData.asdf<1000){
			surveillanceData.asdf+=10;
		}else{
			surveillanceData.asdf = -1000;
		}

		//Publish the value
		SurveillanceDataTopic.publish(surveillanceData);

		suspendCallerUntil(NOW()+100*MILLISECONDS);
	}
}












//void Surveillance::readout(HAL_GPIO &pin, uint8_t *targetregister, int16_t *dataarray, int numberOfBits) {
//	pin.setPins(0);
//	HAL_Voltage.write(targetregister, 1);
//	HAL_Voltage.read((uint8_t*) dataarray, numberOfBits);
//	pin.setPins(1);
//}
