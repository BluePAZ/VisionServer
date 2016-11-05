/*
 * SURFWorkManager.h
 *
 *  Created on: Nov 28, 2015
 *      Author: peterazientara
 */

#ifndef SURFWORKMANAGER_SURFWORKMANAGER_H_
#define SURFWORKMANAGER_SURFWORKMANAGER_H_

#include <string.h>

#include <WorkManager/WorkManager.hpp>

class SURFIPWorkManager: public WorkManager {
public:
	SURFIPWorkManager();
	virtual ~SURFIPWorkManager();
	std::string getName();
};

#endif /* SURFWORKMANAGER_SURFWORKMANAGER_H_ */
