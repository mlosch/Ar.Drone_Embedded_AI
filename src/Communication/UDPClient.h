/*
 * UDPClient.h
 *
 *  Created on: 22.12.2011
 *      Author: max
 */

#ifndef UDPCLIENT_H_
#define UDPCLIENT_H_

#include "../Common/Types.h"
#include "../Common/Debug.h"
#include "../Common/Logger.h"
#include <netinet/in.h>
#include <iostream>
using namespace std;


class UDPClient
{
public:
	UDPClient(string& ip, int port, bool timeout, long useconds);
	virtual ~UDPClient();

	int sendBytes(chArr* array);
	int readBytes(chArr* array);

protected:
	Logger logger;

private:
	bool connect(string& ip, int port, bool timeout, long useconds);

	string ip;
	int port;
	int sock;
	struct sockaddr_in server;
};

#endif /* UDPCLIENT_H_ */
