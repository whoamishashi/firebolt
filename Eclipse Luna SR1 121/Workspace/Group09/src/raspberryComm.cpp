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

	StarTrackerData starTrackerData;
	RadioData radioData;
	IRData irData;
	ObjectRecognitionData objectRecognitionData;

	char buffer;
	string rsp_output = "";
	stringstream ss;
	vector<string> params;

	while (1) {

		RSPUART.suspendUntilDataReady();
		RSPUART.read(&buffer, 1);

		PRINTF("\ninside rspcom before switch");

		switch (buffer) {
		case '$':
			rsp_output = "";
			break;
		case '#':
			PRINTF("\nmeasurements received: %s", rsp_output.c_str());

			ss.str(rsp_output);

			while( ss.good() )
			{
			    string substr;
			    getline( ss, substr, ';' );
			    params.push_back( substr );
			}

			if(params[0].c_str() == "ST")
			{
				starTrackerData.x = atof(params[1].c_str());
				starTrackerData.y = atof(params[2].c_str());
				starTrackerData.angle = atof(params[3].c_str());
				StarTrackerDataTopic.publish(starTrackerData);
			}
			else if(params[0] == "RD")
			{
				radioData.x = atof(params[1].c_str());
				radioData.y = atof(params[2].c_str());
				RadioDataTopic.publish(radioData);
			}
			else if(params[0] == "OT")
			{
				objectRecognitionData.alpha = atof(params[1].c_str());
				objectRecognitionData.G0 = atof(params[2].c_str());
				objectRecognitionData.g0 = atof(params[3].c_str());
				objectRecognitionData.trusted = atoi(params[4].c_str());
				ObjectRecognitionDataTopic.publish(objectRecognitionData);
			}
			else
			{
				continue;
			}
			params.clear();
			break;
		default:
			rsp_output += buffer;
			break;
		}
	}
}
