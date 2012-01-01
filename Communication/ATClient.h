/*
 * ATClient.h
 *
 *  Created on: 23.12.2011
 *      Author: max
 */

#ifndef ATCLIENT_H_
#define ATCLIENT_H_

#include "UDPClient.h"
#include "../Common/Thread.h"
#include <time.h>
#include <semaphore.h>

using namespace std;

class ATClient: public UDPClient, public Thread {
public:
	ATClient(string& ip, int port, bool timeout, long useconds);
	ATClient(string& ip, int port);
	virtual ~ATClient();

	void stopSending();
	bool isActive();

	bool sendATCmd(const string& cmd);
	bool sendPCMD(int enable, float pitch, float roll, float gaz, float yaw);

	//Defaults
	bool sendTakeoffCmd();
	bool sendLandingCmd();
	bool sendHoveringCmd();
	bool sendEmergencyCmd();
	bool sendFlatTrimCmd();
	bool sendUnpairCmd();

	static volatile unsigned int at_seq;
	unsigned int getSeq();

	timespec timediff(timespec start, timespec end);

protected:
	virtual void setup();
	virtual void execute();

	volatile bool active;

private:
	timespec lastSendTimestamp;
	sem_t timeRWSemaphore;
};

#endif /* ATCLIENT_H_ */
