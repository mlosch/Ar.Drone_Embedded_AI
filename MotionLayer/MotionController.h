/*
 * MotionController.h
 *
 *  Created on: 24.12.2011
 *      Author: max
 */

#ifndef MOTIONCONTROLLER_H_
#define MOTIONCONTROLLER_H_

#include "Trajectory.h"
#include "../Communication/NavDataReceiver.h"
#include "../Common/Thread.h"
#include "../Common/Logger.h"
#include <queue>
#include <semaphore.h>

class MotionController : public Thread, public NavDataObserver{
public:
	MotionController(ATClient* atClient, NavDataReceiver* navReceiver);
	virtual ~MotionController();

	void setTimeInterval(float tau);
	float getTimeInterval();
	navdata getCurrentState();
	ardrone_control getCurrentControl();
	void enqueueMotion(Trajectory& trajectory);

	static void setControlValues(ardrone_control* c, float pitch, float roll, float throttle, float yaw, float t);
	static int compareControls(ardrone_control* c1, ardrone_control* c2);
protected:
	virtual void setup();
	virtual void execute();
	virtual void newNavData(navdata* navData);

	Logger logger;
	volatile bool active;
	ATClient* atClient;
	queue<Trajectory> motions;
	navdata lastState;
	ardrone_control lastControl;
	float tau;
private:
	pthread_mutex_t mutex;
	sem_t stateWriteSemaphore;
	sem_t controlWriteSemaphore;
	NavDataReceiver* navReceiver;
};

#endif /* MOTIONCONTROLLER_H_ */
