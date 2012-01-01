/*
 * Logger.cpp
 *
 *  Created on: 24.12.2011
 *      Author: max
 */

#include "Logger.h"
#include <iostream>
#include "Debug.h"

using namespace std;

Logger::Logger() {
}

Logger::Logger(const string& prefix) {
	this->prefix = prefix;
}

Logger::~Logger() {
}

void Logger::setPrefix(const string& prefix) {
	this->prefix = prefix;
}

void Logger::log(stringstream& stream) {
	if(DEBUG) {
		cout << prefix << ": " << stream.str() << endl;
	}
}

void Logger::log(const string& str) {
	if(DEBUG) {
		cout << prefix << ": " << str << endl;
	}
}

void Logger::logErr(stringstream& stream) {
	if(DEBUG) {
		cerr << prefix << ": " << stream.str() << endl;
	}
}

void Logger::logErr(const string& str) {
	if(DEBUG) {
		cerr << prefix << ": " << str << endl;
	}
}

