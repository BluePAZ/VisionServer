/*
 * SURFWorkManager.h
 *
 *  Created on: Nov 28, 2015
 *      Author: peterazientara
 */

#ifndef HOGWORKMANGER_H__
#define HOGWORKMANGER_H__

#include <string.h>

#include <WorkManager/WorkManager.hpp>

class HOGWorkManager: public WorkManager {
public:
	HOGWorkManager();
	virtual ~HOGWorkManager();
	std::string getName();
};
#endif /* HOGWORKMANGER_H__ */
