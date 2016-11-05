/*
 * AIMWorkManager.h
 *
 *  Created on: Nov 24, 2015
 *      Author: peterazientara
 */

#ifndef __CAPITESTS_WORKMANAGER_H_
#define __CAPITESTS_WORKMANAGER_H_

#include "WorkManager/WorkManager.hpp"
#include <string>

class CAPITestsWorkManager: public WorkManager {
public:
	CAPITestsWorkManager();
	virtual ~CAPITestsWorkManager();
	std::string getName();
};

#endif /* __CAPITESTS_WORKMANAGER_H_ */
