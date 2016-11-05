#include "hog_engine_v1_00_a_device_config.h"

hog_engine_v1_00_a_device_config::hog_engine_v1_00_a_device_config(uint16_t device_id = 1, bool last_message = true)
{
	m_Device.DeviceID = device_id;
	m_Device.LastMessage = last_message ? (uint8_t)1 : (uint8_t)0;
}

hog_engine_v1_00_a_device_config::~hog_engine_v1_00_a_device_config(void)
{
}

void hog_engine_v1_00_a_device_config::SetDeviceID(uint16_t device_id)
{
	m_Device.DeviceID = device_id;
}

void hog_engine_v1_00_a_device_config::SetLast(bool islast)
{
	m_Device.LastMessage = islast ? (uint8_t) 1 : (uint8_t) 0;
}

uint16_t hog_engine_v1_00_a_device_config::GetDeviceID(void)
{
	return m_Device.DeviceID;
}

bool hog_engine_v1_00_a_device_config::GetLast(void)
{
	return m_Device.LastMessage;
}

uint8_t* hog_engine_v1_00_a_device_config::GetBytes()
{
	return m_Device.GetBytes();
}

