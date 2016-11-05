/*
 * SURFIPWorkTask.cpp
 *
 *  Created on: Nov 28, 2015
 *      Author: peterazientara
 */

#include "../OpenCVSurfFullDemoWorkTask/OpenCVSurfFullDemoWorkTask.h"

OpenCVSurfFullDemoWorkTask::OpenCVSurfFullDemoWorkTask() {
	// TODO Auto-generated constructor stub
}
OpenCVSurfFullDemoWorkTask::~OpenCVSurfFullDemoWorkTask() {
	// TODO Auto-generated destructor stub
}

void OpenCVSurfFullDemoWorkTask::init(){
	fprintf(stderr, "OpenCVSurfFullDemoWorkTask called init()\n");
	//TODO:: implement any startup logic
}
void OpenCVSurfFullDemoWorkTask::deInit(){
    	fprintf(stderr, "OpenCVSurfFullDemoWorkTask called deInit()\n");
	//TODO: implement cleanup
}
void OpenCVSurfFullDemoWorkTask::parseParams(uint8_t * params){
	//TODO: implement any parameter parsing
}

std::string OpenCVSurfFullDemoWorkTask::returnTaskName(){
	return "opencv_surf_full_demo";
}
std::string OpenCVSurfFullDemoWorkTask::getTaskName(){
	return "opencv_surf_full_demo";
}

void OpenCVSurfFullDemoWorkTask::setParams(WorkItem * i, uint32_t width, uint32_t height, uint32_t depth){
	i->params = (uint8_t *)malloc(12);
	i->params[3] = width >> 24;
	i->params[2] = width >> 16;
	i->params[1] = width >> 8;
	i->params[0] = width;
	//set height

	i->params[7] = height >> 24;
	i->params[6] = height >> 16;
	i->params[5] = height >> 8;
	i->params[4] = height;

	i->params[11] = depth >> 24;
	i->params[10] = depth >> 16;
	i->params[9] = depth >> 8;
	i->params[8] = depth;

	i->params_length = 12;
}
void OpenCVSurfFullDemoWorkTask::runTask(SocketData * data_in){
	fprintf(stderr, "OpenCVSurfFullDemoWorkTask called runTask()\n");
	//convert socket data into work item;
	OpenCVSurfFullWorkItem * i = new OpenCVSurfFullWorkItem();
	uint32_t width = data_in->stream_id;
	uint32_t height = data_in->message_id;
	uint32_t depth = 4;
	uint32_t params[3] = {width, height, depth};
    ((WorkItem *)i)->uuid = data_in->frame_id;
	((WorkItem *)i)->setParameters(params, (uint32_t)3);
	i->sde = new SocketDataExtended(data_in->data, data_in->message_length);
	//i->setData(data_in->data, data_in->message_length);
	struct WorkTaskThreadArgs * thread_args = (struct WorkTaskThreadArgs *)malloc(sizeof(struct WorkTaskThreadArgs));
	thread_args->caller = this;
	thread_args->item = (WorkItem *)i;
	pthread_t p;
	int err = pthread_create(&p, NULL, RunTaskThreadEntry, (void *)thread_args);
	if(err != 0){
		fprintf(stderr, "OpenCVSurfFullDemoWorkTask, error creating thread\n");
		fprintf(stderr,"%s\n", strerror(err));
	}else{
		pthread_detach(p);
	}
}
//actual work flow to run;
void OpenCVSurfFullDemoWorkTask::runTaskThreadWork(struct WorkTaskThreadArgs * arg){
#warning change "cpu_only" to enable or diable gpu, 0=use cpu, 1=cpuonly
	int cpu_only = 0;
	fprintf(stderr, "OpenCVSurfFullDemoWorkTask running\n");
	OpenCVSurfFullWorkItem * i = (OpenCVSurfFullWorkItem *)arg->item;
	SocketData * returnData;
	if(arg->caller != NULL){
		//set the image from i->sde->data;
		uint32_t width = ((uint32_t *)((WorkItem *)i)->params)[0];
		uint32_t height = ((uint32_t *)((WorkItem *)i)->params)[1];
		uint32_t depth = ((WorkItem *)i)->params[2];
		//set image
		i->image = cv::Mat(height,width,CV_8UC1, i->sde->data);
		//Set mask for operations
		i->mask = cv::Mat::zeros(height, width, CV_8UC1);
		std::vector<cv::Point> fillContSingle;
		fillContSingle.push_back(cv::Point(i->sde->region.x0, i->sde->region.y0));
		fillContSingle.push_back(cv::Point(i->sde->region.x1, i->sde->region.y1));
		fillContSingle.push_back(cv::Point(i->sde->region.x2, i->sde->region.y2));
		fillContSingle.push_back(cv::Point(i->sde->region.x3, i->sde->region.y3));
		std::vector< std::vector<cv::Point> > fillContAll;
		fillContAll.push_back(fillContSingle);
		cv::fillPoly(i->mask, fillContAll, cv::Scalar(255));
		if(width < 256 || height < 256 || cpu_only == 0){
			i->is_cpu = true;
		}
		//submit opencv_surf_keypoint_and_ip
		arg->caller->dispatcher->submitWork("opencv_surf_ip_descriptor", (WorkItem *)i);
		while(((WorkItem *)i)->completed == 0){
			//spin and wait for work to be done
		}
		fprintf(stderr, "Num Keypoints: %i\n", i->keypoints.size());

		uint8_t * retData = NULL;
		int data_len = 0;
		//determine third stage
		switch(i->sde->mode){
			case 0:{  //match agains model
				break;
			}
			case 1:{ //match against model
				//process to send to matcher
				i->model_descriptors_matrix = ((OpenCVSurfFullWorkItem *)model_map[i->sde->model_id])->descriptors_matrix;
				i->model_keypoints = ((OpenCVSurfFullWorkItem *)model_map[i->sde->model_id])->keypoints;
				i->model_is_cpu = ((OpenCVSurfFullWorkItem *)model_map[i->sde->model_id])->is_cpu;
				//gpu fallback;
				if(i->model_is_cpu || i->is_cpu){
					//cpu matcher
					arg->caller->dispatcher->submitWork("opencv_surf_bffmatcher", (WorkItem *)i);
				}else{
					//gpu matcher
					arg->caller->dispatcher->submitWork("opencv_gpu_bffmatcher", (WorkItem *)i);
				}
				while(((WorkItem *)i)->completed == 0){
						//spin and wait for work to be done
				}
				std::vector<cv::Point2f> obj;
				std::vector<cv::Point2f> scene;
				for( int n = 0; n < ((std::vector<cv::DMatch>)i->matches[0]).size(); n++ )
				  {
				    //-- Get the keypoints from the good matches
				    obj.push_back( i->keypoints[ i->matches[0][n].queryIdx ].pt );
				    scene.push_back( i->model_keypoints[ i->matches[0][n].trainIdx ].pt );
				  }
				cv::Mat * homography = OpenCVSurfSupport::computeHomography(&scene, &obj);
				data_len = homography->cols * homography->rows * homography->depth();
				retData = (uint8_t *)malloc(data_len);
				memcpy(retData, homography->data, data_len);
				break;
			}
			case 2:{
				break;
			}
			case 3:{ //fill model hash
				model_map[i->sde->model_id] = i;
				break;
			}
		}
		//determine return packet
		switch(i->sde->mode){
			case 0:{  //match against model
				break;
			}
			case 1:{ //match against model
				returnData = new SocketData(HOMOGRAPHY, 0,0,((WorkItem *)i)->uuid, data_len,(uint8_t *) retData);
				break;
			}
			case 2:{
				break;
			}
			case 3:{
				returnData = new SocketData(WORK_ACK, 0, 0, i->sde->model_id, NULL);
				break;
			}
		}
		arg->caller->sendDataPacket(returnData);
		delete returnData;
	}else{
		fprintf(stderr, "OpenCVSurfFullDemoWorkTask the caller no longer exists!");
	}
	if(i != NULL){
		delete i;
	}
	free(arg);
	pthread_exit(0);
}


