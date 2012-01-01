/*
 * Logger.h
 *
 *  Created on: 24.12.2011
 *      Author: max
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <sstream>

using namespace std;

class Logger {
public:
	Logger();
	Logger(const string& prefix);
	virtual ~Logger();
	void setPrefix(const string& prefix);

	void log(stringstream& stream);
	void log(const string& str);
	void logErr(stringstream& stream);
	void logErr(const string& str);
protected:
	string prefix;
};

#endif /* LOGGER_H_ */
