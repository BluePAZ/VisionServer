/*
 * SURFWorkManager.h
 *
 *  Created on: Nov 28, 2015
 *      Author: peterazientara
 */

#ifndef SVM_WORKMANAGER_H__
#define SVM_WORKMANAGER_H__

#include <string.h>

#include <WorkManager/WorkManager.hpp>

class SVMWorkManager: public WorkManager {
public:
	SVMWorkManager();
	virtual ~SVMWorkManager();
	std::string getName();
};

#endif /* SURFWORKMANAGER_SURFWORKMANAGER_H_ */
