#ifndef __HOG_ENGINE_V1_00_A_H__
#define __HOG_ENGINE_V1_00_A_H__

#include <VortexTypes.h>
#include <VortexSAPTypes.h>
#include <VortexSOPTypes.h>
#include "hog_engine_v1_00_a_device_config.h"
#include "hog_engine_v1_00_a_controller_config.h"
#include "hog_engine_v1_00_a_notification_config.h"
#include "hog_engine_v1_00_a_opcode_config.h"
// Datapath Configurations
#include "hog_engine_v1_00_a_convolution_image_config.h"
#include "hog_engine_v1_00_a_convolution_kernel_config.h"
#include "hog_engine_v1_00_a_histogram_image_config.h"
#include "hog_engine_v1_00_a_histogram_minmax_config.h"
#include "hog_engine_v1_00_a_histogram_pagetable_config.h"


enum class PIPELINE_ADDRESS_RANGE_INDEX : uint64_t { CONVOLUTION = 0x0000000000000000, HISTOGRAM = 0x0000000000400000};

class hog_engine_v1_00_a
{
public:
	hog_engine_v1_00_a(string Name, uint8_t BusID, uint8_t SwitchID, uint8_t PortID, IVortexNIFSAP* NIFSAP);
	~hog_engine_v1_00_a(void);

	string GetName();

	IVortexNIFSAP* GetNetworkInterface();
	VortexDeviceAddress* GetSAPAddress();
	
	hog_engine_v1_00_a_controller_config* GetControllerConfiguration();
	hog_engine_v1_00_a_opcode_config* GetOpcodeConfiguration();
	vector<hog_engine_v1_00_a_notification_config*> GetNotificationConfiguration();
	
	hog_engine_v1_00_a_convolution_image_config* GetConvolutionImageConfiguration();
	hog_engine_v1_00_a_convolution_kernel_config* GetConvolutionKernelConfiguration();
	hog_engine_v1_00_a_histogram_image_config* GetHistogramImageConfiguration();
	hog_engine_v1_00_a_histogram_minmax_config* GetHistogramMinMaxConfiguration();
	hog_engine_v1_00_a_histogram_pagetable_config* GetHistogramPageTableConfiguration();

	void AcceleratorAddNotificationTarget(uint8_t BusID, uint8_t SwitchID, uint8_t PortID);
	void AcceleratorSetImageConfiguration(uint16_t ImageWidth, uint16_t ImageHeight, 
										  uint8_t KernelWidth, uint8_t KernelHeight,
		                                  IMAGE_DESCRIPTOR_FORMAT_COLOR FormatColor, 
										  IMAGE_DESCRIPTOR_FORMAT_DEPTH FormatDepth, 
										  IMAGE_DESCRIPTOR_TYPE FormatType, uint32_t Threshold);
	
	void Reset();
	bool Configure();
	uint32_t* Process(VortexMemoryAllocation* ImageHandle, int ImageWidth, int ImageHeight, int KernelWidth, int KernelHeight, int Threshold);

private:
	string m_Name;

	int m_Simulation;

	bool m_IsConfigured;
	
	VortexMemoryAllocation* m_ScratchMemory;
	IVortexNIFSAP* m_NetworkInterface;
	VortexDeviceAddress* m_SAPAddress;
	
	uint64_t m_SlaveBaseAddress[4];
	uint64_t m_DatapathBaseAddress;
	
	uint64_t m_PipelineBaseAddress;
	
	hog_engine_v1_00_a_controller_config* m_ControllerConfig;
	hog_engine_v1_00_a_convolution_image_config* m_ConvolutionImageConfig;
	hog_engine_v1_00_a_convolution_kernel_config* m_ConvolutionKernelConfig;
	hog_engine_v1_00_a_histogram_image_config* m_HistogramImageConfig;
	hog_engine_v1_00_a_histogram_minmax_config* m_HistogramMinMaxConfig;
	hog_engine_v1_00_a_histogram_pagetable_config* m_HistogramPageTableConfig;
	vector<hog_engine_v1_00_a_notification_config*> m_NotificationConfig;
	hog_engine_v1_00_a_opcode_config* m_OpcodeConfig;	
	hog_engine_v1_00_a_device_config* m_DeviceConfig;
	
	void SetNetworkInterface(IVortexNIFSAP* value);
	void SetSAPAddress(uint8_t BusID, uint8_t SwitchID, uint8_t PortID);
	void AcceleratorAllocateFlowID();
	void AcceleratorAllocateScratchMemory(uint64_t ScratchSize);

	void ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, IVortexConfiguration** Configurations, int NumConfigurations);
	void ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, hog_engine_v1_00_a_opcode_config* Config);
	void ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, vector<hog_engine_v1_00_a_opcode_config*> Configs);
	void ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, vector<hog_engine_v1_00_a_notification_config*> Configs);
	void ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, vector<hog_engine_v1_00_a_controller_config*> Configs);
	void ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, hog_engine_v1_00_a_controller_config* Config);
	void ConfigureDeviceID(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress);
};

#endif
