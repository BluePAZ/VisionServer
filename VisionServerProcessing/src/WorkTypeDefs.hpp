//#define _GLIBCXX_USE_CXX11_ABI 0

#include <stdio.h>
#include <stdlib.h>
//WORK FLOWS
#define SURF_FLOW 0x00
#define SURF_KEYPOINT_EXTRACT 0x01
#define SURF_KEYPOINT_DETECT 0x02
#define SURF_SALIENCY 0x04
#define AIM_SALIENCY 0x08
#define CMT_TRACKING 0x10

//WORK STATUS
#define WORK_STARTED 0xFF
#define WORK_COMPLETED 0xFE
#define WORK_STOPPED 0xFD
#define WORK_STOP_ERROR 0xFC

/*
void decodeWorkStatus(uint8_t workStatus){
	case WORK_STARTED:{
		fprintf(stdout, "Work Status:WORK_STARTED\n");
		break;
	}
	case WORK_COMPLETED:{
		fprintf(stdout, "Work Status:WORK_COMPLETED\n");
		break;
	}
	case WORK_STOPPED:{
		fprintf(stdout, "Work Status:WORK_STOPPED\n");
		break;
	}
	case WORK_STOP_ERROR:{
		fprintf(stdout, "Work Status:WORK_STOPPED_ERROR\n");
		break;
	}	
}
*/
