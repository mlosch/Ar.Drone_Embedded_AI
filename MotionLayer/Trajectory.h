/*
 * Trajectory.h
 *
 *  Created on: 24.12.2011
 *      Author: max
 */

#ifndef TRAJECTORY_H_
#define TRAJECTORY_H_

#include <vector>
using namespace std;

typedef struct ardrone_control_t {
	float pitch;
	float roll;
	float throttle;
	float yaw;
	float t;
} ardrone_control;

class Trajectory {
public:
	Trajectory();
	virtual ~Trajectory();

	void addControl(ardrone_control* control);
	bool hasNext();
	ardrone_control* getNext();

	virtual void interpolate(ardrone_control* q0, ardrone_control* q1, ardrone_control* target, float tau);
protected:
	vector<ardrone_control> controls;
	int controlIndex;
};

#endif /* TRAJECTORY_H_ */
