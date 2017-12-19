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

HAL_GPIO HBRIDGE_A_INA(GPIO_036); /* declare HAL_GPIO for GPIO_036 = PC4 (HBRIDGE-A INA pin) */
HAL_GPIO HBRIDGE_A_INB(GPIO_017); /* declare HAL_GPIO for GPIO_017 = PB1 (HBRIDGE-B INA pin) */
HAL_GPIO HBRIDGE_EN(GPIO_066); /* declare HAL_GPIO for GPIO_066 = PE2 (HBRIDGE Power Enable pin) */
HAL_PWM ReactionWheel(PWM_IDX12); /* declare HAL_PWM for PWM_IDX12 = TIM4-CH1 (HBRIDGE-A), please refer to hal_pwm.h for correct PWM mapping*/

ActuatorRW actuatorRW;

CommBuffer<TelecommandData> ActuatorRW_TelecommandDataBuffer;
Subscriber ActuatorRW_TelecommandDataSubscriber(TelecommandDataTopic,
		ActuatorRW_TelecommandDataBuffer);

ActuatorRW::ActuatorRW() {
	dir = 0; //cw
	m_speed = 0;
}

void ActuatorRW::init() {
	HBRIDGE_EN.init(true, 1, 1); /* initialization of the HAL object should be called one time only in the project*/
	HBRIDGE_A_INA.init(true, 1, 1); /* initialization of the HAL object should be called one time only in the project*/
	HBRIDGE_A_INB.init(true, 1, 0); /* initialization of the HAL object should be called one time only in the project*/
	ReactionWheel.init(5000, 1000); /* initialization of the HAL object should be called one time only in the project*/
	//ReactionWheel.write(250); /* Set the Duty Cycle to 25% */

}

void ActuatorRW::run() {
	init();	//WHY DO WE HAVE TO INITIALIZE THE FUNCTION MANUALLY?!?!?!?!?!?!?!
	//WHY DO WE HAVE TO INITIALIZE THE FUNCTION MANUALLY?!?!?!?!?!?!?!
	//WHY DO WE HAVE TO INITIALIZE THE FUNCTION MANUALLY?!?!?!?!?!?!?!
	//WHY DO WE HAVE TO INITIALIZE THE FUNCTION MANUALLY?!?!?!?!?!?!?!
	//ReactionWheel.write(1);
	while (1) {

		TelecommandData telecommandData;
		ActuatorRW_TelecommandDataBuffer.get(telecommandData);

		vector<string> splitted = split(telecommandData.telecommand.c_str(), ';');
		dir = atoi(splitted[2].c_str()) == 0 ? 0 : 1;
		m_speed = atoi(splitted[3].c_str());
		PRINTF("\n dir %d", dir);
		PRINTF("\n m_speed %d", m_speed);

		//ReactionWheel.write(200);
		if (dir == 0) {
			//calm motor before direction is changed

			//suspendCallerUntil(NOW()+10*MILLISECONDS);
			//change direction
			HBRIDGE_A_INB.setPins(0);
			HBRIDGE_A_INA.setPins(1);
			ReactionWheel.write(m_speed);
		} else if (dir == 1) {
			//calm motor before direction is changed
			//ReactionWheel.write(1);
			//suspendCallerUntil(NOW()+10*MILLISECONDS);
			//change direction
			HBRIDGE_A_INB.setPins(1);
			HBRIDGE_A_INA.setPins(0);
			ReactionWheel.write(m_speed);
		} else {
			HBRIDGE_A_INB.setPins(0);
			HBRIDGE_A_INA.setPins(0);
			//ReactionWheel.write(200);
		}
		//ReactionWheel.write(250); /* Set the Duty Cycle to 25% */
		suspendCallerUntil(NOW()+1000*MILLISECONDS);
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
