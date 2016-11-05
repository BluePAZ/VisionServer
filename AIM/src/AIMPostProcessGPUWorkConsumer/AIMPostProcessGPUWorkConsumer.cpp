/*
 * AIMPostProcessWorkConsumer.cpp
 *
 *  Created on: Apr 6, 2016
 *      Author: root
 */
#include "AIMPostProcessGPUWorkConsumer.h"

AIMPostProcessGPUWorkConsumer::AIMPostProcessGPUWorkConsumer() {
	// TODO Auto-generated constructor stub
}
AIMPostProcessGPUWorkConsumer::~AIMPostProcessGPUWorkConsumer() {
	// TODO Auto-generated destructor stub
}
void AIMPostProcessGPUWorkConsumer::init(){
}
void AIMPostProcessGPUWorkConsumer::runWork(WorkItem * w){
	parseParams((uint8_t *)w->params, w->params_length);
	uint32_t len = w->result_length;
	auto ta = std::chrono::system_clock::now();
	uint8_t * data = postProcess((uint8_t *)w->result, &len);
	auto tb = std::chrono::system_clock::now();
									std::cout << "AIM POST PROCESS GPU TIME: "
									  << std::chrono::duration_cast<std::chrono::milliseconds>(tb - ta).count()
									  << " milliseconds" << std::endl;
	w->time_ms_2 = std::chrono::duration_cast<std::chrono::milliseconds>(tb - ta).count();
	//avoid unnecessary copy
	w->setResults(data, len, false, true, false, true);
}
void AIMPostProcessGPUWorkConsumer::deInit(){

}
void AIMPostProcessGPUWorkConsumer::parseParams(uint8_t * params, uint32_t paramsLength){
	if(paramsLength >= 12){
		newWidth = (int)((uint32_t)((((((params[3] << 8) | params[2]) << 8) | params[1]) << 8) | params[0]));
		newHeight = (int)((uint32_t)((((((params[7] << 8) | params[6]) << 8) | params[5]) << 8) | params[4]));
		newDepth = (int)((uint32_t)((((((params[11] << 8) | params[10]) << 8) | params[9]) << 8) | params[8]));
	}
	configure();
}
void AIMPostProcessGPUWorkConsumer::configure(){
	if((newWidth != currentWidth) || (newHeight != currentHeight) || (newDepth != currentDepth)){
		currentWidth = newWidth;
		currentHeight = newHeight;
		currentDepth = newDepth;
		dataSize = currentWidth * currentHeight * currentDepth;
	}
}
uint8_t * AIMPostProcessGPUWorkConsumer::postProcess(uint8_t * rawResult, uint32_t * length){
	cv::Mat in_image = cv::Mat(currentHeight, currentWidth, CV_32SC1,  (void*)rawResult);
	cv::Mat infoMap;
	//cv::imshow("in_image", in_image);
	//cv::waitKey(5);
	in_image.convertTo(infoMap, CV_32FC1, 1.0 / (float)pow(2.0, 17), 0);
	// *************************************************
		// Post-Processing of Information Map
		// *************************************************
	/*
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
		cv::Mat threshMap;
		cv::compare(infoMap, dispRank, threshMap, cv::CMP_GT);
		threshMap.convertTo(threshMap, CV_32FC1, 1.0 / 255.0, 0);
		*/
	// Normalize
	cv::Mat threshMap;
	cv::normalize(infoMap, infoMap, 0, 65535, cv::NORM_MINMAX);
	cv::Mat blob_grey = GetSalienceMask(infoMap, 0.83, 1, 2);
	//cv::Mat grey_image;

	std::vector<std::vector<cv::Point> > contours; // Vector for storing contour
	std::vector<cv::Vec4i> hierarchy;
	//cv::threshold(threshMap, threshMap, 0, 1, cv::THRESH_BINARY);
	//cvtColor(blob_grey, blob_grey, COLOR_BGR2GRAY);
	//threshMap.convertTo(blob_grey, CV_8UC1);
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
		//if ((area <= 10000.0f) && (area >= 50.0f))
		//{
			cv::Rect boundingBox = cv::boundingRect(contours[k]);
			if(boundingBox.width > 16 && boundingBox.height > 16 && boundingBox.width <= 64 && boundingBox.height <= 64){
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
		//}
	}
	*length = actualSize * 16;
	uint8_t * resultLocation2 = (uint8_t *)malloc(*length);
	memcpy(resultLocation2, resultLocation, *length);
	free(resultLocation);
	return resultLocation2;
}
cv::Mat AIMPostProcessGPUWorkConsumer::GetSalienceMask(const cv::Mat &infomap, double thresholdPercent, int ErosionSize, int ClosureSize){
	cv::Mat infoMapLine = infomap.clone();
	cv::Mat infoMapVector = infoMapLine.reshape(1,infomap.rows * infomap.cols);
	double rank = calcPercentile(infoMapVector, thresholdPercent);
	//double rank = calculatePercentile(infomap, thresholdPercent);
	 int Height = infomap.rows;
	 int Width = infomap.cols;
	 binaryMask = cv::Mat(Height, Width, CV_8UC1);
	 uint8_t * binaryMaskData = binaryMask.data;

#pragma omp parallel for
	 for (int y = 0; y < Height; y++)
		 for (int x = 0; x < Width; x++)
			 binaryMaskData[(y * Width) + x] = (uint8_t)(((((float*)(infomap.data))[(y * Width) + x]) < rank) ? 0 : 1);
	 //Image<Gray, Byte> BinaryMask = new Image<Gray, Byte>(Threshmap.Bitmap.Width, Threshmap.Bitmap.Height);
	 //m_BinaryMask = m_BinaryMask.SmoothGaussian(11);
	 cv::gpu::GpuMat gpuBinaryMask;
	 gpuBinaryMask.upload(binaryMask);
	 cv::gpu::blur(gpuBinaryMask,gpuBinaryMask,cv::Size(11, 11));
	 cv::gpu::erode(gpuBinaryMask,gpuBinaryMask, cv::Mat());
	 cv::gpu::dilate(gpuBinaryMask,gpuBinaryMask,cv::Mat());
	 cv::gpu::erode(gpuBinaryMask,gpuBinaryMask, cv::Mat());
	 /*
	 cv::GaussianBlur(binaryMask, binaryMask, {11, 11}, 1.0);
	 cv::erode(binaryMask,binaryMask,ClosureSize);
	 cv::dilate(binaryMask, binaryMask, ClosureSize*2);
	 cv::erode(binaryMask,binaryMask,ErosionSize);
	 */
	 //InputImage.Save("stanford_original_frame.jpg");
	 //Mask.Save("stanford_threshold_frame.jpg");
	 // Close the image mask
	 //m_BinaryMask = m_BinaryMask.Erode(ClosureSize).Dilate(ClosureSize * 2);
	 //Mask.Save(String.Format("closemap.jpg"));
	 // Apply post-closure erosion
	 //m_BinaryMask = m_BinaryMask.Erode(ErosionSize);
	 //Mask.Save("stanford_eroded_frame.jpg");
	 //Mask.Save(String.Format("erodemap.jpg"));
	 gpuBinaryMask.download(binaryMask);
	 return binaryMask;
}
double AIMPostProcessGPUWorkConsumer::calculatePercentile(const cv::Mat &infomap, double percentile){
#pragma omp parallel for
	for(int i = 0; i < 65535; ++i){
		percentileHistogram[i] = 0;
	}
	int NumPoints = infomap.rows * infomap.cols * 1;
#pragma omp parallel for
	for(int i = 0; i<NumPoints; ++i){
		int tmp = ((int)(infomap.data)[i]);
		//fprintf(stderr, "index: %i\n", tmp);
		percentileHistogram[tmp]++;
	}
	int sum = 0;
	int percentileInt = (int)(percentile*(double)NumPoints);
	for(int i=0; i<NumPoints;++i)
	{
		sum += percentileHistogram[i];
		if (sum >= percentileInt)
			return (double)i;
	}
	return (double)(NumPoints - 1);
}
double AIMPostProcessGPUWorkConsumer::calcPercentile(const cv::Mat &sequence, double percentile)
{
	// Get the length of infomap
	double nRows = sequence.rows;
	// Sort the sequence
	cv::sort(sequence, sequence, CV_SORT_EVERY_COLUMN + CV_SORT_ASCENDING);
	// Calculate Rank (To check for extremes i.e. 0 and 1)
	double nRankExtremes = (nRows - 1) * percentile + 1;
	// Calculate Rank (For linear interpolation)
	double nRankLinear = (nRows*percentile) + 0.5;

	if (nRankExtremes == 1)
	{
		return sequence.at<float>(0);
	}
	else if (nRankExtremes == nRows)
	{
		return sequence.at<float>(nRows - 1);
	}
	else  // Linear Interpolation
	{
		int k = static_cast<int>(nRankExtremes);// round down to nearest integer
		double d = nRankLinear - k;
		return sequence.at<float>(k - 1) + d * (sequence.at<float>(k) - sequence.at<float>(k - 1));
	}
}

