/*
 * CMTWorkTask.cpp
 *
 *  Created on: Nov 23, 2015
 *      Author: peterazientara
 */
#include "CMTWorkTask.h"

CMTWorkTask::CMTWorkTask(){
	// TODO Auto-generated constructor stub
	//h264_decoder = new CAPIFrameDecoder();
	isSingleThreaded = true;
	cmt_tracker = cmt::CMT();
	detector = cv::SurfFeatureDetector(minHessian);
	extractor = cv::SurfDescriptorExtractor();
	fprintf(stderr, "CMTWorkTask created\n");
}
CMTWorkTask::~CMTWorkTask(){
	// TODO Auto-generated constructor stub
	fprintf(stderr, "CMTWorkTask destroyed\n");
}
void CMTWorkTask::init(){
	fprintf(stderr, "CMTWorkTask called init()\n");
	//TODO:: implement any startup logic
	OpenCVSurfSupport::setWorkDispatcher(dispatcher);
}
void CMTWorkTask::runTask(SocketData * data_in){
	fprintf(stderr, "CMTWorkTask called runTask()\n");

	switch(data_in->message_type){
		case MODEL_CHANGE:{
			//OpenCVSurfSupport::clear_loaded_models();
			//std::vector<std::string> model_names = parseConfig(data_in->data, data_in->message_length);
			////OpenCVSurfSupport::load_models_from_files( NULL, &model_names, false);
			//TODO: Get frame from the network
			//Initialize CMT;
			cv::Mat originalFrame = cv::Mat(500, 500, CV_8UC3, (void *)data_in->data);
			//cv::resize(originalFrame, originalFrame, cv::Size(640,480),0,0, CV_INTER_CUBIC);
			//initializeTracker(im_gray);
			changeModel(originalFrame);
			SocketData * returnData = new SocketData(WORK_ACK,0,0,0,0, NULL);
			sendDataPacket(returnData);
			delete returnData;
			break;
		}
		case VIDEO_FRAME:{
			fprintf(stderr, "CMT Tracker Frame Received!\n");
			//process and match
			//WorkItem * i = new WorkItem();
			//i->setData(data_in->data, data_in->message_length);
			cv::Mat greyFrame;
			cv::Mat originalFrame = cv::Mat(720, 1280, CV_8UC4, (void *)data_in->data);
			//cv::resize(originalFrame, originalFrame, cv::Size(640,480),0,0, CV_INTER_CUBIC);
			cv::cvtColor(originalFrame, originalFrame, CV_BGRA2BGR);
			processFrame(originalFrame);
			/*
			struct WorkTaskThreadArgs * thread_args = (struct WorkTaskThreadArgs *)malloc(sizeof(struct WorkTaskThreadArgs));
			thread_args->caller = this;
			thread_args->item = i;
			if(!isSingleThreaded){
				pthread_t p;
				int err = pthread_create(&p, NULL, RunTaskThreadEntry, (void *)thread_args);
				if(err != 0){
					fprintf(stderr, "CMTWorkTask, error creating thread\n");
					fprintf(stderr,"%s\n", strerror(err));
				}else{
					pthread_detach(p);
				}
			}else{
				runTaskThreadWork(thread_args);
			}
			*/
			break;
		}
		default:{
			break;
		}
	}
}
//actual work flow to run;
void CMTWorkTask::runTaskThreadWork(struct WorkTaskThreadArgs * arg){
	fprintf(stderr, "CMTWorkTask running\n");
	//TODO: step 1 decode frame
	WorkItem *i = arg->item;
	cv::Mat greyFrame;
	/*
	cv::Mat originalFrame = cv::Mat(720, 1280, CV_8UC4, (void *)i->data);
	cv::resize(originalFrame, originalFrame, cv::Size(640,480),0,0, CV_INTER_CUBIC);
	cv::cvtColor(originalFrame, greyFrame, CV_BGR2GRAY);
	int data_length = greyFrame.total() * greyFrame.elemSize();
	//uint8_t * data = (uint8_t *)malloc(data_length);
	uint32_t params[3] = { greyFrame.cols, greyFrame.rows, greyFrame.channels() };
	i->setParameters(params, 3);
	i->setData(greyFrame.data ,data_length);
*/
	/*
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
	*/
	free(arg);
	pthread_exit(0);
}
void CMTWorkTask::deInit(){
	//TODO: implement cleanup
	fprintf(stderr, "CMTWorkTask called deInit()\n");
}
void CMTWorkTask::parseParams(uint8_t * params){
	//TODO: implement any parameter parsing
	fprintf(stderr, "CMTWorkTask called parseParams()\n");
}
/*
void CAPITestsWorkTask::packetReceived(SocketData * data){
	fprintf(stderr, "CAPITestsWorkTask called packetReceived()\n");
	runTask(data);
}
*/
std::string CMTWorkTask::returnTaskName(){
	return "cmt_full_task";
}
std::string CMTWorkTask::getTaskName(){
	return "cmt_full_task";
}
std::vector<std::string> CMTWorkTask::parseConfig(uint8_t * data, uint32_t length){
	std::string * rawString = new std::string((char *)data, length);
	std::vector<std::string> config = split(*rawString, ':');
	delete rawString;
	return config;
}
std::vector<std::string>& CMTWorkTask::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}
std::vector<std::string> CMTWorkTask::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

void CMTWorkTask::processFrame(cv::Mat image)
{
	if (trackerInitialized)
	{
		cmt_tracker.processFrame(image);
		cv::Rect roi = cmt_tracker.bb_rot.boundingRect();
		cv::rectangle(image, roi, cv::Scalar(0, 255, 0));
	}
	else
	{
		if (attemptObjectDetection)
		{
			initializeTracker(image);
		}
	}
}
void CMTWorkTask::initializeTracker(cv::Mat im_gray)
{
	cv::Mat scene_image = im_gray;
	int err = OpenCVSurfSupport::openCVSurfDetectAndCompute(scene_image, scene_keypoints, scene_descriptors);
	if(err){
		detector.detect(scene_image, scene_keypoints);
		extractor.compute(scene_image, scene_keypoints, scene_descriptors);
	}
	cv::FlannBasedMatcher matcher;
	std::vector< cv::DMatch > matches;
	matcher.match(object_descriptors, scene_descriptors, matches);
	double max_dist = 0; double min_dist = 100;
	for (int i = 0; i < object_descriptors.rows; i++)
	{
		double dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}
	std::vector< cv::DMatch > good_matches;
	for (int i = 0; i < object_descriptors.rows; i++)
	{
		if (matches[i].distance < 3 * min_dist)
		{
			good_matches.push_back(matches[i]);
		}
	}
	//-- Localize the object
	std::vector<Point2f> obj;
	std::vector<Point2f> scene;
	for (int i = 0; i < good_matches.size(); i++)
	{
		//-- Get the keypoints from the good matches
		obj.push_back(object_keypoints[good_matches[i].queryIdx].pt);
		scene.push_back(scene_keypoints[good_matches[i].trainIdx].pt);
	}

	Mat H = findHomography(obj, scene, CV_RANSAC);
	//-- Get the corners from the image_1 ( the object to be "detected" )
	std::vector<Point2f> obj_corners(4);
	obj_corners[0] = cvPoint(0, 0); obj_corners[1] = cvPoint(object_image.cols, 0);
	obj_corners[2] = cvPoint(object_image.cols, object_image.rows); obj_corners[3] = cvPoint(0, object_image.rows);
	std::vector<Point2f> scene_corners(4);
	perspectiveTransform(obj_corners, scene_corners, H);
	int minX = 10000, maxX = 0, minY = 10000, maxY = 0;
	for (int i = 0; i < 4; i++)
	{
		Point2f pt = scene_corners[i];
		int x = pt.x, y = pt.y;
		if (x < minX) minX = x;
		if (x > maxX) maxX = x;
		if (y < minY) minY = y;
		if (y > maxY) maxY = y;
	}

	Rect roi = Rect(minX, minY, maxX - minX, maxY - minY);
	// Validity of ROI checking goes here.
	bool roiValid = false;
	// Get area and center location
	double area = roi.area();
	cv::Point2d center = cv::Point2d(roi.x + roi.width / 2, roi.y + roi.height / 2);
	// If we have previous measurements
	if (lastArea >= 0.0f && lastLocation.x >= 0 && lastLocation.y >= 0)
	{
		positionDelta.x = abs(lastLocation.x - center.x) / scene_image.cols;
		positionDelta.y = abs(lastLocation.y - center.y) / scene_image.rows;
		scaleDelta = abs(lastArea - area) / lastArea;
		// Log
		printf("New deltas: position (%f, %f), scale %f\n", positionDelta.x, positionDelta.y, scaleDelta);
		// Check if valid
		if (scaleDelta < scaleDeltaThreshold && positionDelta.x < positionDeltaThreshold.x && positionDelta.y < positionDeltaThreshold.y)
		{
			roiValid = true;
			std::cout << "ROI determined to be valid!" << std::endl;
		}
		else
		{
			std::cout << "ROI determined to be invalid!" << std::endl;
		}
	}
	// Log the current measurements.
	lastArea = area;
	lastLocation = center;
	// Finally, initialize if SURF worked appropriately.
	if (roiValid)
	{
		cmt_tracker.initialize(scene_image, roi);
		trackerInitialized = true;
	}
}
void CMTWorkTask::setObjectToFind(cv::Mat image)
{
	object_image = image;
	int err = OpenCVSurfSupport::openCVSurfDetectAndCompute(object_image, object_keypoints, object_descriptors);
	if(err){
		detector.detect(object_image, object_keypoints);
		extractor.compute(object_image, object_keypoints, object_descriptors);
	}
	attemptObjectDetection = true;
}
void CMTWorkTask::changeModel(cv::Mat image){
	object_image = image;
	int err = OpenCVSurfSupport::openCVSurfDetectAndCompute(object_image, object_keypoints, object_descriptors);
	if(err){
		detector.detect(object_image, object_keypoints);
		extractor.compute(object_image, object_keypoints, object_descriptors);
	}
	attemptObjectDetection = true;
	trackerInitialized = false;
}


