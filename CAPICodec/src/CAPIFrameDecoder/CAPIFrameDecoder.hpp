#ifndef __CAPI_FRAME_DECODER_HPP__
#define __CAPI_FRAME_DECODER_HPP__

#include <stdio.h>
#include <stdint.h>

//FFMPEG includes
extern "C"
{
	#include "libavcodec/avcodec.h"
	#include "libavformat/avformat.h"
	#include "libavfilter/avfilter.h"
	#include "libswresample/swresample.h"
	#include "libavdevice/avdevice.h"
	#include "libavutil/avutil.h"
	//#include "libpostproc/postprocess.h"
	#include "libswscale/swscale.h"
}

#define H264_DECODER 0x00;

class CAPIFrameDecoder{
public:
	CAPIFrameDecoder();
	~CAPIFrameDecoder();	
	int init_codec(int width, int height);
	uint8_t * decode_frame(uint32_t length, uint8_t * bytes);
	uint8_t * convertYUVtoRGB(int width, int height);
private:
	uint16_t decoder;
	AVCodec * codec;
	AVCodecContext * context;
	AVFrame * frame_yuv;
	AVPacket packet;
	int width;
	int height;
	//For Converting betweeen colorspaces
	AVFrame* avFrameYUV = NULL;
	AVFrame* avFrameRGB = NULL;
	struct SwsContext* ctxt = NULL;

	int init_libraries();
};

#endif
