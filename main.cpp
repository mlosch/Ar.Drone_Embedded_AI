/*
 * main.cpp
 *
 *  Created on: 22.12.2011
 *      Author: max
 */

#include <iostream>
#include "Communication/ATClient.h"
#include "Communication/NavDataReceiver.h"
#include "MotionLayer/MotionController.h"

using namespace std;

int main(int argc, char **argv) {
	cout << "Hello World" << endl;
	string ip = "127.0.0.1";
	int atport = 5556;
	int navport = 5554;

	ATClient atClient = ATClient(ip,atport);
	NavDataReceiver navReceiver = NavDataReceiver(ip,navport);
	MotionController motionController = MotionController(&atClient,&navReceiver);

	atClient.start(0);
	navReceiver.start(0);
	motionController.start(0);

	string input="";
	while(true) {
		getline(cin,input);
		if(input.compare("quit")==0) break;
		else if(input.compare("sos")==0) atClient.sendEmergencyCmd();
		else if(input.compare("landing")==0) atClient.sendLandingCmd();
		else if(input.compare("takeoff")==0) {
			atClient.sendTakeoffCmd();
			usleep(100*1000);
			atClient.sendHoveringCmd();
		}
		else if(input.compare("trim")==0) atClient.sendFlatTrimCmd();
		else if(input.compare("unpair")==0) atClient.sendUnpairCmd();
		else if(input.compare("test1")==0) {
			Trajectory traj = Trajectory();
			ardrone_control q1,q2,q3;
			ardrone_control q0 = motionController.getCurrentControl();
			/*MotionController::setControlValues(&q1,0.1f,q0.roll,0.0f,q0.yaw,3.0f);
			MotionController::setControlValues(&q2,-0.1f,q0.roll,0.0f,q0.yaw,3.0f);
			MotionController::setControlValues(&q3,0.0f,q0.roll,0.0f,q0.yaw,0.1f);*/

			MotionController::setControlValues(&q1,0.0f,q0.roll,0.0f,1.0f,1.0f);
			MotionController::setControlValues(&q2,0.0f,q0.roll,0.0f,-1.0f,1.0f);
			MotionController::setControlValues(&q3,0.0f,q0.roll,0.0f,0.0f,0.1f);

			traj.addControl(&q1);
			traj.addControl(&q2);
			traj.addControl(&q3);
			motionController.enqueueMotion(traj);
		}
		else if(input.compare("navstop")==0) navReceiver.stopReceiving();
		else if(input.compare("navstart")==0) {
			if(navReceiver.isActive()==false)
				navReceiver.start(0);
		}
		else {
			cout << "##########################" << endl;
			cout << "Unknown command" << endl;
			cout << "Commands: " << endl;
			cout << "##########################" << endl;
		}
	}

	//navReceiver.join();
}


