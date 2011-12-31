/*
 * NavDataReceiver.cpp
 *
 *  Created on: 23.12.2011
 *      Author: max
 */

#include "NavDataReceiver.h"
#include <stdlib.h>
#include <iostream>
#include <sstream>

NavDataReceiver::NavDataReceiver(string& ip, int port)
	:ATClient(ip,port,true,100E6){
	active = false;
	navdataHeader.droneState = 0;
	navdataHeader.sequenceNumber = 0;
	navdataHeader.visionFlag = 0;

	logger.setPrefix("NavDataReceiver");
}

NavDataReceiver::~NavDataReceiver() {
	logger.log("Destruction called. Waiting for thread to terminate...");
	stopReceiving();
	join();
	logger.log("OK");
}

void NavDataReceiver::stopReceiving() {
	active = false;
}

void NavDataReceiver::addObserver(NavDataObserver* observer) {
	this->observer.push_back(observer);
}

void NavDataReceiver::removeObserver(NavDataObserver* o) {
	vector<NavDataObserver*> temp = vector<NavDataObserver*>();
	for(int i=0; i<observer.size(); i++) {
		if(observer.at(i)!=o) {
			temp.push_back(observer.at(i));
		}
	}
	observer = temp;
}

void NavDataReceiver::notifyObserver(navdata* obj) {
	for(int i=0; i<observer.size(); i++) {
		//observer[i]->newNavData(obj);
		observer.at(i)->newNavData(obj);
	}
}

void NavDataReceiver::setup() {
	logger.log("Sending trigger-data...");
	sendATCmd("\1\0\0\0");
	logger.log("Trigger-data sent");
	active = true;
}

void NavDataReceiver::execute() {
	logger.log("Thread started");
	chArr array;
	array.bytes = (const char*)malloc(1024*sizeof(char));
	array.length = 1024;

	while(active) {
		int n = UDPClient::readBytes(&array);
		if(n<=0) {
			logger.log("UDP-Receive-Timeout");
			continue;	//timeout
		}

		//stringstream stream;
		//stream << "Received data with length: " << n;
		//logger.log(stream.str());

		if(!parseIncomingData(&array)) logger.logErr("Received old income");
		else if(n>24){
			array.length = n;
			notifyObserver(&navData);
			array.length = 1024;
		}
	}

	free((void*)array.bytes);
}

bool NavDataReceiver::parseIncomingData(chArr* array) {
	int offset = 8;
	int incomingSeq = getIntFromIncome(array,&offset);

	if(incomingSeq!=1 && incomingSeq<navdataHeader.sequenceNumber) return false;
	else navdataHeader.sequenceNumber=incomingSeq;

	offset = 4;
	//navdataHeader.droneState = getIntFromIncome(array,4);

	navData.header.droneState = getIntFromIncome(array,&offset);
	navData.header.sequenceNumber = incomingSeq;
	navData.header.visionFlag = getIntFromIncome(array,&offset);

	navData.tag = getShortFromIncome(array,&offset);
	navData.size = getShortFromIncome(array,&offset);

	navData.demo.tag = getShortFromIncome(array,&offset);
	navData.demo.size = getShortFromIncome(array,&offset);
	navData.demo.ctrl_state = getIntFromIncome(array,&offset);
	navData.demo.battery = getIntFromIncome(array,&offset);
	navData.demo.pitch = getFloatFromIncome(array,&offset)/1000.0f;
	navData.demo.roll = getFloatFromIncome(array,&offset)/1000.0f;
	navData.demo.yaw = getFloatFromIncome(array,&offset)/1000.0f;
	navData.demo.altitude = ((float)getIntFromIncome(array,&offset))/1000.0f;
	navData.demo.vx = getFloatFromIncome(array,&offset);
	navData.demo.vy = getFloatFromIncome(array,&offset);
	navData.demo.vz = getFloatFromIncome(array,&offset);

	return true;
}

float NavDataReceiver::getFloatFromIncome(chArr* array, int* offset) {
	/*int tmp=0, i;
	float n;
	for(i=3; i>=0; i--) {
		n <<= 8;
		tmp = array->bytes[offset + i] & 0xFF;
		n |= tmp;
	}*/
	//*offset += 4;
	int val = getIntFromIncome(array,offset);
	return *((float*)(&val));
	//return reinterpret_cast<float&>(val);
}

short NavDataReceiver::getShortFromIncome(chArr* array, int* offset) {
	short tmp=0, n=0;
	int i;
	for(i=1; i>=0; i--) {
		n <<= 8;
		tmp = array->bytes[*offset + i] & 0xFF;
		n |= tmp;
	}
	*offset += 2;
	return n;
}

int NavDataReceiver::getIntFromIncome(chArr* array, int* offset) {
	int tmp=0, n=0, i;
	for(i=3; i>=0; i--) {
		n <<= 8;
		tmp = array->bytes[*offset + i] & 0xFF;
		n |= tmp;
	}
	*offset += 4;
	return n;
}
