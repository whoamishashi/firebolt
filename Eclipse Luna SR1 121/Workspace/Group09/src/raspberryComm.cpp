/*
 * raspberryComm.cpp
 *
 *  Created on: Jan 31, 2018
 *      Author: shashi
 */

/*
 * telecommand.cpp
 *
 *  Created on: 03.12.2017
 *      Author: I8FL-PC01-G01
 */

#include "../lib/raspberryComm.h"

using std::string;

RaspberryComm raspberryComm;

RaspberryComm::RaspberryComm() {
}

void RaspberryComm::init() {
}

void RaspberryComm::run() {

	//TelecommandData telecommandData;
	char buffer;
	string rsp_output = "";

	while (1) {
		RSPUART.write("Hello Raspberry.....", 20);

		RSPUART.suspendUntilDataReady();
		PRINTF("\nInside RSPUART....................");

		RSPUART.read(&buffer, 1);

		switch (buffer) {
		case '$':
			rsp_output = "";
			break;
		case '#':
			PRINTF("\n%s", rsp_output.c_str());
//			telecommandData.telecommand = rsp_output;
//			TelecommandDataTopic.publish(telecommandData);
			break;
		default:
			rsp_output += buffer;
			break;
		}
	}
}
