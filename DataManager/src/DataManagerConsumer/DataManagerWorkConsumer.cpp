#include "DataManagerWorkConsumer.hpp"

DataManagerWorkConsumer::DataManagerWorkConsumer(){

}

DataManagerWorkConsumer::~DataManagerWorkConsumer(){
	
}
void DataManagerWorkConsumer::init(){
	currentWidth = 800;
	currentHeight = 600;
	currentDepth = 4;
	fprintf(stdout, "DataManagerWorkConsumer init success.\n");
}
void DataManagerWorkConsumer::runWork(WorkItem * w){
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
void DataManagerWorkConsumer::deInit(){
	AcceleratorInterface->DeallocateMemory(ImageHandle);
	delete(AIMEngine);
	delete(AcceleratorInterface);
}
void DataManagerWorkConsumer::parseParams(uint8_t * params, uint32_t paramsLength){
	if(paramsLength >= 12){
		newWidth = (int)((uint32_t)((((((params[3] << 8) | params[2]) << 8) | params[1]) << 8) | params[0]));
		newHeight = (int)((uint32_t)((((((params[7] << 8) | params[6]) << 8) | params[5]) << 8) | params[4]));
		newDepth = (int)((uint32_t)((((((params[11] << 8) | params[10]) << 8) | params[9]) << 8) | params[8]));
	}
	configure();
}
void DataManagerWorkConsumer::configure(){
	if((newWidth != currentWidth) || (newHeight != currentHeight) || (newDepth != currentDepth)){
		currentWidth = newWidth;
		currentHeight = newHeight;
		currentDepth = newDepth;
		dataSize = currentWidth * currentHeight * currentDepth;
		AcceleratorInterface->DeallocateMemory(ImageHandle);
		ImageHandle = AcceleratorInterface->AllocateMemory(dataSize);
	}
}
