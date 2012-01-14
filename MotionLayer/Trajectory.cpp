/*
 * Trajectory.cpp
 *
 *  Created on: 24.12.2011
 *      Author: max
 */

#include "Trajectory.h"
#include <iostream>

Trajectory::Trajectory() {
	controlIndex = 0;
	controls = vector<ardrone_control>();
}

Trajectory::~Trajectory() {
	//cout << "Trajectory : Destruction called" << endl;
}


void Trajectory::addControl(ardrone_control* control) {
	controls.push_back(*control);
}

bool Trajectory::hasNext(){
	return controlIndex<controls.size();
}

ardrone_control* Trajectory::getNext() {
	return &controls[controlIndex++];
}

/**
 * @param q0 Initial control (should not change in one trajectory)
 * @param q1 Target control (should not change in one trajectory)
 * @param target Result is stored here
 * @param tau Time. Result is a control at timestamp tau
 */
void Trajectory::interpolate(const ardrone_control* q0, const ardrone_control* q1, ardrone_control* target, float tau) {
	float factor = min(1.0f,tau/q1->t);
	target->pitch = q0->pitch+(q1->pitch-q0->pitch)*factor;
	target->roll = q0->roll+(q1->roll-q0->roll)*factor;
	target->throttle = q0->throttle+(q1->throttle-q0->throttle)*factor;
	target->yaw = q0->yaw+(q1->yaw-q0->yaw)*factor;
	//*target = *q1;
}

