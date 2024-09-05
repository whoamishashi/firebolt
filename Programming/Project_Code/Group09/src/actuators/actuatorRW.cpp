/*
 * ActuatorRW.cpp
 *
 *  Created on: 15 Dec 2017
 *      Author: Felix Hessinger
 */

#include "../../lib/actuators/actuatorRW.h"

#include <stdio.h>
#include "hal.h"
#include "math.h"

#include <string>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <iterator>
using namespace std;

using std::string;
HAL_GPIO HBRIDGE_EN(GPIO_066); /* declare HAL_GPIO for GPIO_066 = PE2 (HBRIDGE Power Enable pin) */
HAL_GPIO LEDS_ARRAY(HBRIDGEBB); //enabling the pins for LEDs

HAL_GPIO HBRIDGE_RW_A(HBRIDGECA);
HAL_GPIO HBRIDGE_RW_B(HBRIDGECB);
HAL_PWM ReactionWheel(HBRIDGE_TIMER_C);

////Optional GPIO pins and timers for alternative writing without defined expressions like HBRIDGECA:
////HBridge-A
//HAL_GPIO HBRIDGE_A_INA(GPIO_036); /* -declare HAL_GPIO for GPIO_036 = PC4 (A-HBRIDGE-A INA pin) */
//HAL_GPIO HBRIDGE_A_INB(GPIO_017); /* declare HAL_GPIO for GPIO_017 = PB1 (A-HBRIDGE-B INA pin) */
//HAL_PWM ReactionWheel(PWM_IDX12); /* declare HAL_PWM for PWM_IDX12 = TIM4-CH1 (HBRIDGE-A), please refer to hal_pwm.h for correct PWM mapping*/
////HBridge-B
//HAL_GPIO HBRIDGE_A_INA(GPIO_016); /* -declare HAL_GPIO for GPIO_036 = PB0 (B-HBRIDGE-A INA pin) */
//HAL_GPIO HBRIDGE_A_INB(GPIO_071); /* declare HAL_GPIO for GPIO_017 = PE7 (B-HBRIDGE-B INA pin) */
//HAL_PWM ReactionWheel(PWM_IDX13); /* declare HAL_PWM for PWM_IDX13 = TIM4-CH2 (HBRIDGE-B), please refer to hal_pwm.h for correct PWM mapping*/
////HBridge-C
//HAL_GPIO HBRIDGE_A_INA(GPIO_072); /* declare HAL_GPIO for GPIO_076 = PE08 (C-HBRIDGE-A INA pin) */
//HAL_GPIO HBRIDGE_A_INB(GPIO_074); /* declare HAL_GPIO for GPIO_079 = PE10 (C-HBRIDGE-B INA pin) */
//HAL_PWM ReactionWheel(PWM_IDX14); /* declare HAL_PWM for PWM_IDX14 = TIM4-CH3 (HBRIDGE-C), please refer to hal_pwm.h for correct PWM mapping*/
////HBridge-D
//HAL_GPIO HBRIDGE_A_INA(GPIO_076); /* declare HAL_GPIO for GPIO_076 = PE12 (D-HBRIDGE-A INA pin) */
//HAL_GPIO HBRIDGE_A_INB(GPIO_079); /* declare HAL_GPIO for GPIO_079 = PE15 (D-HBRIDGE-B INA pin) */
//HAL_PWM ReactionWheel(PWM_IDX15); /* declare HAL_PWM for PWM_IDX15 = TIM4-CH4 (HBRIDGE-D), please refer to hal_pwm.h for correct PWM mapping*/

ActuatorRW actuatorRW;

CommBuffer<TelecommandData> ActuatorRW_TelecommandDataBuffer;
Subscriber ActuatorRW_TelecommandDataSubscriber(TelecommandDataTopic,
		ActuatorRW_TelecommandDataBuffer);

CommBuffer<MotorData> MotorDataBuffer2;
Subscriber MotorDataSubscriber2(MotorDataTopic, MotorDataBuffer2);

CommBuffer<SurveillanceData> SurveillanceDataBuffer3;
Subscriber SurveillanceDataSubscriber3(SurveillanceDataTopic,
		SurveillanceDataBuffer3);

int speed;

ActuatorRW::ActuatorRW() {
	dir = 0; //cw
	m_speed = 0;
}

void ActuatorRW::init() {
	HBRIDGE_EN.init(true, 1, 1); /* initialization of the HAL object should be called one time only in the project*/
	HBRIDGE_RW_A.init(true, 1, 1); /* initialization of the HAL object should be called one time only in the project*/
	HBRIDGE_RW_B.init(true, 1, 0); /* initialization of the HAL object should be called one time only in the project*/
	LEDS_ARRAY.init(true, 1, 1); /* turn on the LED-Array that has to be used for mission: OBJECT-DETECTION
	 ReactionWheel.init(5000, 1000); /* initialization of the HAL object should be called one time only in the project*/
}

void ActuatorRW::run() {
	init();	//to be set since rodos didn't execute the init function in this thread some times
	suspendCallerUntil(WAITINGTIME_UNTIL_START * SECONDS); 		//makes sure that motor doesn't move during calibration
	while (1) {

		MotorData motorData;
		MotorDataBuffer2.get(motorData);

		TelecommandData telecommandData;
		ActuatorRW_TelecommandDataBuffer.get(telecommandData);

		speed = (int) (motorData.controlled_m_speed);

		if ((motorData.controlled_m_speed > 0)
				&& (motorData.controlled_m_speed <= 1000)) {
			//change direction
			HBRIDGE_RW_B.setPins(0);
			HBRIDGE_RW_A.setPins(0);
			suspendCallerUntil(NOW()+10*NANOSECONDS);
			HBRIDGE_RW_B.setPins(0);
			HBRIDGE_RW_A.setPins(1);
			speed--;
			ReactionWheel.write(speed);
		} else if ((motorData.controlled_m_speed < 0)
				&& (motorData.controlled_m_speed >= -1000)) {
			//change direction
			HBRIDGE_RW_B.setPins(0);
			HBRIDGE_RW_A.setPins(0);
			suspendCallerUntil(NOW()+10*NANOSECONDS);
			HBRIDGE_RW_B.setPins(1);
			HBRIDGE_RW_A.setPins(0);
			speed++;					//secures that 100% is not reached. write(100% duty cycle) causes sometimes no movement of the motor
			int _speed = (int) abs(speed);
			ReactionWheel.write(_speed);
		} else { // WATCHDOG for wrong inputs
			HBRIDGE_RW_B.setPins(0);
			HBRIDGE_RW_A.setPins(0);
		}
		suspendCallerUntil(NOW()+50*MILLISECONDS);

	}
}

