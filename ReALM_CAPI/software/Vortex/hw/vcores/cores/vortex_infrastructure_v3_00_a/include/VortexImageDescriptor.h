#ifndef __VORTEX_IMAGE_DESCRIPTOR_H__
#define __VORTEX_IMAGE_DESCRIPTOR_H__

#include "VortexTypes.h"

enum class IMAGE_DESCRIPTOR_TYPE : uint8_t {
	RAW								= 0,
	MONOCULAR_DEPTH					= 1,
	STEREO_DEPTH					= 2,
	INTRA_BAND_CONTRAST_ENHANCED	= 3,
	INTER_BAND_CONTRAST_ENHANCED	= 4,
	GABOR_FEATURE_MAP				= 5,
	SALIENCY_MAP					= 6,
	SIFT							= 7,
	SURF							= 8,
	INTEGRAL_IMAGE					= 9
};

enum class IMAGE_DESCRIPTOR_FORMAT_COLOR : uint8_t {
	NONE				= 0,
	GRAY				= 1,
	RGB					= 2,
	RGBA				= 3,
	BGR					= 4,
	BGRA				= 5,
	HSV					= 6,
	HLS					= 7,
	YCC					= 8,
	LAB					= 9,
	LUV					= 10,
	XYZ					= 11
};

enum class IMAGE_DESCRIPTOR_FORMAT_DEPTH : uint8_t {
	BYTE				= 0,
	UINT16				= 1,
	INT16				= 2,
	UINT32				= 3,
	INT32				= 4,
	FLOAT				= 5,
	DOUBLE				= 6,
	UINT64				= 7,
	INT64				= 8
};

typedef struct _ImageDescriptor {
    uint16_t Identifier : 10;
    uint16_t Width      : 12;
    uint16_t Height     : 12;
    uint8_t  Type       : 6;
    uint8_t  Color      : 4;
    uint8_t  Depth      : 4;
    uint16_t DeviceID   : 16;
    uint64_t Address    : 64;

    uint8_t Bytes[16];

    uint8_t* GetBytes()
    {
        *((uint64_t*)&Bytes[0]) =   (((uint64_t)Identifier & 0x03FF) << 0 ) |
                                    (((uint64_t)Width      & 0x0FFF) << 10) |
                                    (((uint64_t)Height     & 0x0FFF) << 22) |
                                    (((uint64_t)Type       & 0x003F) << 34) |
                                    (((uint64_t)Color      & 0x000F) << 40) |
                                    (((uint64_t)Depth      & 0x000F) << 44) |
                                    (((uint64_t)DeviceID   & 0xFFFF) << 48) ;

        *((uint64_t*)&Bytes[8]) =   Address;

        return Bytes;
    }
} ImageDescriptor;

class ImageContainer
{
public:
    uint8_t* data;
    int BytesPerChannel;
    int NumDimensions;
    int Length[16];

    ImageContainer(int bytesPerChannel, int Dim0, int Dim1=0, int Dim2=0, int Dim3=0, int Dim4=0, int Dim5=0, int Dim6=0, int Dim7=0)
    {
        BytesPerChannel = bytesPerChannel;

        NumDimensions = 0;

        if (Dim0 != 0) { NumDimensions++; Length[0] = Dim0; }
        if (Dim0 != 1) { NumDimensions++; Length[1] = Dim1; }
        if (Dim0 != 2) { NumDimensions++; Length[2] = Dim2; }
        if (Dim0 != 3) { NumDimensions++; Length[3] = Dim3; }
        if (Dim0 != 4) { NumDimensions++; Length[4] = Dim4; }
        if (Dim0 != 5) { NumDimensions++; Length[5] = Dim5; }
        if (Dim0 != 6) { NumDimensions++; Length[6] = Dim6; }
        if (Dim0 != 7) { NumDimensions++; Length[7] = Dim7; }
    }

    ~ImageContainer();
};

class VortexImageDescriptor
{
public:
	VortexImageDescriptor(void);
	VortexImageDescriptor(uint8_t* value);
	~VortexImageDescriptor(void);

	VortexMemoryAllocation* GetImageHandle();
	void SetImageHandle(VortexMemoryAllocation* value);

	uint16_t GetIdentifier();
	void SetIdentifier(uint16_t value);

	uint16_t GetWidth();
	void SetWidth(uint16_t value);

	uint16_t GetHeight();
	void SetHeight(uint16_t value);

	IMAGE_DESCRIPTOR_TYPE GetType();
	void SetType(IMAGE_DESCRIPTOR_TYPE value);

	IMAGE_DESCRIPTOR_FORMAT_COLOR GetFormatColor();
	void SetFormatColor(IMAGE_DESCRIPTOR_FORMAT_COLOR value);

	IMAGE_DESCRIPTOR_FORMAT_DEPTH GetFormatDepth();
	void SetFormatDepth(IMAGE_DESCRIPTOR_FORMAT_DEPTH value);

	uint8_t* GetBytes(int& Length);
	uint8_t* GetBytes();

	int GetImageSize();

private:
	uint16_t m_Identifier;
	uint16_t m_Width;
	uint16_t m_Height;
	IMAGE_DESCRIPTOR_TYPE m_Type;
	IMAGE_DESCRIPTOR_FORMAT_COLOR m_FormatColor;
	IMAGE_DESCRIPTOR_FORMAT_DEPTH m_FormatDepth;

	VortexMemoryAllocation* m_ImageHandle;
	ImageDescriptor m_Descriptor;
};

#endif
