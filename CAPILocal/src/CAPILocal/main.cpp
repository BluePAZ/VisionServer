#define WITH_AKAZE 0
#define WITH_SURF 1
#define WITH_GPU_SURF 0
#define WITH_AIM 0
#define WITH_HOG 0
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sstream>
#include <vector>
#include <errno.h>

#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "WorkItem/WorkItem.hpp"
#include "WorkTask/WorkTask.h"
#include "WorkManagerDispatcher/WorkManagerDispatcher.h"
#include "WorkTaskFactory/WorkTaskFactory.h"
#include "WorkTypeDefs.hpp"

#if(WITH_SURF)
#include "OpenCVSurfBFFMatcher/OpenCVSurfBFFMatcherWorkConsumer/OpenCVSurfBFFMatcherWorkConsumer.h"
#include "OpenCVSurfBFFMatcher/OpenCVSurfBFFMatcherWorkManager/OpenCVSurfBFFMatcherWorkManager.h"
#include "OpenCVSurfIpDescriptor/OpenCVSurfIpDescriptorWorkConsumer/OpenCVSurfIpDescriptorWorkConsumer.h"
#include "OpenCVSurfIpDescriptor/OpenCVSurfIpDescriptorWorkManager/OpenCVSurfIpDescriptorWorkManager.h"
#include "OpenCVSurfDescriptor/OpenCVSurfDescriptorWorkConsumer/OpenCVSurfDescriptorWorkConsumer.h"
#include "OpenCVSurfDescriptor/OpenCVSurfDescriptorWorkManager/OpenCVSurfDescriptorWorkManager.h"
#include "OpenCVSurfIp/OpenCVSurfIpWorkConsumer/OpenCVSurfIpWorkConsumer.h"
#include "OpenCVSurfIp/OpenCVSurfIpWorkManager/OpenCVSurfIpWorkManager.h"
#include "OpenCVSurfFull/OpenCVSurfFullWorkTask/OpenCVSurfFullWorkTask.h"
#include "OpenCVSurfSupport/OpenCVSurfSupport.h"
#include <OpenCVSurfFullWorkItem/OpenCVSurfFullWorkItem.hpp>

#include "SURFIPWorkConsumer/SURFIPWorkConsumer.h"
#include "SURFIPWorkManager/SURFIPWorkManager.h"
#endif

#if(WITH_GPU_SURF)
#include "OpenCVOclSurfIpDescriptor/OpenCVOclSurfIpDescriptorWorkConsumer/OpenCVOclSurfIpDescriptorWorkConsumer.h"
#include "OpenCVGpuSurfIpDescriptor/OpenCVGpuSurfIpDescriptorWorkConsumer/OpenCVGpuSurfIpDescriptorWorkConsumer.h"
#include "OpenCVGpuSurfIpDescriptor/OpenCVGpuSurfIpDescriptorWorkManager/OpenCVGpuSurfIpDescriptorWorkManager.h"
#include "OpenCVGpuSurfDescriptor/OpenCVGpuSurfDescriptorWorkManager/OpenCVGpuSurfDescriptorWorkManager.h"
#include "OpenCVGpuSurfDescriptor/OpenCVGpuSurfDescriptorWorkConsumer/OpenCVGpuSurfDescriptorWorkConsumer.h"
#include "OpenCVGpuBFFMatcher/OpenCVGpuBFFMatcherWorkManager/OpenCVGpuBFFMatcherWorkManager.h"
#include "OpenCVGpuBFFMatcher/OpenCVGpuBFFMatcherWorkConsumer/OpenCVGpuBFFMatcherWorkConsumer.h"
#endif

#if(WITH_AKAZE)
#include "AKAZEFull/AKAZEFullWorkConsumer/AKAZEFullWorkConsumer.h"
#include "AKAZEFull/AKAZEFullWorkManager/AKAZEFullWorkManager.h"
#endif

#if(WITH_AIM)
#include "AIMWorkManager/AIMWorkManager.h"
#include "AIMWorkConsumer/AIMWorkConsumer.hpp"
#endif

#if(WITH_HOG)
#include "HOGWorkConsumer/HOGWorkConsumer.h"
#include "HOGWorkManager/HOGWorkManager.h"
#include "SVMWorkConsumer/SVMWorkConsumer.h"
#include "SVMWorkManager/SVMWorkManager.h"
#include "PersonDetectorPipeline/PersonDetectorPipeline.h"
#endif

enum HARDWAREHOGPARAMS : uint32_t
	{
		KERNELWIDTH = 3,
		KERNELHEIGHT = 3,
		NUMLAYERS = 15,
		BASESCALE = 1,
		SVMROIWIDTH = 14,
		SVMROICHANNELS = 31,
		CELLSIZE = 8,
		ROIPIXWIDTH = 64,
		ROIPIXHEIGHT = 128,
		DETECTIONTHRESHOLD = 122000,
		BITSPERSVMBIN = 32,
		NUMSVMBIN = 18,
		NUMRHABIN = 32,
		RHA_NCA_SCALEFACTOR = 64,
		RHABATCHSIZE = 256 // 128 histograms are stored by RHA in one batch
	};

#include "ROIParser.h"

using namespace cv;
WorkTaskFactory * taskFactory;
WorkManagerDispatcher * dispatcher;

void nonMaxSurpression(std::vector<cv::Rect>& rectList, std::vector<double>& weights, int groupThreshold, double eps);

void init(){
	taskFactory = WorkTaskFactory::get();
	IVortexNIFSAP * AcceleratorInterface = new nifsap_capi_v2_00_b(false,0,"",7,0,0);
	if (!AcceleratorInterface->Initialize(0))
	{
		fprintf(stderr, "Failed to init accelerator\n");
		delete(AcceleratorInterface);
		return;
	}
	AcceleratorInterface->AddMemoryInterface(0,0,0);
	dispatcher = new WorkManagerDispatcher();
	/*
	 * Create worker and manager
	 */
#if(WITH_AIM)
	AIMWorkManager *aimManager = new AIMWorkManager();
	aimManager->enqueueWorker(new AIMWorkConsumer());
	dispatcher->addManager(aimManager);
#endif
#if(WITH_SURF)
	OpenCVSurfBFFMatcherWorkManager *surfBffManager = new OpenCVSurfBFFMatcherWorkManager();
	surfBffManager->enqueueWorker(new OpenCVSurfBFFMatcherWorkConsumer());
	OpenCVSurfIpDescriptorWorkManager *surfIpDescManager = new OpenCVSurfIpDescriptorWorkManager();
	surfIpDescManager->enqueueWorker(new OpenCVSurfIpDescriptorWorkConsumer());
	OpenCVSurfIpWorkManager *surfIpManager = new OpenCVSurfIpWorkManager();
	surfIpManager->enqueueWorker(new OpenCVSurfIpWorkConsumer());
	OpenCVSurfDescriptorWorkManager *surfDescManager = new OpenCVSurfDescriptorWorkManager();
	surfDescManager->enqueueWorker(new OpenCVSurfDescriptorWorkConsumer());

	dispatcher->addManager(surfIpDescManager);
	dispatcher->addManager(surfBffManager);
	dispatcher->addManager(surfDescManager);
	dispatcher->addManager(surfIpManager);
#endif
#if(WITH_GPU_SURF)
	OpenCVGpuSurfIpDescriptorWorkManager *surfGPUIpDescManager = new OpenCVGpuSurfIpDescriptorWorkManager();
	surfGPUIpDescManager->enqueueWorker(new OpenCVGpuSurfIpDescriptorWorkConsumer());
	OpenCVGpuSurfDescriptorWorkManager *surfGPUDescManager = new OpenCVGpuSurfDescriptorWorkManager();
	surfGPUDescManager->enqueueWorker(new OpenCVGpuSurfDescriptorWorkConsumer());
	OpenCVGpuBFFMatcherWorkManager * gpuBFFMatcher = new OpenCVGpuBFFMatcherWorkManager();
	gpuBFFMatcher->enqueueWorker(new OpenCVGpuBFFMatcherWorkConsumer());

	dispatcher->addManager(surfGPUIpDescManager);
	dispatcher->addManager(surfGPUDescManager);
	dispatcher->addManager(gpuBFFMatcher);
#endif
#if(WITH_AKAZE)
	AKAZEFullWorkManager *akazeFullManager = new AKAZEFullWorkManager();
	akazeFullManager->enqueueWorker(new AKAZEFullWorkConsumer());

	dispatcher->addManager(akazeFullManager);
#endif

#if(WITH_HOG)
	HOGWorkManager *hogManager = new HOGWorkManager();
	hogManager->enqueueWorker(new HOGWorkConsumer(AcceleratorInterface));
	dispatcher->addManager(hogManager);

	SVMWorkManager *svmManager = new SVMWorkManager();
	svmManager->enqueueWorker(new SVMWorkConsumer(AcceleratorInterface));
	dispatcher->addManager(svmManager);

	//taskFactory->registerTask(PersonDetectorPipelineWorkTask::returnTaskName(), &PersonDetectorPipelineWorkTask::Create);
#endif

}
int opencv_demo();
int run_pipeline();
int extract_models();
int run_hog();

int main(int argc, char* argv[]){
	//init();
	opencv_demo();
	//run_hog();
	//extract_models();

}
int opencv_demo(){
	Mat img_object = imread( "/home/mdl/Wegmans1080.jpg", CV_LOAD_IMAGE_GRAYSCALE );
	cv::resize(img_object, img_object, cv::Size(1024,768),0,0, CV_INTER_CUBIC);

	  Mat img_scene = imread( "/home/mdl/Wegmans1080.jpg", CV_LOAD_IMAGE_GRAYSCALE );
	cv::resize(img_scene, img_scene, cv::Size(1024,768),0,0, CV_INTER_CUBIC);

	  if( !img_object.data || !img_scene.data )
	  { std::cout<< " --(!) Error reading images " << std::endl; return -1; }

	  //-- Step 1: Detect the keypoints using SURF Detector
	  int minHessian = 500;

	  SurfFeatureDetector detector( minHessian );

	  std::vector<KeyPoint> keypoints_object, keypoints_scene;


	  detector.detect( img_object, keypoints_object );
	  detector.detect( img_scene, keypoints_scene );

	  //-- Step 2: Calculate descriptors (feature vectors)
	  SurfDescriptorExtractor extractor;

	  Mat descriptors_object, descriptors_scene;

	  extractor.compute( img_object, keypoints_object, descriptors_object );
	  extractor.compute( img_scene, keypoints_scene, descriptors_scene );

	  //-- Step 3: Matching descriptor vectors using FLANN matcher
	  FlannBasedMatcher matcher;
	  std::vector< DMatch > matches;
	  matcher.match( descriptors_object, descriptors_scene, matches );

	  double max_dist = 0; double min_dist = 100;

	  //-- Quick calculation of max and min distances between keypoints
	  for( int i = 0; i < descriptors_object.rows; i++ )
	  { double dist = matches[i].distance;
	    if( dist < min_dist ) min_dist = dist;
	    if( dist > max_dist ) max_dist = dist;
	  }
	  printf("-- Max dist : %f \n", max_dist );
	  printf("-- Min dist : %f \n", min_dist );
	  //-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
	  std::vector< DMatch > good_matches;
	  for( int i = 0; i < descriptors_object.rows; i++ )
	  { if( matches[i].distance < 3*min_dist )
	     { good_matches.push_back( matches[i]); }
	  }
	  Mat img_matches;
	  cv::drawMatches( img_object, keypoints_object, img_scene, keypoints_scene,
	               good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
	               vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

	  //-- Localize the object
	  std::vector<Point2f> obj;
	  std::vector<Point2f> scene;

	  for( int i = 0; i < good_matches.size(); i++ )
	  {
	    //-- Get the keypoints from the good matches
	    obj.push_back( keypoints_object[ good_matches[i].queryIdx ].pt );
	    scene.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt );
	  }

	  imshow("window", img_scene);
	  cv::waitKey(0);
	  Mat H = findHomography( obj, scene, CV_RANSAC );

	  //-- Get the corners from the image_1 ( the object to be "detected" )
	  std::vector<Point2f> obj_corners(4);
	  obj_corners[0] = cvPoint(0,0); obj_corners[1] = cvPoint( img_object.cols, 0 );
	  obj_corners[2] = cvPoint( img_object.cols, img_object.rows ); obj_corners[3] = cvPoint( 0, img_object.rows );
	  std::vector<Point2f> scene_corners(4);

	  perspectiveTransform( obj_corners, scene_corners, H);

	  //-- Draw lines between the corners (the mapped object in the scene - image_2 )
	  line( img_matches, scene_corners[0] + Point2f( img_object.cols, 0), scene_corners[1] + Point2f( img_object.cols, 0), Scalar(0, 255, 0), 4 );
	  line( img_matches, scene_corners[1] + Point2f( img_object.cols, 0), scene_corners[2] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );
	  line( img_matches, scene_corners[2] + Point2f( img_object.cols, 0), scene_corners[3] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );
	  line( img_matches, scene_corners[3] + Point2f( img_object.cols, 0), scene_corners[0] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );

	  //-- Show detected matches
	  imshow( "Good Matches & Object detection", img_matches );

	  waitKey(0);
	return 0;
}

int extract_models(){
	init();
	cv::Mat originalFrame;
	std::vector<std::string> names;
	names.push_back("shoping_shelf");
	//names.push_back("cpu_fan");
	//names.push_back("nvidia_gpu");
	//names.push_back("asus_mobo");

	for(uint32_t ij = 0; ij < (uint32_t) names.size(); ++ij){
		WorkItem * i = NULL;
		originalFrame = cv::imread("/home/mdl/" + names[ij] + ".jpg");
		//cv::resize(originalFrame, originalFrame, cv::Size(), .75, .75, CV_INTER_CUBIC);
		cv::resize(originalFrame, originalFrame, cv::Size(1280,720),0,0, CV_INTER_CUBIC);
		int tmp =0 ;
		if(originalFrame.data == NULL){
			fprintf(stderr,"cant read image");
		}
		cv::cvtColor(originalFrame, originalFrame, CV_BGR2GRAY);
		int data_length = originalFrame.total() * originalFrame.elemSize();
		i = new WorkItem();
		uint32_t params[3] = { originalFrame.cols, originalFrame.rows, originalFrame.channels() };
		i->setParameters(params, 3);
		i->setData(originalFrame.data, data_length);
		i->freeResults();
		dispatcher->submitWork("opencv_surf_ip", i);
		while(i->completed == 0){
			//spin and wait for work to be done
		}
		uint32_t params1[3] = { (uint32_t) originalFrame.cols, (uint32_t) originalFrame.rows, (uint32_t) originalFrame.channels() };
		i->setParameters(params1, 3);
		dispatcher->submitWork("opencv_surf_descriptor", i);
		while(i->completed == 0){
			//spin and wait for work to be done
		}
		fprintf(stderr, "num keypoints : %u\n", ((std::vector<cv::KeyPoint> *)i->result)->size());
		OpenCVSurfSupport::write_model_to_file(originalFrame.cols, originalFrame.rows, *((cv::Mat *)i->data), (std::vector<cv::KeyPoint> *)i->result, "", names[ij] ,false);
		delete i;
	}
}
int run_hog(){
		cv::Mat originalFrame = cv::imread("/home/ska130/capi_workspace/HOGDetectorTest/data/I00250.bmp");
		//cv::imshow("window", originalFrame);
		//cv::waitKey(0);
		//cv::rectangle(originalFrame, cv::Point(50,50),  cv::Point(150,150),  cv::Scalar(140.0,140.0,0.0));
		//cv::imshow("test", originalFrame);
		//cv::waitKey(0);
		if(originalFrame.data == NULL){
			fprintf(stderr,"cant read image\n");
		}

		//cv::imshow("window", originalFrame);
		//	cv::waitKey(0);
			//cv::Mat originalFrame = cv::Mat(720, 1280, CV_8UC4, (void *)i->data);
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
				im_scale *= (1.0f / 1.1f);
			cv::Mat scaledImage;
			int maxScoreLayer = 0; // variable for debug/tuning
			cv::resize(originalFrame, scaledImage, cv::Size(originalFrame.cols*im_scale, originalFrame.rows*im_scale), NULL, NULL, CV_INTER_CUBIC);
			cv::Size imageSize = cv::Size((int)(((double)originalFrame.cols * im_scale) / (double)CELLSIZE) * CELLSIZE,
				(int)(((double)originalFrame.rows * im_scale) / (double)CELLSIZE) * CELLSIZE);
			// Crop image into multipe of CellSize
			cv::Mat inputImage = scaledImage(cv::Rect(0, 0, imageSize.width, imageSize.height));
			cout << "Main:" << inputImage.rows << endl;
			cout << "Main:" << inputImage.cols << endl;
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
			int cttt = 0;
			while(t->completed == 0){
				//spin and wait for work to be done

			}
			uint32_t params1[3] = { inputImage.cols, inputImage.rows, 2 };
			t->setParameters(params1, 3);
			fprintf(stderr, "PersonDetectorPipelineWorkTask Submitting SVM Layer: %i\n", layer);
			dispatcher->submitWork("svm", t);
			int ctt = 0;
			while(t->completed == 0){
				//spin and wait for work to be done
			}
		}
		//wait for all svm's to finish
		fprintf(stderr, "All layers done! \n");


		std::vector<cv::Rect> foundLocations;
		std::vector<double> foundWeights;
		double finalThreshold = 2.0; // Used in NMS

		for(int i = 0; i < NUMLAYERS; ++i){
			if(scaleWork[i] != NULL){
				fprintf(stderr ,"Waiting for layer %i\n", i);
				while(scaleWork[i]->completed == 0){
					//wait for complete;
				}
				fprintf(stderr ,"Done with layer %i\n", i);

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
		fprintf(stderr, "Non max suppression before\n");
		//nonMaxSurpression(foundLocations, foundWeights, (int)finalThreshold, 0.2);
		fprintf(stderr, "Non max suppression after\n");


		for(int iii = 0; iii<foundLocations.size(); iii++){
			printf("x: %i, y: %i, width:%i, height:%i, score: %i\n", foundLocations[iii].x, foundLocations[iii].y, foundLocations[iii].width, foundLocations[iii].height, (int)foundWeights[iii]);
		}

}
/*
void nonMaxSurpression(std::vector<cv::Rect>& rectList, std::vector<double>& weights, int groupThreshold, double eps)
{
	if (groupThreshold <= 0 || rectList.empty())
	{
		return;
	}
	fprintf(stderr, "Weight Size: %i, RecList Size: %i\n", weights.size(), rectList.size());
	CV_Assert(rectList.size() == weights.size());
	fprintf(stderr, "After assert Bitch! \n");

	std::vector<int> labels;
	fprintf(stderr, "before partition! \n");

	int nclasses = cv::partition(rectList, labels, cv::SimilarRects(eps));
	fprintf(stderr, "after partition! \n");

	std::vector<cv::Rect_<double> > rrects(nclasses);
	std::vector<int> numInClass(nclasses, 0);
	fprintf(stderr, "before max! \n");

	std::vector<double> foundWeights(nclasses, std::numeric_limits<double>::max());
	fprintf(stderr, "after max! \n");

	int i, j, nlabels = (int)labels.size();

	fprintf(stderr, "nclasses: %i\n", nclasses);

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
	fprintf(stderr, "nclasses: %i\n", nclasses);

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
	fprintf(stderr, "nclasses: %i\n", nclasses);
	for (i = 0; i < nclasses; i++)
	{

		fprintf(stderr, "Weight Size: %i, RecList Size: %i\n", weights.size(), rectList.size());

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
		fprintf(stderr, "Weight Size: %i, RecList Size: %i\n", weights.size(), rectList.size());

	}
}
*/
int run_pipeline(){

	cv::VideoCapture vid;
	bool err;
	init();
	cv::Mat originalFrame;
	cv::Mat greyFrame;
	//int err1 = vid.open("/home/mdl/tmp.avi");
	//if(!vid.isOpened()){
	//	fprintf(stderr, "not openend");
	//}
	//read frame
	WorkItem * surfData = NULL;
	WorkItem * surfGPUData = NULL;

	WorkItem * akazeData = NULL;
	WorkItem * aimData = NULL;

	//err = vid.retrieve(originalFrame, 0);
	//while(err != 0){
	originalFrame = cv::imread("/home/mdl/Wegmans1080.jpg");
	//cv::rectangle(originalFrame, cv::Point(50,50),  cv::Point(150,150),  cv::Scalar(140.0,140.0,0.0));
	//cv::imshow("test", originalFrame);
	//cv::waitKey(0);
	if(originalFrame.data == NULL){
		fprintf(stderr,"cant read image\n");
	}
	//resize to try to speed up computation
	//cv::resize(originalFrame, originalFrame, cv::Size(1920,1080),0,0, CV_INTER_CUBIC);
	cv::cvtColor(originalFrame, greyFrame, CV_BGR2GRAY);
	int grey_data_length = greyFrame.total() * greyFrame.elemSize();
	int original_data_length = originalFrame.total() * originalFrame.elemSize();

	/*
	uint32_t params[3] = {(uint32_t) originalFrame.cols, (uint32_t) originalFrame.rows, (uint32_t) originalFrame.channels() };
	aimData = new WorkItem();
	aimData->setParameters(params, 3);
	aimData->setData(originalFrame.data, original_data_length);
	aimData->freeResults();
	while(1){
	auto ta = std::chrono::system_clock::now();
	dispatcher->submitWork("aim_saliency", aimData);
	while(aimData->completed == 0){
		//spin and wait for work to be done
	}
	auto tb = std::chrono::system_clock::now();
					std::cout << "AIM TIME: "
					  << std::chrono::duration_cast<std::chrono::milliseconds>(tb - ta).count()
					  << " milliseconds" << std::endl;
	}
	*/
	//uint32_t params0[3] = {(uint32_t) greyFrame.cols, (uint32_t) greyFrame.rows, (uint32_t) greyFrame.channels() };
	//surfGPUData = new WorkItem();
	//surfGPUData->setParameters(params0, 3);
	//surfGPUData->setData(greyFrame.data, grey_data_length);
	//OpenCVSurfWorkItem * i = new OpenCVSurfWorkItem();
	//i->image = originalFrame;
	//fprintf(stderr,"set image\n");


	/*
	uint32_t params2[3] = {(uint32_t) greyFrame.cols, (uint32_t) greyFrame.rows, (uint32_t) greyFrame.channels() };
	akazeData = new WorkItem();
	akazeData->setParameters(params2, 3);
	akazeData->setData(greyFrame.data, grey_data_length);
	akazeData->freeResults();
	*/
	//DO THE SURF PIPELINE
	//while(1){
	//	auto t1 = std::chrono::system_clock::now();
	//	fprintf(stderr,"submitting\n");

	//	dispatcher->submitWork("opencv_surf_gpu_full", (WorkItem *)i);
	//	while(((WorkItem *)i)->completed == 0){
			//spin and wait for work to be done
//	}
	//	dispatcher->submitWork("opencv_surf_gpu_descriptor", (WorkItem *)i);
	//	while(((WorkItem *)i)->completed == 0){
			//spin and wait for work to be done
	//	}
	//	auto t2 = std::chrono::system_clock::now();
	//						std::cout << "GPU SURF PIPE TIME: "
	//						  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
	//						  << " milliseconds" << std::endl;
	//}
	/*
	{
		dispatcher->submitWork("opencv_gpu_bffmatcher", surfGPUData);
		while(surfGPUData->completed == 0){
			//spin and wait for work to be done
		}
		dispatcher->submitWork("opencv_surf_bffmatcher", surfGPUData);
		while(surfGPUData->completed == 0){
			//spin and wait for work to be done
		}
	}
	*/
/*
	dispatcher->submitWork("opencv_surf_ip", (WorkItem *)i);
	while(((WorkItem *)i)->completed == 0){
		//spin and wait for work to be done
	}
	fprintf(stderr, "Kpts Size: %u\n",i->keypoints.size());
	uint32_t params1[3] = { (uint32_t) greyFrame.cols,(uint32_t) greyFrame.rows,(uint32_t) greyFrame.channels() };
	((WorkItem *)i)->setParameters(params1, 3);
	dispatcher->submitWork("opencv_surf_descriptor", (WorkItem *)i);
	while(((WorkItem *)i)->completed == 0){
		//spin and wait for work to be done
	}
	*/
	//t2 = std::chrono::system_clock::now();
	//			std::cout << "CPU SURF PIPE TIME: "
	//			  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
	//			  << " milliseconds" << std::endl;

	//process to send to matcher
	/*
	dispatcher->submitWork("opencv_surf_bffmatcher", i);
	while(i->completed == 0){
		//spin and wait for work to be done
	}

	HomographyMatches ** match_results = (HomographyMatches **)i->result;
	int num_res = i->result_length;
	for(int nm = 0; nm < num_res; ++nm){
		if(match_results[nm] != NULL){
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
			  //-- Draw lines between the corners (the mapped object in the scene - image_2 )
			 // cv::line( originalFrame, scene_corners[0] + cv::Point2f( 1024.0f, 0), scene_corners[1] + cv::Point2f( 1024.0f, 0), cv::Scalar(0, 255, 0), 4 );
			 // cv::line( originalFrame, scene_corners[1] + cv::Point2f( 1024.0f, 0), scene_corners[2] + cv::Point2f( 1024.0f, 0), cv::Scalar( 0, 255, 0), 4 );
			 // cv::line( originalFrame, scene_corners[2] + cv::Point2f( 1024.0f, 0), scene_corners[3] + cv::Point2f( 1024.0f, 0), cv::Scalar( 0, 255, 0), 4 );
			 // cv::line( originalFrame, scene_corners[3] + cv::Point2f( 1024.0f, 0), scene_corners[0] + cv::Point2f( 1024.0f, 0), cv::Scalar( 0, 255, 0), 4 );
			  cv::line( originalFrame, scene_corners[0] , scene_corners[1] , cv::Scalar(0, 255, 0), 4 );
			  cv::line( originalFrame, scene_corners[1] , scene_corners[2] , cv::Scalar( 0, 255, 0), 4 );
			  cv::line( originalFrame, scene_corners[2] , scene_corners[3] , cv::Scalar( 0, 255, 0), 4 );
			  cv::line( originalFrame, scene_corners[3] , scene_corners[0] , cv::Scalar( 0, 255, 0), 4 );
		}
	}
	*/
				/*
	dispatcher->submitWork("akaze_full", akazeData);
	while(akazeData->completed == 0){
		//spin and wait for work to be done
	}
	*/
/*
	auto t2 = std::chrono::system_clock::now();
			std::cout << "DUAL PIPE TIME: "
			  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
			  << " milliseconds" << std::endl;
*/
//	std::vector< cv::KeyPoint > * akaze_kpts = (std::vector< cv::KeyPoint > *)akazeData->data;
	std::vector< cv::KeyPoint > * surf_kpts = (std::vector< cv::KeyPoint > *)surfData->result;

//	fprintf(stderr, "Akaze kpt length: %i\n", akaze_kpts->size());
	fprintf(stderr, "Surf kpt length: %i\n", surf_kpts->size());

	cv::Mat surfDisplay;// = cv::Mat(originalFrame);
	cv::Mat akazeDisplay;// = cv::Mat(originalFrame);


	cv::drawKeypoints(originalFrame, *surf_kpts, surfDisplay);
//	cv::drawKeypoints(originalFrame, *akaze_kpts, akazeDisplay);


	cv::imshow("surfKeyPoints", surfDisplay);
//	cv::imshow("azazeKeyPoints", akazeDisplay);

	cv::waitKey(0);

	delete akazeData;
	delete surfData;
	//err = vid.retrieve(originalFrame, 0);
	//}
	return 0;
}

