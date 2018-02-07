/*
 * telecommand.cpp
 *
 *  Created on: 03.12.2017
 *      Author: I8FL-PC01-G01
 */

#include "../lib/telecommand.h"
#include <sstream>

using std::string;

Telecommand telecommand;

Telecommand::Telecommand() {
}

void Telecommand::init() {
}

void Telecommand::run() {

	TelecommandData telecommandData;
	char buffer;
	string received = "";
	stringstream ss;
	vector<string> params;

	while (1) {
		BT2UART.suspendUntilDataReady();
		BT2UART.read(&buffer, 1);

		switch (buffer) {
		case '$':
			received = "";
			break;
		case '#':
			PRINTF("\nTelecommand received: %s", received.c_str());

			ss.str(received);
			while( ss.good() )
			{
			    string substr;
			    getline( ss, substr, ';' );
			    params.push_back( substr );
			}

			//put range checks
			telecommandData.telecommand = params[0].c_str();
			telecommandData.mode = atoi(params[1].c_str());
			telecommandData.rwRotationSpeed = atoi(params[2].c_str());
			telecommandData.t1 = atoi(params[3].c_str());
			telecommandData.t2 = atoi(params[4].c_str());
			telecommandData.t3 = atoi(params[5].c_str());
			telecommandData.servoAngle = atoi(params[6].c_str());
			telecommandData.rotate180 = atoi(params[7].c_str());
			telecommandData.rotateDelta = atoi(params[8].c_str());
			telecommandData.targetPosition_x = atof(params[9].c_str());
			telecommandData.targetPosition_y = atof(params[10].c_str());
			telecommandData.desiredAngle = atoi(params[11].c_str());
			telecommandData.desiredAngularVel = atoi(params[12].c_str());
			telecommandData.activateMagnet = atoi(params[13].c_str());

			TelecommandDataTopic.publish(telecommandData);

			params.clear();

			if (telecommandData.mode == 4) RSPUART.write("$OBT#", 7);
			if (telecommandData.mode == 5) RSPUART.write("$RAD#", 7);
			else RSPUART.write("$STT\n", 7);

			break;
		default:
			received += buffer;
			break;
		}
	}
}
