/*
 * SURFWorkConsumer.cpp
 *
 *  Created on: Nov 28, 2015
 *      Author: peterazientara
 */

#include "../SURFIPWorkConsumer/SURFIPWorkConsumer.h"

SURFIPWorkConsumer::SURFIPWorkConsumer() {
	// TODO Auto-generated constructor stub

}
SURFIPWorkConsumer::~SURFIPWorkConsumer() {
	// TODO Auto-generated destructor stub
}

void SURFIPWorkConsumer::init(){
	//_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	currentWidth = 1920;
	currentHeight = 1080;
	currentDepth = 1;
	currentThreshold = 13107200;
	//IVortexNIFSAP* AcceleratorInterface = new nifsap_logger_v1_00_a(true,0,"config.dat");
	IVortexNIFSAP* AcceleratorInterface = new nifsap_capi_v2_00_b(false,0,"",7,0,0);
	if (!AcceleratorInterface->Initialize(0))
	{
		fprintf(stderr, "Failed to init accelerator\n");
		delete(AcceleratorInterface);
		return;
	}
	AcceleratorInterface->AddMemoryInterface(0,0,0);

	SURFIPEngine = new surf_ip_detector_v1_00_b("SURF_IP",0,0,1,AcceleratorInterface);
	SURFIPEngine->AcceleratorAddNotificationTarget(0,0,0);
	SURFIPEngine->AcceleratorSetImageConfiguration(
		currentWidth,
		currentHeight,
		IMAGE_DESCRIPTOR_FORMAT_COLOR::GRAY,
		IMAGE_DESCRIPTOR_FORMAT_DEPTH::BYTE,
		IMAGE_DESCRIPTOR_TYPE::RAW,
		currentThreshold);
	ImageHandle	= AcceleratorInterface->AllocateMemory(currentWidth * currentHeight * currentDepth);
}
void SURFIPWorkConsumer::runWork(WorkItem * w){
	//TODO:: implement man logic
	parseParams((uint8_t *)w->params, w->params_length);
	memcpy((void *)ImageHandle->GetMemoryOffset(), (const void *)w->data, (unsigned long long)w->data_length);
	#warning -- change Proccess to take output arr as param

	fprintf(stderr, "Starting Process Configure\n");

    SURFIPEngine->ProcessConfigure(ImageHandle,currentWidth,currentHeight,currentThreshold);
	fprintf(stderr, "Starting Process\n");
	auto t1 = std::chrono::system_clock::now();
	w->result = SURFIPEngine->Process(ImageHandle,currentWidth,currentHeight, currentThreshold);
	auto t2 = std::chrono::system_clock::now();
		    	std::cout << "Surf IP: "
		    	  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
		    	  << " milliseconds" << std::endl;
	//AcceleratorInterface->DeallocateMemory(ImageHandle);
}
void SURFIPWorkConsumer::deInit(){
	//TODO:: implement cleanup
	AcceleratorInterface->DeallocateMemory(ImageHandle);
	delete(SURFIPEngine);
	delete(AcceleratorInterface);
}
void SURFIPWorkConsumer::parseParams(uint8_t * params, uint32_t paramsLength){
	if(paramsLength > 16){
		newWidth = (int)((uint32_t)((((((params[3] << 8) | params[2]) << 8) | params[1]) << 8) | params[0]));
		newHeight = (int)((uint32_t)((((((params[7] << 8) | params[6]) << 8) | params[5]) << 8) | params[4]));
		newDepth = (int)((uint32_t)((((((params[11] << 8) | params[10]) << 8) | params[9]) << 8) | params[8]));
		newThreshold = (int)((uint32_t)((((((params[15] << 8) | params[14]) << 8) | params[13]) << 8) | params[12]));
	}
	configure();
}
void SURFIPWorkConsumer::configure(){
	if((newWidth != currentWidth) || (newHeight != currentHeight) || (newDepth != currentDepth)){
		currentWidth = newWidth;
		currentHeight = newHeight;
		currentDepth = newDepth;
		dataSize = currentWidth * currentHeight * currentDepth;
		AcceleratorInterface->DeallocateMemory(ImageHandle);
		ImageHandle = AcceleratorInterface->AllocateMemory(dataSize);
	}
}
hardware_ip_point * decode_keypoints(uint8_t * byteData, uint32_t numKeypoints){

	hardware_ip_point * points = (hardware_ip_point *)malloc(numKeypoints * sizeof(hardware_ip_point));
	for(int i = 0; i < numKeypoints; ++i){
		uint32_t data = ((uint32_t *)byteData)[i];
		uint32_t x = (data & 0x000000FF);
		x |= (data & 0x00000700);
		points[i].x = x;
		uint32_t y = ((data & 0x0000F800) >> 11);
        y |= ((data & 0x003F0000) >> 11);
		points[i].y = y;
		uint32_t scale = (data & 0x00C00000) >> (22);
        scale |= (data & 0x07000000) >> (22);
		points[i].scale = scale;
		uint32_t lapacian = (data & 0x08000000) >> (27);
		points[i].lapacian = lapacian;
	}
	return points;
}

