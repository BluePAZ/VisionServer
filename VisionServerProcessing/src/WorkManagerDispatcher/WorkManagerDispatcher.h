/*
 * WorkManagerDispatcher.h
 *
 *  Created on: Nov 24, 2015
 *      Author: peterazientara
 */

#ifndef WORKMANAGERDISPATCHER_WORKMANAGERDISPATCHER_H_
#define WORKMANAGERDISPATCHER_WORKMANAGERDISPATCHER_H_
//#define _GLIBCXX_USE_CXX11_ABI 0

#include <unordered_map>
#include <string>
#include "../WorkManager/WorkManager.hpp"

class WorkManagerDispatcher {
public:
	WorkManagerDispatcher();
	~WorkManagerDispatcher();

	void addManager(WorkManager *);
	void removeManager(std::string manager_name);
	void submitWork(std::string, WorkItem *);
	WorkManager * getManager(std::string);

private:
	std::unordered_map<std::string, WorkManager *> * managers;
};

#endif /* WORKMANAGERDISPATCHER_WORKMANAGERDISPATCHER_H_ */
