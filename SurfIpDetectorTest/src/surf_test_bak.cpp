/*
 * surf_test.cpp
 *
 *  Created on: Dec 9, 2015
 *      Author: root
 */

#include "nif_capi_v2_00_b.h"
#include "surf_ip_detector_v1_00_b.h"
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

timespec diff(timespec start, timespec end);
int main(int argc, char* argv[])
{
	//_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	//int ImageWidth = 192;
	//int ImageHeight = 252;

	int ImageWidth = 1920;
	int ImageHeight = 1080;

	cv::Mat frame = cv::imread("/home/eng107/imagefiles/jpegs/Wegmans-21_1920x1080.jpg");

	std::vector<cv::KeyPoint>  keypoint_array;

	//IVortexNIFSAP* AcceleratorInterface = new nifsap_logger_v1_00_a(true,0,"config.dat");
	IVortexNIFSAP* AcceleratorInterface = new nifsap_capi_v2_00_b(false,0,"",7,0,0);
	if (!AcceleratorInterface->Initialize(0))
	{
        delete(AcceleratorInterface);
        return -1;
	}

	AcceleratorInterface->AddMemoryInterface(0,0,0);


	VortexMemoryAllocation* ImageHandle	= AcceleratorInterface->AllocateMemory(ImageWidth * ImageHeight * 4);

	surf_ip_detector_v1_00_b* SURFIPEngine = new surf_ip_detector_v1_00_b("SURF_IP",0,0,1,AcceleratorInterface);
	SURFIPEngine->AcceleratorAddNotificationTarget(0,0,0);
	SURFIPEngine->AcceleratorSetImageConfiguration(
		ImageWidth,
		ImageHeight,
		IMAGE_DESCRIPTOR_FORMAT_COLOR::GRAY,
		IMAGE_DESCRIPTOR_FORMAT_DEPTH::BYTE,
		IMAGE_DESCRIPTOR_TYPE::RAW,
		13107200);

	std::ifstream infile("/home/eng107/imagefiles/input_image_weg21_1920X1080_single_line.dat");
	//std::ifstream infile("/home/eng107/imagefiles/input_image_can_192_252_single_line.dat");
	int a;
	int i=0;
	int j = 0;
	while(infile >> hex >> a)
	{
		*((uint8_t*)(ImageHandle->GetMemoryOffset() + i++)) = a;
	}


// reading sample output
		/*std::ifstream sampleoutfile("/home/eng107/sample_dataout/KeyPoints_1920_1080_fpga_hw_uf_checked");
		uint32_t sample_keypoint;
		vector<uint32_t> expected_keypoint_array ;
		i=0;
		while(sampleoutfile >> sample_keypoint)
		{
			expected_keypoint_array.push_back(sample_keypoint);
			i = i+1;
		}*/

       //int expectedNoOfKeypoints =i;
	//uint8_t* KeypointsRaw = SURFIPEngine->Process(ImageHandle,ImageWidth,ImageHeight,13107200);

		std::ofstream outputKeyPointsFile;
		outputKeyPointsFile.open("/home/eng107/results/KeyPoints_1920_1080_regression.dat", std::ios::binary);

	//	outputKeyPointsFile << "Keypoints.\n";
	//	for (i = 0; i < 100 * 4; i++)
	//	{
	//		uint8_t data = *(KeypointsRaw + i);
	//		outputKeyPointsFile << " " << unsigned(data) << endl;
	//	}
	//	outputKeyPointsFile.close();


	 SURFIPEngine->ProcessConfigure(ImageHandle,ImageWidth,ImageHeight,13107200);


	//std::ofstream outputKeyPointsFile;
	//outputKeyPointsFile.open("KeyPoints.dat", std::ios_base::app);

	 int k;

	for (j = 0; j < 10 ; j++)
	{
		//SURFIPEngine->ProcessConfigure(ImageHandle,ImageWidth,ImageHeight,13107200);
		VortexMemoryAllocation* resultCountHandle	= AcceleratorInterface->AllocateMemory(128);
		timespec time1, time2;
		clock_gettime(CLOCK_REALTIME, &time1);
		uint8_t* KeypointsRaw = SURFIPEngine->Process(ImageHandle, ImageWidth, ImageHeight, 13107200);
		clock_gettime(CLOCK_REALTIME, &time2);
		cout<<diff(time1,time2).tv_sec<<":"<<diff(time1,time2).tv_nsec<<endl;
		uint32_t* KeyPointSingle = (uint32_t*)KeypointsRaw;



		//outputKeyPointsFile << "Keypoints.\n";
			uint32_t data;
			float x , y , scale , laplacian ;
			vector<uint32_t> actual_keypoint_array;
			for (i = 0; SURFIPEngine->m_NoOfKeypoints > i; i++)
			{
				data = *(KeyPointSingle);
				y= (float)((data & 0x000007FF));
				x= (float)((data & 0x003FF800) >> 11) ;
				scale= (float)((data & 0x07C00000) >> 22) ;
				laplacian= ((data & 0x08000000) >>27);

				cv::KeyPoint tempKeypoint =  cv::KeyPoint(x,y,scale);
				keypoint_array.push_back(tempKeypoint);

				KeyPointSingle++;
				//actual_keypoint_array.push_back(data);
				//outputKeyPointsFile << " " << data << endl;
				//std::cout << data << std::endl;
			}
			cv::Mat surfDisplay;
			cv::drawKeypoints(frame,keypoint_array,surfDisplay);
			cv::imshow("surfKeyPoints", surfDisplay);
			cv::waitKey(0);
			// checking logic
	/*		for (i = 0 ; i < SURFIPEngine->m_NoOfKeypoints ; i = i+ 1 )
			{
				bool match_not_found = true;
				k= 0;
				while(match_not_found && (k <= expectedNoOfKeypoints ))
				{
					if (expected_keypoint_array[k] == actual_keypoint_array[i])
					{
							match_not_found = false ;
							//expected_keypoint_array.erase(expected_keypoint_array.begin()+k);
					}
					else
					{
						k = k+1 ;
					}
				}
				if (match_not_found == true )
				{
					outputKeyPointsFile << "Actual data  " << actual_keypoint_array[i] << "not found in expected data"<< endl;

				}
			} */
			cout << " Done " << j << endl;
			outputKeyPointsFile << " Done " << j << endl;
	}


	outputKeyPointsFile.close();



	return 0;
}

timespec diff(timespec start, timespec end)
{
	timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}

