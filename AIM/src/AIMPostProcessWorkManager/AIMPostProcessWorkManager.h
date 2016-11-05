/*
 * AIMPostProcessWorkManager.h
 *
 *  Created on: Apr 6, 2016
 *      Author: root
 */

#ifndef SRC_AIMPOSTPROCESSWORKMANAGER_AIMPOSTPROCESSWORKMANAGER_H_
#define SRC_AIMPOSTPROCESSWORKMANAGER_AIMPOSTPROCESSWORKMANAGER_H_

#include <WorkManager/WorkManager.hpp>

class AIMPostProcessWorkManager: public WorkManager {
public:
	AIMPostProcessWorkManager();
	virtual ~AIMPostProcessWorkManager();
	std::string getName();
};

#endif /* SRC_AIMPOSTPROCESSWORKMANAGER_AIMPOSTPROCESSWORKMANAGER_H_ */
