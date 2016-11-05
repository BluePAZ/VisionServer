#include "surf_ip_detector_v1_00_b.h"
#include "VortexImageDescriptor.h"
#include "VortexMessage.h"
#include <cmath>
#include <string>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>


surf_ip_detector_v1_00_b::surf_ip_detector_v1_00_b(string Name, uint8_t BusID, uint8_t SwitchID, uint8_t PortID, IVortexNIFSAP* NIFSAP)
{
	m_Name = Name;

	m_IsConfigured = false;

	m_SlaveBaseAddress[0] = 0x0000000000000000;
	m_SlaveBaseAddress[1] = 0x0000000400000000;
	m_SlaveBaseAddress[2] = 0x0000000800000000;
	m_SlaveBaseAddress[3] = 0x0000000C00000000;

	//Datapath Address
	m_DatapathBaseAddress = m_SlaveBaseAddress[1];
	//Pixel Distribution Address
	
	/////////// Pipeline Base Address //////////////////////////////////////////////
    m_PipelineBaseAddress = m_DatapathBaseAddress + 0x1000000;
    /////////// Pipeline Base Address //////////////////////////////////////////////
	
	m_ControllerConfig = new surf_ip_detector_v1_00_b_controller_config();
	m_DatapathControlConfig = new VortexSOPControlUnitConfiguration();
	m_PixelDistributionConfig = new surf_ip_detector_v1_00_b_pixel_distribution_config();
	m_IntegralImageConfig = new surf_ip_detector_v1_00_b_integral_image_config();
	m_HessianConfig = new surf_ip_detector_v1_00_b_hessian_config();
	m_IPLocalizerConfig = new surf_ip_detector_v1_00_b_ip_localizer_config();
	m_OrientationAssignmentConfig = new surf_ip_detector_v1_00_b_orientation_assignment_config();

	SetNetworkInterface(NIFSAP);
	SetSAPAddress(BusID, SwitchID, PortID);
}


surf_ip_detector_v1_00_b::~surf_ip_detector_v1_00_b(void)
{
	delete(m_SAPAddress);
	
	for(std::vector<surf_ip_detector_v1_00_b_notification_config*>::iterator it = m_NotificationConfig.begin(); it != m_NotificationConfig.end(); ++it)
		delete (*it);

	delete(m_ControllerConfig);
	delete(m_DatapathControlConfig);
	delete(m_PixelDistributionConfig);
	delete(m_IntegralImageConfig);
	delete(m_HessianConfig);
	delete(m_IPLocalizerConfig);
	delete(m_OrientationAssignmentConfig);

	GetNetworkInterface()->DeallocateFlowID(GetControllerConfiguration()->GetPixelFetchFlowID());
	GetNetworkInterface()->DeallocateFlowID(GetControllerConfiguration()->GetResultStoreFlowID());
	GetNetworkInterface()->DeallocateMemory(m_ScratchMemory);

	delete(m_ControllerConfig);
}

void surf_ip_detector_v1_00_b::Reset()
{
	GetNetworkInterface()->ResetSAP(m_SAPAddress);
	m_IsConfigured = false;
}

string surf_ip_detector_v1_00_b::GetName()
{
	return m_Name;
}

void surf_ip_detector_v1_00_b::SetNetworkInterface(IVortexNIFSAP* value)
{
	m_NetworkInterface = value;
}

IVortexNIFSAP* surf_ip_detector_v1_00_b::GetNetworkInterface()
{
	return m_NetworkInterface;
}

void surf_ip_detector_v1_00_b::SetSAPAddress(uint8_t BusID, uint8_t SwitchID, uint8_t PortID)
{
	m_SAPAddress = new VortexDeviceAddress(BusID,SwitchID,PortID);
}



VortexDeviceAddress* surf_ip_detector_v1_00_b::GetSAPAddress()
{
	return m_SAPAddress;
}

surf_ip_detector_v1_00_b_controller_config* surf_ip_detector_v1_00_b::GetControllerConfiguration()
{
	return m_ControllerConfig;
}

VortexSOPControlUnitConfiguration* surf_ip_detector_v1_00_b::GetDatapathControlConfiguration()
{
	return m_DatapathControlConfig;
}

surf_ip_detector_v1_00_b_pixel_distribution_config* surf_ip_detector_v1_00_b::GetPixelDistributionConfiguration()
{
	return m_PixelDistributionConfig;
}

surf_ip_detector_v1_00_b_hessian_config* surf_ip_detector_v1_00_b::GetHessianConfiguration()
{
	return m_HessianConfig;
}

surf_ip_detector_v1_00_b_integral_image_config* surf_ip_detector_v1_00_b::GetIntegralImageConfiguration()
{
	return m_IntegralImageConfig;
}

surf_ip_detector_v1_00_b_ip_localizer_config* surf_ip_detector_v1_00_b::GetIPLocalizerConfiguration()
{
	return m_IPLocalizerConfig;
}

surf_ip_detector_v1_00_b_orientation_assignment_config* surf_ip_detector_v1_00_b::GetOrientationAssignmentConfiguration()
{
	return m_OrientationAssignmentConfig;
}

void surf_ip_detector_v1_00_b::AcceleratorAddNotificationTarget(uint8_t BusID, uint8_t SwitchID, uint8_t PortID)
{
	m_NotificationConfig.push_back(new surf_ip_detector_v1_00_b_notification_config(BusID,SwitchID,PortID));
}

void surf_ip_detector_v1_00_b::AcceleratorSetImageConfiguration(uint16_t ImageWidth, uint16_t ImageHeight, IMAGE_DESCRIPTOR_FORMAT_COLOR FormatColor, IMAGE_DESCRIPTOR_FORMAT_DEPTH FormatDepth, IMAGE_DESCRIPTOR_TYPE FormatType, uint32_t Threshold)
{
	GetControllerConfiguration()->SetWidth(ImageWidth);
    GetControllerConfiguration()->SetHeight(ImageHeight);
	GetControllerConfiguration()->SetFormatColor(FormatColor);
	GetControllerConfiguration()->SetFormatDepth(FormatDepth);
	GetControllerConfiguration()->SetType(FormatType);
	
	GetDatapathControlConfiguration()->SetPhysicalOpcode(1);
	GetDatapathControlConfiguration()->SetVirtualOpcode(0);
	GetDatapathControlConfiguration()->SetIndex(0x00000 + m_SlaveBaseAddress[1]);
	
	GetPixelDistributionConfiguration()->SetOpcode(0);
	GetPixelDistributionConfiguration()->SetWidth(ImageWidth);
	GetPixelDistributionConfiguration()->SetHeight(ImageHeight);
	GetPixelDistributionConfiguration()->SetIndex(0x10000 + m_SlaveBaseAddress[1]);

	GetIntegralImageConfiguration()->SetOpcode(0);
	GetIntegralImageConfiguration()->SetWidth(ImageWidth);
	GetIntegralImageConfiguration()->SetHeight(ImageHeight);
	GetIntegralImageConfiguration()->SetIndex(0x20000 + m_SlaveBaseAddress[1]);
	
	GetHessianConfiguration()->SetOpcode(0);
	GetHessianConfiguration()->SetWidth(ImageWidth);
	GetHessianConfiguration()->SetHeight(ImageHeight);
	GetHessianConfiguration()->SetIndex(0x30000 + m_SlaveBaseAddress[1]);

	GetIPLocalizerConfiguration()->SetOpcode(0);
	GetIPLocalizerConfiguration()->SetWidth(ImageWidth);
	GetIPLocalizerConfiguration()->SetHeight(ImageHeight);
	GetIPLocalizerConfiguration()->SetThreshold(Threshold);
	GetIPLocalizerConfiguration()->SetIndex(0x40000 + m_SlaveBaseAddress[1]);

	//AcceleratorSetNumChannels(GetControllerConfiguration()->GetNumChannelsValid());
}

void surf_ip_detector_v1_00_b::AcceleratorAllocateScratchMemory(uint64_t ScratchSize)
{
	m_ScratchMemory = GetNetworkInterface()->AllocateMemory(ScratchSize);
	VortexDeviceAddress* MemoryDevice = m_ScratchMemory->GetMemoryDeviceID();

	GetControllerConfiguration()->SetResultDeviceID(MemoryDevice->GetBusID(),MemoryDevice->GetSwitchID(),MemoryDevice->GetPortID());
	GetControllerConfiguration()->SetResultAddress((uint64_t)m_ScratchMemory->GetMemoryOffset());
}

void surf_ip_detector_v1_00_b::AcceleratorAllocateFlowID()
{
    uint16_t BaseFlowID = GetNetworkInterface()->AllocateFlowID(2);

    if (BaseFlowID != 0)
        GetControllerConfiguration()->SetBaseFlowID(BaseFlowID);
}

bool surf_ip_detector_v1_00_b::Configure()
{
	///////////// Retrieve Variables ////////////////////////////////////
	int ImageWidth = GetControllerConfiguration()->GetWidth();
	int ImageHeight = GetControllerConfiguration()->GetHeight();
	int ImageSize = GetControllerConfiguration()->GetImageSizeBytes();

	AcceleratorAllocateFlowID();
	AcceleratorAllocateScratchMemory(ImageSize);
	
	///////////// Retrieve Variables ////////////////////////////////////

	///////////// Configure Controller ////////////////////////////////////

	///////////// Configure DeviceID ////////////////////////////////////
	ConfigureDeviceID(m_SAPAddress, m_SlaveBaseAddress[0]);
	///////////// Configure DeviceID ////////////////////////////////////
	
	//// Configure Notifications ////
	//ConfigureList(m_SAPAddress, 0x100000 + m_SlaveBaseAddress[0], m_NotificationConfig);
	//// Configure Notifications ////

	//// Configure Image Descriptors ////
	ConfigureList(m_SAPAddress, 0x200000 + m_SlaveBaseAddress[0], m_ControllerConfig);
	//// Configure Image Descriptors ////

	///////////// Configure Controller ////////////////////////////////////

    ///////////// Configure Datapath //////////////////////////////////////
    VortexConfigurationGroup* DatapathConfigs = new VortexConfigurationGroup(m_SAPAddress);

	DatapathConfigs->Insert(GetDatapathControlConfiguration()->GetConfiguration(),VortexEntityType::SAP);
	DatapathConfigs->Insert(GetPixelDistributionConfiguration()->GetConfiguration(),VortexEntityType::SAP);
	DatapathConfigs->Insert(GetIntegralImageConfiguration()->GetConfiguration(), VortexEntityType::SAP);
	DatapathConfigs->Insert(GetHessianConfiguration()->GetConfiguration(), VortexEntityType::SAP);
	DatapathConfigs->Insert(GetIPLocalizerConfiguration()->GetConfiguration(), VortexEntityType::SAP);
	//DatapathConfigs->Insert(GetOrientationAssignmentConfiguration()->GetConfiguration(), VortexEntityType::SAP);

	//// Datapath Functional Unit Configurations ////
	
	///////////// Configure Datapath ////////////////////////////////////
	GetNetworkInterface()->Configure(DatapathConfigs);
	///////////// Configure Datapath ////////////////////////////////////

	delete(DatapathConfigs);


	return (m_IsConfigured = true);
}

void surf_ip_detector_v1_00_b::ConfigureDeviceID(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress)
{
    uint8_t* DeviceIDBytes = SAPDevice->GetBytes();
	VortexConfigurationGroup* vcgDeviceID = new VortexConfigurationGroup(SAPDevice, new VortexConfigurationPacket(DeviceIDBytes, 2, SAPAddress), VortexEntityType::SAP);

	//Configure
	GetNetworkInterface()->Configure(vcgDeviceID);
	delete(vcgDeviceID);
}

void surf_ip_detector_v1_00_b::ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, surf_ip_detector_v1_00_b_controller_config* Config)
{
	IVortexConfiguration** ivc = new IVortexConfiguration*[1];
	ivc[0] = Config;

	ConfigureList(SAPDevice, SAPAddress, ivc, 1);

	delete(ivc);
}

void surf_ip_detector_v1_00_b::ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, vector<surf_ip_detector_v1_00_b_controller_config*> Configs)
{
	IVortexConfiguration** ivc = new IVortexConfiguration*[Configs.size()];
	for (int i=0; i<Configs.size(); i++)
		ivc[i] = Configs[i];

	ConfigureList(SAPDevice, SAPAddress, ivc, Configs.size());

	delete[](ivc);
}

void surf_ip_detector_v1_00_b::ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, vector<surf_ip_detector_v1_00_b_notification_config*> Configs)
{
	IVortexConfiguration** ivc = new IVortexConfiguration*[Configs.size()];
	for (int i=0; i<Configs.size(); i++)
		ivc[i] = Configs[i];

	ConfigureList(SAPDevice, SAPAddress, ivc, Configs.size());

	delete[](ivc);
}

void surf_ip_detector_v1_00_b::ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, IVortexConfiguration** Configurations, int NumConfigurations)
{
	VortexConfigurationGroup** ConfigurationGroup = new VortexConfigurationGroup*[2];
	ConfigurationGroup[0] = new VortexConfigurationGroup(SAPDevice);
	ConfigurationGroup[1] = new VortexConfigurationGroup(SAPDevice);
	ConfigurationGroup[0]->Insert(new VortexConfigurationPacket(SAPAddress),VortexEntityType::SAP);
	for (int i=0; i<NumConfigurations; i++)
		ConfigurationGroup[1]->Insert(Configurations[i],SAPAddress + 0x10,VortexEntityType::SAP);

	GetNetworkInterface()->Configure(ConfigurationGroup, 2);

	delete(ConfigurationGroup[0]);
	delete(ConfigurationGroup[1]);
	delete[](ConfigurationGroup);
}

uint8_t surf_ip_detector_v1_00_b::ProcessConfigure(VortexMemoryAllocation* ImageHandle, int ImageWidth, int ImageHeight, int Threshold)
{

	int ConfiguredImageWidth = GetControllerConfiguration()->GetWidth();
	int ConfiguredImageHeight = GetControllerConfiguration()->GetHeight();
    int ConfiguredImageSize = GetControllerConfiguration()->GetImageSizeBytes();
	int ConfiguredThreshold = GetIPLocalizerConfiguration()->GetThreshold();


	if (!m_IsConfigured || (ConfiguredImageWidth != ImageWidth) || (ConfiguredImageHeight != ImageHeight) || (ConfiguredThreshold != Threshold))
	{
		//Reset();

		AcceleratorSetImageConfiguration(
			ImageWidth,
			ImageHeight,
			IMAGE_DESCRIPTOR_FORMAT_COLOR::GRAY,
			IMAGE_DESCRIPTOR_FORMAT_DEPTH::BYTE,
			IMAGE_DESCRIPTOR_TYPE::RAW,
			Threshold);

		if (!Configure())
			return -1;

		ConfiguredImageWidth = GetControllerConfiguration()->GetWidth();
		ConfiguredImageHeight = GetControllerConfiguration()->GetHeight();
		ConfiguredImageSize = GetControllerConfiguration()->GetImageSizeBytes();
		ConfiguredThreshold = GetIPLocalizerConfiguration()->GetThreshold();
	}

	return 0;
	
}

uint8_t* surf_ip_detector_v1_00_b::Process(VortexMemoryAllocation* ImageHandle, int ImageWidth, int ImageHeight, int Threshold)
{

	int ConfiguredImageWidth = GetControllerConfiguration()->GetWidth();
	int ConfiguredImageHeight = GetControllerConfiguration()->GetHeight();
	int ConfiguredImageSize = GetControllerConfiguration()->GetImageSizeBytes();
	int ConfiguredThreshold = GetIPLocalizerConfiguration()->GetThreshold();

	VortexImageDescriptor* ImageDescriptor = new VortexImageDescriptor();
	ImageDescriptor->SetIdentifier(0xEEFF);
	ImageDescriptor->SetWidth(ConfiguredImageWidth);
	ImageDescriptor->SetHeight(ConfiguredImageHeight);
	ImageDescriptor->SetType(IMAGE_DESCRIPTOR_TYPE::RAW);
	ImageDescriptor->SetFormatColor(IMAGE_DESCRIPTOR_FORMAT_COLOR::GRAY);
	ImageDescriptor->SetFormatDepth(IMAGE_DESCRIPTOR_FORMAT_DEPTH::BYTE);
	ImageDescriptor->SetImageHandle(ImageHandle);

	VortexMessage* ProcessMessage = new VortexMessage(
		VortexMessageType::EXECUTE_REQUEST,
		NULL,
		GetSAPAddress(),
		true);

	ProcessMessage->AddBytes(ImageDescriptor->GetBytes(), 16);
	GetNetworkInterface()->SendMessage(ProcessMessage);

	delete(ProcessMessage);
	delete(ImageDescriptor);

	VortexMessage* ProcessMessageResponse = GetNetworkInterface()->WaitMessage();
	if ((ProcessMessageResponse == NULL) || ProcessMessageResponse->GetType() != VortexMessageType::EXECUTE_COMPLETE)
	{
		delete(ProcessMessageResponse);
		return NULL;
	}

	/*uint8_t* header = ProcessMessageResponse->GetHeaderBytes();
	uint8_t* payload = ProcessMessageResponse->GetPayloadBytes();
	uint32_t* payload_int =(uint32_t *)payload;
	
	uint32_t extract_no_of_kp = (*payload_int);
	extract_no_of_kp = ((extract_no_of_kp & 0x00FFFE00) >> 10)/4;

	m_NoOfKeypoints = extract_no_of_kp;*/

	delete(ProcessMessageResponse);

	//return (uint8_t*)(GetControllerConfiguration()->GetResultAddress());
	return 0;

}
