/*
 * SURFWorkManager.h
 *
 *  Created on: Nov 28, 2015
 *      Author: peterazientara
 */

#ifndef __OPENCVSURFBFFMATCHER_WORKMANAGER_H__
#define __OPENCVSURFBFFMATCHER_WORKMANAGER_H__
//#define _GLIBCXX_USE_CXX11_ABI 0

#include <string.h>

#include <WorkManager/WorkManager.hpp>

class OpenCVSurfBFFMatcherWorkManager: public WorkManager {
public:
	OpenCVSurfBFFMatcherWorkManager();
	virtual ~OpenCVSurfBFFMatcherWorkManager();
	std::string getName();
};

#endif /* SURFWORKMANAGER_SURFWORKMANAGER_H_ */
