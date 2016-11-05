/*
 * CAPISaliencyTask.cpp
 *
 *  Created on: Nov 23, 2015
 *      Author: peterazientara
 */
#include "AIMWorkTask.h"

AIMWorkTask::AIMWorkTask(){
	// TODO Auto-generated constructor stub
}
AIMWorkTask::~AIMWorkTask(){
	// TODO Auto-generated constructor stub
}
void AIMWorkTask::init(){
	fprintf(stderr, "AIMWorkTask called init()\n");
	//TODO:: implement any startup logic
}
void AIMWorkTask::runTask(SocketData * data_in){
	fprintf(stderr, "AIMWorkTask called runTask()\n");
	//convert socket data into work item;
	WorkItem * i = new WorkItem();
	i->uuid = data_in->frame_id;
	i->setData(data_in->data, data_in->message_length);
	struct WorkTaskThreadArgs * thread_args = (struct WorkTaskThreadArgs *)malloc(sizeof(struct WorkTaskThreadArgs));
	thread_args->caller = this;
	thread_args->item = i;
<<<<<<< HEAD
	i->uuid = data_in->frame_id;

=======
>>>>>>> 698da7b5d377ea1471e41e33c38057e3f909881b
	//runTaskThreadWork(thread_args);
	pthread_t p;
	int err = pthread_create(&p, NULL, RunTaskThreadEntry, (void *)thread_args);
	if(err != 0){
		fprintf(stderr, "AIMWorkTask, error creating thread\n");
	}
	pthread_detach(p);

}
void AIMWorkTask::runTaskThreadWork(struct WorkTaskThreadArgs * arg){
	//fprintf(stderr, "AIMWorkTask running\n");
	//auto ta = std::chrono::system_clock::now();

	WorkItem * aimData = arg->item;
	//OpenCVSurfWorkItem * surfItem = new OpenCVSurfWorkItem();
	//cv::Mat bgrImage = cv::Mat(600,800,CV_8UC4, (void *)aimData->data);
	cv::Mat inputImage = cv::Mat(600, 800, CV_8UC4, (void *)aimData->data);
<<<<<<< HEAD
	cv::Mat inputImageRGBColor;

	//cv::cvtColor(inputImage, inputImageRGBColor, CV_BGRA2RGBA);

	//cv::GaussianBlur(inputImageRGBColor, inputImageRGBColor, {5, 5}, 1.0);

	//aimData->setData(inputImage.data, aimData->data_length);
	uint32_t params[3] = {(uint32_t) 800, (uint32_t) 600, (uint32_t) 4 };
	//uint32_t params[3] = {(uint32_t) originalFrame.cols, (uint32_t) originalFrame.rows, (uint32_t) originalFrame.channels() };
=======
	//cv::Mat bgrImage = cv::Mat(inputImage);
	//cv::cvtColor(bgrImage, bgrImage, CV_BGRA2BGR);
	//surfItem->image = bgrImage;
	//((WorkItem *)surfItem)->uuid = aimData->uuid;
	//arg->caller->dispatcher->submitWork("opencv_surf_gpu_full",(WorkItem *)surfItem);
	uint32_t params[3] = {(uint32_t) 800, (uint32_t) 600, (uint32_t) 4};
>>>>>>> 698da7b5d377ea1471e41e33c38057e3f909881b
	aimData->setParameters(params, 3);
	arg->caller->dispatcher->submitWork("aim_saliency",aimData);
	while(aimData->completed == 0){

	}
	arg->caller->dispatcher->submitWork("aim_post_process",aimData);
	while(aimData->completed == 0){
	}

	//auto tb = std::chrono::system_clock::now();
	//uint32_t full_time = std::chrono::duration_cast<std::chrono::milliseconds>(tb - ta).count();
	//SEND AIM RESULTS
	if(arg->caller != NULL){
<<<<<<< HEAD
		SocketData * returnData = new SocketData(BOX_REGIONS,0,0,aimData->uuid,resLength,resultData);
=======
		SocketData * returnData = new SocketData(BOX_REGIONS,aimData->time_ms_2,aimData->time_ms,aimData->uuid,aimData->result_length, (uint8_t *)aimData->result);
>>>>>>> 698da7b5d377ea1471e41e33c38057e3f909881b
		//SocketData * returnData = new SocketData(BOX_REGIONS,0,0,0,0,NULL);
		arg->caller->sendDataPacket(returnData);
		delete returnData;
	}else{
		fprintf(stderr, "THE CALLER HAS BEEN REMOVED");
	}

	/*
	while(((WorkItem *)surfItem)->completed == 0){
	}
	//SEND SURF KEYPOINT RESULTS
	if(arg->caller != NULL){
        SURFKeyPointLite * keypoints = OpenCVSurfSupport::opencv_to_surfkeypointlite(surfItem->keypoints);
        int length = surfItem->keypoints.size();
        uint32_t * bytesArray = OpenCVSurfSupport::keypointslite_to_byte(keypoints, length, &length);
		SocketData * returnData = new SocketData(KEYPOINTS,0,((WorkItem *)surfItem)->time_ms,((WorkItem *)surfItem)->uuid, length, (uint8_t *)bytesArray);
		//SocketData * returnData = new SocketData(BOX_REGIONS,0,0,0,0,NULL);
		arg->caller->sendDataPacket(returnData);
		delete returnData;
	}else{
		fprintf(stderr, "THE CALLER HAS BEEN REMOVED");
	}
	*/
	//uint32_t resLength = aimData->result_length;
	//uint8_t * resultData = postProcess((uint8_t *)aimData->result, &resLength);
	delete aimData;
	//delete surfItem;
	free(arg);
	pthread_exit(0);
}
void AIMWorkTask::deInit(){
	//TODO: implement cleanup
	fprintf(stderr, "AIMWorkTask called deInit()\n");
}
void AIMWorkTask::parseParams(uint8_t * params){
	//TODO: implement any parameter parsing
}
<<<<<<< HEAD
uint8_t * AIMWorkTask::postProcess(uint8_t * rawResult, uint32_t * length){
	cv::Mat in_image = cv::Mat(600, 800, CV_32SC1,  (void*)rawResult);
	cv::Mat infoMap;
	//cv::imshow("in_image", in_image);
	//cv::waitKey(5);

	in_image.convertTo(infoMap, CV_32FC1, 1.0 / (float)pow(2.0, 17), 0);

	// *************************************************
		// Post-Processing of Information Map
		// *************************************************
		double minVal, maxVal;
		minMaxLoc(infoMap, &minVal, &maxVal, NULL, NULL);

		// Min values scaled to 0
		infoMap = infoMap - minVal;

		// Normalize
		infoMap.convertTo(infoMap, -1, 1 / (maxVal - minVal), 0);



		cv::Mat gKernel = cv::getGaussianKernel(11, 31, CV_32FC1);
		cv::sepFilter2D(infoMap, infoMap, -1, gKernel, gKernel);// separable convolution (speeds up execution)

		// *************************************************
		// Pixel Masking
		//**************************************************
		std::vector<cv::Mat> saliencyMap (1);
		for (int i = 0; i < 1; i++)
		{
			saliencyMap[i] = cv::Mat::zeros(600, 800, CV_32FC1);
		}

		cv::Mat infoMapLine = infoMap.clone();
		cv::Mat infoMapVector = infoMapLine.reshape(1,infoMap.rows * infoMap.cols);

		double dispRank = calcPercentile(infoMapVector, 0.83);
	    	//printf("dispRank = %3.4f\n", dispRank);

		cv::Mat threshMap;
		cv::compare(infoMap, dispRank, threshMap, cv::CMP_GT);

		threshMap.convertTo(threshMap, CV_32FC1, 1.0 / 255.0, 0);
		//cv::imshow("map", threshMap);
		//cv::waitKey(5);


	// Normalize
	//cv::normalize(infoMap, infoMap, 0, 65535, cv::NORM_MINMAX);
	//cv::Mat blob_grey = GetSalienceMask(threshMap, 0.7, 1, 2);

	//cv::Mat grey_image;
	cv::Mat blob_grey;
	std::vector<std::vector<cv::Point> > contours; // Vector for storing contour
	std::vector<cv::Vec4i> hierarchy;
	cv::threshold(threshMap, threshMap, 0, 1, cv::THRESH_BINARY);
	//cvtColor(blob_grey, blob_grey, COLOR_BGR2GRAY);
	threshMap.convertTo(blob_grey, CV_8UC1);
	cv::findContours(blob_grey,
					 contours,
					 hierarchy,
					 CV_RETR_TREE,
					 CV_CHAIN_APPROX_SIMPLE );

	int originalSize = contours.size();
	int actualSize = 0;
	uint8_t * resultLocation = (uint8_t *)malloc(originalSize * 16);
	uint32_t x;
	uint32_t y;
	uint32_t width;
	uint32_t height;
	for (size_t k = 0; k < contours.size(); k++ )
	{
		double area = cv::contourArea(contours[k]);
		if ((area <= 10000.0f) && (area >= 50.0f))
		{
			cv::Rect boundingBox = cv::boundingRect(contours[k]);
			x = boundingBox.x;
			resultLocation[actualSize*16 + 3] = x >> 24;
			resultLocation[actualSize*16 + 2] = x >> 16;
			resultLocation[actualSize*16 + 1] = x >> 8;
			resultLocation[actualSize*16 + 0] = x;
			//set y
			y = boundingBox.y;
			resultLocation[actualSize*16 + 7] = y >> 24;
			resultLocation[actualSize*16 + 6] = y >> 16;
			resultLocation[actualSize*16 + 5] = y >> 8;
			resultLocation[actualSize*16 + 4] = y;
			//set width
			width = boundingBox.width;
			resultLocation[actualSize*16 + 11] = width >> 24;
			resultLocation[actualSize*16 + 10] = width >> 16;
			resultLocation[actualSize*16 + 9] = width >> 8;
			resultLocation[actualSize*16 + 8] = width;
			//set height
			height = boundingBox.width;
			resultLocation[actualSize*16 + 15] = height >> 24;
			resultLocation[actualSize*16 + 14] = height >> 16;
			resultLocation[actualSize*16 + 13] = height >> 8;
			resultLocation[actualSize*16 + 12] = height;
			actualSize += 1;
			//fprintf(stderr, "X: %u, Y: %u, Width: %u, Height: %u\n", x,y,width, height);
		}
	}
	*length = actualSize * 16;
	uint8_t * resultLocation2 = (uint8_t *)malloc(*length);
	memcpy(resultLocation2, resultLocation, *length);
	free(resultLocation);
	return resultLocation2;
}
=======
>>>>>>> 698da7b5d377ea1471e41e33c38057e3f909881b
std::string AIMWorkTask::returnTaskName(){
	return "aim_saliency";
}
std::string AIMWorkTask::getTaskName(){
	return "aim_saliency";
}




