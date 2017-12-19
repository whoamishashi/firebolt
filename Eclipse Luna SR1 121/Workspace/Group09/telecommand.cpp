/*
 * telecommand.cpp
 *
 *  Created on: 03.12.2017
 *      Author: I8FL-PC01-G01
 */

#include "telecommand.h"

using std::string;

Telecommand telecommand;

Telecommand::Telecommand() {
}

void Telecommand::init() {
}

void Telecommand::run() {

	TelecommandData telecommandData;
	char buffer;
	string command = "";

	while (1) {
		BT2UART.suspendUntilDataReady();
		BT2UART.read(&buffer, 1);

		switch (buffer) {
		case '$':
			command = "";
			break;
		case '#':
			telecommandData.telecommand = command;
			TelecommandDataTopic.publish(telecommandData);
			break;
		default:
			command += buffer;
			break;
		}
	}

//    DpCommand comPack;
//    decodeCommand(comPack);
}
