#ifndef __CAPI_SERVER_H__
#define __CAPI_SERVER_H__

#define WITH_GPU_SURF 1
#define WITH_AIM 1
#define WITH_HOG 0
#define WITH_CMT 1
#define WITH_SURF 1




#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sstream>
#include <vector>

//#include "../../Common/open_surf/include/ipoint.h"
#include "capi_network_defs.h"
extern "C"{
	#include "capi_network_utility/capi_network_utility.h"
}
//#include "IntelProject2016DemoWorkTask/IntelProject2016DemoWorkTask.h"
#if(WITH_AIM)
#include "AIMWorkManager/AIMWorkManager.h"
#include "AIMWorkConsumer/AIMWorkConsumer.hpp"
#include "AIMWorkTask/AIMWorkTask.h"
#include "AIMPostProcessWorkConsumer/AIMPostProcessWorkConsumer.h"
#include "AIMPostProcessWorkManager/AIMPostProcessWorkManager.h"
#endif

/*
 * FULL TASK INCLUDES
 */
#if(WITH_HOG)
#include "HOGWorkConsumer/HOGWorkConsumer.h"
#include "HOGWorkManager/HOGWorkManager.h"
#include "SVMWorkConsumer/SVMWorkConsumer.h"
#include "SVMWorkManager/SVMWorkManager.h"
#include "PersonDetectorPipeline/PersonDetectorPipeline.h"
#endif

#if(WITH_CMT)
#include "CMT/CMTWorkTask/CMTWorkTask.h"
#endif

#if(WITH_SURF)
#include "OpenCVSurfBFFMatcher/OpenCVSurfBFFMatcherWorkConsumer/OpenCVSurfBFFMatcherWorkConsumer.h"
#include "OpenCVSurfBFFMatcher/OpenCVSurfBFFMatcherWorkManager/OpenCVSurfBFFMatcherWorkManager.h"
#include "OpenCVSurfIpDescriptor/OpenCVSurfIpDescriptorWorkConsumer/OpenCVSurfIpDescriptorWorkConsumer.h"
#include "OpenCVSurfIpDescriptor/OpenCVSurfIpDescriptorWorkManager/OpenCVSurfIpDescriptorWorkManager.h"
#include "OpenCVSurfDescriptor/OpenCVSurfDescriptorWorkConsumer/OpenCVSurfDescriptorWorkConsumer.h"
#include "OpenCVSurfDescriptor/OpenCVSurfDescriptorWorkManager/OpenCVSurfDescriptorWorkManager.h"
#include "OpenCVSurfIp/OpenCVSurfIpWorkConsumer/OpenCVSurfIpWorkConsumer.h"
#include "OpenCVSurfIp/OpenCVSurfIpWorkManager/OpenCVSurfIpWorkManager.h"
#include "OpenCVSurfIp/OpenCVSurfIpWorkTask/OpenCVSurfIpWorkTask.h"
#include "OpenCVSurfFull/OpenCVSurfFullWorkTask/OpenCVSurfFullWorkTask.h"
#include "OpenCVSurfFullDemo/OpenCVSurfFullDemoWorkTask/OpenCVSurfFullDemoWorkTask.h"

#include "OpenCVSurfSupport/OpenCVSurfSupport.h"
#include <OpenCVSurfFullWorkItem/OpenCVSurfFullWorkItem.hpp>
#endif

#if(WITH_GPU_SURF)
#include "OpenCVGpuSurfIpDescriptor/OpenCVGpuSurfIpDescriptorWorkConsumer/OpenCVGpuSurfIpDescriptorWorkConsumer.h"
#include "OpenCVGpuSurfIpDescriptor/OpenCVGpuSurfIpDescriptorWorkManager/OpenCVGpuSurfIpDescriptorWorkManager.h"
#include "OpenCVGpuSurfDescriptor/OpenCVGpuSurfDescriptorWorkManager/OpenCVGpuSurfDescriptorWorkManager.h"
#include "OpenCVGpuSurfDescriptor/OpenCVGpuSurfDescriptorWorkConsumer/OpenCVGpuSurfDescriptorWorkConsumer.h"
#include "OpenCVGpuBFFMatcher/OpenCVGpuBFFMatcherWorkManager/OpenCVGpuBFFMatcherWorkManager.h"
#include "OpenCVGpuBFFMatcher/OpenCVGpuBFFMatcherWorkConsumer/OpenCVGpuBFFMatcherWorkConsumer.h"
#include "AIMPostProcessGPUWorkConsumer/AIMPostProcessGPUWorkConsumer.h"
#endif
//////////////////////////////////////////////////////////////////////////////////////////
//Necessary Includes//
//////////////////////////////////////////////////////////////////////////////////////////
#include "WorkItem/WorkItem.hpp"
#include "WorkTask/WorkTask.h"
#include "WorkManagerDispatcher/WorkManagerDispatcher.h"
#include "WorkTaskFactory/WorkTaskFactory.h"
#include "WorkTypeDefs.hpp"
#include "DataSharer/DataSharer.hpp"
#include "VisionServerNetworkUtility/VisionServerNetworkUtility.hpp"
#include "VisionServerNetworkUtilityDefs.hpp"
#include "SocketData/SocketData.hpp"

uint32_t client_count;
uint32_t client_id;
pthread_t server_connect_thread;
pthread_t threads[MAX_SOCKET_CONNECTIONS];

WorkTaskFactory * taskFactory;
WorkManagerDispatcher * dispatcher;
DataSharer shared_data;

using namespace std;

typedef struct{
	uint32_t stream_id;
	uint32_t frame_id;
	uint8_t * data; //will be the cv:mat of the heatmap
}thread_data;
typedef struct{
	uint32_t stream_id;
	uint32_t socket_fd;
}server_start_thread_data_struct;
typedef struct{
	uint32_t stream_id;
	uint32_t socket_fd;
}image_server_thread_data_struct;
void init();
//called once to set up client connections
void * server_accept_thread(void * server_start_data_args);
//main worker thread
void * image_server_thread(void * image_server_thread_data_args);

std::vector<std::string> parseConfig(uint8_t * data, uint32_t length);
std::vector<std::string> split(const std::string &s, char delim);
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
#endif
