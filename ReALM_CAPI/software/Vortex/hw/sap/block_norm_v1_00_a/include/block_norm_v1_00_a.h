#ifndef __BLOCK_NORM_V1_00_A_H__
#define __BLOCK_NORM_V1_00_A_H__

#include <VortexTypes.h>
#include <VortexSAPTypes.h>
#include <VortexSOPTypes.h>
#include "block_norm_v1_00_a_notification_config.h"
#include "block_norm_v1_00_a_image_configuration.h"
#include "block_norm_v1_00_a_mode_descriptor.h"
#include "block_norm_v1_00_a_mem_descriptor.h"
#include "block_norm_v1_00_a_out_addr_descriptor.h"

class block_norm_v1_00_a
{
public:
block_norm_v1_00_a(string Name, uint8_t BusID, uint8_t SwitchID, uint8_t PortID, IVortexNIFSAP* NIFSAP);
	~block_norm_v1_00_a(void);

	string GetName();

	IVortexNIFSAP* GetNetworkInterface();
	VortexDeviceAddress* GetSAPAddress();
	
        block_norm_v1_00_a_image_configuration* GetImageConfiguraiton();
	vector<block_norm_v1_00_a_notification_config*> GetNotificationConfiguration();
	
	block_norm_v1_00_a_mode_descriptor* GetModeDescriptor();
	block_norm_v1_00_a_mem_descriptor* GetMemDescriptor();
	block_norm_v1_00_a_out_addr_descriptor* GetOutAddrDescriptor();

	void AcceleratorAddNotificationTarget(uint8_t BusID, uint8_t SwitchID, uint8_t PortID);
	void AcceleratorSetImageConfiguration(uint16_t ImageWidth, 
					      uint16_t ImageHeight, 
					      IMAGE_DESCRIPTOR_FORMAT_COLOR FormatColor, 
					      IMAGE_DESCRIPTOR_FORMAT_DEPTH FormatDepth, 
					      IMAGE_DESCRIPTOR_TYPE FormatType);
	void Reset();
	bool Configure();
	int32_t* Process(VortexMemoryAllocation* ImageHandle, 
			 uint16_t ImageWidth, 
			 uint16_t ImageHeight,
			 // uint8_t hog_ld_mem_id, // now this is in ImageHandle
			 //uint64_t _hog_ld_mem_address,// now this is in ImageHandle
			 // uint8_t block_norm_device_id,
			 uint8_t nmlz_out_sel, // 0(default)=outputNmlzdBin, 1=outputDotProduct
			 uint8_t feature_pedro_sel, // 0=Dalal, 1=FeaturePedro,
			 uint8_t CellPixWidth,
			 uint8_t CellPixHeight,
			 uint8_t RoiPixWidth,
			 uint8_t RoiPixHeight,
			 uint8_t BinNum,
			 uint16_t ScaleAdj,
			 uint8_t ScaleCst,
			 float ProjShift,
			 float TextScale,
			 uint8_t NormMax,
			 uint8_t NormAdjust,
			 uint64_t SvmAddr,
			 uint16_t SvmMemID);

	uint64_t GetOutputSize(void);
 private:
	uint64_t hog_ld_mem_address;
	string m_Name;

	int m_Simulation;

	bool m_IsConfigured;
	
	VortexMemoryAllocation* m_ScratchMemory;
	IVortexNIFSAP* m_NetworkInterface;
	VortexDeviceAddress* m_SAPAddress;
	
	uint64_t m_SlaveBaseAddress[4];
	uint64_t m_DatapathBaseAddress;
	
	uint64_t m_PipelineBaseAddress;

	vector<block_norm_v1_00_a_notification_config*> m_NotificationConfig;	

	block_norm_v1_00_a_image_configuration* m_ImageConfig;
	block_norm_v1_00_a_mode_descriptor* m_ModeConfig;
	block_norm_v1_00_a_mem_descriptor* m_MemConfig;
	block_norm_v1_00_a_out_addr_descriptor* m_OutAddrConfig;
	
	void SetNetworkInterface(IVortexNIFSAP* value);
	void SetSAPAddress(uint8_t BusID, uint8_t SwitchID, uint8_t PortID);
	void AcceleratorAllocateFlowID();
	void AcceleratorAllocateScratchMemory(uint64_t ScratchSize);
	/*
	  void ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, IVortexConfiguration** Configurations, int NumConfigurations);
	  void ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, block_norm_v1_00_a_opcode_config* Config);
	  void ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, vector<block_norm_v1_00_a_opcode_config*> Configs);
	  void ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, vector<block_norm_v1_00_a_notification_config*> Configs);
	  void ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, vector<block_norm_v1_00_a_controller_config*> Configs);
	  void ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, block_norm_v1_00_a_controller_config* Config);
	*/
	void ConfigureDeviceID(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress);
	
	void SetDescriptors(uint16_t ImageWidth, 
			    uint16_t ImageHeight, 
			    IMAGE_DESCRIPTOR_FORMAT_COLOR FormatColor, 
			    IMAGE_DESCRIPTOR_FORMAT_DEPTH FormatDepth, 
			    IMAGE_DESCRIPTOR_TYPE FormatType,
							  
			    // uint16_t hog_ld_flow_id, // this is not required for C++ version
			    //uint8_t hog_ld_mem_id, // now this is in ImageHandle
			    //uint64_t _hog_ld_mem_address,// now this is in ImageHandle
			    // uint8_t block_norm_device_id,
			    uint8_t nmlz_out_sel, // 0(default)=outputNmlzdBin, 1=outputDotProduct
			    uint8_t feature_pedro_sel, // 0=Dalal, 1=FeaturePedro,
			    uint8_t CellPixWidth,
			    uint8_t CellPixHeight,
			    uint8_t RoiPixWidth,
			    uint8_t RoiPixHeight,
			    uint8_t BinNum,
			    uint16_t ScaleAdj,
			    uint8_t ScaleCst,
			    float ProjShift,
			    float TextScale,
			    uint8_t NormMax,
			    uint8_t NormAdjust,
			    // uint16_t svm_ld_flow_id, // this is not required 
			    uint64_t SvmAddr,
			    uint16_t SvmMemID);
};

#endif
