/*
 * telecommand.cpp
 *
 *  Created on: 03.12.2017
 *      Author: I8FL-PC01-G01
 */

#include "telecommand.h"

Telecommand telecommand;

Telecommand::Telecommand() {
}

int Telecommand::decodeCommand(DpCommand &comPack) {
	if (comPack.sync != SYNC_COMM)
		return -1;

	if (Murmur::mm_hash_32((uint8_t*) &comPack,
			sizeof(comPack) - sizeof(comPack.check)) != comPack.check)
		return -1;

	//PRINTF("Hash: %x", Murmur::mm_hash_32((uint8_t*)&comPack, 4));

	switch (comPack.id) {
	case FORMAT_SD:
		//storageController.format();
		break;

	default:
		return -1;
	}

	//Telecommand::counter++;
	return 0;
}

void Telecommand::run() {
    DpCommand comPack;

    decodeCommand(comPack);

}
