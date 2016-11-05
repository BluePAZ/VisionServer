#include "aim_engine_v1_00_a.h"
#include "AIMGaborKernel.h"
#include "VortexImageDescriptor.h"
#include "VortexMessage.h"
#include <cmath>
#include <string>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>


aim_engine_v1_00_a::aim_engine_v1_00_a(string Name, uint8_t BusID, uint8_t SwitchID, uint8_t PortID, IVortexNIFSAP* NIFSAP)
{
	m_Name = Name;

	m_IsConfigured = false;

	m_SlaveBaseAddress[0] = 0x0000000000000000;
	m_SlaveBaseAddress[1] = 0x0000000400000000;
	m_SlaveBaseAddress[2] = 0x0000000800000000;
	m_SlaveBaseAddress[3] = 0x0000000C00000000;

	m_NumPipelines = 4;

	//Datapath Address
	m_DatapathBaseAddress = m_SlaveBaseAddress[1];

	//Pixel Distribution Address
	m_PixelDistributionAddress = m_DatapathBaseAddress + 0x000000;

	/////////// Pipeline Base Address //////////////////////////////////////////////
    m_PipelineBaseAddress.resize(4);
    m_PipelineBaseAddress[0] = m_DatapathBaseAddress + 0x1000000;
    m_PipelineBaseAddress[1] = m_DatapathBaseAddress + 0x2000000;
    m_PipelineBaseAddress[2] = m_DatapathBaseAddress + 0x3000000;
    m_PipelineBaseAddress[3] = m_DatapathBaseAddress + 0x4000000;
    /////////// Pipeline Base Address //////////////////////////////////////////////

	/////////// Convolution Engines ////////////////////////////////////////////////
    m_ConvolutionEngineAddress.resize(4);
    m_ConvolutionEngineAddress[0] = m_PipelineBaseAddress[0] + (uint64_t)PIPELINE_ADDRESS_RANGE_INDEX::CONVOLUTION;
    m_ConvolutionEngineAddress[1] = m_PipelineBaseAddress[1] + (uint64_t)PIPELINE_ADDRESS_RANGE_INDEX::CONVOLUTION;
    m_ConvolutionEngineAddress[2] = m_PipelineBaseAddress[2] + (uint64_t)PIPELINE_ADDRESS_RANGE_INDEX::CONVOLUTION;
    m_ConvolutionEngineAddress[3] = m_PipelineBaseAddress[3] + (uint64_t)PIPELINE_ADDRESS_RANGE_INDEX::CONVOLUTION;

    m_ConvolutionEngines.resize(4);
    m_ConvolutionEngines[0] = new convolution_v1_00_a(m_ConvolutionEngineAddress[0], 13, 13);
    m_ConvolutionEngines[1] = new convolution_v1_00_a(m_ConvolutionEngineAddress[1], 11, 11);
    m_ConvolutionEngines[2] = new convolution_v1_00_a(m_ConvolutionEngineAddress[2], 9, 9);
    m_ConvolutionEngines[3] = new convolution_v1_00_a(m_ConvolutionEngineAddress[3], 7, 7);
    /////////// Convolution Engines ////////////////////////////////////////////////

	/////////// Histogram Engines //////////////////////////////////////////////////
    m_HistogramEngineAddress.resize(4);
    m_HistogramEngineAddress[0] = m_PipelineBaseAddress[0] + (uint64_t)PIPELINE_ADDRESS_RANGE_INDEX::HISTOGRAM;
    m_HistogramEngineAddress[1] = m_PipelineBaseAddress[1] + (uint64_t)PIPELINE_ADDRESS_RANGE_INDEX::HISTOGRAM;
    m_HistogramEngineAddress[2] = m_PipelineBaseAddress[2] + (uint64_t)PIPELINE_ADDRESS_RANGE_INDEX::HISTOGRAM;
    m_HistogramEngineAddress[3] = m_PipelineBaseAddress[3] + (uint64_t)PIPELINE_ADDRESS_RANGE_INDEX::HISTOGRAM;

    m_HistogramEngines.resize(4);
    m_HistogramEngines[0] = new histogram_v3_00_a(m_HistogramEngineAddress[0], 256, 32);
    m_HistogramEngines[1] = new histogram_v3_00_a(m_HistogramEngineAddress[1], 256, 32);
    m_HistogramEngines[2] = new histogram_v3_00_a(m_HistogramEngineAddress[2], 256, 32);
    m_HistogramEngines[3] = new histogram_v3_00_a(m_HistogramEngineAddress[3], 256, 32);
    /////////// Histogram Engines //////////////////////////////////////////////////

	/////////// Interpolation Engines //////////////////////////////////////////////
    m_InterpolationEngineAddress.resize(4);
    m_InterpolationEngineAddress[0] = m_PipelineBaseAddress[0] + (uint64_t)PIPELINE_ADDRESS_RANGE_INDEX::LOG_LOOKUP;
    m_InterpolationEngineAddress[1] = m_PipelineBaseAddress[1] + (uint64_t)PIPELINE_ADDRESS_RANGE_INDEX::LOG_LOOKUP;
    m_InterpolationEngineAddress[2] = m_PipelineBaseAddress[2] + (uint64_t)PIPELINE_ADDRESS_RANGE_INDEX::LOG_LOOKUP;
    m_InterpolationEngineAddress[3] = m_PipelineBaseAddress[3] + (uint64_t)PIPELINE_ADDRESS_RANGE_INDEX::LOG_LOOKUP;

    m_InterpolationEngines.resize(4);
    m_InterpolationEngines[0] = new linear_interpolator_v1_00_a(m_InterpolationEngineAddress[0],Log, 1, 262143, 18, 0, 5, 28, 0.00001);
	m_InterpolationEngines[1] = new linear_interpolator_v1_00_a(m_InterpolationEngineAddress[1],m_InterpolationEngines[0]->GetCoefficientTable(),m_InterpolationEngines[0]->GetRangeTable());
	m_InterpolationEngines[2] = new linear_interpolator_v1_00_a(m_InterpolationEngineAddress[2],m_InterpolationEngines[0]->GetCoefficientTable(),m_InterpolationEngines[0]->GetRangeTable());
	m_InterpolationEngines[3] = new linear_interpolator_v1_00_a(m_InterpolationEngineAddress[3],m_InterpolationEngines[0]->GetCoefficientTable(),m_InterpolationEngines[0]->GetRangeTable());
	//m_InterpolationEngines[1] = new linear_interpolator_v1_00_a(m_InterpolationEngineAddress[1],log, 1, 262143, 18, 0, 5, 28, 0.00001);
    //m_InterpolationEngines[2] = new linear_interpolator_v1_00_a(m_InterpolationEngineAddress[2],log, 1, 262143, 18, 0, 5, 28, 0.00001);
    //m_InterpolationEngines[3] = new linear_interpolator_v1_00_a(m_InterpolationEngineAddress[3],log, 1, 262143, 18, 0, 5, 28, 0.00001);
    /////////// Interpolation Engines //////////////////////////////////////////////

	m_PipelineLinks.resize(2);
	m_PipelineLinks[0] = new VortexOperatorLink(m_SlaveBaseAddress[1] + 0x0000000, 8);
	m_PipelineLinks[1] = new VortexOperatorLink(m_SlaveBaseAddress[1] + 0x5000000, 32);

	m_AcceleratorConfig = new AIMProcessorConfig();

	SetNetworkInterface(NIFSAP);
	SetSAPAddress(BusID, SwitchID, PortID);
}


aim_engine_v1_00_a::~aim_engine_v1_00_a(void)
{
	delete(m_SAPAddress);

	delete(m_ConvolutionEngines[0]);
    delete(m_ConvolutionEngines[1]);
	delete(m_ConvolutionEngines[2]);
	delete(m_ConvolutionEngines[3]);

	delete(m_HistogramEngines[0]);
	delete(m_HistogramEngines[1]);
	delete(m_HistogramEngines[2]);
	delete(m_HistogramEngines[3]);

	delete(m_InterpolationEngines[0]);
	delete(m_InterpolationEngines[1]);
	delete(m_InterpolationEngines[2]);
	delete(m_InterpolationEngines[3]);

	delete(m_PipelineLinks[0]);
	delete(m_PipelineLinks[1]);

	for(std::vector<AIMNotificationConfig*>::iterator it = m_NotificationConfig.begin(); it != m_NotificationConfig.end(); ++it)
		delete (*it);

	for(std::vector<AIMOpcodeConfig*>::iterator it = m_OpcodeConfig.begin(); it != m_OpcodeConfig.end(); ++it)
		delete (*it);

	GetNetworkInterface()->DeallocateFlowID(GetAcceleratorConfiguration()->GetPixelFetchFlowID());
	GetNetworkInterface()->DeallocateFlowID(GetAcceleratorConfiguration()->GetAccumulatorFetchFlowID());
	GetNetworkInterface()->DeallocateFlowID(GetAcceleratorConfiguration()->GetAccumulatorStoreFlowID());
	GetNetworkInterface()->DeallocateMemory(m_ScratchMemory);

	delete(m_AcceleratorConfig);
}

void aim_engine_v1_00_a::Reset()
{
	GetNetworkInterface()->ResetSAP(m_SAPAddress);
	m_IsConfigured = false;
}

string aim_engine_v1_00_a::GetName()
{
	return m_Name;
}

void aim_engine_v1_00_a::SetNetworkInterface(IVortexNIFSAP* value)
{
	m_NetworkInterface = value;
}

IVortexNIFSAP* aim_engine_v1_00_a::GetNetworkInterface()
{
	return m_NetworkInterface;
}

void aim_engine_v1_00_a::SetSAPAddress(uint8_t BusID, uint8_t SwitchID, uint8_t PortID)
{
	m_SAPAddress = new VortexDeviceAddress(BusID,SwitchID,PortID);
}

VortexDeviceAddress* aim_engine_v1_00_a::GetSAPAddress()
{
	return m_SAPAddress;
}

AIMProcessorConfig* aim_engine_v1_00_a::GetAcceleratorConfiguration()
{
	return m_AcceleratorConfig;
}

void aim_engine_v1_00_a::AcceleratorAddNotificationTarget(uint8_t BusID, uint8_t SwitchID, uint8_t PortID)
{
	m_NotificationConfig.push_back(new AIMNotificationConfig(BusID,SwitchID,PortID));
}

void aim_engine_v1_00_a::AcceleratorSetImageConfiguration(uint16_t ImageWidth, uint16_t ImageHeight, IMAGE_DESCRIPTOR_FORMAT_COLOR FormatColor, IMAGE_DESCRIPTOR_FORMAT_DEPTH FormatDepth, IMAGE_DESCRIPTOR_TYPE FormatType)
{
	GetAcceleratorConfiguration()->SetWidth(ImageWidth);
    GetAcceleratorConfiguration()->SetHeight(ImageHeight);
	GetAcceleratorConfiguration()->SetFormatColor(FormatColor);
	GetAcceleratorConfiguration()->SetFormatDepth(FormatDepth);
	GetAcceleratorConfiguration()->SetType(FormatType);

	AcceleratorSetNumChannels(GetAcceleratorConfiguration()->GetNumChannelsValid());
}

void aim_engine_v1_00_a::AcceleratorAllocateScratchMemory(uint64_t ScratchSize)
{
	m_ScratchMemory = GetNetworkInterface()->AllocateMemory(ScratchSize);
	VortexDeviceAddress* MemoryDevice = m_ScratchMemory->GetMemoryDeviceID();

	GetAcceleratorConfiguration()->SetResultDeviceID(MemoryDevice->GetBusID(),MemoryDevice->GetSwitchID(),MemoryDevice->GetPortID());
	GetAcceleratorConfiguration()->SetResultAddress((uint64_t)m_ScratchMemory->GetMemoryOffset());
}

void aim_engine_v1_00_a::AcceleratorAllocateFlowID()
{
    uint16_t BaseFlowID = GetNetworkInterface()->AllocateFlowID(3);

    if (BaseFlowID != 0)
        GetAcceleratorConfiguration()->SetBaseFlowID(BaseFlowID);
}

void aim_engine_v1_00_a::AcceleratorSetNumChannels(int NumChannels)
{
	m_NumChannels = NumChannels;
}

void aim_engine_v1_00_a::AcceleratorSetNumOrientations(int NumOrientations)
{
	m_NumOrientations = NumOrientations;
}

void aim_engine_v1_00_a::AcceleratorAddKernel(int KernelWidth, int KernelHeight)
{
	m_KernelScales.push_back(KernelWidth);
}

bool aim_engine_v1_00_a::Configure()
{
	///////////// Retrieve Variables ////////////////////////////////////
	int ImageWidth = GetAcceleratorConfiguration()->GetWidth();
	int ImageHeight = GetAcceleratorConfiguration()->GetHeight();
	int ImageSize = GetAcceleratorConfiguration()->GetImageSizeBytes();

	AcceleratorAllocateFlowID();
	AcceleratorAllocateScratchMemory(ImageSize);

	m_OpcodeConfig = GenerateOpcodes(m_NumOrientations, m_NumChannels);
	///////////// Retrieve Variables ////////////////////////////////////

	///////////// Configure Controller ////////////////////////////////////

	///////////// Configure DeviceID ////////////////////////////////////
	ConfigureDeviceID(m_SAPAddress, m_SlaveBaseAddress[0]);
	///////////// Configure DeviceID ////////////////////////////////////

	//// Configure Opcodes ////
	ConfigureList(m_SAPAddress, 0x100000 + m_SlaveBaseAddress[0], m_OpcodeConfig);
	//// Configure Opcodes ////

	//// Configure Notifications ////
	ConfigureList(m_SAPAddress, 0x200000 + m_SlaveBaseAddress[0], m_NotificationConfig);
	//// Configure Notifications ////

	//// Configure Image Descriptors ////
	ConfigureList(m_SAPAddress, 0x300000 + m_SlaveBaseAddress[0], m_AcceleratorConfig);
	//// Configure Image Descriptors ////

	///////////// Configure Controller ////////////////////////////////////

    ///////////// Configure Datapath //////////////////////////////////////
    VortexConfigurationGroup* DatapathConfigs = new VortexConfigurationGroup(m_SAPAddress);

	//// Generate kernels ////
	vector<vector<vector<vector<int16_t>>>> Kernels = AIMGaborKernel::CreateGaborFilterBank(m_KernelScales, m_NumOrientations);
	vector<vector<vector<int>>> MinMax = GetMinMaxMultiple(Kernels);
	//// Generate kernels ////

	//// Datapath Functional Unit Configurations ////
	HistogramPartition* HistogramGrid = new HistogramPartition(1024, 1024, 8, 8);
	HistogramGrid->CreateRegularizedGrid(ImageWidth, ImageHeight, 104, 104);

	for (int i=0; i<Kernels.size(); i++)
	{
		vector<VortexConfigurationPacket*> ConvolutionEnginesConfiguration = m_ConvolutionEngines[i]->GetConfiguration(Kernels[i], ImageWidth, ImageHeight);
		vector<VortexConfigurationPacket*> HistogramEnginesConfiguration = m_HistogramEngines[i]->GetConfiguration(MinMax[i],ImageWidth, ImageHeight, HistogramGrid);
		vector<VortexConfigurationPacket*> InterpolationEnginesConfiguration = m_InterpolationEngines[i]->GetConfiguration();

		DatapathConfigs->Insert(ConvolutionEnginesConfiguration.data(),ConvolutionEnginesConfiguration.size(), VortexEntityType::SAP);
		DatapathConfigs->Insert(HistogramEnginesConfiguration.data(),HistogramEnginesConfiguration.size(), VortexEntityType::SAP);
		DatapathConfigs->Insert(InterpolationEnginesConfiguration.data(),InterpolationEnginesConfiguration.size(), VortexEntityType::SAP);
	}
	//// Datapath Functional Unit Configurations ////

	//// Datapath Link Configurations ////
	m_PipelineLinks[0]->SetIndexConfiguration(0, (uint64_t)(ImageWidth * ImageHeight),VortexEntityType::SAP);
	m_PipelineLinks[1]->SetIndexConfiguration(0, (uint64_t)(ImageWidth * ImageHeight * 4),VortexEntityType::SAP);

	for (int i = 0; i < m_PipelineLinks[0]->GetConfiguration().size(); i++)
	    DatapathConfigs->Insert(m_PipelineLinks[0]->GetConfiguration()[i]);

	for (int i = 0; i < m_PipelineLinks[1]->GetConfiguration().size(); i++)
	    DatapathConfigs->Insert(m_PipelineLinks[1]->GetConfiguration()[i]);

	//// Datapath Link Configurations ////

	///////////// Configure Datapath ////////////////////////////////////
	GetNetworkInterface()->Configure(DatapathConfigs);
	///////////// Configure Datapath ////////////////////////////////////

	delete(DatapathConfigs);
	delete(HistogramGrid);

	return (m_IsConfigured = true);
}

void aim_engine_v1_00_a::ConfigureDeviceID(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress)
{
    uint8_t* DeviceIDBytes = SAPDevice->GetBytes();
	VortexConfigurationGroup* vcgDeviceID = new VortexConfigurationGroup(SAPDevice, new VortexConfigurationPacket(DeviceIDBytes, 2, SAPAddress), VortexEntityType::SAP);

	//Configure
	GetNetworkInterface()->Configure(vcgDeviceID);
	delete(vcgDeviceID);
}

void aim_engine_v1_00_a::ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, AIMProcessorConfig* Config)
{
	IVortexConfiguration** ivc = new IVortexConfiguration*[1];
	ivc[0] = Config;

	ConfigureList(SAPDevice, SAPAddress, ivc, 1);

	delete(ivc);
}

void aim_engine_v1_00_a::ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, vector<AIMProcessorConfig*> Configs)
{
	IVortexConfiguration** ivc = new IVortexConfiguration*[Configs.size()];
	for (int i=0; i<Configs.size(); i++)
		ivc[i] = Configs[i];

	ConfigureList(SAPDevice, SAPAddress, ivc, Configs.size());

	delete[](ivc);
}

void aim_engine_v1_00_a::ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, vector<AIMOpcodeConfig*> Configs)
{
	IVortexConfiguration** ivc = new IVortexConfiguration*[Configs.size()];
	for (int i=0; i<Configs.size(); i++)
		ivc[i] = Configs[i];

	ConfigureList(SAPDevice, SAPAddress, ivc, Configs.size());

	delete[](ivc);
}

void aim_engine_v1_00_a::ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, vector<AIMNotificationConfig*> Configs)
{
	IVortexConfiguration** ivc = new IVortexConfiguration*[Configs.size()];
	for (int i=0; i<Configs.size(); i++)
		ivc[i] = Configs[i];

	ConfigureList(SAPDevice, SAPAddress, ivc, Configs.size());

	delete[](ivc);
}

void aim_engine_v1_00_a::ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, IVortexConfiguration** Configurations, int NumConfigurations)
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

uint8_t* aim_engine_v1_00_a::Process(VortexMemoryAllocation* ImageHandle, int ImageWidth, int ImageHeight)
{

	int ConfiguredImageWidth = GetAcceleratorConfiguration()->GetWidth();
	int ConfiguredImageHeight = GetAcceleratorConfiguration()->GetHeight();
    int ConfiguredImageSize = GetAcceleratorConfiguration()->GetImageSizeBytes();


	if (!m_IsConfigured || (ConfiguredImageWidth != ImageWidth) || (ConfiguredImageHeight != ImageHeight))
	{
		//Reset();

		AcceleratorSetImageConfiguration(
			ImageWidth,
			ImageHeight,
			IMAGE_DESCRIPTOR_FORMAT_COLOR::BGRA,
			IMAGE_DESCRIPTOR_FORMAT_DEPTH::BYTE,
			IMAGE_DESCRIPTOR_TYPE::RAW);

		if (!Configure())
			return NULL;

		ConfiguredImageWidth = GetAcceleratorConfiguration()->GetWidth();
		ConfiguredImageHeight = GetAcceleratorConfiguration()->GetHeight();
		ConfiguredImageSize = GetAcceleratorConfiguration()->GetImageSizeBytes();
	}


	VortexImageDescriptor* ImageDescriptor = new VortexImageDescriptor();
	ImageDescriptor->SetIdentifier(0xEEFF);
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
	GetNetworkInterface()->SendMessage(ProcessMessage);

	delete(ProcessMessage);
	delete(ImageDescriptor);

	VortexMessage* ProcessMessageResponse = GetNetworkInterface()->WaitMessage();
	if ((ProcessMessageResponse == NULL) || ProcessMessageResponse->GetType() != VortexMessageType::EXECUTE_COMPLETE)
	{
		delete(ProcessMessageResponse);
		return NULL;
	}

	delete(ProcessMessageResponse);

	return (uint8_t*)(GetAcceleratorConfiguration()->GetResultAddress());
}

vector<vector<vector<int>>> aim_engine_v1_00_a::GetMinMaxMultiple(vector<vector<vector<vector<int16_t>>>> KernelValues)
{
	int NumScales = KernelValues.size();

    vector<vector<vector<int>>> MinMax(NumScales);

	for (int i = 0; i < NumScales; i++)
	{
		MinMax[i] = GetMinMax(KernelValues[i]);
	}

	return MinMax;
}

vector<vector<int>> aim_engine_v1_00_a::GetMinMax(vector<vector<vector<int16_t>>> KernelValues)
{
    int MAX_PIXEL_VALUE = 255;
    int NumOrientations = KernelValues.size();

    vector<vector<int>> MinMax(NumOrientations);

    for (int o = 0; o < NumOrientations; o++)
    {
        int Height = KernelValues[o].size();
        int Width = KernelValues[o][0].size();

		MinMax[o].resize(3);
        MinMax[o][0] = o;
        for (int y = 0; y < Height; y++)
        {
            for (int x = 0; x < Width; x++)
            {
                if (KernelValues[o][y][x] < 0)
                    MinMax[o][1] += KernelValues[o][y][x] * MAX_PIXEL_VALUE;
                else
                    MinMax[o][2] += KernelValues[o][y][x] * MAX_PIXEL_VALUE;
            }
        }
    }

    return MinMax;
}

vector<AIMOpcodeConfig*> aim_engine_v1_00_a::GenerateOpcodes(int NumOrientations, int NumChannels)
{
    vector<AIMOpcodeConfig*> OpcodeConfigs(NumChannels * NumOrientations * 2);

    for (int c = 0; c < NumChannels; c++)
    {
        for (int o = 0; o < NumOrientations; o++)
        {
            bool IsFirst = (c == 0 && o == 0);
            OpcodeConfigs[c * NumOrientations * 2 + o * 2 + 0] = new AIMOpcodeConfig(AIMMode::BUILD, (uint8_t)o, (uint8_t)c, 0, IsFirst);
            OpcodeConfigs[c * NumOrientations * 2 + o * 2 + 1] = new AIMOpcodeConfig(AIMMode::INDEX, (uint8_t)o, (uint8_t)c, 0, IsFirst);
        }
    }

    return OpcodeConfigs;
}

vector<vector<int64_t>> aim_engine_v1_00_a::GenerateAccumulatorTestData(vector<vector<vector<vector<int64_t>>>> Image, int Bitwidth, string OutputFilepath)
{
    int NumChannels = 3;//Image.size();
    int NumIterations = Image[0].size();

    int Height = Image[0][0].size();
    int Width = Image[0][0][0].size();

    vector<vector<int64_t>> Accumulator(Height);
	for (int r=0; r<Height; r++)
		Accumulator[r].resize(Width);

    ofstream* sw =  new ofstream(OutputFilepath + "Accumulator.dat");

    for (int c = 0; c < NumChannels; c++)
    {
        for (int i = 0; i < NumIterations; i++)
        {
            for (int y = 0; y < Height; y++)
            {
                for (int x = 0; x < Width; x++)
                {
                    Accumulator[y][x] += Image[c][i][y][x];
                }
            }
            WriteToFile(Accumulator, Bitwidth, sw);
        }
    }

    sw->close();

    return Accumulator;
}

vector<vector<vector<vector<vector<int64_t>>>>> aim_engine_v1_00_a::GenerateFormatConverterTestData(vector<vector<vector<vector<vector<int64_t>>>>> Image, int InputIntegerBits, int InputFractionBits, int OutputIntegerBits, int OutputFractionBits, string OutputFilepath)
{
    int NumEngines = Image.size();
    int NumChannels = Image[0].size();
    int NumIterations = Image[0][0].size();
    vector<vector<vector<vector<vector<int64_t>>>>> ConvertedResult(NumEngines);

    int lshift = (InputFractionBits < OutputFractionBits) ? OutputFractionBits - InputFractionBits : 0;
    int rshift = (InputFractionBits > OutputFractionBits) ? InputFractionBits - OutputFractionBits : 0;

    int BitWidth = OutputFractionBits + OutputIntegerBits;

    for (int e = 0; e < NumEngines; e++)
    {
		std::ostringstream path;
		path << OutputFilepath << "Converted_" << std::setfill('0') << std::setw(2) << e << ".dat";
        ofstream* sw = new ofstream(path.str());

        ConvertedResult[e].resize(NumChannels);

        for (int c = 0; c < NumChannels; c++)
        {
            ConvertedResult[e][c].resize(NumIterations);
            for (int o = 0; o < NumIterations; o++)
            {
                int Height = Image[e][c][o].size();
                int Width = Image[e][c][o][0].size();
                ConvertedResult[e][c][o].resize(Height);

                for (int h = 0; h < Height; h++)
                {
					ConvertedResult[e][c][o][h].resize(Width);
                    for (int w = 0; w < Width; w++)
                    {
                        ConvertedResult[e][c][o][h][w] = (Image[e][c][o][h][w] >> rshift) << lshift;
                    }
                }
                WriteToFile(ConvertedResult[e][c][o], BitWidth, sw);
            }
        }
        sw->close();
    }

    return ConvertedResult;
}

vector<vector<vector<vector<int64_t>>>> aim_engine_v1_00_a::GenerateAdderTestData(vector<vector<vector<vector<vector<int64_t>>>>> Image, int Bitwidth, string OutputFilepath)
{
    int NumEngines = Image.size();
    int NumChannels = Image[0].size();
    int NumIterations = Image[0][0].size();
    vector<vector<vector<vector<int64_t>>>> AdderResult(NumChannels);

	ofstream* sw = new ofstream(OutputFilepath + "Adder.dat");

    for (int c = 0; c < NumChannels; c++)
    {
        AdderResult[c].resize(NumIterations);
        for (int o = 0; o < NumIterations; o++)
        {
            int Height = Image[0][c][o].size();
            int Width = Image[0][c][o][0].size();

            AdderResult[c][o].resize(Height);

            for (int h = 0; h < Height; h++)
            {
				AdderResult[c][o][h].resize(Width);
                for (int w = 0; w < Width; w++)
                {
                    int x = 0;
                    for (int e = 0; e < NumEngines; e++)
                    {
                        AdderResult[c][o][h][w] += Image[e][c][o][h][w];
                    }
                }
            }
            WriteToFile(AdderResult[c][o], Bitwidth, sw);
        }
    }

    sw->close();

    return AdderResult;
}

vector<vector<vector<vector<vector<int64_t>>>>> aim_engine_v1_00_a::GenerateNegateTestData(vector<vector<vector<vector<vector<int64_t>>>>> Image, int BitWidth, string OutputFilepath)
{
    int NumEngines = Image.size();
    int NumChannels = Image[0].size();
    int NumIterations = Image[0][0].size();
    vector<vector<vector<vector<vector<int64_t>>>>> NegatedResult(NumEngines);

    for (int e = 0; e < NumEngines; e++)
    {
		std::ostringstream path;
		path << OutputFilepath << "Negate_" << std::setfill('0') << std::setw(2) << e << ".dat";
        ofstream* sw = new ofstream(path.str());

        NegatedResult[e].resize(NumChannels);

        for (int c = 0; c < NumChannels; c++)
        {
            NegatedResult[e][c].resize(NumIterations);
            for (int o = 0; o < NumIterations; o++)
            {
                int Height = Image[e][c][o].size();
                int Width = Image[e][c][o][0].size();
                NegatedResult[e][c][o].resize(Height);

                for (int h = 0; h < Height; h++)
                {
					NegatedResult[e][c][o][h].resize(Width);
                    for (int w = 0; w < Width; w++)
                    {
                        NegatedResult[e][c][o][h][w] = -Image[e][c][o][h][w];
                    }
                }
                WriteToFile(NegatedResult[e][c][o], BitWidth, sw);
            }
        }
        sw->close();
    }

    return NegatedResult;
}

vector<vector<vector<vector<vector<int64_t>>>>> aim_engine_v1_00_a::GenerateConvolutionTestData(vector<vector<vector<uint8_t>>> Image, vector<vector<vector<vector<int16_t>>>> Kernel, string OutputFilepath)
{
    int NumEngines = m_ConvolutionEngines.size();
    int NumIterations = Kernel[0].size();
    int NumChannels = Image[0][0].size();
    vector<vector<vector<vector<vector<int64_t>>>>> ConvolvedResult(NumEngines);

    for (int e = 0; e < NumEngines; e++)
    {
		std::ostringstream path;
		path << OutputFilepath << "ConvolverOutput_" << std::setfill('0') << std::setw(2) << e << ".dat";
        ofstream* sw = new ofstream(path.str());

        ConvolvedResult[e].resize(NumChannels);

        for (int c = 0; c < NumChannels; c++)
        {
            ConvolvedResult[e][c].resize(NumIterations);
            for (int o = 0; o < NumIterations; o++)
            {
                int KernelHeight = Kernel[e][o].size();
                int KernelWidth = Kernel[e][o][0].size();
                int NumBits = 24 + (int)ceil(Log2(KernelWidth * KernelHeight));

                ConvolvedResult[e][c][o] = m_ConvolutionEngines[e]->Convolve(Image, Kernel[e][o], c);
                WriteToFile(ConvolvedResult[e][c][o], NumBits, sw);
                WriteToFile(ConvolvedResult[e][c][o], NumBits, sw);
            }
        }
        sw->close();
    }

    return ConvolvedResult;
}

vector<vector<vector<vector<vector<int64_t>>>>> aim_engine_v1_00_a::GenerateHistogramTestData(vector<vector<vector<vector<vector<int64_t>>>>> Image, vector<vector<vector<int>>> MinMax, string OutputFilepath)
{
    int NumEngines = Image.size();
    int NumChannels = Image[0].size();
    int NumIterations = Image[0][0].size();
    vector<vector<vector<vector<vector<int64_t>>>>> BackProjectedResult(NumEngines);

    for (int e = 0; e < NumEngines; e++)
    {
		std::ostringstream pathIndexMode;
		pathIndexMode << OutputFilepath << "HistogramIndex_" << std::setfill('0') << std::setw(2) << e << ".dat";
        ofstream* swIndexMode = new ofstream(pathIndexMode.str());

		std::ostringstream pathBuildMode;
		pathBuildMode << OutputFilepath << "HistogramBuild_" << std::setfill('0') << std::setw(2) << e << ".dat";
        ofstream* swBuildMode = new ofstream(pathBuildMode.str());

        BackProjectedResult[e].resize(NumChannels);

        for (int c = 0; c < NumChannels; c++)
        {
            BackProjectedResult[e][c].resize(NumIterations);
            for (int o = 0; o < NumIterations; o++)
            {
                vector<int64_t> HistogramBuildIndices = m_HistogramEngines[e]->BuildHistogram(Image[e][c][o], MinMax[e][o][1], MinMax[e][o][2]);
                BackProjectedResult[e][c][o] = m_HistogramEngines[e]->IndexHistogram(Image[e][c][o], MinMax[e][o][1], MinMax[e][o][2]);
                WriteToFile(BackProjectedResult[e][c][o], 32, swIndexMode);
                WriteToFile(HistogramBuildIndices, m_HistogramEngines[e]->GetHistogramIndexWidth(), swBuildMode);
            }
        }
        swIndexMode->close();
        swBuildMode->close();
    }

    return BackProjectedResult;
}

vector<vector<vector<vector<vector<int64_t>>>>> aim_engine_v1_00_a::GenerateLogLookupTestData(vector<vector<vector<vector<vector<int64_t>>>>> Image, string OutputFilepath)
{
    int NumEngines = Image.size();
    int NumChannels = Image[0].size();
    int NumIterations = Image[0][0].size();
    vector<vector<vector<vector<vector<int64_t>>>>> LogLookupResult(NumEngines);

    for (int e = 0; e < NumEngines; e++)
    {
        std::ostringstream path;
		path << OutputFilepath << "LogLookup_" << std::setfill('0') << std::setw(2) << e << ".dat";
        ofstream* sw = new ofstream(path.str());

        LogLookupResult[e].resize(NumChannels);

        for (int c = 0; c < NumChannels; c++)
        {
            LogLookupResult[e][c].resize(NumIterations);
            for (int o = 0; o < NumIterations; o++)
            {
                LogLookupResult[e][c][o] = m_InterpolationEngines[e]->Interpolate(Image[e][c][o]);
                WriteToFile(LogLookupResult[e][c][o], m_InterpolationEngines[e]->GetOutputWidth(), sw);
            }
        }
        sw->close();
    }

    return LogLookupResult;
}

void aim_engine_v1_00_a::WriteToFile(vector<vector<int64_t>> Image, int OutputDatawidth, ofstream* sw)
{
    int ImageHeight = Image.size();
    int ImageWidth = Image[0].size();
    uint64_t Mask = 0xFFFFFFFFFFFFFFFF >> (64 - OutputDatawidth);
	int formatWidth = (int)ceil((float)OutputDatawidth/4);

    for (int y = 0; y < ImageHeight; y++)
    {
        for (int x = 0; x < ImageWidth; x++)
        {
			(*sw) << std::ios_base::hex << std::setfill('0') << std::setw(formatWidth) << ((uint64_t)Image[y][x] & Mask) << endl;
        }
    }
}

void aim_engine_v1_00_a::WriteToFile(vector<int64_t> Data, int OutputDatawidth, ofstream* sw)
{
    uint64_t Mask = 0xFFFFFFFFFFFFFFFF >> (64 - OutputDatawidth);
    int formatWidth = (int)ceil((float)OutputDatawidth / 4);

    for (int i = 0; i < Data.size(); i++)
    {
		(*sw) << std::ios_base::hex << std::setfill('0') << std::setw(formatWidth) << ((uint64_t)Data[i] & Mask) << endl;
    }
}

void aim_engine_v1_00_a::WriteKernelDataArray(vector<vector<int16_t>> Kernel, string OutputFile)
{
    int CoefficientWidth = 16;
    int KernelHeight = Kernel.size();
    int KernelWidth = Kernel[0].size();

    uint64_t Mask = 0xFFFFFFFFFFFFFFFF >> (64 - CoefficientWidth);

    ofstream* sw = new ofstream(OutputFile);

    int formatWidth = CoefficientWidth / 4;

    for (int y = 0; y < KernelHeight; y++)
    {
        for (int x = 0; x < KernelWidth; x++)
        {
			(*sw) << std::ios_base::hex << std::setfill('0') << std::setw(formatWidth) << ((uint64_t)Kernel[y][x] & Mask);
        }
        (*sw) << endl;
    }

    sw->close();
}

void aim_engine_v1_00_a::WriteInputDataArray(vector<vector<vector<uint8_t>>> Image, int Channel, string OutputFile)
{
    int PixelWidth = 8;
    int ImageHeight = Image.size();
    int ImageWidth = Image[0].size();

    uint64_t Mask = 0xFFFFFFFFFFFFFFFF >> (64 - PixelWidth);

    ofstream* sw = new ofstream(OutputFile);

    int formatWidth = PixelWidth / 4;

    for (int y = 0; y < ImageHeight; y++)
    {
        for (int x = 0; x < ImageWidth; x++)
        {
			(*sw) << std::ios_base::hex << std::setfill('0') << std::setw(formatWidth) << ((uint64_t)Image[y][x][Channel] & Mask) << ",";
        }
        (*sw) << endl;
    }

    sw->close();
}

double aim_engine_v1_00_a::Log2(double X)
{
	return log(X) / log(2.0);
}

double aim_engine_v1_00_a::Log(double X)
{
	return log(X);
}
