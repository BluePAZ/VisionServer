/*
 * SURFIPWorkTask.h
 *
 *  Created on: Nov 28, 2015
 *      Author: peterazientara
 */

#ifndef __OPENCVSURFIPDESCRIPTORWORKTASK_H__
#define __OPENCVSURFIPDESCRIPTORWORKTASK_H__

#include <WorkTask/WorkTask.h>

class SURFIPWorkTask: public WorkTask {
public:
	SURFIPWorkTask();
	virtual ~SURFIPWorkTask();

	std::string getTaskName();
	static std::string returnTaskName();

	static WorkTask * Create() { return new SURFIPWorkTask(); };

	virtual void init();
	virtual void runTask(SocketData * data);
	virtual void deInit();
	virtual void parseParams(uint8_t * params);
	//virtual void packetReceived(SocketData * data);
	void runTaskThreadWork(struct WorkTaskThreadArgs *);

};

#endif /* __OPENCVSURFIPDESCRIPTORWORKTASK_H__ */
