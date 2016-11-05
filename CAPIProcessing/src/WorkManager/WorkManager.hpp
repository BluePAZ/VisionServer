#ifndef __WORK_MANAGER_H__
#define __WORK_MANAGER_H__
//#define _GLIBCXX_USE_CXX11_ABI 0

#include <stdint.h>
#include <vector>
#include <unordered_map>
#include <utility>      // std::pair
#include <string>


#include "../WorkTypeDefs.hpp"
#include "../ConcurrentQueue/ConcurrentQueue.hpp"
#include "../WorkConsumer/WorkConsumer.hpp"

class WorkManager{
	public:
		WorkManager();
		virtual ~WorkManager();
		
		static void workItemStatusChanged(uint32_t id, uint8_t new_status);
		void enqueueWorker(WorkConsumer * worker);
		void dequeueWorker(uint32_t id);
		
		virtual std::string getName()=0;

		ConcurrentQueue * todoWork;
		
	private:
		uint32_t numWorkers;
		uint32_t highestId;
		std::unordered_map<uint32_t, WorkConsumer *> workers;
		std::vector<uint32_t> availableIds;
		uint32_t getNewWorkId();
		void returnWorkId(uint32_t id);
};
#endif
