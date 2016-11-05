#include "AKAZESupport.h"

std::vector<cv::Mat *> * AKAZESupport::frame_models;
std::vector<cv::Mat *> * AKAZESupport::subframe_models;
std::vector< std::vector < cv::KeyPoint > * > * AKAZESupport::frame_keypoints;
std::vector< std::vector < cv::KeyPoint > * > * AKAZESupport::subframe_keypoints;
std::vector< CAPISize > * AKAZESupport::model_sizes;

ROIParser * AKAZESupport::rois;

int AKAZESupport::num_frame_models;
int AKAZESupport::num_subframe_models;

cv::Mat * AKAZESupport::computeHomography(std::vector<cv::Point2f> * model, std::vector<cv::Point2f> * unknown){
	cv::Mat * H = new cv::Mat();
	*H = cv::findHomography(*model, *unknown,  CV_RANSAC);
/*
	fprintf(stderr, "Homography (0,0): %lf\n", H->at<double>(0,0));
	fprintf(stderr, "Homography (0,1): %lf\n", H->at<double>(0,1));
	fprintf(stderr, "Homography (0,2): %lf\n", H->at<double>(0,2));
	fprintf(stderr, "Homography (1,0): %lf\n", H->at<double>(1,0));
	fprintf(stderr, "Homography (1,1): %lf\n", H->at<double>(1,1));
	fprintf(stderr, "Homography (1,2): %lf\n", H->at<double>(1,2));
	fprintf(stderr, "Homography (2,0): %lf\n", H->at<double>(2,0));
	fprintf(stderr, "Homography (2,1): %lf\n", H->at<double>(2,1));
	fprintf(stderr, "Homography (2,2): %lf\n", H->at<double>(2,2));
	/*
	H->at<double>(0,0) = 0.512765844507661f;
	H->at<double>(0,1) = -0.00679424489712396f;
	H->at<double>(0,2) = 253.19051404197f;
	H->at<double>(1,0) = 0.0184852094091346f;
	H->at<double>(1,1) = 0.582344390657904f;
	H->at<double>(1,2) = 145.35072187567f;
	H->at<double>(2,0) = 3.34380486610434E-05f;
	H->at<double>(2,1) = -4.96544230812344E-06f;
	H->at<double>(2,2) = 1.0f;
	*/
	if(H->empty()){
		return NULL;
	}else{
		return H;
	}
}
AKAZEKeyPointLite * AKAZESupport::opencv_to_akazekeypointlite(std::vector< cv::KeyPoint > keypoints){
	AKAZEKeyPointLite * points = (AKAZEKeyPointLite *)malloc(keypoints.size() * sizeof(AKAZEKeyPointLite));
	for(unsigned int i = 0; i < keypoints.size(); i++) 
	{
		points[i].scale.rawValue = keypoints[i].size;
		points[i].orientation.rawValue = keypoints[i].angle;
		points[i].laplacian = keypoints[i].response;
		points[i].r1 = (int)(keypoints[i].pt.x);
		points[i].c1 = (int)(keypoints[i].pt.y);
		points[i].c2 = round(points[i].scale.rawValue * cos(points[i].orientation.rawValue)) + points[i].c1;
		points[i].r2 = round(points[i].scale.rawValue * sin(points[i].orientation.rawValue)) + points[i].r1;
	}
	return points;
}
uint32_t * AKAZESupport::roiregion_to_byte(ROIRegion * regions, int num, int * length){
	*length = num * 4 * (8);
	uint32_t * data = (uint32_t *)malloc(*length);
	for(int i = 0; i < num; ++i){
		int idx = i*(8);
		data[idx + 0] = *((uint32_t *)(&(regions[i].x1)));
		data[idx + 1] = *((uint32_t *)(&(regions[i].y1)));
		data[idx + 2] = *((uint32_t *)(&(regions[i].x2)));
		data[idx + 3] = *((uint32_t *)(&(regions[i].y2)));
		data[idx + 4] = *((uint32_t *)(&(regions[i].x3)));
		data[idx + 5] = *((uint32_t *)(&(regions[i].y3)));
		data[idx + 6] = *((uint32_t *)(&(regions[i].x4)));
		data[idx + 7] = *((uint32_t *)(&(regions[i].y4)));
	}
	free(regions);
	return data;
}
uint32_t * AKAZESupport::keypointslite_to_byte(AKAZEKeyPointLite * keyPoints, int num, int * length){
	*length = num * 4 * (7);
	uint32_t * data = (uint32_t *)malloc(*length);
	for(int i = 0; i < num; ++i){
		int idx = i*(7);
		memcpy(&(data[idx + 0]), &(keyPoints[i].scale.bytes[0]), sizeof(float));
		memcpy(&(data[idx + 1]), &(keyPoints[i].orientation.bytes[0]), sizeof(float));
		data[idx + 2] = *((uint32_t *)(&(keyPoints[i].laplacian)));
		data[idx + 3] = *((uint32_t *)(&(keyPoints[i].r1)));
		data[idx + 4] = *((uint32_t *)(&(keyPoints[i].c1)));
		data[idx + 5] = *((uint32_t *)(&(keyPoints[i].r2)));
		data[idx + 6] = *((uint32_t *)(&(keyPoints[i].c2)));
	}	
	free(keyPoints);
	return data;
}
void AKAZESupport::write_model_to_file(int width, int height, cv::Mat descriptors, std::vector < cv::KeyPoint > * keypoints, std::string className, std::string modelName, bool extended){
	// Declare what you need
	std::string basePath = std::string("/home/mdl/shared/vision/models/opencv");
	std::string filePath;
	if(extended){
		filePath = std::string("/extended/");// + className + std::string("/");
	}else{
		filePath = std::string("/regular/");// + className + std::string("/");
	}
	//std::string fileName = std::string("wegmans.bin");
	std::string fileName = modelName + std::string(".bin");
	std::string fullPath = basePath + filePath + fileName;
	cv::FileStorage file(fullPath, cv::FileStorage::WRITE);
	// Write to file!
	file << "width" << width;
	file << "height" << height;
	file << "keypoints" << *keypoints;
	file << "descriptors" << descriptors;
	file.release();
  	return;	
}
void AKAZESupport::clear_loaded_models(){
	if(AKAZESupport::frame_models != NULL){
		AKAZESupport::frame_models->clear();
		delete AKAZESupport::frame_models;
		AKAZESupport::frame_models = NULL;
	}
	if(AKAZESupport::frame_keypoints != NULL){
		AKAZESupport::frame_keypoints->clear();
		delete AKAZESupport::frame_keypoints;
		AKAZESupport::frame_keypoints = NULL;
	}
	if(AKAZESupport::model_sizes != NULL){
		AKAZESupport::model_sizes->clear();
		delete AKAZESupport::model_sizes;
		AKAZESupport::model_sizes = NULL;
	}
	AKAZESupport::num_frame_models = 0;
}
void AKAZESupport::load_models_from_files(std::vector<std::string> * class_names, std::vector<std::string> * model_names, bool extended){
	fprintf(stderr, "num of names: %i\n", model_names->size());

	AKAZESupport::frame_models = new std::vector< cv::Mat * >();
	AKAZESupport::frame_keypoints = new std::vector< std::vector < cv::KeyPoint > * >();
	AKAZESupport::model_sizes = new std::vector< CAPISize >();
	////////////////////////////////////////////////////////////////////
	//////////////FOR SPECIFIC DEMO ONLY////////////////////////////////
	////////////////////////////////////////////////////////////////////
	/*
	 * Load in rois from xml files
	 */
	/*
	OpenCVSurfSupport::rois = ROIParser::CreateFromFile(std::string("/home/mdl/shared/Wegmans-145.xml"));
	std::vector<RegionOfInterest *> xd =OpenCVSurfSupport::rois->GetROI();
	for(int k = 0; k < xd.size(); ++k){
		xd[k]->SetScale(0.2078,0.2353);
	}
	*/
	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////
	std::string basePath = std::string("/home/mdl/shared/vision/models/akaze");
	std::string filePath;
	if(extended){
		filePath = std::string("/extended/");// + className + std::string("/");
	}else{
		filePath = std::string("/regular/");// + className + std::string("/");
	}
	//std::string fileName = "wegmans.bin";//modelName + std::string(".bin");
	for(int i = 0; i<model_names->size(); ++i){
		std::string fileName = (*model_names)[i] + std::string(".bin");
		std::string fullPath = basePath + filePath + fileName;
		cv::FileStorage fs(fullPath, cv::FileStorage::READ);
		cv::Mat * img = new cv::Mat();
		fs["descriptors"] >> *img;
		CAPISize s;
		s.width = (int)fs["width"];
		s.height = (int)fs["height"];
		AKAZESupport::model_sizes->push_back(s);
		AKAZESupport::frame_models->push_back(img);
		std::vector < cv::KeyPoint > * kpts = new std::vector < cv::KeyPoint >();
		cv::read(fs["keypoints"], *kpts);
		fprintf(stderr,"Number of loaded keypoints %i\n", kpts->size());
		AKAZESupport::frame_keypoints->push_back(kpts);
		AKAZESupport::num_frame_models++;
		fs.release();
	}
}
AKAZEKeyPointMatch * AKAZESupport::opencv_to_akazekeypointmatch(std::vector < std::vector< cv::DMatch > > matches1, std::vector< cv::KeyPoint > keypoints, std::vector< std::vector< cv::KeyPoint > >  model_keypoints, int  * num){
	float * score  = (float *)malloc(keypoints.size() * sizeof(float));
	*num = 0;
	for(int k = 0; k<matches1.size(); k++){
		for(int l = 0; l < matches1[k].size(); l++){
			*num++;
		}
	}
	AKAZEKeyPointMatch * res = (AKAZEKeyPointMatch *)malloc(*num * sizeof(AKAZEKeyPointMatch));
	for(int j = 0; j<matches1.size(); j++){
		std::vector< cv::DMatch > matches2 = matches1[j];
		for(int i =0; i<matches2.size(); i++){
			cv::DMatch match = matches2[i];
			uint32_t model_number = match.imgIdx;
			score[model_number] += match.distance;
			cv::KeyPoint model_keypoint = model_keypoints[model_number][match.trainIdx];
			cv::KeyPoint match_keypoint = keypoints[match.queryIdx];
			res[i].model_id = model_number;
			res[i].model_x = (int)(model_keypoint.pt.x);
			res[i].model_y = (int)(model_keypoint.pt.y);
			res[i].model_size.rawValue = model_keypoint.size;
			res[i].model_angle.rawValue = model_keypoint.angle;
			res[i].frame_x = (int)(match_keypoint.pt.x);
			res[i].frame_y = (int)(match_keypoint.pt.y);
			res[i].frame_size.rawValue = match_keypoint.size;
			res[i].frame_angle.rawValue = match_keypoint.angle;
			res[i].model_score.rawValue = score[model_number];
		}
	}
	return res;
}
