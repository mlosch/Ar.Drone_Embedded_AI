/*
 * UDPClient.cpp
 *
 *  Created on: 22.12.2011
 *      Author: max
 */

#include "UDPClient.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <pthread.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
using namespace std;

UDPClient::UDPClient(string& ip, int port, bool timeout, long useconds) {
	sock = 0;
	this->ip = ip;
	this->port = port;
	stringstream prefix;
	prefix << "ATClient[" <<ip<< ":" <<port<< "]";
	logger = Logger(prefix.str());
	if(!connect(ip,port,timeout,useconds)) {
		logger.logErr("Unable to connect");
		throw 1;
	}
}

UDPClient::~UDPClient() {
	logger.log("Disconnecting...");
	if(sock!=0) close(sock);
	logger.log("OK");
}

bool UDPClient::connect(string& ip, int port, bool timeout, long useconds) {
	struct hostent *hp;

	logger.log("Connecting...");

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock<0) {
		if(DEBUG) logger.logErr("Socket creation");
		return false;
	}
	struct timeval time;
	if(timeout) {
		time.tv_usec = useconds;
		time.tv_sec = 0;
		setsockopt(sock,SOL_SOCKET, SO_RCVTIMEO, &time, sizeof(struct timeval));
	}

	server.sin_family = AF_INET;
	hp = gethostbyname(ip.c_str());
	if(hp==0) {
		close(sock);
		if(DEBUG) logger.logErr("Getting hostname");
		return false;
	}

	bcopy((char*)hp->h_addr, (char*)&server.sin_addr, hp->h_length);
	server.sin_port = htons(port);

	logger.log("Connection accomplished");

	return true;
}

int UDPClient::sendBytes(chArr* array) {
	int n = sendto(sock,array->bytes,array->length,0,(const sockaddr*)&server,sizeof(struct sockaddr_in));
	return n;
}

int UDPClient::readBytes(chArr* array) {
	int n = recv(sock,(void*)array->bytes,array->length,0);
	return n;
}

