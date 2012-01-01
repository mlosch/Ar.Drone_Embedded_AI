/*
 * ATClient.cpp
 *
 *  Created on: 23.12.2011
 *      Author: max
 */

#include "ATClient.h"
#include <sstream>

volatile unsigned int ATClient::at_seq = 1;

ATClient::ATClient(string& ip, int port, bool timeout, long useconds)
	:UDPClient(ip,port,timeout,useconds){
	active = false;
	sem_init(&timeRWSemaphore,0,1);
	clock_gettime(CLOCK_REALTIME,&lastSendTimestamp);
}

ATClient::ATClient(string& ip, int port)
	:UDPClient(ip,port,false,0){
	ATClient(ip,port,false,0);
}

ATClient::~ATClient(){
	logger.log("Destruction called. Waiting for thread to terminate...");
	stopSending();
	join();
	logger.log("OK");
	sem_destroy(&timeRWSemaphore);
}

bool ATClient::sendATCmd(const string& cmd) {
	string atcmd = cmd+"\r";
	chArr array;
	array.bytes = atcmd.c_str();
	array.length = atcmd.length();

	//logger.log(cmd);
	int n = sendBytes(&array);
	if(n!=array.length) logger.logErr("Error while sending AT-CMD");

	if(n==array.length) return true;
	else return false;
}

int makeInt(float f) {
	//if(sizeof(int)!=sizeof(float)) error("Assertion error",FALSE);
	return *(int*)(&f);
}

bool ATClient::sendPCMD(int enable, float pitch, float roll, float gaz, float yaw) {
	stringstream stream;
	stream << "Sending PCMD: " << enable << ", pitch: " << pitch << ", roll: " << roll << ", gaz " << gaz << ", yaw: " << yaw;
	logger.log(stream);
	stream.str(std::string());
	stream << "AT*COMWDG=" << getSeq() << "\r";
	stream << "AT*PCMD=" << getSeq() << "," << enable << "," << makeInt(pitch) << "," << makeInt(roll) << "," << makeInt(gaz) << "," << makeInt(yaw);

	sem_wait(&timeRWSemaphore);
	clock_gettime(CLOCK_REALTIME,&lastSendTimestamp);
	sem_post(&timeRWSemaphore);

	return sendATCmd(stream.str());
}

bool ATClient::sendTakeoffCmd() {
	stringstream stream;
	stream << "AT*REF=" << getSeq() << ",290718208";
	return sendATCmd(stream.str());
}

bool ATClient::sendLandingCmd() {
	stringstream stream;
	stream << "AT*REF=" << getSeq() << ",290717696";
	return sendATCmd(stream.str());
}

bool ATClient::sendHoveringCmd() {
	stringstream stream;
	stream << "AT*PCMD=" << getSeq() << ",1,0,0,0,0";
	return sendATCmd(stream.str());
}

bool ATClient::sendEmergencyCmd() {
	stringstream stream;
	stream << "AT*REF=" << getSeq() << ",290717952";
	return sendATCmd(stream.str());
}

bool ATClient::sendFlatTrimCmd() {
	stringstream stream;
	stream << "AT*FTRIM=" << getSeq();
	return sendATCmd(stream.str());
}

bool ATClient::sendUnpairCmd() {
	stringstream stream;
	stream << "AT*CONFIG=" << getSeq() << ",\"network:owner_mac\",\"00:00:00:00:00:00\"";
	return sendATCmd(stream.str());
}

unsigned int ATClient::getSeq() {
	at_seq++;
	return at_seq-1;
}

void ATClient::setup() {
	logger.log("Sending config-data...");

	stringstream stream;
	stream << "AT*PMODE=" << getSeq() << ",2";
	sendATCmd(stream.str());
	usleep(100*1000);

	stream.str(std::string());
	stream << "AT*MISC=" << getSeq() << ",2,20,2000,3000";
	sendATCmd(stream.str());
	usleep(100*1000);

	stream.str(std::string());
	stream << "AT*REF=" << getSeq() << ",290717696";
	sendATCmd(stream.str());
	usleep(100*1000);

	stream.str(std::string());
	stream << "AT*COMWDG=" << getSeq();
	sendATCmd(stream.str());
	usleep(100*1000);

	stream.str(std::string());
	stream << "AT*CONFIG=" << getSeq() << ",\"general:control_level\",\"0\"";
	sendATCmd(stream.str());
	usleep(100*1000);

	stream.str(std::string());
	stream << "AT*CONFIG=" << getSeq() << ",\"general:navdata_demo\",\"TRUE\"";
	sendATCmd(stream.str());
	usleep(100*1000);

	stream.str(std::string());
	stream << "AT*REF=" << getSeq() << ",290717696";
	sendATCmd(stream.str());
	usleep(100*1000);

	sendPCMD(0,0.0f,0.0f,0.0f,0.0f);
	usleep(100*1000);

	stream.str(std::string());
	stream << "AT*REF=" << getSeq() << ",290717696";
	sendATCmd(stream.str());
	usleep(100*1000);

	stream.str(std::string());
	stream << "AT*REF=" << getSeq() << ",290717696";
	sendATCmd(stream.str());

	logger.log("Config-data sent");
	active = true;
}

void ATClient::execute() {
	stringstream stream;
	timespec temp,start;
	while(active) {
		//usleep(1000*1500);
		usleep(30*1000);

		sem_wait(&timeRWSemaphore);
		start = lastSendTimestamp;
		sem_post(&timeRWSemaphore);

		//send only, if last 25ms was nothing sent
		clock_gettime(CLOCK_REALTIME,&temp);
		temp = timediff(lastSendTimestamp,temp);
		long t = (temp.tv_sec*1000)+(temp.tv_nsec/1000000);
		if(t>=25) {
			stream.str(std::string());
			stream << "AT*COMWDG=" << getSeq();
			sendATCmd(stream.str());
		}
	}
}

timespec ATClient::timediff(timespec start, timespec end) {
	timespec temp;
	if((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000L+end.tv_nsec-start.tv_nsec;
	}
	else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}

void ATClient::stopSending() {
	active = false;
}

bool ATClient::isActive() {
	return active;
}
