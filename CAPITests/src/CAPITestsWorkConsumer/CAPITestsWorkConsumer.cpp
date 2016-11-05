#include "CAPITestsWorkConsumer.hpp"

CAPITestsWorkConsumer::CAPITestsWorkConsumer(){
	fprintf(stderr, "CAPITestsWorkConsumer created\n");
}
CAPITestsWorkConsumer::~CAPITestsWorkConsumer(){
	fprintf(stderr, "CAPITestsWorkConsumer destroyed\n");
}
void CAPITestsWorkConsumer::init(){
	fprintf(stderr, "CAPITestsWorkConsumer called init()\n");
}
void CAPITestsWorkConsumer::runWork(WorkItem * w){
	fprintf(stderr, "CAPITestsWorkConsumer called runWork()\n");
	usleep(15000);
	w->result_length = 2048;
	w->result = (uint8_t *)malloc(w->result_length);
	if(w->result != NULL){
		for(int i = 0; i<2048; i++){
			(w->result)[i] = (uint8_t) i;
		}
	}else{
		fprintf(stderr,"%s\n", strerror(errno));
	}
}
void CAPITestsWorkConsumer::deInit(){
	fprintf(stderr, "CAPITestsWorkConsumer called deInit()\n");
}
void CAPITestsWorkConsumer::parseParams(uint8_t * params, uint32_t paramsLength){
	fprintf(stderr, "CAPITestsWorkConsumer called parseParams()\n");
}

