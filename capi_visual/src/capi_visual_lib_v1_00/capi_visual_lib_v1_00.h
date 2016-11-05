#ifndef __CAPI_VISUAL_LIB_v1_00_H__
#define __CAPI_VISUAL_LIB_v1_00_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <vector>
#include <pthread.h>
/* for opencv rect not sure which one is needed */
#include "opencv2/opencv.hpp" 
#include "opencv2/core/core.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"

#include "../../Common/open_surf/include/ipoint.h"
#include "../../Common/matching/include/BruteKeypointMatcher.h"
#include "../../Common/matching/include/KeyFeature.h"
#include "../../Common/matching/include/KeyFeatureModel.h"
#include "../../Common/matching/include/KeyFeatureDescriptor.h"
#include "../../Common/matching/include/ClassifierMatch.h"
#include "../../Common/open_surf/include/surflib.h"

#define MIN_ROI_SIZE 25.0f
#define MAX_ROI_SIZE 20000.0f

#define MODEL_COUNT 12

typedef uint32_t CAPIHeight;
typedef uint32_t CAPIWidth;

typedef struct CAPIPoint{
	uint32_t x;
	uint32_t y;
}CAPIPoint;
typedef struct CAPISize{
	CAPIHeight width;
	CAPIWidth height;
}CAPISize;
typedef struct CAPIRoi{
	CAPIPoint origin;
	CAPISize size;
}CAPIRoi;
typedef union CAPIFloat{
	float rawValue;
	uint8_t bytes[sizeof(float)];
}CAPIFloat;
typedef struct SURFKeyPoint{
	CAPIFloat scale;
	CAPIFloat orientation;
	uint32_t laplacian;
	uint32_t r1;
	uint32_t c1;
	uint32_t r2;
	uint32_t c2;
	CAPIFloat descriptor[64];
}SURFKeyPoint;
typedef struct SURFKeyPointMatch{
	uint32_t model_id;
	uint32_t model_x;
	uint32_t model_y;
	CAPIFloat model_size;
	CAPIFloat model_angle;
	uint32_t frame_x;
	uint32_t frame_y;
	CAPIFloat frame_size;
	CAPIFloat frame_angle;	
	CAPIFloat model_score;
}SURFKeyPointMatch;


/* utility functions */
cv::Rect roi_to_cv_rect_scale(CAPIRoi roi, float scale_factor);
cv::Rect roi_to_cv_rect(CAPIRoi roi);
CAPIRoi cv_rect_to_roi(cv::Rect rect);
uint8_t * runSURF(cv::Mat mat, int * length, bool use_opencv);
uint8_t * extractModels(cv::Mat mat, int * length, bool use_opencv);
void models_finished();
SURFKeyPoint * opensurf_to_surfkeypoint(std::vector<Ipoint> &ipts);
SURFKeyPoint * opencv_to_surfkeypoint(std::vector< cv::KeyPoint > keypoints, cv::Mat descriptors);
KeyFeature *  keyFeatureFromIpVec(std::vector<Ipoint> &ipts);
uint32_t * keypoints_to_byte(SURFKeyPoint * keyPoints, int num, int * length);
uint32_t * keypointsmatches_to_byte(SURFKeyPointMatch * matches, int *num);
SURFKeyPointMatch * opencv_to_surfkeypointmatch_thread_agg(int * result_lengths, SURFKeyPointMatch ** matches, int * total_length);
SURFKeyPointMatch * opencv_to_surfkeypointmatch(std::vector < std::vector< cv::DMatch > > matches, std::vector< cv::KeyPoint > keypoints, std::vector< std::vector< cv::KeyPoint > >  model_keypoints, int * count);
SURFKeyPointMatch * opensurf_to_surfkeypointmatch(std::vector<ClassifierMatch *> * matches, int * len);
void * find_matches_thread(void * image_server_thread_data_args);
/* print functions */
void print_capi_size(CAPISize * size);
void print_capi_point(CAPIPoint * point);
void print_capi_roi(CAPIRoi * roi);


#endif
