/*
 * Thread.h
 *
 *  Created on: 23.12.2011
 *      Author: max
 */

#ifndef THREAD_H_
#define THREAD_H_

#include <pthread.h>

class Thread {
public:
	Thread();
	int start(void* arg);
	void join();
	virtual ~Thread();
protected:
	void run(void* arg);
	static void* EntryPoint(void*);
	virtual void setup();
	virtual void execute();
	void* getArg() const {return Arg_;}
	void setArg(void* a){Arg_ = a;}
private:
	pthread_t threadId;
	//THREADID ThreadId_;
	void* Arg_;
};

#endif /* THREAD_H_ */
