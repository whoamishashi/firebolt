/*
 * ActuatorRW.cpp
 *
 *  Created on: 15 Dec 2017
 *      Author: felix
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

HAL_GPIO HBRIDGE_A_INA(GPIO_036); /* -declare HAL_GPIO for GPIO_036 = PC4 (A-HBRIDGE-A INA pin) */
HAL_GPIO HBRIDGE_A_INB(GPIO_017); /* declare HAL_GPIO for GPIO_017 = PB1 (A-HBRIDGE-B INA pin) */
//HAL_GPIO HBRIDGE_A_INA(GPIO_016); /* -declare HAL_GPIO for GPIO_036 = PB0 (B-HBRIDGE-A INA pin) */
//HAL_GPIO HBRIDGE_A_INB(GPIO_071); /* declare HAL_GPIO for GPIO_017 = PE7 (B-HBRIDGE-B INA pin) */


//HAL_GPIO HBRIDGE_A_INA(GPIO_076); /* declare HAL_GPIO for GPIO_076 = PE12 (D-HBRIDGE-A INA pin) */
//HAL_GPIO HBRIDGE_A_INB(GPIO_079); /* declare HAL_GPIO for GPIO_079 = PE15 (D-HBRIDGE-B INA pin) */
HAL_GPIO HBRIDGE_EN(GPIO_066); /* declare HAL_GPIO for GPIO_066 = PE2 (HBRIDGE Power Enable pin) */
HAL_PWM ReactionWheel(PWM_IDX12); /* declare HAL_PWM for PWM_IDX12 = TIM4-CH1 (HBRIDGE-A), please refer to hal_pwm.h for correct PWM mapping*/

ActuatorRW actuatorRW;

CommBuffer<TelecommandData> ActuatorRW_TelecommandDataBuffer;
Subscriber ActuatorRW_TelecommandDataSubscriber(TelecommandDataTopic, ActuatorRW_TelecommandDataBuffer);

CommBuffer<MotorData> MotorDataBuffer2;
Subscriber MotorDataSubscriber2(MotorDataTopic, MotorDataBuffer2);

CommBuffer<SurveillanceData> SurveillanceDataBuffer3;
Subscriber SurveillanceDataSubscriber3(SurveillanceDataTopic, SurveillanceDataBuffer3);


ActuatorRW::ActuatorRW() {
	dir = 0; //cw
	m_speed = 0;
}

void ActuatorRW::init() {
	HBRIDGE_EN.init(true, 1, 1); /* initialization of the HAL object should be called one time only in the project*/
	HBRIDGE_A_INA.init(true, 1, 1); /* initialization of the HAL object should be called one time only in the project*/
	HBRIDGE_A_INB.init(true, 1, 0); /* initialization of the HAL object should be called one time only in the project*/
	ReactionWheel.init(5000, 1000); /* initialization of the HAL object should be called one time only in the project*/
	//ReactionWheel.init(10000, 8400);
	//ReactionWheel.write(250); /* Set the Duty Cycle to 25% */

}

void ActuatorRW::run() {
	init();	//WHY DO WE HAVE TO INITIALIZE THE FUNCTION MANUALLY?!?!?!?!?!?!?!
	//WHY DO WE HAVE TO INITIALIZE THE FUNCTION MANUALLY?!?!?!?!?!?!?!
	//WHY DO WE HAVE TO INITIALIZE THE FUNCTION MANUALLY?!?!?!?!?!?!?!
	//WHY DO WE HAVE TO INITIALIZE THE FUNCTION MANUALLY?!?!?!?!?!?!?!
	//ReactionWheel.write(1);
	while (1) {

		MotorData motorData;
		MotorDataBuffer2.get(motorData);

		TelecommandData telecommandData;
		ActuatorRW_TelecommandDataBuffer.get(telecommandData);

		vector<string> splitted = split(telecommandData.telecommand.c_str(), ';');
		dir = atoi(splitted[2].c_str()) == 0 ? 0 : 1;
		m_speed = atoi(splitted[3].c_str());
		//PRINTF("\ndir:%d, m_speed:%d", dir, m_speed);

		//ReactionWheel.write(200);
		int speed = (int) (motorData.controlled_m_speed*0.9);
		//int speed1 = (int) (surveillanceData.asdf*8.4); // should be set to 8.4 for full range
		if ((motorData.controlled_m_speed>0)&& (motorData.controlled_m_speed<=1000)) {
			//calm motor before direction is changed

			//suspendCallerUntil(NOW()+10*MILLISECONDS);
			//change direction
			HBRIDGE_A_INB.setPins(0);
			HBRIDGE_A_INA.setPins(0);
			suspendCallerUntil(NOW()+1*MILLISECONDS);
			HBRIDGE_A_INB.setPins(0);
			HBRIDGE_A_INA.setPins(1);
			speed--;
			//PRINTF("speedA %d",speed);
			ReactionWheel.write(speed);
		} else if ((motorData.controlled_m_speed<0) && (motorData.controlled_m_speed>=-1000)) {
			//calm motor before direction is changed
			//ReactionWheel.write(1);
			//suspendCallerUntil(NOW()+10*MILLISECONDS) ;
			//change direction
			HBRIDGE_A_INB.setPins(0);
			HBRIDGE_A_INA.setPins(0);
			suspendCallerUntil(NOW()+1*MILLISECONDS);

			HBRIDGE_A_INB.setPins(1);
			HBRIDGE_A_INA.setPins(0);
			speed++;
			int _speed = abs(speed);
			//PRINTF("speedB %d",_speed);

			ReactionWheel.write(_speed);
		} else {
			PRINTF("speedC %d",speed);
			HBRIDGE_A_INB.setPins(0);
			HBRIDGE_A_INA.setPins(0);
			//ReactionWheel.write(200);
		}
		suspendCallerUntil(NOW()+10*MILLISECONDS);



//		if (dir == 0) {
//			//calm motor before direction is changed
//
//			//suspendCallerUntil(NOW()+10*MILLISECONDS);
//			//change direction
//			HBRIDGE_A_INB.setPins(0);
//			HBRIDGE_A_INA.setPins(1);
//			ReactionWheel.write(m_speed);
//		} else if (dir == 1) {
//			//calm motor before direction is changed
//			//ReactionWheel.write(1);
//			//suspendCallerUntil(NOW()+10*MILLISECONDS);
//			//change direction
//			HBRIDGE_A_INB.setPins(1);
//			HBRIDGE_A_INA.setPins(0);
//			ReactionWheel.write(m_speed);
//		} else {
//			HBRIDGE_A_INB.setPins(0);
//			HBRIDGE_A_INA.setPins(0);
//			//ReactionWheel.write(200);
//		}
		//ReactionWheel.write(250); /* Set the Duty Cycle to 25% */

	}
}

vector<string> ActuatorRW::split(string str, char delimiter) {
	vector<string> internal;
	stringstream ss(str); // Turn the string into a stream.
	string tok;

	while (getline(ss, tok, delimiter)) {
		internal.push_back(tok);
	}

	return internal;
}

//class ActuatorRW_TelecommandDataSubscriber: public SubscriberReceiver<
//		TelecommandData> {
//public:
//	ActuatorRW_TelecommandDataSubscriber() :
//			SubscriberReceiver<TelecommandData>(TelecommandDataTopic,
//					"ActuatorRW_TelecommandDataSubscriber") {
//	}
//	void put(TelecommandData &data) {
//		vector<string> splitted = split(data.telecommand.c_str(), ';');
//
////		switch(splitted[0])
////		{
////			case "ARW":
////				PRINTF("ActuatorRW %s %s %s %s",
////						splitted[0].c_str(),
////						splitted[1].c_str(),
////						splitted[2].c_str(),
////						splitted[2].c_str());
////				break;
////			default:
////		}
////		PRINTF("\nActuatorRW %s", data.telecommand.c_str());
//		PRINTF("\nActuatorRW %s   %s   %s   %s",
//				splitted[0].c_str(),  //ARWR
//				splitted[1].c_str(),  //mode
//				splitted[2].c_str(),  //direction
//				splitted[3].c_str()); //speed
//	}
//
//	vector<string> split(string str, char delimiter) {
//		vector<string> internal;
//		stringstream ss(str); // Turn the string into a stream.
//		string tok;
//
//		while (getline(ss, tok, delimiter)) {
//			internal.push_back(tok);
//		}
//
//		return internal;
//	}
//} actuatorRW_TelecommandDataSubscriber;
