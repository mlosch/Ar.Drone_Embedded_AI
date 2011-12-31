/*
 * Thread.cpp
 *
 *  Created on: 23.12.2011
 *      Author: max
 */

#include "Thread.h"
//#include <stdlib.h>

Thread::Thread() {
}

Thread::~Thread() {
}

int Thread::start(void* arg) {
	setArg(arg);
	int code = pthread_create(&threadId,0,Thread::EntryPoint,this);
	return code;
}

void Thread::join() {
	if(threadId>0)
		pthread_join(threadId,0);
}

void Thread::run(void* arg) {
	setup();
	execute();
}

void* Thread::EntryPoint(void* pthis) {
	Thread* pt = (Thread*)pthis;
	pt->run(pt->getArg());
	return (void*)0;
}

void Thread::setup() {
}

void Thread::execute() {
}
