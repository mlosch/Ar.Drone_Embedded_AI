/*
 * MotionController.cpp
 *
 *  Created on: 24.12.2011
 *      Author: max
 */

#include "MotionController.h"
#include <sys/time.h>

MotionController::MotionController(ATClient* atClient, NavDataReceiver* navReceiver)
	:Thread(){
	this->atClient = atClient;
	this->navReceiver = navReceiver;
	tau = 0.02f;
	logger.setPrefix("MotionController");
	pthread_mutex_init(&mutex,0);
	sem_init(&stateWriteSemaphore,0,1);
	sem_init(&controlWriteSemaphore,0,1);
	sem_init(&queueAccessSemaphore,0,1);
	setControlValues(&lastControl,0.0f,0.0f,0.0f,0.0f,0.0f);
	navReceiver->addObserver(this);
}

MotionController::~MotionController() {
	logger.log("Destruction called. Waiting for thread to terminate...");
	navReceiver->removeObserver(this);
	active = false;
	pthread_mutex_unlock(&mutex);
	join();
	pthread_mutex_destroy(&mutex);
	sem_destroy(&stateWriteSemaphore);
	sem_destroy(&controlWriteSemaphore);
	sem_destroy(&queueAccessSemaphore);
}

navdata MotionController::getCurrentState() {
	sem_wait(&stateWriteSemaphore);
	navdata result = lastState;
	sem_post(&stateWriteSemaphore);
	return result;
}

ardrone_control MotionController::getCurrentControl() {
	sem_wait(&controlWriteSemaphore);
	ardrone_control result = lastControl;
	sem_post(&controlWriteSemaphore);
	return result;
}

void MotionController::setTimeInterval(float tau) {
	this->tau = tau;
}

float MotionController::getTimeInterval() {
	return tau;
}

void MotionController::newNavData(navdata* navData) {
	/*cout << "----------------------------------------" << endl;
	cout << "Altitude: " << navData->demo.altitude << endl;
	cout << "Roll: " << navData->demo.roll << endl;
	cout << "Yaw: " << navData->demo.yaw << endl;
	cout << "Pitch: " << navData->demo.pitch << endl;
	cout << "Vel-X: " << navData->demo.vx << endl;
	cout << "Vel-Y: " << navData->demo.vy << endl;
	cout << "Vel-Z: " << navData->demo.vz << endl;
	cout << "----------------------------------------" << endl;*/

	sem_wait(&stateWriteSemaphore);
	lastState = *navData;
	sem_post(&stateWriteSemaphore);
}

void MotionController::enqueueMotion(Trajectory& trajectory) {
	sem_wait(&queueAccessSemaphore);
	motions.push(trajectory);
	sem_post(&queueAccessSemaphore);
	pthread_mutex_unlock(&mutex);
}

void MotionController::setup() {
	active = true;
	if(motions.size()==0)
		pthread_mutex_lock(&mutex);
}

void MotionController::execute() {
	struct timespec sleepTime, remainingTime;
	sleepTime.tv_sec = 0;
	sleepTime.tv_nsec = (unsigned long)(tau*1E9);
	timeval workTimeStart, workTimeEnd;
	stringstream stream;

	while(active) {
		logger.log("Waiting...");
		pthread_mutex_lock(&mutex);
		logger.log("Running...");

		sem_wait(&queueAccessSemaphore);
		if(motions.size()>0) {
			cout << "Battery: " << lastState.demo.battery << endl;
			Trajectory trajectory = motions.front();
			motions.pop();
			sem_post(&queueAccessSemaphore);

			logger.log("################################################\nRunning trajectory\n################################################");

			while(trajectory.hasNext()) {
				float t = 0.0f;
				ardrone_control* targetControl = trajectory.getNext();
				ardrone_control temp, lastTemp=lastControl;
				ardrone_control q0 = lastControl;
				logger.log("running trajectory...");
				stream.str(std::string());
				do {
					gettimeofday(&workTimeStart,NULL);
					sleepTime.tv_nsec = (unsigned long)(tau*1E9);
					t = min(t+tau,targetControl->t);
					trajectory.interpolate(&lastControl,targetControl,&temp,t);
					if(compareControls(&temp,&lastTemp)>0) {
						lastTemp = temp;
						atClient->sendPCMD(1,temp.pitch,temp.roll,temp.throttle,temp.yaw);
					}
					else {
						atClient->sendPCMD(1,temp.pitch,temp.roll,temp.throttle,temp.yaw);
						//cout << "### Controls equal ###" << endl;
					}
					gettimeofday(&workTimeEnd,NULL);
					sleepTime.tv_nsec -= (workTimeEnd.tv_usec-workTimeStart.tv_usec)*1000;
					nanosleep(&sleepTime,&remainingTime);

				} while(t<targetControl->t);
				sem_wait(&controlWriteSemaphore);
				lastControl = temp;
				sem_post(&controlWriteSemaphore);
				logger.log("completed");

				/*stream << "Time: " << (((double)(clock()-test))/((double)CLOCKS_PER_SEC/1000.0)) << "s";
				logger.log(stream);*/
			}
		}
		else {
			sem_post(&queueAccessSemaphore);
		}
		sem_wait(&queueAccessSemaphore);
		if(motions.size()>0)
			pthread_mutex_unlock(&mutex);
		sem_post(&queueAccessSemaphore);
	}
}

void MotionController::setControlValues(ardrone_control* c, float pitch, float roll, float throttle, float yaw, float t) {
	c->pitch = pitch;
	c->roll = roll;
	c->throttle = throttle;
	c->yaw = yaw;
	c->t = t;
}

int MotionController::compareControls(ardrone_control* c1, ardrone_control* c2) {
	if( c1->throttle==c2->throttle &&
		c1->pitch==c2->pitch &&
		c1->roll==c2->roll &&
		c1->yaw==c2->yaw )
	{
		//controls are equal
		if(c1->t==c2->t) return 0;
		//controls are equal, but times are not
		else return -1;
	}
	else return 1;
}
