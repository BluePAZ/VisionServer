#ifndef __OPENCVSURF_SUPPORT_H__
#define __OPENCVSURF_SUPPORT_H__

//#define _GLIBCXX_USE_CXX11_ABI 0

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <vector>
#include <string.h>


/* for opencv rect not sure which one is needed */
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"

#include "ROIParser.h"
#include "../OpenCVSurfFullDemoWorkItem/OpenCVSurfFullDemoWorkItem.hpp"

#include "WorkManagerDispatcher/WorkManagerDispatcher.h"

#define MIN_HESSIAN 700

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
typedef struct SURFKeyPointLite{
	CAPIFloat scale;
	CAPIFloat orientation;
	uint32_t laplacian;
	uint32_t r1;
	uint32_t c1;
	uint32_t r2;
	uint32_t c2;
}SURFKeyPointLite;
typedef struct ROIRegion{
	uint32_t x1;
	uint32_t x2;
	uint32_t x3;
	uint32_t x0;
	uint32_t y1;
	uint32_t y2;
	uint32_t y3;
	uint32_t y0;
}ROIRegion;

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
typedef struct HomographyMatches{
	std::vector<cv::Point2f> * model;
	std::vector<cv::Point2f> * frame;
	cv::Mat * homographyMatrix;
	CAPISize model_size;
	uint32_t score;
}HomographyMatches;
class OpenCVSurfSupport{
    public:
        static WorkManagerDispatcher * dispatcher;
        static int setWorkDispatcher(WorkManagerDispatcher *d);
        static int checkForWorkDispatcher();
        static int openCVSurfDetect(const cv::Mat &m, std::vector<cv::KeyPoint> &k);
        static int openCVSurfCompute(const cv::Mat &m, std::vector<cv::KeyPoint> &k, cv::Mat &d);
        static int openCVSurfDetectAndCompute(const cv::Mat &m, std::vector<cv::KeyPoint> &k, cv::Mat &desc);
        static int openCVSurfKnnMatch(const cv::Mat& queryDescriptors,std::vector<std::vector<cv::DMatch> >& matches,
                   int k, const std::vector<cv::Mat>& masks=std::vector<cv::Mat>(), bool compactResult=false);

		static std::vector<CAPISize> * model_sizes;
    	static std::vector<cv::Mat *> * frame_models;
    	static std::vector<cv::Mat *> * subframe_models;
    	static std::vector< std::vector < cv::KeyPoint > * > * frame_keypoints;
    	static std::vector< std::vector < cv::KeyPoint > * > * subframe_keypoints;
    	static int num_frame_models;
    	static int num_subframe_models;
    	static ROIParser * rois;
    	OpenCVSurfSupport(){};
		~OpenCVSurfSupport(){};
        static SURFKeyPointLite * opencv_to_surfkeypointlite(std::vector< cv::KeyPoint > keypoints);
        static uint32_t * roiregion_to_byte(ROIRegion * regions, int num, int * length);
        static uint32_t * keypointslite_to_byte(SURFKeyPointLite * keyPoints, int num, int * length);
        static SURFKeyPointMatch * opencv_to_surfkeypointmatch(std::vector < std::vector< cv::DMatch > > matches1, std::vector< cv::KeyPoint > keypoints, std::vector< std::vector< cv::KeyPoint > >  model_keypoints, int  * num);
        static void load_models_from_files(std::vector<std::string> * classes, std::vector<std::string> * models, bool extended);
        static void clear_loaded_models();
        static void write_model_to_file(int width, int height, cv::Mat mat,  std::vector < cv::KeyPoint > * keypoints, std::string className, std::string modelName, bool extended);
        static cv::Mat * computeHomography(std::vector<cv::Point2f> * model, std::vector<cv::Point2f> * unknown);

};
#endif
