#ifndef __OPENCVSURFFULLDEMOWORKITEM_HPP__
#define __OPENCVSURFFULLDEMOWORKITEM_HPP__



#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <stdint.h>

#include "opencv2/gpu/gpu.hpp"
#include "opencv2/nonfree/gpu.hpp"

#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"


#include "WorkItem/WorkItem.hpp"

class OpenCVSurfFullDemoWorkItem : WorkItem {
    
public:

	std::vector<float> descriptors_vector;
    cv::Mat image;
    std::vector<cv::KeyPoint> keypoints;
    std::vector<cv::KeyPoint> model_keypoints;
    cv::Mat model_descriptors_matrix;
    cv::Mat descriptors_matrix;

	std::vector<std::vector<cv::DMatch>> matches;
	cv::Mat mask;
	bool is_cpu;
	bool model_is_cpu;
/*
    cv::gpu::GpuMat gpuImage;
    std::vector<cv::KeyPoint> gpu_keypoints;
    cv::gpu::GpuMat gpu_descriptors;
    std::vector<float> descriptors_vector;
*/
};

#endif
