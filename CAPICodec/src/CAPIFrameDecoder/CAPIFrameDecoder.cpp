#include "CAPIFrameDecoder.hpp"

CAPIFrameDecoder::CAPIFrameDecoder(){
	
	
	
}
CAPIFrameDecoder::~CAPIFrameDecoder(){
	
	
	
}
int CAPIFrameDecoder::init_codec(int width, int height){
	this->width = width;
	this->height = height;
	int err = init_libraries();
	return err;
}
int CAPIFrameDecoder::init_libraries(){
	//Initialize the lib
	avcodec_register_all();
	avformat_network_init();
	codec = avcodec_find_decoder(AV_CODEC_ID_H264);
	if (!codec)
	{
		fprintf(stderr, "Codec not found\n");
		return 1;
	}
	context = avcodec_alloc_context3(codec);
	if (!context)
	{
		fprintf(stderr, "Could not allocate video codec context\n");
		return 2;
	}
	avcodec_get_context_defaults3(context, codec);
	context->flags |= CODEC_FLAG_LOW_DELAY;
	context->flags2 |= CODEC_FLAG2_CHUNKS;

	context->bit_rate = 4000000;
	/* resolution must be a multiple of two */
	context->width = width;
	context->height = height;
	/* frames per second */
	context->time_base = (AVRational) { 1, 30 };
	context->gop_size = 1; /* emit one intra frame every ten frames */
	context->pix_fmt = AV_PIX_FMT_YUV420P;
	//context->thread_count = 20; //become higher?
	//context->thread_type = FF_THREAD_SLICE;
	//context->strict_std_compliance = FF_COMPLIANCE_EXPERIMENTAL;
	//set pixel formal

	if (avcodec_open2(context, codec, NULL) < 0)
	{
		fprintf(stderr, "Could not open codec\n");
		return 3;
	}
	avFrameYUV = av_frame_alloc();
	if (!avFrameYUV)
	{
		fprintf(stderr, "Could not allocate video frame\n");
		return 4; 
	}

		avFrameRGB = av_frame_alloc();
		int bytes = avpicture_get_size(AV_PIX_FMT_RGBA, width, height);
		uint8_t * buffer = (uint8_t *)av_malloc(bytes*sizeof(uint8_t));
		avpicture_fill((AVPicture *)avFrameRGB, buffer, AV_PIX_FMT_RGBA, width, height);

	return 0;
}
uint8_t * CAPIFrameDecoder::decode_frame(uint32_t length, uint8_t * bytes){
	
	AVPacket packet;
	av_init_packet(&packet);
	packet.pts = AV_NOPTS_VALUE;
	packet.dts = AV_NOPTS_VALUE;
	packet.data = bytes;// frame data
	packet.size = (int)length;// frame data size
	int got_frame = 0;
	int len = avcodec_decode_video2(context, avFrameYUV, &got_frame, &packet);
	if (len >= 0 && got_frame)
	{
		fprintf(stderr, "------------------------GOT FRAME\n");
		struct SwsContext* tmpCxt = sws_getCachedContext(NULL,
				avFrameYUV->width,
				avFrameYUV->height,
					AV_PIX_FMT_YUV420P,
					avFrameYUV->width,
					avFrameYUV->height,
					AV_PIX_FMT_RGBA,
					SWS_BICUBIC,
					NULL,
					NULL,
					NULL
					);
		sws_scale(tmpCxt, avFrameYUV->data, avFrameYUV->linesize, 0, avFrameYUV->height, avFrameRGB->data, avFrameRGB->linesize);
		sws_freeContext(tmpCxt);
		return (uint8_t *)avFrameRGB->data[0];
		//return convertYUVtoRGB(width, height);
	}else{
		av_register_all();
		return NULL;
	}
}
uint8_t * CAPIFrameDecoder::convertYUVtoRGB(int width, int height){
	struct SwsContext* tmpCxt = sws_getCachedContext(NULL,
		width,
		height,
		AV_PIX_FMT_NV21,
		width,
		height,
		AV_PIX_FMT_RGBA,
		SWS_BICUBIC,
		NULL,
		NULL,
		NULL
		);
	/* fill the yuv frame with the raw data */
	//avpicture_fill((AVPicture *)avFrameYUV, (const uint8_t *)yuv, AV_PIX_FMT_NV21, width, height);
	/* perform the conversion */
	sws_scale(tmpCxt, avFrameYUV->data, avFrameYUV->linesize, 0, height, avFrameRGB->data, avFrameRGB->linesize);
	/* return the rgba data */
	return (uint8_t *)avFrameRGB->data[0];
}

