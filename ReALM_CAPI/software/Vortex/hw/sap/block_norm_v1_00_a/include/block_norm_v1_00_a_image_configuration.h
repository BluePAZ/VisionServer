#ifndef __HOG_ENGINE_V1_00_A_IMAGE_CONFIGURATION_H__
#define __HOG_ENGINE_V1_00_A_IMAGE_CONFIGURATION_H__

#include <VortexTypes.h>
#include "VortexImageDescriptor.h"

class block_norm_v1_00_a_image_configuration 
{
 public:
typedef struct image_configuration{
	uint16_t Width;
	uint16_t Height;
	uint8_t  Type;
	uint8_t  Color;
	uint8_t  Depth;
} ImageConfiguraion;

public:
	block_norm_v1_00_a_image_configuration(void);
	~block_norm_v1_00_a_image_configuration(void);
	
	//Width
	uint16_t GetWidth();
	void SetWidth(uint16_t value);

	//Height
	uint16_t GetHeight();
	void SetHeight(uint16_t value);

	//Type
	IMAGE_DESCRIPTOR_TYPE GetType();
	void SetType(IMAGE_DESCRIPTOR_TYPE value);

	//Format
	uint8_t GetFormat();
	void SetFormat(uint8_t value);

	uint16_t GetNumChannels();
	uint16_t GetNumChannelsValid();

	IMAGE_DESCRIPTOR_FORMAT_COLOR GetFormatColor();
	void SetFormatColor(IMAGE_DESCRIPTOR_FORMAT_COLOR value);

	IMAGE_DESCRIPTOR_FORMAT_DEPTH GetFormatDepth();
	void SetFormatDepth(IMAGE_DESCRIPTOR_FORMAT_DEPTH value);


	uint64_t GetImageSizeBytes();

private:
	ImageConfiguraion m_config;
};

#endif // !1

