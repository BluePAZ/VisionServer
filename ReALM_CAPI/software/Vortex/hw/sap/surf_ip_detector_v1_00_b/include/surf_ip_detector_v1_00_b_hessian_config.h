#ifndef __SURF_IP_DETECTOR_HESSIAN_CONFIG_H__
#define __SURF_IP_DETECTOR_HESSIAN_CONFIG_H__

#include <VortexTypes.h>
#include "VortexImageDescriptor.h"

typedef struct _HessianConfig {
	
	// Hessian configurations:
    // -------------------------------------------------------------------------
    // |   RSVRD                   | IMG HEIGHT    | IMG WIDTH     | OPCODE    |
    // -------------------------------------------------------------------------
    //  127                      25|24           14|13            3|2         0

    uint16_t Width          ;
    uint16_t Height         ;
    uint16_t Opcode			;
	
	uint8_t Bytes[16];

    uint8_t* GetBytes()
    {
        *((uint64_t*)&Bytes[0]) =   (((uint64_t)Opcode			& 0x0007) << 0 ) |
                                    (((uint64_t)Width			& 0x07FF) << 3 ) |
                                    (((uint64_t)Height			& 0x07FF) << 14) ;

        *((uint64_t*)&Bytes[8]) =   0;

        return Bytes;
    }
} HessianConfig;

class surf_ip_detector_v1_00_b_hessian_config : public IVortexConfiguration
{
public:
	surf_ip_detector_v1_00_b_hessian_config(void);
	~surf_ip_detector_v1_00_b_hessian_config(void);

	//Width
	uint16_t GetWidth();
	void SetWidth(uint16_t value);

	//Height
	uint16_t GetHeight();
	void SetHeight(uint16_t value);

	//Opcode
	uint8_t GetOpcode();
	void SetOpcode(uint8_t value);

	VortexEntityType GetConfigurationType();
	void SetConfigurationType(VortexEntityType ConfigType);
	uint64_t GetIndex();
	void SetIndex(uint64_t Index);
	VortexConfigurationPacket* GetConfiguration();

private:
	VortexConfigurationPacket* m_Vcp;
	HessianConfig m_HessianConfig;
};

#endif // !1

