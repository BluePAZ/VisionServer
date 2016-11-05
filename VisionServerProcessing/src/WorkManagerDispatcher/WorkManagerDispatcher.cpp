/*
 * WorkManagerDispatcher.cpp
 *
 *  Created on: Nov 24, 2015
 *      Author: peterazientara
 */

#include "WorkManagerDispatcher.h"

WorkManagerDispatcher::WorkManagerDispatcher() {
	// TODO Auto-generated constructor stub
	managers = new std::unordered_map<std::string, WorkManager *>();
}
WorkManagerDispatcher::~WorkManagerDispatcher() {
	// TODO Auto-generated destructor stub
	// TODO call destuctor of every element in unordered map

	// TODO all destructor of unordered map
	delete managers;

}
WorkManager * WorkManagerDispatcher::getManager(std::string manager_name){
	if(managers->count(manager_name) > 0){
			WorkManager * tmp = (*managers)[manager_name];
			return tmp;
	}else{
		return NULL;
	}
}
void WorkManagerDispatcher::addManager(WorkManager * manager){
	if(managers->count(manager->getName()) > 0){
		//workManager already in do nothing
	}else{
		//add work manager
		(*managers)[manager->getName()] = manager;
	}
}
void WorkManagerDispatcher::removeManager(std::string manager_name){
	if(managers->count(manager_name) > 0){
		//workManager already in erase
		managers->erase(manager_name);
	}
}
void WorkManagerDispatcher::submitWork(std::string manager_name, WorkItem * workStuff){
	if(managers->count(manager_name) > 0){
		WorkManager * tmp = (*managers)[manager_name];
		workStuff->completed = 0;
		tmp->todoWork->setWork(workStuff);
	}
}

