/*
 * SURFWorkConsumer.cpp
 *
 *  Created on: Nov 28, 2015
 *      Author: peterazientara
 */

#include "OpenCVSurfBFFMatcherWorkConsumer.h"

OpenCVSurfBFFMatcherWorkConsumer::OpenCVSurfBFFMatcherWorkConsumer() {
	// TODO Auto-generated constructor stub
	match_threads = NULL;

}
OpenCVSurfBFFMatcherWorkConsumer::~OpenCVSurfBFFMatcherWorkConsumer() {
	// TODO Auto-generated destructor stub
}
void OpenCVSurfBFFMatcherWorkConsumer::init(){

}
void OpenCVSurfBFFMatcherWorkConsumer::runWork(WorkItem * w){
	//TODO:: implement man logic
	mode = 2;
	//if(OpenCVSurfSupport::num_frame_models != 0){
		//parseParams((uint8_t *)w->params, w->params_length);
		switch(mode){
			case 0:{ //match the frame
				int * thread_result_length = (int *)malloc( OpenCVSurfSupport::num_frame_models * sizeof(int));
				HomographyMatches ** thread_matches_result = (HomographyMatches **)malloc(sizeof(HomographyMatches *) * OpenCVSurfSupport::num_frame_models);
				match_thread_args * thread_args = (match_thread_args *)malloc(sizeof(match_thread_args) * OpenCVSurfSupport::num_frame_models);
				for(int i = 0; i<OpenCVSurfSupport::num_frame_models; i++){
					thread_args[i].unknown_descriptors = (cv::Mat *)w->data;
					thread_args[i].model_descriptors = (*OpenCVSurfSupport::frame_models)[i];
					thread_args[i].unknown_keypoints = (std::vector<cv::KeyPoint> *)w->result;
					thread_args[i].model_keypoints = (std::vector<cv::KeyPoint> *)(*OpenCVSurfSupport::frame_keypoints)[i];
					thread_args[i].results = thread_matches_result;
					thread_args[i].result_length = &thread_result_length[i];
					thread_args[i].thread_id = i;
					pthread_create(&match_threads[i], NULL, OpenCVSurfBFFMatcherWorkConsumer::find_matches_thread, (void *)(&(thread_args[i])));
				}
				for(int i = 0; i<OpenCVSurfSupport::num_frame_models; i++){
					pthread_join(match_threads[i], NULL);
				}
				w->setResults((uint8_t *)thread_matches_result, (uint32_t)OpenCVSurfSupport::num_frame_models, false, true, false, true);
				break;
			}
			case 1:{ //match region given
				break;
			}
			case 2:{ //match from demo pipeline
				OpenCVSurfFullWorkItem * wi =(OpenCVSurfFullWorkItem *)w;
				int * thread_result_length = (int *)malloc( 1 * sizeof(int));
				HomographyMatches ** thread_matches_result = (HomographyMatches **)malloc(sizeof(HomographyMatches *) * 1);
				match_thread_args * thread_args = (match_thread_args *)malloc(sizeof(match_thread_args) * 1);
				match_threads = (pthread_t *)malloc(sizeof(pthread_t) * 1);
				for(int i = 0; i<1; i++){
					cv::Mat * mat = new cv::Mat(((std::vector<cv::KeyPoint> *)w->result)->size(),64,CV_32FC1,&((std::vector<float> *)((WorkItem *)wi)->data)[0]);
					thread_args[i].unknown_descriptors = &wi->descriptors_matrix;//new cv::Mat(*((std::vector<float> *)w->data), true);
					thread_args[i].model_descriptors = &wi->model_descriptors_matrix;//new cv::Mat(*((std::vector<float> *)w->data), true);
					//thread_args[i].unknown_keypoints = (std::vector<cv::KeyPoint> *)w->result;
					//thread_args[i].model_keypoints = (std::vector<cv::KeyPoint> *)(*OpenCVSurfSupport::frame_keypoints)[i];
					thread_args[i].results = thread_matches_result;
					thread_args[i].result_length = &thread_result_length[i];
					thread_args[i].thread_id = i;
					pthread_create(&match_threads[i], NULL, OpenCVSurfBFFMatcherWorkConsumer::find_matches_thread, (void *)(&(thread_args[i])));
				}
				for(int i = 0; i<1; i++){
					pthread_join(match_threads[i], NULL);
				}
				w->setResults((uint8_t *)thread_matches_result, (uint32_t)OpenCVSurfSupport::num_frame_models, false, true, false, true);
				break;
			}
		}
	//}
}
	//w->result = (uint8_t *)keypointsmatches_to_byte(keypointmatchstruct_thread, (int *)(&(w->result_length)));
void OpenCVSurfBFFMatcherWorkConsumer::deInit(){
	//TODO:: implement cleanup
	if(match_threads != NULL){
		free(match_threads);
		match_threads = NULL;
	}
}
void OpenCVSurfBFFMatcherWorkConsumer::parseParams(uint8_t * params, uint32_t paramsLength){
	mode = paramsLength;
	configure();
}
void OpenCVSurfBFFMatcherWorkConsumer::configure(){
	update_match_threads();
}
void OpenCVSurfBFFMatcherWorkConsumer::update_match_threads(){
	if(match_threads != NULL){
		free(match_threads);
		match_threads = NULL;
	}
	match_threads = (pthread_t *)malloc(sizeof(pthread_t) * OpenCVSurfSupport::num_frame_models);
}
void * OpenCVSurfBFFMatcherWorkConsumer::find_matches_thread(void * matches_args){
	auto t1 = std::chrono::system_clock::now();
	std::vector<std::vector< cv::DMatch >> matches;
	//std::vector< cv::DMatch > matches;
	cv::Mat *descriptors = (((match_thread_args *)matches_args)->unknown_descriptors);
	cv::Mat *model = (((match_thread_args *)matches_args)->model_descriptors);
	std::vector < cv::KeyPoint > *unknown_keypoints = (((match_thread_args *)matches_args)->unknown_keypoints);
	std::vector < cv::KeyPoint > *model_keypoints = (((match_thread_args *)matches_args)->model_keypoints);
	//place to store the output
	HomographyMatches ** results = ((match_thread_args *)matches_args)->results;
	int * length = ((match_thread_args *)matches_args)->result_length;
	cv::BFMatcher mchr(cv::NORM_L2, false);
	cv::Mat masks;
	mchr.knnMatch( *descriptors, *model, matches, 2);
	*length = matches.size();
	std::vector< cv::DMatch > good_matches;
	 //-- Quick calculation of max and min distances between keypoints
	/*
	double max_dist = 0; double min_dist = 100;

		  //-- Quick calculation of max and min distances between keypoints
  for( int i = 0; i < (*descriptors).rows; i++ )
  { double dist = matches[i][0].distance;
	if( dist < min_dist ) min_dist = dist;
	if( dist > max_dist ) max_dist = dist;
  }

	for( int i = 0; i < *length; i++ )
	{
		double dist1 = matches[i][0].distance;
		cv::DMatch tmp1 = matches[i][0];
		cv::DMatch tmp2 = matches[i][1];
		double dist2 = matches[i][1].distance;
		double ratio = dist1 / dist2;
		if(ratio <= 0.8){
			//if( dist1 < 3*min_dist ){
			//}
			good_matches.push_back(tmp1);

		}

	}
	*/
	HomographyMatches * res = (HomographyMatches *)malloc(sizeof(HomographyMatches));

/*
	res->frame = new std::vector<cv::Point2f>();
	res->model = new std::vector<cv::Point2f>();
	res->model_size = (*OpenCVSurfSupport::model_sizes)[((match_thread_args *)matches_args)->thread_id];
	for(int i=0; i<good_matches.size(); i++){

			cv::DMatch match = good_matches[i];
			res->frame->push_back( (*unknown_keypoints)[match.queryIdx].pt);
		    res->model->push_back( (*model_keypoints)[match.trainIdx].pt);
			res->score += match.distance;
			/*
			cv::KeyPoint model_keypoint = keypoints[match.trainIdx];
			cv::KeyPoint match_keypoint = keypoints[match.queryIdx];
			res[i].model_id = id;
			res[i].model_x = (int)(model_keypoint.pt.x);
			res[i].model_y = (int)(model_keypoint.pt.y);
			res[i].model_size.rawValue = model_keypoint.size;
			res[i].model_angle.rawValue = model_keypoint.angle;
			res[i].frame_x = (int)(match_keypoint.pt.x);
			res[i].frame_y = (int)(match_keypoint.pt.y);
			res[i].frame_size.rawValue = match_keypoint.size;
			res[i].frame_angle.rawValue = match_keypoint.angle;
			res[i].model_score.rawValue = score;


	}
	*/
/*
	int model_len = model_keypoints->size();
	float pctMatch = (float)((float)good_matches.size()/(float)model_len);
	fprintf(stderr, "Number of model keypoints= %u, Number of good matches %i, Percent Match = %f \n", model_len, good_matches.size(), pctMatch);
	if(good_matches.size() > 40){
		res->homographyMatrix = OpenCVSurfSupport::computeHomography(res->model, res->frame);
	}else{
		res = NULL;
	}
	*/
	results[((match_thread_args *)matches_args)->thread_id] = res;
	auto t2 = std::chrono::system_clock::now();
			std::cout << "Surf MATCHING: "
			  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
			  << " milliseconds" << std::endl;
	return NULL;
}
SURFKeyPointMatch * OpenCVSurfBFFMatcherWorkConsumer::opencv_to_surfkeypointmatch_thread_agg(int * result_lengths, SURFKeyPointMatch ** matches, int * total_length){
	//fprintf(stdout, "aggregating thread results\n");
	/*
	*total_length = 0;
	for(int i = 0; i<MODEL_COUNT; i++){
		*total_length += result_lengths[i];
	}
	*/
	/*
	int currentIdx = 0;
	SURFKeyPointMatch * data = (SURFKeyPointMatch *)malloc(*total_length * sizeof(SURFKeyPointMatch));
	for(int i = 0; i<MODEL_COUNT; i++){
		memcpy( &(data[currentIdx]),  matches[i], result_lengths[i] * sizeof(SURFKeyPointMatch));
		currentIdx += result_lengths[i];
		free(matches[i]);
	}
	*/
	return NULL;
}
uint32_t * OpenCVSurfBFFMatcherWorkConsumer::keypointsmatches_to_byte(SURFKeyPointMatch * matches, int *num){
	int count = *num;
	*num = *num * 4 * (10);
	uint32_t * data = (uint32_t *)malloc(*num);
	for(int i = 0; i < count; ++i){
		int idx = i*(10);
		data[idx + 0] = *((uint32_t *)(&(matches[i].model_id)));
		data[idx + 1] = *((uint32_t *)(&(matches[i].model_x)));
		data[idx + 2] = *((uint32_t *)(&(matches[i].model_y)));
		memcpy(&(data[idx + 3]), &(matches[i].model_size.bytes[0]), sizeof(float));
		memcpy(&(data[idx + 4]), &(matches[i].model_angle.bytes[0]), sizeof(float));
		data[idx + 5] = *((uint32_t *)(&(matches[i].frame_x)));
		data[idx + 6] = *((uint32_t *)(&(matches[i].frame_y)));
		memcpy(&(data[idx + 7]), &(matches[i].frame_size.bytes[0]), sizeof(float));
		memcpy(&(data[idx + 8]), &(matches[i].frame_angle.bytes[0]), sizeof(float));
		memcpy(&(data[idx + 9]), &(matches[i].model_score.bytes[0]), sizeof(float));
	}
	free(matches);
	return data;
}


