/*
 * SURFWorkManager.h
 *
 *  Created on: Nov 28, 2015
 *      Author: peterazientara
 */

#ifndef __OPENCVSURFDESCRIPTOR_WORKMANAGER_H__
#define __OPENCVSURFDESCRIPTOR_WORKMANAGER_H__
//#define _GLIBCXX_USE_CXX11_ABI 0

#include <string.h>

#include <WorkManager/WorkManager.hpp>

class OpenCVSurfDescriptorWorkManager: public WorkManager {
public:
	OpenCVSurfDescriptorWorkManager();
	virtual ~OpenCVSurfDescriptorWorkManager();
	std::string getName();
};

#endif /* SURFWORKMANAGER_SURFWORKMANAGER_H_ */
