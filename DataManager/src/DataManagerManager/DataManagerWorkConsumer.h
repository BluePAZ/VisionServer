/*
 * AIMWorkManager.h
 *
 *  Created on: Nov 24, 2015
 *      Author: peterazientara
 */

#ifndef AIM_SRC_AIMWORKMANAGER_AIMWORKMANAGER_H_
#define AIM_SRC_AIMWORKMANAGER_AIMWORKMANAGER_H_

#include "WorkManager/WorkManager.hpp"
#include <string>

class DataManagerWorkManager: public WorkManager {
public:
	DataManagerWorkManager();
	virtual ~DataManagerWorkManager();
	std::string getName();
};

#endif /* AIM_SRC_DataManagerWorkManager_DataManagerWorkManager_H_ */
