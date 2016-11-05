#ifndef __HOG_ENGINE_V1_00_A_DEVICE_CONFIG_H__
#define __HOG_ENGINE_V1_00_A_DEVICE_CONFIG_H__

#include <stdint.h>

typedef struct _HOGEngineDeviceConfig
{
	// Device configurations: (Add port id later too)
	// -----------------------------------------------
	// | IS LAST (1)  | RSVD		  |  DEVICE ID    |
	// -----------------------------------------------
	//      127       |			      |15             0


	uint16_t DeviceID;
	uint8_t LastMessage;

	uint8_t Bytes[16];

	uint8_t* GetBytes()
	{

		*((uint64_t*)&Bytes[0]) = (((uint64_t)DeviceID & 0xFFFF) << 0);
		*((uint64_t*)&Bytes[8]) = (((uint64_t)LastMessage & 0x00FF) << 63);

		return Bytes;
	}

} HOGEngineDeviceConfig;


class hog_engine_v1_00_a_device_config
{
public:
	hog_engine_v1_00_a_device_config(void);
	hog_engine_v1_00_a_device_config(uint16_t device_id, bool last_message);
	~hog_engine_v1_00_a_device_config(void);
	
	void SetDeviceID(uint16_t device_id);
	void SetLast(bool islast);

	uint16_t GetDeviceID(void);
	bool GetLast(void);

	uint8_t* GetBytes();

private:
	HOGEngineDeviceConfig m_Device;
};

#endif	