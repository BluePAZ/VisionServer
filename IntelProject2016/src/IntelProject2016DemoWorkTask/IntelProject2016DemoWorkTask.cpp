/*
 * IntelProject2016DemoWorkTask.cpp
 *
 *  Created on: Nov 23, 2015
 *      Author: peterazientara
 */
#include "IntelProject2016DemoWorkTask.h"

IntelProject2016DemoWorkTask::IntelProject2016DemoWorkTask(){
	// TODO Auto-generated constructor stub
	h264_decoder = new CAPIFrameDecoder();

	fprintf(stderr, "IntelProject2016DemoWorkTask created\n");
}
IntelProject2016DemoWorkTask::~IntelProject2016DemoWorkTask(){
	// TODO Auto-generated constructor stub
	fprintf(stderr, "IntelProject2016DemoWorkTask destroyed\n");
}
void IntelProject2016DemoWorkTask::init(){
	fprintf(stderr, "IntelProject2016DemoWorkTask called init()\n");
	h264_decoder->init_codec(1280,720);
	//TODO:: implement any startup logic
}
void IntelProject2016DemoWorkTask::runTask(SocketData * data_in){
	fprintf(stderr, "IntelProject2016DemoWorkTask called runTask()\n");

	switch(data_in->message_type){
		case MODEL_CHANGE:{
			OpenCVSurfSupport::clear_loaded_models();
			std::vector<std::string> model_names = parseConfig(data_in->data, data_in->message_length);
			OpenCVSurfSupport::load_models_from_files( NULL, &model_names, false);
			break;
		}
		case VIDEO_FRAME:{
			//process and match
			WorkItem * i = new WorkItem();
			i->setData(data_in->data, data_in->message_length);
			struct WorkTaskThreadArgs * thread_args = (struct WorkTaskThreadArgs *)malloc(sizeof(struct WorkTaskThreadArgs));
			thread_args->caller = this;
			thread_args->item = i;
			pthread_t p;
			int err = pthread_create(&p, NULL, RunTaskThreadEntry, (void *)thread_args);
			if(err != 0){
				fprintf(stderr, "IntelProject2016DemoWorkTask, error creating thread\n");
				fprintf(stderr,"%s\n", strerror(err));
			}else{
				pthread_detach(p);
			}
			break;
		}
		default:{
			break;
		}
	}

	//convert socket data into work item;
	//WorkItem * i = new WorkItem();
	//uint8_t * decoded_frame = h264_decoder->decode_frame((uint32_t)data_in->message_length, (uint8_t *)data_in->data);
	//if(decoded_frame != NULL){
		//fprintf(stderr, "A frame has been decoded");
		//cv::Mat originalFrame = cv::Mat(720, 1280, CV_8UC4, (void *)decoded_frame);
		/*
		cv::Mat greyFrame;
			cv::cvtColor(originalFrame, greyFrame, CV_BGR2GRAY);
			//cv::resize(originalFrame, originalFrame, cv::Size(1024,768),0,0, CV_INTER_CUBIC);
			data_length = greyFrame.total() * greyFrame.elemSize();
			//uint8_t * data = (uint8_t *)malloc(data_length);
			uint32_t params[3] = { greyFrame.cols, greyFrame.rows, greyFrame.channels() };
			i->setParameters(params, 3);
			i->data = (uint8_t *)malloc(data_length);
			fprintf(stderr, "Data length %i\n", data_length);
			memcpy((void *)i->data, greyFrame.data, data_length);
			i->data_length = data_length;
			i->result = NULL;
			i->result_length = 0;
			fprintf(stderr, "SUBMIT TO IP\n");
			dispatcher->submitWork("opencv_surf_ip", i);
			while(i->completed == 0){
				//spin and wait for work to be done
			}
			uint32_t params1[3] = { greyFrame.cols, greyFrame.rows, greyFrame.channels() };
			i->setParameters(params1, 3);
			fprintf(stderr, "SUBMIT TO DESC\n");
			dispatcher->submitWork("opencv_surf_descriptor", i);
			while(i->completed == 0){
				//spin and wait for work to be done
			}
			//process to send to matcher
			fprintf(stderr, "SUBMIT TO MATCHER\n");
			dispatcher->submitWork("opencv_surf_bffmatcher", i);
			while(i->completed == 0){
				//spin and wait for work to be done
			}
			fprintf(stderr, "DRAWING\n");
			HomographyMatches ** match_results = (HomographyMatches **)i->result;
			int num_res = i->result_length;
			for(int nm = 0; nm < num_res; ++nm){
				if(match_results[nm] != NULL){
					fprintf(stderr, "NOT NULL\n");
					//Not null means a high enough score;
					//-- Get the corners from the image_1 ( the object to be "detected" )
					  CAPISize s = match_results[nm]->model_size;
						fprintf(stderr, "A\n");

					  std::vector<cv::Point2f> obj_corners(4);
					  obj_corners[0] = cvPoint(0,0);
					  obj_corners[1] = cvPoint( s.width, 0 );
					  obj_corners[2] = cvPoint( s.width,s.height);
					  obj_corners[3] = cvPoint( 0, s.height);
					  std::vector<cv::Point2f> scene_corners(4);
					  cv::Mat * H = (cv::Mat *)(match_results[nm]->homographyMatrix);
					cv::perspectiveTransform( obj_corners, scene_corners, (cv::Mat)(*H));
					  //-- Draw lines between the corners (the mapped object in the scene - image_2 )
					 // cv::line( originalFrame, scene_corners[0] + cv::Point2f( 1024.0f, 0), scene_corners[1] + cv::Point2f( 1024.0f, 0), cv::Scalar(0, 255, 0), 4 );
					 // cv::line( originalFrame, scene_corners[1] + cv::Point2f( 1024.0f, 0), scene_corners[2] + cv::Point2f( 1024.0f, 0), cv::Scalar( 0, 255, 0), 4 );
					 // cv::line( originalFrame, scene_corners[2] + cv::Point2f( 1024.0f, 0), scene_corners[3] + cv::Point2f( 1024.0f, 0), cv::Scalar( 0, 255, 0), 4 );
					 // cv::line( originalFrame, scene_corners[3] + cv::Point2f( 1024.0f, 0), scene_corners[0] + cv::Point2f( 1024.0f, 0), cv::Scalar( 0, 255, 0), 4 );
					  cv::line( originalFrame, scene_corners[0] , scene_corners[1] , cv::Scalar(0, 255, 0), 4 );
					  cv::line( originalFrame, scene_corners[1] , scene_corners[2] , cv::Scalar( 0, 255, 0), 4 );
					  cv::line( originalFrame, scene_corners[2] , scene_corners[3] , cv::Scalar( 0, 255, 0), 4 );
					  cv::line( originalFrame, scene_corners[3] , scene_corners[0] , cv::Scalar( 0, 255, 0), 4 );
				}else{
				}
			}
			cv::imshow("res_window", originalFrame);
			cv::waitKey(1);
			*/
}
//actual work flow to run;
void IntelProject2016DemoWorkTask::runTaskThreadWork(struct WorkTaskThreadArgs * arg){
	fprintf(stderr, "IntelProject2016DemoWorkTask running\n");
	//TODO: step 1 decode frame
	WorkItem *i = arg->item;
	cv::Mat greyFrame;
	cv::Mat originalFrame = cv::Mat(720, 1280, CV_8UC4, (void *)i->data);
	cv::resize(originalFrame, originalFrame, cv::Size(640,480),0,0, CV_INTER_CUBIC);
	cv::cvtColor(originalFrame, greyFrame, CV_BGR2GRAY);

	int data_length = greyFrame.total() * greyFrame.elemSize();
	//uint8_t * data = (uint8_t *)malloc(data_length);
	uint32_t params[3] = { greyFrame.cols, greyFrame.rows, greyFrame.channels() };
	i->setParameters(params, 3);
	i->setData(greyFrame.data ,data_length);
	fprintf(stderr, "SUBMIT TO IP\n");
	dispatcher->submitWork("opencv_surf_ip", i);
	while(i->completed == 0){
		//spin and wait for work to be done
	}
	uint32_t params1[3] = { greyFrame.cols, greyFrame.rows, greyFrame.channels() };
	i->setParameters(params1, 3);
	dispatcher->submitWork("opencv_surf_descriptor", i);
	while(i->completed == 0){
		//spin and wait for work to be done
	}
	//process to send to matcher
	i->freeParameters();
	dispatcher->submitWork("opencv_surf_bffmatcher", i);
	while(i->completed == 0){
		//spin and wait for work to be done
	}
	uint32_t x = 0;
	uint32_t y = 0;
	uint32_t no_draw = 0;
	HomographyMatches ** match_results = (HomographyMatches **)i->result;
	int num_res = i->result_length;
	fprintf(stderr, "num_res %i\n", num_res);
	 ROIRegion  * r = (ROIRegion *)malloc(sizeof(ROIRegion));
	for(int nm = 0; nm < num_res; ++nm){
		if(match_results[nm] != NULL){
			fprintf(stderr, "NOT NULL\n");
			no_draw = 0;
			//Not null means a high enough score;
			//-- Get the corners from the image_1 ( the object to be "detected" )
			  CAPISize s = match_results[nm]->model_size;
			  std::vector<cv::Point2f> obj_corners(4);
			  obj_corners[0] = cvPoint(0,0);
			  obj_corners[1] = cvPoint( s.width, 0 );
			  obj_corners[2] = cvPoint( s.width,s.height);
			  obj_corners[3] = cvPoint( 0, s.height);

			  std::vector<cv::Point2f> scene_corners(4);
			  cv::Mat * H = (cv::Mat *)(match_results[nm]->homographyMatrix);
			  cv::perspectiveTransform( obj_corners, scene_corners, (cv::Mat)(*H));

			  x =  (uint32_t)scene_corners[0].x;
			  y = (uint32_t)scene_corners[0].y;

			  r[0].x1 = scene_corners[0].x;
			  r[0].y1 = scene_corners[0].y;
			  r[0].x2 = scene_corners[1].x;
			  r[0].y2 = scene_corners[1].y;
			  r[0].x3 = scene_corners[2].x;
			  r[0].y3 = scene_corners[2].y;
			  r[0].x4 = scene_corners[3].x;
			  r[0].y4 = scene_corners[3].y;


			  for(int ik= 0; ik<4; ++ik){
				  fprintf(stderr, "[%i], x: %f , y: %f\n", ik, scene_corners[ik].x, scene_corners[ik].y);
			  }
			  //-- Draw lines between the corners (the mapped object in the scene - image_2 )

		}else{
			no_draw = 1;
			fprintf(stderr, "NULL\n");
		}
	}
	if(arg->caller != NULL){
		int lden = 0;
		uint8_t * dat = (uint8_t *)OpenCVSurfSupport::roiregion_to_byte(r, 1, &lden);
		SocketData * returnData = new SocketData(WORK_ACK,x,y,no_draw,lden, dat);
		arg->caller->sendDataPacket(returnData);
		delete returnData;
	}else{
		fprintf(stderr, "THE CALLER HAS BEEN REMOVED");
	}
	if(i != NULL){
		delete i;
	}
	free(arg);
	pthread_exit(0);
}
void IntelProject2016DemoWorkTask::deInit(){
	//TODO: implement cleanup
	fprintf(stderr, "IntelProject2016DemoWorkTask called deInit()\n");
}
void IntelProject2016DemoWorkTask::parseParams(uint8_t * params){
	//TODO: implement any parameter parsing
	fprintf(stderr, "IntelProject2016DemoWorkTask called parseParams()\n");
}
/*
void CAPITestsWorkTask::packetReceived(SocketData * data){
	fprintf(stderr, "CAPITestsWorkTask called packetReceived()\n");
	runTask(data);
}
*/
std::string IntelProject2016DemoWorkTask::returnTaskName(){
	return "intel_demo_task";
}
std::string IntelProject2016DemoWorkTask::getTaskName(){
	return "intel_demo_task";
}
std::vector<std::string> IntelProject2016DemoWorkTask::parseConfig(uint8_t * data, uint32_t length){
	std::string * rawString = new std::string((char *)data, length);
	std::vector<std::string> config = split(*rawString, ':');
	delete rawString;
	return config;
}
std::vector<std::string>& IntelProject2016DemoWorkTask::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}
std::vector<std::string> IntelProject2016DemoWorkTask::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}


