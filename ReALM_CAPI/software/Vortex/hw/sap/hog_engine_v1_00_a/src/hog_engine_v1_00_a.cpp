#include "hog_engine_v1_00_a.h"
#include "VortexImageDescriptor.h"
#include "VortexMessage.h"
#include <cmath>
#include <string>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

// for timer
#include "opencv2/core/core.hpp"

// Not using OPCODE AND NOTIFICATION - ska - 03/23/2016
//enum HOG_PIPELINE_CONTROLLER_ADDRESS : uint64_t { DEVICE = 0x000000, OPCODE = 0x100000, NOTIFICATION = 0x200000, IMAGE = 0x300000 };
enum HOG_PIPELINE_CONTROLLER_ADDRESS : uint64_t { DEVICE = 0x000000, IMAGE = 0x100000 };
enum HOG_PIPELINE_DATAPATH_ADDRESS: uint64_t { CONVOLUTION = 0x0000000, HISTOGRAM = 0x0800000 };
enum HOG_PIPELINE_CONVOLUTION_ADDRESS : uint64_t { CONV_IMAGE = 0x0000, CONV_KERNEL = 0x4000 };
enum HOG_PIPELINE_HISTOGRAM_ADDRESS : uint64_t { HIST_MINMAX = 0x000000, HIST_PAGETABLE = 0x100000, HIST_IMAGE = 0x200000 };
enum HOG_IMAGE_PARAMS : uint16_t { MAXIMAGEHEIGHT = 2048, MAXIMAGEWIDTH = 2048, CELLSIZE = 8 };
enum HOG_SIM_PARAMS : uint16_t { SIMIMAGEHEIGHT = 256, SIMIMAGEWIDTH = 256 };

hog_engine_v1_00_a::hog_engine_v1_00_a(string Name, uint8_t BusID, uint8_t SwitchID, uint8_t PortID, IVortexNIFSAP* NIFSAP)
{
	m_Name = Name;

	m_IsConfigured = false;

	m_SlaveBaseAddress[0] = 0x0000000000000000;   // sap-controller
	m_SlaveBaseAddress[1] = 0x0000000400000000;   // sap-datapath
	m_SlaveBaseAddress[2] = 0x0000000800000000;   // sap-output
	m_SlaveBaseAddress[3] = 0x0000000C00000000;

	//Datapath Address
	m_DatapathBaseAddress = m_SlaveBaseAddress[1];
	//Pixel Distribution Address
	
	/////////// Pipeline Base Address //////////////////////////////////////////////
    m_PipelineBaseAddress = m_DatapathBaseAddress + 0x1000000;
    /////////// Pipeline Base Address //////////////////////////////////////////////
	
	m_ControllerConfig = new hog_engine_v1_00_a_controller_config();
	m_ConvolutionImageConfig = new hog_engine_v1_00_a_convolution_image_config();
	m_ConvolutionKernelConfig = new hog_engine_v1_00_a_convolution_kernel_config();
	m_HistogramImageConfig = new hog_engine_v1_00_a_histogram_image_config();
	m_HistogramMinMaxConfig = new hog_engine_v1_00_a_histogram_minmax_config();
	m_HistogramPageTableConfig = new hog_engine_v1_00_a_histogram_pagetable_config();
	m_OpcodeConfig = new hog_engine_v1_00_a_opcode_config(true, true);
	m_DeviceConfig = new hog_engine_v1_00_a_device_config((uint16_t)PortID, true);

	SetNetworkInterface(NIFSAP);
	SetSAPAddress(BusID, SwitchID, PortID);

	// IMPORTANT - if running on FPGA turn this to false else page table configuration won't be loaded completely
	// NOTE: The page table configuration takes 95% of the time the RHA is in execution. As a work-around we preload the page with PageTable.coe (see the xci) and just send a dummy configuration to reduce configuration time
	// BOTTOM LINE: if using preloaded page table, set m_Simulation = true else set m_Simulation = false (will take a long time to configure at run time)
	m_Simulation = false;
}

hog_engine_v1_00_a::~hog_engine_v1_00_a(void)
{
	//cout << __func__ << endl;
	delete(m_SAPAddress);
	for(std::vector<hog_engine_v1_00_a_notification_config*>::iterator it = m_NotificationConfig.begin(); it != m_NotificationConfig.end(); ++it)
		delete (*it);
	delete(m_ControllerConfig);
	delete(m_ConvolutionImageConfig);
	delete(m_ConvolutionKernelConfig);
	delete(m_HistogramImageConfig);
	delete(m_HistogramMinMaxConfig);
	delete(m_HistogramPageTableConfig);
	delete(m_OpcodeConfig);
	delete(m_DeviceConfig);
	GetNetworkInterface()->DeallocateFlowID(GetControllerConfiguration()->GetResultStoreFlowID());
	GetNetworkInterface()->DeallocateMemory(m_ScratchMemory);

}

void hog_engine_v1_00_a::Reset()
{
	GetNetworkInterface()->ResetSAP(m_SAPAddress);
	m_IsConfigured = false;
}

string hog_engine_v1_00_a::GetName()
{
	return m_Name;
}

void hog_engine_v1_00_a::SetNetworkInterface(IVortexNIFSAP* value)
{
	m_NetworkInterface = value;
}

IVortexNIFSAP* hog_engine_v1_00_a::GetNetworkInterface()
{
	return m_NetworkInterface;
}

void hog_engine_v1_00_a::SetSAPAddress(uint8_t BusID, uint8_t SwitchID, uint8_t PortID)
{
	m_SAPAddress = new VortexDeviceAddress(BusID,SwitchID,PortID);
}

VortexDeviceAddress* hog_engine_v1_00_a::GetSAPAddress()
{
	return m_SAPAddress;
}

hog_engine_v1_00_a_controller_config* hog_engine_v1_00_a::GetControllerConfiguration()
{
	return m_ControllerConfig;
}

hog_engine_v1_00_a_opcode_config* hog_engine_v1_00_a::GetOpcodeConfiguration()
{	
	return m_OpcodeConfig;
}

vector<hog_engine_v1_00_a_notification_config*> hog_engine_v1_00_a::GetNotificationConfiguration()
{
	return m_NotificationConfig;
}

hog_engine_v1_00_a_convolution_image_config* hog_engine_v1_00_a::GetConvolutionImageConfiguration()
{
	return m_ConvolutionImageConfig;
}

hog_engine_v1_00_a_convolution_kernel_config* hog_engine_v1_00_a::GetConvolutionKernelConfiguration()
{
	return m_ConvolutionKernelConfig;
}

hog_engine_v1_00_a_histogram_image_config* hog_engine_v1_00_a::GetHistogramImageConfiguration()
{
	return m_HistogramImageConfig;
}

hog_engine_v1_00_a_histogram_minmax_config* hog_engine_v1_00_a::GetHistogramMinMaxConfiguration()
{
	return m_HistogramMinMaxConfig;
}

hog_engine_v1_00_a_histogram_pagetable_config* hog_engine_v1_00_a::GetHistogramPageTableConfiguration()
{
	return m_HistogramPageTableConfig;
}

void hog_engine_v1_00_a::AcceleratorAddNotificationTarget(uint8_t BusID, uint8_t SwitchID, uint8_t PortID)
{
	m_NotificationConfig.push_back(new hog_engine_v1_00_a_notification_config(BusID,SwitchID,PortID));
}

void hog_engine_v1_00_a::AcceleratorSetImageConfiguration(uint16_t ImageWidth,
														  uint16_t ImageHeight,														  
	                                                      uint8_t  KernelWidth,
														  uint8_t  KernelHeight,
														  IMAGE_DESCRIPTOR_FORMAT_COLOR FormatColor, 
														  IMAGE_DESCRIPTOR_FORMAT_DEPTH FormatDepth, 
														  IMAGE_DESCRIPTOR_TYPE FormatType, 
														  uint32_t Threshold)
{
	GetControllerConfiguration()->SetIndex(m_SlaveBaseAddress[0]);
	GetControllerConfiguration()->SetWidth(ImageWidth);
	GetControllerConfiguration()->SetHeight(ImageHeight);	
	GetControllerConfiguration()->SetFormatColor(FormatColor);
	GetControllerConfiguration()->SetFormatDepth(FormatDepth);
	GetControllerConfiguration()->SetType(FormatType);

	//if (m_Simulation)
	//{
	//GetConvolutionKernelConfiguration()->SetIndex(m_PipelineBaseAddress + CONVOLUTION + CONV_KERNEL);
	// Need to CreateDerivativeFilter
	GetConvolutionKernelConfiguration()->SetKernel(GetConvolutionKernelConfiguration()->CreateDerivativeFilters());
	//}

	//GetConvolutionImageConfiguration()->SetIndex(m_PipelineBaseAddress + CONVOLUTION + CONV_IMAGE);
	//GetConvolutionImageConfiguration()->SetHeight(ImageHeight);
	//GetConvolutionImageConfiguration()->SetWidth(ImageWidth);
	//GetConvolutionImageConfiguration()->SetKernelHeight(KernelHeight);
	//GetConvolutionImageConfiguration()->SetKernelWidth(KernelWidth);

	//GetHistogramImageConfiguration()->SetIndex(m_PipelineBaseAddress + HISTOGRAM + HIST_IMAGE);
	//GetHistogramImageConfiguration()->SetHeight(ImageHeight);
	//GetHistogramImageConfiguration()->SetWidth(ImageWidth);
	//GetHistogramImageConfiguration()->SetHistograms((ImageHeight*ImageWidth)/(CELLSIZE*CELLSIZE));

	//GetHistogramMinMaxConfiguration()->SetIndex(m_PipelineBaseAddress + HISTOGRAM + HIST_MINMAX);
	//GetHistogramMinMaxConfiguration()->SetMin(0);
	//GetHistogramMinMaxConfiguration()->SetMax(6588397); // dec2hex(6487ED) which is 2pi in 3.20 format

	//GetHistogramPageTableConfiguration()->SetIndex(m_PipelineBaseAddress + HISTOGRAM + HIST_PAGETABLE);
	
	//if (m_Simulation)
	//	GetHistogramPageTableConfiguration()->SetGroupIDs(GetHistogramPageTableConfiguration()->CreateHOGHistogramPartition(SIMIMAGEWIDTH, SIMIMAGEHEIGHT, CELLSIZE, CELLSIZE));
	//else
		//GetHistogramPageTableConfiguration()->SetGroupIDs(GetHistogramPageTableConfiguration()->CreateHOGHistogramPartition(MAXIMAGEWIDTH, MAXIMAGEHEIGHT, CELLSIZE, CELLSIZE));  // not needed anymore if preloading page table - ska - 03/16/2016

}

void hog_engine_v1_00_a::AcceleratorAllocateScratchMemory(uint64_t ScratchSize)
{
	m_ScratchMemory = GetNetworkInterface()->AllocateMemory(ScratchSize);
	VortexDeviceAddress* MemoryDevice = m_ScratchMemory->GetMemoryDeviceID();

	GetControllerConfiguration()->SetResultDeviceID(MemoryDevice->GetBusID(),MemoryDevice->GetSwitchID(),MemoryDevice->GetPortID());
	GetControllerConfiguration()->SetResultAddress((uint64_t)m_ScratchMemory->GetMemoryOffset());
}

void hog_engine_v1_00_a::AcceleratorAllocateFlowID()
{
    uint16_t BaseFlowID = GetNetworkInterface()->AllocateFlowID(2);

    if (BaseFlowID != 0)
		GetControllerConfiguration()->SetBaseFlowID(BaseFlowID);
}

bool hog_engine_v1_00_a::Configure()
{
	///////////// Retrieve Variables ////////////////////////////////////
	//int ImageWidth = GetControllerConfiguration()->GetWidth();
	//int ImageHeight = GetControllerConfiguration()->GetHeight();
	int ImageSize = GetControllerConfiguration()->GetImageSizeBytes();
	//uint64_t ControllerBasAddress = GetControllerConfiguration()->GetIndex();

	AcceleratorAllocateFlowID();  // This is the Pixel Fetch Flow ID. Histogram Store Flow ID will be +1 of this 
	AcceleratorAllocateScratchMemory(ImageSize * 2); // This is the allocation for results 

	///////////// Retrieve Variables ////////////////////////////////////

//->///////////// Configure Controller ////////////////////////////////////
//|
//|	///////////// Configure DeviceID ////////////////////////////////////
	//ConfigureDeviceID(m_SAPAddress, DEVICE + ControllerBasAddress);
//|	///////////// Configure DeviceID ////////////////////////////////////
//|
//|	//// Configure Opcodes ////
	//ConfigureList(m_SAPAddress, OPCODE + ControllerBasAddress, GetOpcodeConfiguration());
//|	//// Configure Opcodes ////
//|	//
//|	//// Configure Notifications ////
	//ConfigureList(m_SAPAddress, NOTIFICATION + ControllerBasAddress, GetNotificationConfiguration());
//|	//// Configure Notifications ////
//|	//
//|	//// Configure Image Descriptors ////
	//ConfigureList(m_SAPAddress, IMAGE + ControllerBasAddress, GetControllerConfiguration());
//|	//// Configure Image Descriptors ////
//|	//
//|->///////////// Configure Controller ////////////////////////////////////


//|->///////////// Configure Datapath //////////////////////////////////////
    //VortexConfigurationGroup* DatapathConfigs = new VortexConfigurationGroup(m_SAPAddress);
//|
	//DatapathConfigs->Insert(GetConvolutionKernelConfiguration()->GetConfiguration(), VortexEntityType::SAP);
	//DatapathConfigs->Insert(GetConvolutionImageConfiguration()->GetConfiguration(),VortexEntityType::SAP);    // not needed anymore since the rha controller sends this now - ska - 03/23/2016
	//DatapathConfigs->Insert(GetHistogramMinMaxConfiguration()->GetConfiguration(), VortexEntityType::SAP);    // not needed anymore since we don't use min max anymore - ska - 03/17/2016
	//DatapathConfigs->Insert(GetHistogramPageTableConfiguration()->GetConfiguration(), VortexEntityType::SAP); // not needed anymore since we preload the page table - ska - 03/17/2016
	//DatapathConfigs->Insert(GetHistogramImageConfiguration()->GetConfiguration(), VortexEntityType::SAP);     // not needed anymore since the rha controller sends this now - ska - 03/22/2016
//|
//|	//// Datapath Functional Unit Configurations ////
//|	
//|	
	//GetNetworkInterface()->Configure(DatapathConfigs);

	//delete(DatapathConfigs);
//|->	///////////// Configure Datapath ////////////////////////////////////

	return (m_IsConfigured = true);
}

void hog_engine_v1_00_a::ConfigureDeviceID(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress)
{
    uint8_t* DeviceIDBytes = SAPDevice->GetBytes();
	VortexConfigurationGroup* vcgDeviceID = new VortexConfigurationGroup(SAPDevice, new VortexConfigurationPacket(DeviceIDBytes, 2, SAPAddress), VortexEntityType::SAP);

	//Configure
	GetNetworkInterface()->Configure(vcgDeviceID);
	delete(vcgDeviceID);
}

// Just one opcode configuration
void hog_engine_v1_00_a::ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, hog_engine_v1_00_a_opcode_config* Config)
{
	IVortexConfiguration** ivc = new IVortexConfiguration*[1];
	ivc[0] = Config;

	ConfigureList(SAPDevice, SAPAddress, ivc, 1);

	delete(ivc);
}

// Vector of opcode configurations
void hog_engine_v1_00_a::ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, vector<hog_engine_v1_00_a_opcode_config*> Configs)
{
	IVortexConfiguration** ivc = new IVortexConfiguration*[Configs.size()];
	for (int i = 0; i<Configs.size(); i++)
		ivc[i] = Configs[i];

	ConfigureList(SAPDevice, SAPAddress, ivc, Configs.size());

	delete[](ivc);
}

void hog_engine_v1_00_a::ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, hog_engine_v1_00_a_controller_config* Config)
{
	IVortexConfiguration** ivc = new IVortexConfiguration*[1];
	ivc[0] = Config;

	ConfigureList(SAPDevice, SAPAddress, ivc, 1);

	delete(ivc);
}

void hog_engine_v1_00_a::ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, vector<hog_engine_v1_00_a_controller_config*> Configs)
{
	IVortexConfiguration** ivc = new IVortexConfiguration*[Configs.size()];
	for (int i=0; i<Configs.size(); i++)
		ivc[i] = Configs[i];

	ConfigureList(SAPDevice, SAPAddress, ivc, Configs.size());

	delete[](ivc);
}

void hog_engine_v1_00_a::ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, vector<hog_engine_v1_00_a_notification_config*> Configs)
{
	IVortexConfiguration** ivc = new IVortexConfiguration*[Configs.size()];
	for (int i=0; i<Configs.size(); i++)
		ivc[i] = Configs[i];

	ConfigureList(SAPDevice, SAPAddress, ivc, Configs.size());

	delete[](ivc);
}

void hog_engine_v1_00_a::ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, IVortexConfiguration** Configurations, int NumConfigurations)
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

uint32_t* hog_engine_v1_00_a::Process(VortexMemoryAllocation* ImageHandle, int ImageWidth, int ImageHeight, int KernelWidth, int KernelHeight, int Threshold)
{

	int ConfiguredImageWidth = GetControllerConfiguration()->GetWidth();
	int ConfiguredImageHeight = GetControllerConfiguration()->GetHeight();
	int ConfiguredImageSize = GetControllerConfiguration()->GetImageSizeBytes();
	int ConfiguredThreshold = 0;//GetIPLocalizerConfiguration()->GetThreshold();

	
	if (!m_IsConfigured || (ConfiguredImageWidth != ImageWidth) || (ConfiguredImageHeight != ImageHeight) || (ConfiguredThreshold != Threshold))
	{
		//Reset();

		AcceleratorSetImageConfiguration(
			ImageWidth,
			ImageHeight,
			KernelWidth,
			KernelHeight,
			IMAGE_DESCRIPTOR_FORMAT_COLOR::BGRA,
			IMAGE_DESCRIPTOR_FORMAT_DEPTH::BYTE,
			IMAGE_DESCRIPTOR_TYPE::RAW,
			Threshold);

double start = (double)cv::getTickCount();
		if (!Configure())
			return NULL;
double end = (double)cv::getTickCount();

//double exec_time = end - start;

//cout << __func__ << ": Configuration took " << exec_time/cv::getTickFrequency() << " seconds" << endl;

		ConfiguredImageWidth = GetControllerConfiguration()->GetWidth();
		ConfiguredImageHeight = GetControllerConfiguration()->GetHeight();
		ConfiguredImageSize = GetControllerConfiguration()->GetImageSizeBytes();
	}
	

	VortexImageDescriptor* ImageDescriptor = new VortexImageDescriptor();
	ImageDescriptor->SetIdentifier(0xEEFF); // this is now 10 bit not 16 bit .. need to figure out what impact this has 
	ImageDescriptor->SetWidth(ConfiguredImageWidth);
	ImageDescriptor->SetHeight(ConfiguredImageHeight);
	ImageDescriptor->SetType(IMAGE_DESCRIPTOR_TYPE::RAW);
	ImageDescriptor->SetFormatColor(IMAGE_DESCRIPTOR_FORMAT_COLOR::BGRA);
	ImageDescriptor->SetFormatDepth(IMAGE_DESCRIPTOR_FORMAT_DEPTH::BYTE);
	ImageDescriptor->SetImageHandle(ImageHandle);

	VortexMessage* ProcessMessage = new VortexMessage(
		VortexMessageType::EXECUTE_REQUEST,
		NULL,
		GetSAPAddress(),
		true);

	ProcessMessage->AddBytes(ImageDescriptor->GetBytes(),16);

	ProcessMessage->AddBytes(m_ConvolutionKernelConfig->GetBytes(), 48); // Kernel Coeffs
	ProcessMessage->AddBytes(m_ControllerConfig->GetBytes(), 16); // This contains result info (naming convention should be improved)
	ProcessMessage->AddBytes(m_DeviceConfig->GetBytes(), 16);  // Device ID

	//printf("Sending Message\n");

	GetNetworkInterface()->SendMessage(ProcessMessage);

	printf("RHA: Message Sent\n");

	delete(ProcessMessage);
	delete(ImageDescriptor);

	VortexMessage* ProcessMessageResponse = GetNetworkInterface()->WaitMessage();
	if ((ProcessMessageResponse == NULL) || ProcessMessageResponse->GetType() != VortexMessageType::EXECUTE_COMPLETE)
	{
		delete(ProcessMessageResponse);
		return NULL;
	}

	delete(ProcessMessageResponse);

	return (uint32_t*)(GetControllerConfiguration()->GetResultAddress());
}



