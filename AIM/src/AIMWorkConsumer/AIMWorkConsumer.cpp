#include "AIMWorkConsumer.hpp"

AIMWorkConsumer::AIMWorkConsumer(){

}
AIMWorkConsumer::AIMWorkConsumer(IVortexNIFSAP *accel){
	AcceleratorInterface = accel;
}

AIMWorkConsumer::~AIMWorkConsumer(){

}
void AIMWorkConsumer::init(){
	//IVortexNIFSAP* AcceleratorInterface = new nifsap_logger_v1_00_a(true,0,"config.dat");
	currentWidth = 800;
	currentHeight = 600;
	currentDepth = 4;
	//AcceleratorInterface = new nifsap_capi_v2_00_b(false,0,"",7,0,0);
	//if (!AcceleratorInterface->Initialize(0))
	//{
	//	delete(AcceleratorInterface);
	//}
	//AcceleratorInterface->AddMemoryInterface(0,0,0);
	AIMEngine = new aim_engine_v2_00_b("AIM", 0, 0, 1, AcceleratorInterface);
	AIMEngine->AcceleratorSetNumOrientations(6);
	//AIMEngine->AcceleratorAddKernel(13,13);
	//AIMEngine->AcceleratorAddKernel(11,11);
	AIMEngine->AcceleratorAddKernel(9,9);
	//AIMEngine->AcceleratorAddKernel(7,7);
	AIMEngine->AcceleratorAddNotificationTarget(0,0,0);
	AIMEngine->AcceleratorSetImageConfiguration(
			currentWidth,
			currentHeight,
			IMAGE_DESCRIPTOR_FORMAT_COLOR::BGRA,
			IMAGE_DESCRIPTOR_FORMAT_DEPTH::BYTE,
			IMAGE_DESCRIPTOR_TYPE::RAW);
	ImageHandle = AcceleratorInterface->AllocateMemory(currentWidth * currentHeight * currentDepth);
}
void AIMWorkConsumer::runWork(WorkItem * w){
	parseParams((uint8_t *)w->params, w->params_length);
	memcpy((void *)ImageHandle->GetMemoryOffset(), (const void *)w->data, (unsigned long long)w->data_length);
	#warning -- change Proccess to take output arr as param
	auto ta = std::chrono::system_clock::now();
	uint8_t * res = AIMEngine->Process(ImageHandle,currentWidth,currentHeight);
	auto tb = std::chrono::system_clock::now();
								std::cout << "AIM COMPUTE TIME: "
								  << std::chrono::duration_cast<std::chrono::milliseconds>(tb - ta).count()
								  << " milliseconds" << std::endl;
	w->time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(tb - ta).count();
	w->setResults(res, w->data_length);
}
void AIMWorkConsumer::deInit(){
	AcceleratorInterface->DeallocateMemory(ImageHandle);
	delete(AIMEngine);
	delete(AcceleratorInterface);
}
void AIMWorkConsumer::parseParams(uint8_t * params, uint32_t paramsLength){
	if(paramsLength >= 12){
		newWidth = (int)((uint32_t)((((((params[3] << 8) | params[2]) << 8) | params[1]) << 8) | params[0]));
		newHeight = (int)((uint32_t)((((((params[7] << 8) | params[6]) << 8) | params[5]) << 8) | params[4]));
		newDepth = (int)((uint32_t)((((((params[11] << 8) | params[10]) << 8) | params[9]) << 8) | params[8]));
	}
	configure();
}
void AIMWorkConsumer::configure(){
	if((newWidth != currentWidth) || (newHeight != currentHeight) || (newDepth != currentDepth)){
		currentWidth = newWidth;
		currentHeight = newHeight;
		currentDepth = newDepth;
		dataSize = currentWidth * currentHeight * currentDepth;
		AcceleratorInterface->DeallocateMemory(ImageHandle);
		ImageHandle = AcceleratorInterface->AllocateMemory(dataSize);
	}
}
