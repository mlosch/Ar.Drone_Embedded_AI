/*
 * NavDataObserver.h
 *
 *  Created on: 26.12.2011
 *      Author: max
 */

#ifndef NAVDATAOBSERVER_H_
#define NAVDATAOBSERVER_H_

#include "NavdataTypes.h"

class NavDataObserver {
public:
	NavDataObserver();
	virtual ~NavDataObserver();
	virtual void newNavData(navdata* navData);
};

#endif /* NAVDATAOBSERVER_H_ */
