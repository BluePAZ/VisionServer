/*
 * WorkTaskFactory.h
 *
 *  Created on: Nov 23, 2015
 *      Author: peterazientara
 */

#ifndef WORKTASKFACTORY_H_
#define WORKTASKFACTORY_H_
//#define _GLIBCXX_USE_CXX11_ABI 0

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unordered_map>

#include "../WorkTask/WorkTask.h"

class WorkTaskFactory {
public:
	WorkTaskFactory();
	~WorkTaskFactory();
	static WorkTaskFactory * get()
	{
		static WorkTaskFactory inst;
		return &inst;
	}
	void registerTask(std::string taskName, taskCreateFunction createFunction);
	WorkTask * CreateTask(std::string);
private:
	std::unordered_map<std::string, taskCreateFunction> taskMap;

};

#endif /* WORKTASKFACTORY_H_ */
