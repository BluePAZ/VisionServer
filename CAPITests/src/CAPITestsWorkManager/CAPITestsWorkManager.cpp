/*
 * AIMWorkManager.cpp
 *
 *  Created on: Nov 24, 2015
 *      Author: peterazientara
 */

#include "CAPITestsWorkManager.h"

CAPITestsWorkManager::CAPITestsWorkManager() {
	// TODO Auto-generated constructor stub
	fprintf(stderr, "CAPITestsWorkManager created\n");
}
CAPITestsWorkManager::~CAPITestsWorkManager() {
	// TODO Auto-generated destructor stub
	fprintf(stderr, "CAPITestsWorkManager destroyed\n");
}
std::string CAPITestsWorkManager::getName(){
	return "test_work";
}

