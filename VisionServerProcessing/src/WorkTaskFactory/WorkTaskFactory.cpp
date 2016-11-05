/*
 * WorkTaskFactory.cpp
 *
 *  Created on: Nov 23, 2015
 *      Author: peterazientara
 */

#include "WorkTaskFactory.h"

WorkTaskFactory::WorkTaskFactory() {
	// TODO Auto-generated constructor stub
}
WorkTaskFactory::~WorkTaskFactory() {
	// TODO Auto-generated destructor stub
	taskMap.clear();
}
void WorkTaskFactory::registerTask(std::string taskName, taskCreateFunction createFunction){
	taskMap[taskName] = createFunction;
}
WorkTask * WorkTaskFactory::CreateTask(std::string taskName){
	std::unordered_map<std::string, taskCreateFunction>::iterator it = taskMap.find(taskName);
	if( it != taskMap.end() ){
	    return it->second();
	}
	return NULL;
}
