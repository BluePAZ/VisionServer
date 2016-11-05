#ifndef __SURF_IP_DETECTOR_ENGINE_V2_00_B_H__
#define __SURF_IP_DETECTOR_ENGINE_V2_00_B_H__

#include <VortexTypes.h>
#include <VortexSAPTypes.h>
#include <VortexSOPTypes.h>
#include "surf_ip_detector_v1_00_b_configuration_list.h"
#include "surf_ip_detector_v1_00_b_controller_config.h"
#include "surf_ip_detector_v1_00_b_hessian_config.h"
#include "surf_ip_detector_v1_00_b_ip_localizer_config.h"
#include "surf_ip_detector_v1_00_b_integral_image_config.h"
#include "surf_ip_detector_v1_00_b_orientation_assignment_config.h"
#include "surf_ip_detector_v1_00_b_pixel_distribution_config.h"
#include "surf_ip_detector_v1_00_b_notification_config.h"

//enum class PIPELINE_ADDRESS_RANGE_INDEX : uint64_t { CONVOLUTION = 0x0000000000000000, HISTOGRAM = 0x0000000000400000, LOG_LOOKUP = 0x0000000000800000};

class surf_ip_detector_v1_00_b
{
public:
	surf_ip_detector_v1_00_b(string Name, uint8_t BusID, uint8_t SwitchID, uint8_t PortID, IVortexNIFSAP* NIFSAP);
	~surf_ip_detector_v1_00_b(void);

	string GetName();

	IVortexNIFSAP* GetNetworkInterface();
	VortexDeviceAddress* GetSAPAddress();

	uint32_t   m_NoOfKeypoints;
	surf_ip_detector_v1_00_b_controller_config* GetControllerConfiguration();
	VortexSOPControlUnitConfiguration* GetDatapathControlConfiguration();
	surf_ip_detector_v1_00_b_pixel_distribution_config* GetPixelDistributionConfiguration();
	surf_ip_detector_v1_00_b_hessian_config* GetHessianConfiguration();
	surf_ip_detector_v1_00_b_integral_image_config* GetIntegralImageConfiguration();
	surf_ip_detector_v1_00_b_ip_localizer_config* GetIPLocalizerConfiguration();
	surf_ip_detector_v1_00_b_orientation_assignment_config* GetOrientationAssignmentConfiguration();

	
	void AcceleratorAddNotificationTarget(uint8_t BusID, uint8_t SwitchID, uint8_t PortID);
	void AcceleratorSetImageConfiguration(uint16_t ImageWidth, uint16_t ImageHeight, IMAGE_DESCRIPTOR_FORMAT_COLOR FormatColor, IMAGE_DESCRIPTOR_FORMAT_DEPTH FormatDepth, IMAGE_DESCRIPTOR_TYPE FormatType, uint32_t Threshold);
	
	void Reset();
	bool Configure();
	uint8_t* Process(VortexMemoryAllocation* ImageHandle, int ImageWidth, int ImageHeight, int Threshold);
	uint8_t ProcessConfigure(VortexMemoryAllocation* ImageHandle, int ImageWidth, int ImageHeight, int Threshold);
	//aim_test_data* GenerateTestData(ImageContainer* Image, vector<vector<vector<vector<int16_t>>>> Kernel, string OutputFilepath);

private:
	string m_Name;

	bool m_IsConfigured;
	
	VortexMemoryAllocation* m_ScratchMemory;
	IVortexNIFSAP* m_NetworkInterface;
	VortexDeviceAddress* m_SAPAddress;
	
	uint64_t m_SlaveBaseAddress[4];
	uint64_t m_DatapathBaseAddress;
	
	uint64_t m_PipelineBaseAddress;
	

	surf_ip_detector_v1_00_b_controller_config* m_ControllerConfig;
	VortexSOPControlUnitConfiguration* m_DatapathControlConfig;
	surf_ip_detector_v1_00_b_pixel_distribution_config* m_PixelDistributionConfig;
	surf_ip_detector_v1_00_b_ip_localizer_config* m_IPLocalizerConfig;
	surf_ip_detector_v1_00_b_hessian_config* m_HessianConfig;
	surf_ip_detector_v1_00_b_integral_image_config* m_IntegralImageConfig;
	surf_ip_detector_v1_00_b_orientation_assignment_config* m_OrientationAssignmentConfig;
	vector<surf_ip_detector_v1_00_b_notification_config*> m_NotificationConfig;
	void	 SetNoOfKeypoints(uint32_t  no_of_keypoints);
	uint32_t GetNoOfKeypoints();
	void SetNetworkInterface(IVortexNIFSAP* value);
	void SetSAPAddress(uint8_t BusID, uint8_t SwitchID, uint8_t PortID);
	void AcceleratorAllocateFlowID();
	void AcceleratorAllocateScratchMemory(uint64_t ScratchSize);

	void ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, IVortexConfiguration** Configurations, int NumConfigurations);
	void ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, vector<surf_ip_detector_v1_00_b_controller_config*> Configs);
	void ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, surf_ip_detector_v1_00_b_controller_config* Config);
	void ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, vector<surf_ip_detector_v1_00_b_notification_config*> Configs);
	void ConfigureDeviceID(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress);
};

#endif
