/*
 * NavDataReceiver.h
 *
 *  Created on: 23.12.2011
 *      Author: max
 */

#ifndef NAVDATARECEIVER_H_
#define NAVDATARECEIVER_H_

#include "ATClient.h"
#include "../Common/Thread.h"
#include "NavdataTypes.h"
#include "NavDataObserver.h"
#include <vector>

using namespace std;

class NavDataReceiver: public ATClient {
public:
	NavDataReceiver(string& ip, int port);
	virtual ~NavDataReceiver();

	void stopReceiving();
	//void addObserver(void(*updateMethod)(navdata*));
	void addObserver(NavDataObserver* observer);
	void removeObserver(NavDataObserver* observer);

	unsigned int getDroneState();

	static int getIntFromIncome(chArr* array, int* offset);
	static short getShortFromIncome(chArr* array, int* offset);
	static float getFloatFromIncome(chArr* array, int* offset);
protected:
	virtual void setup();
	virtual void execute();
	virtual bool parseIncomingData(chArr* array);

private:
	void notifyObserver(navdata* obj);

	//vector<void(*)(navdata*)> observer;
	vector<NavDataObserver*> observer;
	navdata_header navdataHeader;
	navdata navData;
};

#endif /* NAVDATARECEIVER_H_ */
