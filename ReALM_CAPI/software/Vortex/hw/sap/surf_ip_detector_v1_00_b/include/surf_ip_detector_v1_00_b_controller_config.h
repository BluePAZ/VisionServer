#ifndef __SURF_IP_DETECTOR_CONTROLLER_CONFIG_H__
#define __SURF_IP_DETECTOR_CONTROLLER_CONFIG_H__

#include <VortexTypes.h>
#include "VortexImageDescriptor.h"

typedef struct _IPDetectorControllerConfig {
    uint16_t FlowID         ;
    uint16_t Width          ;
    uint16_t Height         ;
    uint8_t  Type           ;
    uint8_t  Color          ;
    uint8_t  Depth          ;
    uint16_t ResultDeviceID ; 
    uint64_t ResultAddress  ; 
	uint32_t IPThreshold	;

    uint8_t Bytes[16];

    uint8_t* GetBytes()
    {
        *((uint64_t*)&Bytes[0]) =   (((uint64_t)FlowID			& 0x03FF) << 0 ) |
                                    (((uint64_t)Width			& 0x0FFF) << 10) |
                                    (((uint64_t)Height			& 0x0FFF) << 22) |
                                    (((uint64_t)Type			& 0x003F) << 34) |
                                    (((uint64_t)Color			& 0x000F) << 40) |
                                    (((uint64_t)Depth			& 0x000F) << 44) |
                                    (((uint64_t)ResultDeviceID & 0xFFFF) << 48) ;

        *((uint64_t*)&Bytes[8]) =   ResultAddress;

        return Bytes;
    }
} IPDetectorControllerConfig;

class surf_ip_detector_v1_00_b_controller_config : public IVortexConfiguration
{
public:
	surf_ip_detector_v1_00_b_controller_config(void);
	~surf_ip_detector_v1_00_b_controller_config(void);

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

	uint32_t GetIPThreshold();
	void SetIPThreshold(uint32_t value);

	//FlowIDs
	void SetBaseFlowID(uint16_t value);
    uint16_t GetBaseFlowID();
	uint16_t GetPixelFetchFlowID();
	uint16_t GetResultStoreFlowID();

	//Result Device and Address
	VortexDeviceAddress* GetResultDeviceID();
	void SetResultDeviceID(uint8_t BusID, uint8_t SwitchID, uint8_t PortID);

	uint64_t GetResultAddress();
	void SetResultAddress(uint64_t value);

	uint64_t GetImageSizeBytes();

	VortexEntityType GetConfigurationType();
	void SetConfigurationType(VortexEntityType ConfigType);
	uint64_t GetIndex();
	void SetIndex(uint64_t Index);
	VortexConfigurationPacket* GetConfiguration();

private:
	VortexDeviceAddress* m_ResultDeviceID;
	uint64_t m_ResultAddress;

	VortexConfigurationPacket* m_Vcp;
	IPDetectorControllerConfig m_ControllerConfig;
};

#endif // !1

