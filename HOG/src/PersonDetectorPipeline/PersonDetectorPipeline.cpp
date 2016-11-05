/*
 * IntelProject2016DemoWorkTask.cpp
 *
 *  Created on: Nov 23, 2015
 *      Author: peterazientara
 */
#include "PersonDetectorPipeline.h"

PersonDetectorPipelineWorkTask::PersonDetectorPipelineWorkTask(){
	// TODO Auto-generated constructor stub
	fprintf(stderr, "PersonDetectorPipelineWorkTask created\n");
}
PersonDetectorPipelineWorkTask::~PersonDetectorPipelineWorkTask(){
	// TODO Auto-generated constructor stub
	fprintf(stderr, "PersonDetectorPipelineWorkTask destroyed\n");
}
void PersonDetectorPipelineWorkTask::init(){
	fprintf(stderr, "PersonDetectorPipelineWorkTask called init()\n");
	//TODO:: implement any startup logic
}
void PersonDetectorPipelineWorkTask::runTask(SocketData * data_in){
	fprintf(stderr, "PersonDetectorPipelineWorkTask called runTask()\n");
	WorkItem * i = new WorkItem();
	i->setData(data_in->data, data_in->message_length);
	struct WorkTaskThreadArgs * thread_args = (struct WorkTaskThreadArgs *)malloc(sizeof(struct WorkTaskThreadArgs));
	thread_args->caller = this;
	thread_args->item = i;
	//runTaskThreadWork(thread_args);
	pthread_t p;
	int err = pthread_create(&p, NULL, RunTaskThreadEntry, (void *)thread_args);
	if(err != 0){
		fprintf(stderr, "PersonDetector, error creating thread\n");
	}
	pthread_detach(p);
}
//actual work flow to run;
void PersonDetectorPipelineWorkTask::runTaskThreadWork(struct WorkTaskThreadArgs * arg){
	fprintf(stderr, "PersonDetectorPipelineWorkTask running\n");
	//TODO: step 1 decode frame
	WorkItem *i = arg->item;
	cv::Mat originalFrame = cv::Mat(720, 1280, CV_8UC4, (void *)i->data);
	cv::resize(originalFrame, originalFrame, cv::Size(1056,600),0,0, CV_INTER_CUBIC);
	double im_scale;
	WorkItem ** scaleWork = (WorkItem **)malloc(sizeof(WorkItem *)*NUMLAYERS);
	for(int i = 0; i<NUMLAYERS; ++i){
		scaleWork[i] = NULL;
	}
	int layer = 0;
	for (int layer = 0; layer < NUMLAYERS; layer++)
	{
		if (layer == 0)
			im_scale = BASESCALE;
		else
			im_scale *= (1.0f / hwScale);
		cv::Mat scaledImage;
		int maxScoreLayer = 0; // variable for debug/tuning
		cv::resize(originalFrame, scaledImage, cv::Size(originalFrame.cols*im_scale, originalFrame.rows*im_scale), NULL, NULL, CV_INTER_CUBIC);
		cv::Size imageSize = cv::Size((int)(((double)originalFrame.cols * im_scale) / (double)CELLSIZE) * CELLSIZE,
			(int)(((double)originalFrame.rows * im_scale) / (double)CELLSIZE) * CELLSIZE);
		// Crop image into multipe of CellSize
		cv::Mat inputImage = scaledImage(cv::Rect(0, 0, imageSize.width, imageSize.height));
		if (inputImage.cols < ROIPIXWIDTH || inputImage.rows < ROIPIXHEIGHT)
			break;
		scaleWork[layer] = new WorkItem();
		WorkItem * t = scaleWork[layer];
		int data_length = inputImage.total() * inputImage.elemSize();
		uint32_t params[3] = { inputImage.cols, inputImage.rows, inputImage.channels() };
		t->setParameters(params, 3);
		t->setData(inputImage.data ,data_length);
		fprintf(stderr, "PersonDetectorPipelineWorkTask Submitting HOG Layer: %i\n", layer);
		dispatcher->submitWork("hog", t);
		while(t->completed == 0){
			//spin and wait for work to be done
		}
		uint32_t params1[3] = { inputImage.cols, inputImage.rows, 2 };
		t->setParameters(params1, 3);
		fprintf(stderr, "PersonDetectorPipelineWorkTask Submitting SVM Layer: %i\n", layer);
		dispatcher->submitWork("svm", t);
	}
	//wait for all svm's to finish

	std::vector<cv::Rect> foundLocations;
	std::vector<double> foundWeights;
	double finalThreshold = 2.0; // Used in NMS

	for(int i = 0; i < NUMLAYERS; ++i){
		if(scaleWork[i] != NULL){
			while(scaleWork[i]->completed != 0){
				//wait for complete;
			}
			double dpMaxVal = 0;
			double dpMinVal = 0;
			cv::Mat normdotP;
			cv::Mat * m_dotp_output = (cv::Mat *)(scaleWork[i]->result);
			cv::minMaxLoc(*m_dotp_output, &dpMinVal, &dpMaxVal, NULL, NULL);
			(*m_dotp_output).convertTo(normdotP, -1, 1 / dpMaxVal, 0);
			for (int v = 0; v < (*m_dotp_output).rows; v++)
			{
				for (int h = 0; h < (*m_dotp_output).cols; h++)
				{

					if (DETECTIONTHRESHOLD <= (*m_dotp_output).ptr<int>(v)[h])  // Limits the number of valid detections to the best ones
					{
						int curVal = (*m_dotp_output).ptr<int>(v)[h];

						cv::Rect curRect(
							(int)((double)h / im_scale) * CELLSIZE,
							(int)((double)v / im_scale) * CELLSIZE,
							(int)((double)ROIPIXWIDTH / im_scale),
							(int)((double)ROIPIXHEIGHT / im_scale)
							);
						foundLocations.push_back(curRect);
						foundWeights.push_back((double)curVal);
					}
				}
			}
			delete scaleWork[i];
			scaleWork[i] = NULL;
		}else{
			break;
		}
	}
	nonMaxSurpression(foundLocations, foundWeights, (int)finalThreshold, 0.2);

	if(arg->caller != NULL){
		int lden = 0;
		//uint8_t * dat = (uint8_t *)OpenCVSurfSupport::roiregion_to_byte(r, 1, &lden);
		SocketData * returnData = new SocketData(WORK_ACK,0,0,0,lden, NULL);
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
void PersonDetectorPipelineWorkTask::deInit(){
	//TODO: implement cleanup
	fprintf(stderr, "PersonDetectorPipelineWorkTask called deInit()\n");
}
void PersonDetectorPipelineWorkTask::parseParams(uint8_t * params){
	//TODO: implement any parameter parsing
	fprintf(stderr, "PersonDetectorPipelineWorkTask called parseParams()\n");
}
std::string PersonDetectorPipelineWorkTask::returnTaskName(){
	return "person_detection_task";
}
std::string PersonDetectorPipelineWorkTask::getTaskName(){
	return "person_detection_task";
}
std::vector<std::string> PersonDetectorPipelineWorkTask::parseConfig(uint8_t * data, uint32_t length){
	std::string * rawString = new std::string((char *)data, length);
	std::vector<std::string> config = split(*rawString, ':');
	delete rawString;
	return config;
}
std::vector<std::string>& PersonDetectorPipelineWorkTask::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}
std::vector<std::string> PersonDetectorPipelineWorkTask::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

void PersonDetectorPipelineWorkTask::nonMaxSurpression(std::vector<cv::Rect>& rectList, std::vector<double>& weights, int groupThreshold, double eps)
{
	if (groupThreshold <= 0 || rectList.empty())
	{
		return;
	}
	CV_Assert(rectList.size() == weights.size());
	std::vector<int> labels;
	int nclasses = partition(rectList, labels, cv::SimilarRects(eps));
	std::vector<cv::Rect_<double> > rrects(nclasses);
	std::vector<int> numInClass(nclasses, 0);
	std::vector<double> foundWeights(nclasses, -std::numeric_limits<double>::max());
	int i, j, nlabels = (int)labels.size();
	for (i = 0; i < nlabels; i++)
	{
		int cls = labels[i];
		rrects[cls].x += rectList[i].x;
		rrects[cls].y += rectList[i].y;
		rrects[cls].width += rectList[i].width;
		rrects[cls].height += rectList[i].height;
		foundWeights[cls] = mymax(foundWeights[cls], weights[i]);
		numInClass[cls]++;
	}
	for (i = 0; i < nclasses; i++)
	{
		// find the average of all ROI in the cluster
		cv::Rect_<double> r = rrects[i];
		double s = 1.0 / numInClass[i];
		rrects[i] = cv::Rect_<double>(cv::saturate_cast<double>(r.x*s),
		cv::saturate_cast<double>(r.y*s),
		cv::saturate_cast<double>(r.width*s),
		cv::saturate_cast<double>(r.height*s));
	}
	rectList.clear();
	weights.clear();
	for (i = 0; i < nclasses; i++)
	{
		cv::Rect r1 = rrects[i];
		int n1 = numInClass[i];
		double w1 = foundWeights[i];
		if (n1 <= groupThreshold)
			continue;
		// filter out small rectangles inside large rectangles
		for (j = 0; j < nclasses; j++)
		{
			int n2 = numInClass[j];
			if (j == i || n2 <= groupThreshold)
				continue;
			cv::Rect r2 = rrects[j];
			int dx = cv::saturate_cast<int>(r2.width * eps);
			int dy = cv::saturate_cast<int>(r2.height * eps);
			if (r1.x >= r2.x - dx &&
				r1.y >= r2.y - dy &&
				r1.x + r1.width <= r2.x + r2.width + dx &&
				r1.y + r1.height <= r2.y + r2.height + dy &&
				(n2 > std::max(3, n1) || n1 < 3))
				break;
		}

		if (j == nclasses)
		{
			rectList.push_back(r1);
			weights.push_back(w1);
		}
	}
}
