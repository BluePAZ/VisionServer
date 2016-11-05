#include "capi_visual_lib_v1_00.h"

cv::BFMatcher bfMatcher;
cv::FlannBasedMatcher flannMatcher;
std::vector< std::vector< cv::KeyPoint > >  model_keypoints;
std::vector< cv::Mat > model_descriptors;	
int opensurf_model_count;

 pthread_t match_threads[MODEL_COUNT];

cv::Rect roi_to_cv_rect(CAPIRoi roi){
	return cv::Rect(roi.origin.x, roi.origin.y, roi.size.width, roi.size.height);
}
cv::Rect roi_to_cv_rect_scale(CAPIRoi roi, float scale_factor){
	return cv::Rect(roi.origin.x * scale_factor, roi.origin.y * scale_factor, roi.size.width * scale_factor, roi.size.height * scale_factor);
}
CAPIRoi cv_rect_to_roi(cv::Rect rect){
	CAPIRoi roi;
	roi.origin.x = rect.x; 
	roi.origin.y = rect.y;
	roi.size.width = rect.width;
	roi.size.height = rect.height;
	return roi;
}
typedef struct match_thread_args{
	cv::Mat * models;
	cv::Mat * descriptors;
	std::vector < cv::KeyPoint > * keypoints;
	SURFKeyPointMatch ** results;
	int * result_length;
	int thread_id;
}match_thread_args;
void * find_matches_thread(void * matches_args){
	std::vector< cv::DMatch > matches;
	cv::Mat descriptors = *(((match_thread_args *)matches_args)->descriptors);
	cv::Mat model = *(((match_thread_args *)matches_args)->models);
	std::vector < cv::KeyPoint > keypoints = *(((match_thread_args *)matches_args)->keypoints);
	
	//place to store the output
	SURFKeyPointMatch ** results = ((match_thread_args *)matches_args)->results;
	int * length = ((match_thread_args *)matches_args)->result_length;
	int id = ((match_thread_args *)matches_args)->thread_id;

	fprintf(stdout, "pthreads id:%i started\n", id);
	int score = 0;
	
	fprintf(stdout, "created matcher\n");
	cv::BFMatcher mchr(cv::NORM_L2, true);
	//mchr.knnMatch( descriptors, model, matches, 2, masks, true);
	mchr.match( descriptors, model, matches);
	fprintf(stdout, "ran matcher\n");
	
	*length = matches.size();
	SURFKeyPointMatch * res = (SURFKeyPointMatch *)malloc(*length * sizeof(SURFKeyPointMatch));
	fprintf(stdout, "pthread %i, num matchers: %i\n", id, *length);
	for(int i=0; i<matches.size(); i++){
			cv::DMatch match = matches[i];
			score += match.distance;
			cv::KeyPoint model_keypoint = model_keypoints[id][match.trainIdx];
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
	*results = res;
	fprintf(stdout, "exiting pthread %i\n", id);
	pthread_exit(0);
}
SURFKeyPointMatch * opencv_to_surfkeypointmatch_thread_agg(int * result_lengths, SURFKeyPointMatch ** matches, int * total_length){
	fprintf(stdout, "aggregating thread results\n");
	*total_length = 0;
	for(int i = 0; i<MODEL_COUNT; i++){
		*total_length += result_lengths[i];
	}
	fprintf(stderr, "aggregating thread results1\n");
	int currentIdx = 0;
	SURFKeyPointMatch * data = (SURFKeyPointMatch *)malloc(*total_length * sizeof(SURFKeyPointMatch));
	fprintf(stderr, "total length is  %i aggregating thread results1\n", *total_length);
	for(int i = 0; i<MODEL_COUNT; i++){
		fprintf(stderr, "aggregating thread results2 %i\n", i);
		fprintf(stderr, "matches_length[%i] = %i\n", i, result_lengths[i]);
		memcpy( &(data[currentIdx]),  matches[i], result_lengths[i] * sizeof(SURFKeyPointMatch));
		fprintf(stderr, "aggregating thread results2 %i\n", i);
		currentIdx += result_lengths[i];
		fprintf(stderr, "aggregating thread results3 %i\n", i);
		free(matches[i]);
		fprintf(stderr, "aggregating thread results4 %i\n", i);
	}
	fprintf(stdout, "done aggregating thread results\n");
	return data;
}
uint8_t * runSURF(cv::Mat mat, int * length, bool opencv){
	fprintf(stdout, "running surf\n");
	uint8_t * data = NULL;
	if(opencv){
		fprintf(stdout, "running surf\n");
		//Convert Mat to grey
		cv::Mat greymat;
		cv::cvtColor(mat, greymat, CV_BGR2GRAY);
		
		cv::SURF * surf = new cv::SURF(500, 5, 2, false, false);
		std::vector< cv::KeyPoint > keypoints;
		cv::Mat descriptors;
		//extract for unknown
		fprintf(stdout, "calling opencv surf\n");
		(*surf)(greymat, cv::noArray(), keypoints, descriptors, false);
		/////////////////////
		//Threaded matching//
		/////////////////////
		fprintf(stdout, "threading matching\n");
		int thread_result_length[MODEL_COUNT];
		SURFKeyPointMatch * thread_matches_result[MODEL_COUNT];		
		
		match_thread_args thread_args[MODEL_COUNT];
		for(int i = 0; i<MODEL_COUNT; i++){
			thread_args[i].descriptors = &descriptors;
			thread_args[i].models = (&(model_descriptors[i]));
			thread_args[i].results = &thread_matches_result[i];
			thread_args[i].keypoints = &keypoints;
			thread_args[i].result_length = &thread_result_length[i];
			thread_args[i].thread_id = i;
			pthread_create(&match_threads[i], NULL, find_matches_thread, (void *)(&(thread_args[i])));
		}
		fprintf(stdout, "pthreads created\n");
		for(int i = 0; i<MODEL_COUNT; i++){
			pthread_join(match_threads[i], NULL);
		}
		fprintf(stderr, "PTHREADS HAVE FINSIHED---------++++=++++++++++\n");
		*length = 0;
		SURFKeyPointMatch * keypointmatchstruct_thread = opencv_to_surfkeypointmatch_thread_agg(&thread_result_length[0], &thread_matches_result[0], length);		
		data = (uint8_t *)keypointsmatches_to_byte(keypointmatchstruct_thread, length);
		///////////////////////////////
		/*
		std::vector < std::vector< cv::DMatch > > matches;
		std::vector< cv::Mat > masks;
	    bfMatcher.knnMatch( descriptors, matches, 2, masks, true);
		flannMatcher.knnMatch( descriptors, matches, 2, masks, true);
		
		*length = 0;
		SURFKeyPointMatch * keypointmatchstruct = opencv_to_surfkeypointmatch(matches, keypoints, model_keypoints, length);
		data = (uint8_t *)keypointsmatches_to_byte(keypointmatchstruct, length);
		*/
	}else{
		IplImage copy = mat;
		IplImage* new_image = &copy;
		IpVec ipts;
		surfDetDes(new_image, ipts, false, 5, 4, 2, 0.0004f);
		//ABOVE THIS LINE USED TO WORK
		KeyFeature * unknownPoints = keyFeatureFromIpVec(ipts);
		std::vector<KeyFeatureModel *> * models = new std::vector<KeyFeatureModel *>();
	    BruteKeypointMatcher matcher;
		std::vector<int> * files = new std::vector<int>();
		//#wsssarning change this
		for(int i = 0; i<12; i++){
			files->push_back(i);
		}
		matcher.load_models_from_files(files);
		fprintf(stdout, "# of Models: %i\n", matcher.models->size());
		matcher.write_model_to_file(ipts, std::string("SUPERCLASS"), std::string("SUBCLASS"));
		KeyFeatureModel *mod = new KeyFeatureModel();
		mod->descriptors = unknownPoints;
		models->push_back(mod);
		std::vector<ClassifierMatch *> * matches = matcher.runModels(models, unknownPoints);
		fprintf(stdout, "Matches Count: %i\n", matches->size());
		SURFKeyPointMatch * structs = opensurf_to_surfkeypointmatch(matches, length);
		data = (uint8_t *)keypointsmatches_to_byte(structs, length);
	}
	return data;
}
void models_finished(){
	bfMatcher.add(model_descriptors);
	flannMatcher.add(model_descriptors);
	flannMatcher.train();
}
uint8_t * extractModels(cv::Mat mat, int * length, bool opencv){
	fprintf(stdout, "extract models\n");
	uint8_t * data = NULL;
	if(opencv){	
		//TODO: Convert Mat to grey
		cv::Mat greymat;
		cv::cvtColor(mat, greymat, CV_BGR2GRAY);

		std::vector< cv::KeyPoint > keypoints;
		cv::Mat descriptors;
		cv::SURF * surf = new cv::SURF(500, 5, 2, false, false);
		
		fprintf(stdout, "extracting surf\n");
		(*surf)(greymat, cv::noArray(), keypoints, descriptors, false);
		fprintf(stdout, "extracted surf model\n");

		model_keypoints.push_back(keypoints);
		model_descriptors.push_back(descriptors);
		fprintf(stdout, "models set\n");
		
		SURFKeyPoint * structs = opencv_to_surfkeypoint(keypoints, descriptors);
		data = (uint8_t *)keypoints_to_byte(structs, keypoints.size(), length);
		fprintf(stdout, "returning data::extract models\n");
	}else{
		IplImage copy = mat;
		IplImage* new_image = &copy;
		IpVec ipts;
		surfDetDes(new_image, ipts, false, 5, 4, 2, 0.0004f);
		SURFKeyPoint * keyPoints = opensurf_to_surfkeypoint(ipts);
		data = (uint8_t *)keypoints_to_byte(keyPoints, ipts.size(), length);
		//matcher.write_model_to_file(ipts, std::string("SUPERCLASS"), std::string(open_surf_model_count));
		//opensurf_model_count++;
	}
	return data;
}
KeyFeature *  keyFeatureFromIpVec(std::vector<Ipoint> &ipts){
	KeyFeature * ret = new KeyFeature();
	for(unsigned int i = 0; i < ipts.size(); i++){
		Ipoint val = ipts[i];
		KeyFeatureDescriptor * desc = new KeyFeatureDescriptor();
		KeyPoint * pt = new KeyPoint();
		//copy over descriptor
		for(int j = 0; j<64; ++j){
			desc->descriptor[j] = val.descriptor[j]; 
			//fprintf(stdout, "desc[0]: %f\n", desc->descriptor[j]);
		}
		pt->x = val.x;
		pt->y = val.y;
		pt->scale = val.scale;
		pt->laplacian = val.laplacian;
		pt->orientation = val.orientation;
		ret->descriptors->push_back(desc);
		ret->points->push_back(pt);
	}
	fprintf(stdout, "KF Size: %i\n", ret->descriptors->size());
	return ret;
}
SURFKeyPoint * opensurf_to_surfkeypoint(std::vector<Ipoint> &ipts){
	SURFKeyPoint * points = (SURFKeyPoint *)malloc(ipts.size() * sizeof(SURFKeyPoint));
	Ipoint *ipt;
	float s, o;
	int r1, c1, r2, c2, lap;
	for(unsigned int i = 0; i < ipts.size(); i++) 
	{
		ipt = &ipts.at(i);
		points[i].scale.rawValue = (2.5f * ipt->scale);
		points[i].orientation.rawValue = ipt->orientation;
		points[i].laplacian = ipt->laplacian;
		points[i].r1 = fRound(ipt->y);
		points[i].c1 = fRound(ipt->x);
		points[i].c2 = fRound(points[i].scale.rawValue * cos(points[i].orientation.rawValue)) + points[i].c1;
		points[i].r2 = fRound(points[i].scale.rawValue * sin(points[i].orientation.rawValue)) + points[i].r1;

		for(int j = 0; j<64; ++j){
			points[i].descriptor[j].rawValue = ipt->descriptor[j];
		}
	}
	return points;
}
SURFKeyPoint * opencv_to_surfkeypoint(std::vector< cv::KeyPoint > keypoints, cv::Mat descriptors){
	SURFKeyPoint * points = (SURFKeyPoint *)malloc(keypoints.size() * sizeof(SURFKeyPoint));
	for(unsigned int i = 0; i < keypoints.size(); i++) 
	{
		points[i].scale.rawValue = keypoints[i].size;
		points[i].orientation.rawValue = keypoints[i].angle;
		points[i].laplacian = keypoints[i].response;
		points[i].r1 = (int)(keypoints[i].pt.x);
		points[i].c1 = (int)(keypoints[i].pt.y);
		points[i].c2 = fRound(points[i].scale.rawValue * cos(points[i].orientation.rawValue)) + points[i].c1;
		points[i].r2 = fRound(points[i].scale.rawValue * sin(points[i].orientation.rawValue)) + points[i].r1;

		for(int j = 0; j<64; ++j){
			points[i].descriptor[j].rawValue = descriptors.at<float>(i,j);
		}
	}
	return points;
}
SURFKeyPointMatch * opensurf_to_surfkeypointmatch(std::vector<ClassifierMatch *> * matches, int * len){
	float * score  = (float *)malloc(model_keypoints.size() * sizeof(float));
	SURFKeyPointMatch * res = (SURFKeyPointMatch *)malloc(matches.size() * sizeof(SURFKeyPointMatch));
	for(int i =0; i<matches.size(); i++){
		cv::DMatch match = matches[i];
		uint32_t model_number = match.imgIdx;
		score[model_number] += match.distance;
		cv::KeyPoint model_keypoint = model_keypoints[model_number][match.trainIdx];
		cv::KeyPoint match_keypoint = keypoints[match.queryIdx];
		res[i].model_id = model_number;
		res[i].model_x = model_keypoint.pt.x;
		res[i].model_y = model_keypoint.pt.y;
		res[i].model_size.rawValue = model_keypoint.size;
		res[i].model_angle.rawValue = model_keypoint.angle;
		res[i].frame_x = match_keypoint.pt.x;
		res[i].frame_y = match_keypoint.pt.y;
		res[i].frame_size.rawValue = match_keypoint.size;
		res[i].frame_angle.rawValue = match_keypoint.angle;	
		res[i].model_score.rawValue = score[model_number];
	}
	return res;
	return NULL;
}
SURFKeyPointMatch * opencv_to_surfkeypointmatch(std::vector < std::vector< cv::DMatch > > matches1, std::vector< cv::KeyPoint > keypoints, std::vector< std::vector< cv::KeyPoint > >  model_keypoints, int  * num){
	float * score  = (float *)malloc(keypoints.size() * sizeof(float));
	*num = 0;
	for(int k = 0; k<matches1.size(); k++){
		for(int l = 0; l < matches1[k].size(); l++){
			*num++;
		}
	}
	SURFKeyPointMatch * res = (SURFKeyPointMatch *)malloc(*num * sizeof(SURFKeyPointMatch));
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
uint32_t * keypointsmatches_to_byte(SURFKeyPointMatch * matches, int *num){
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
uint32_t * keypoints_to_byte(SURFKeyPoint * keyPoints, int num, int * length){
	*length = num * 4 * (7 + 64);
	uint32_t * data = (uint32_t *)malloc(*length);
	for(int i = 0; i < num; ++i){
		int idx = i*(7+64);
		memcpy(&(data[idx + 0]), &(keyPoints[i].scale.bytes[0]), sizeof(float));
		memcpy(&(data[idx + 1]), &(keyPoints[i].orientation.bytes[0]), sizeof(float));
		data[idx + 2] = *((uint32_t *)(&(keyPoints[i].laplacian)));
		data[idx + 3] = *((uint32_t *)(&(keyPoints[i].r1)));
		data[idx + 4] = *((uint32_t *)(&(keyPoints[i].c1)));
		data[idx + 5] = *((uint32_t *)(&(keyPoints[i].r2)));
		data[idx + 6] = *((uint32_t *)(&(keyPoints[i].c2)));
		
		for(int j = 0; j<64;++j){
			memcpy(&(data[idx + 7+j]), &(keyPoints[i].descriptor[j].bytes[0]), sizeof(float)); 
		}
	}	
	free(keyPoints);

	return data;
}
void print_capi_size(CAPISize * size){
	fprintf(stdout, "/****CAPI Size Print****/\n");
	fprintf(stdout, "\tWidth: %u\n", size->width);
	fprintf(stdout, "\tHeight: %u\n", size->height);
}
void print_capi_point(CAPIPoint * point){
	fprintf(stdout, "/****CAPI Point Print****/\n");
	fprintf(stdout, "\tX: %u\n", point->x);
	fprintf(stdout, "\tY: %u\n", point->y);
}
void print_capi_roi(CAPIRoi * roi){
	fprintf(stdout, "ROI Origin\n");
	print_capi_point((CAPIPoint *)&(roi->origin));
	fprintf(stdout, "ROI Size:\n");
	print_capi_size((CAPISize *)&(roi->size));
}

#endif 
