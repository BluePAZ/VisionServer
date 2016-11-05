#include "aim_engine_v2_00_b.h"
#include "aim_engine_v2_00_b_gabor_kernel.h"
#include "VortexImageDescriptor.h"
#include "VortexMessage.h"
#include <cmath>
#include <string>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <unistd.h>



aim_engine_v2_00_b::aim_engine_v2_00_b(string Name, uint8_t BusID, uint8_t SwitchID, uint8_t PortID, IVortexNIFSAP* NIFSAP)
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
    m_PipelineBaseAddress = m_DatapathBaseAddress + 0x1000000;
    /////////// Pipeline Base Address //////////////////////////////////////////////

	/////////// Convolution Engines ////////////////////////////////////////////////
    m_GaborEngineAddress = m_PipelineBaseAddress + (uint64_t)PIPELINE_ADDRESS_RANGE_INDEX::CONVOLUTION;
    m_GaborEngine = new gabor_extractor_v1_00_a(m_GaborEngineAddress);
    /////////// Convolution Engines ////////////////////////////////////////////////

	/////////// Histogram Engines //////////////////////////////////////////////////
    m_HistogramEngineAddress = m_PipelineBaseAddress + (uint64_t)PIPELINE_ADDRESS_RANGE_INDEX::HISTOGRAM;
    m_HistogramEngine = new multichannel_histogram_v1_00_a(m_HistogramEngineAddress, 256, 32);
    /////////// Histogram Engines //////////////////////////////////////////////////

	/////////// Interpolation Engines //////////////////////////////////////////////
    m_InterpolationEngineAddress = m_PipelineBaseAddress + (uint64_t)PIPELINE_ADDRESS_RANGE_INDEX::LOG_LOOKUP;
    m_InterpolationEngine = new multichannel_linear_interpolator_v1_00_a(m_InterpolationEngineAddress, Log, 1, 262143, 18, 0, 5, 28, 0.00001);
    /////////// Interpolation Engines //////////////////////////////////////////////

	m_PipelineLinks.resize(2);
	m_PipelineLinks[0] = new VortexOperatorLink(m_SlaveBaseAddress[1] + 0x0000000, 8);
	m_PipelineLinks[1] = new VortexOperatorLink(m_SlaveBaseAddress[1] + 0x5000000, 32);

	m_AcceleratorConfig = new aim_processor_config();

	SetNetworkInterface(NIFSAP);
	SetSAPAddress(BusID, SwitchID, PortID);
}


aim_engine_v2_00_b::~aim_engine_v2_00_b(void)
{
	delete(m_SAPAddress);
	delete(m_GaborEngine);
	delete(m_HistogramEngine);
	delete(m_InterpolationEngine);
	delete(m_PipelineLinks[0]);
	delete(m_PipelineLinks[1]);

	for(std::vector<aim_notification_config*>::iterator it = m_NotificationConfig.begin(); it != m_NotificationConfig.end(); ++it)
		delete (*it);

	for(std::vector<aim_opcode_config*>::iterator it = m_OpcodeConfig.begin(); it != m_OpcodeConfig.end(); ++it)
		delete (*it);

	GetNetworkInterface()->DeallocateFlowID(GetAcceleratorConfiguration()->GetPixelFetchFlowID());
	GetNetworkInterface()->DeallocateFlowID(GetAcceleratorConfiguration()->GetAccumulatorFetchFlowID());
	GetNetworkInterface()->DeallocateFlowID(GetAcceleratorConfiguration()->GetAccumulatorStoreFlowID());
	GetNetworkInterface()->DeallocateMemory(m_ScratchMemory);

	delete(m_AcceleratorConfig);
}

void aim_engine_v2_00_b::Reset()
{
	GetNetworkInterface()->ResetSAP(m_SAPAddress);
	m_IsConfigured = false;
}

string aim_engine_v2_00_b::GetName()
{
	return m_Name;
}

void aim_engine_v2_00_b::SetNetworkInterface(IVortexNIFSAP* value)
{
	m_NetworkInterface = value;
}

IVortexNIFSAP* aim_engine_v2_00_b::GetNetworkInterface()
{
	return m_NetworkInterface;
}

void aim_engine_v2_00_b::SetSAPAddress(uint8_t BusID, uint8_t SwitchID, uint8_t PortID)
{
	m_SAPAddress = new VortexDeviceAddress(BusID,SwitchID,PortID);
}

VortexDeviceAddress* aim_engine_v2_00_b::GetSAPAddress()
{
	return m_SAPAddress;
}

aim_processor_config* aim_engine_v2_00_b::GetAcceleratorConfiguration()
{
	return m_AcceleratorConfig;
}

void aim_engine_v2_00_b::AcceleratorAddNotificationTarget(uint8_t BusID, uint8_t SwitchID, uint8_t PortID)
{
	m_NotificationConfig.push_back(new aim_notification_config(BusID,SwitchID,PortID));
}

void aim_engine_v2_00_b::AcceleratorSetImageConfiguration(uint16_t ImageWidth, uint16_t ImageHeight, IMAGE_DESCRIPTOR_FORMAT_COLOR FormatColor, IMAGE_DESCRIPTOR_FORMAT_DEPTH FormatDepth, IMAGE_DESCRIPTOR_TYPE FormatType)
{
	GetAcceleratorConfiguration()->SetWidth(ImageWidth);
    GetAcceleratorConfiguration()->SetHeight(ImageHeight);
	GetAcceleratorConfiguration()->SetFormatColor(FormatColor);
	GetAcceleratorConfiguration()->SetFormatDepth(FormatDepth);
	GetAcceleratorConfiguration()->SetType(FormatType);

	//AcceleratorSetNumChannels(GetAcceleratorConfiguration()->GetNumChannelsValid());
	AcceleratorSetNumChannels(1);
}

void aim_engine_v2_00_b::AcceleratorAllocateScratchMemory(uint64_t ScratchSize)
{
	m_ScratchMemory = GetNetworkInterface()->AllocateMemory(ScratchSize);
	VortexDeviceAddress* MemoryDevice = m_ScratchMemory->GetMemoryDeviceID();

	GetAcceleratorConfiguration()->SetResultDeviceID(MemoryDevice->GetBusID(),MemoryDevice->GetSwitchID(),MemoryDevice->GetPortID());
	GetAcceleratorConfiguration()->SetResultAddress((uint64_t)m_ScratchMemory->GetMemoryOffset());
}

void aim_engine_v2_00_b::AcceleratorAllocateFlowID()
{
    uint16_t BaseFlowID = GetNetworkInterface()->AllocateFlowID(3);

    if (BaseFlowID != 0)
        GetAcceleratorConfiguration()->SetBaseFlowID(BaseFlowID);
}

void aim_engine_v2_00_b::AcceleratorSetNumChannels(int NumChannels)
{
	m_NumChannels = NumChannels;
}

void aim_engine_v2_00_b::AcceleratorSetNumOrientations(int NumOrientations)
{
	m_NumOrientations = NumOrientations;
}

void aim_engine_v2_00_b::AcceleratorAddKernel(int KernelWidth, int KernelHeight)
{
	m_KernelScales.push_back(KernelWidth);
}

bool aim_engine_v2_00_b::Configure()
{
	///////////// Retrieve Variables ////////////////////////////////////
	int ImageWidth = GetAcceleratorConfiguration()->GetWidth();
	int ImageHeight = GetAcceleratorConfiguration()->GetHeight();
	int ImageSize = GetAcceleratorConfiguration()->GetImageSizeBytes();

	AcceleratorAllocateFlowID();
	AcceleratorAllocateScratchMemory(ImageSize);

	m_OpcodeConfig = GenerateOpcodes(1, 1);
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

	//// Datapath Functional Unit Configurations ////
	histogram_partition* HistogramGrid = new histogram_partition(1024, 1024, 8,8);
	HistogramGrid->CreateRegularizedGrid(ImageWidth, ImageHeight, 256, 256);

	vector<VortexConfigurationPacket*> GaborEngineConfiguration = m_GaborEngine->GetConfiguration(ImageWidth, ImageHeight);
    vector<VortexConfigurationPacket*> HistogramEngineConfiguration = m_HistogramEngine->GetConfiguration(m_GaborEngine->GetMinMax(),ImageWidth, ImageHeight, HistogramGrid);
    vector<VortexConfigurationPacket*> InterpolationEngineConfiguration = m_InterpolationEngine->GetConfiguration();

    DatapathConfigs->Insert(GaborEngineConfiguration.data(),GaborEngineConfiguration.size(), VortexEntityType::SAP);
    DatapathConfigs->Insert(HistogramEngineConfiguration.data(),HistogramEngineConfiguration.size(), VortexEntityType::SAP);
    DatapathConfigs->Insert(InterpolationEngineConfiguration.data(),InterpolationEngineConfiguration.size(), VortexEntityType::SAP);
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

void aim_engine_v2_00_b::ConfigureDeviceID(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress)
{
    uint8_t* DeviceIDBytes = SAPDevice->GetBytes();
	VortexConfigurationGroup* vcgDeviceID = new VortexConfigurationGroup(SAPDevice, new VortexConfigurationPacket(DeviceIDBytes, 2, SAPAddress), VortexEntityType::SAP);

	//Configure
	GetNetworkInterface()->Configure(vcgDeviceID);
	delete(vcgDeviceID);
}

void aim_engine_v2_00_b::ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, aim_processor_config* Config)
{
	IVortexConfiguration** ivc = new IVortexConfiguration*[1];
	ivc[0] = Config;

	ConfigureList(SAPDevice, SAPAddress, ivc, 1);

	delete(ivc);
}

void aim_engine_v2_00_b::ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, vector<aim_processor_config*> Configs)
{
	IVortexConfiguration** ivc = new IVortexConfiguration*[Configs.size()];
	for (int i=0; i<Configs.size(); i++)
		ivc[i] = Configs[i];

	ConfigureList(SAPDevice, SAPAddress, ivc, Configs.size());

	delete[](ivc);
}

void aim_engine_v2_00_b::ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, vector<aim_opcode_config*> Configs)
{
	IVortexConfiguration** ivc = new IVortexConfiguration*[Configs.size()];
	for (int i=0; i<Configs.size(); i++)
		ivc[i] = Configs[i];

	ConfigureList(SAPDevice, SAPAddress, ivc, Configs.size());

	delete[](ivc);
}

void aim_engine_v2_00_b::ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, vector<aim_notification_config*> Configs)
{
	IVortexConfiguration** ivc = new IVortexConfiguration*[Configs.size()];
	for (int i=0; i<Configs.size(); i++)
		ivc[i] = Configs[i];

	ConfigureList(SAPDevice, SAPAddress, ivc, Configs.size());

	delete[](ivc);
}

void aim_engine_v2_00_b::ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, IVortexConfiguration** Configurations, int NumConfigurations)
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

uint8_t* aim_engine_v2_00_b::Process(VortexMemoryAllocation* ImageHandle, int ImageWidth, int ImageHeight)
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
	ProcessMessageResponse = GetNetworkInterface()->WaitMessage();

	if ((ProcessMessageResponse == NULL) || ProcessMessageResponse->GetType() != VortexMessageType::EXECUTE_COMPLETE)
	{
		delete(ProcessMessageResponse);
		return NULL;
	}

	delete(ProcessMessageResponse);
	usleep(10000);
	return (uint8_t*)(GetAcceleratorConfiguration()->GetResultAddress());
}

vector<aim_opcode_config*> aim_engine_v2_00_b::GenerateOpcodes(int NumOrientations, int NumChannels)
{
    vector<aim_opcode_config*> OpcodeConfigs(NumChannels * NumOrientations * 2);

    for (int c = 0; c < NumChannels; c++)
    {
        for (int o = 0; o < NumOrientations; o++)
        {
            bool IsFirst = (c == 0 && o == 0);
            OpcodeConfigs[c * NumOrientations * 2 + o * 2 + 0] = new aim_opcode_config(AIMMode::BUILD, (uint8_t)o, (uint8_t)c, 0, IsFirst);
            OpcodeConfigs[c * NumOrientations * 2 + o * 2 + 1] = new aim_opcode_config(AIMMode::INDEX, (uint8_t)o, (uint8_t)c, 0, IsFirst);
        }
    }

    return OpcodeConfigs;
}

/*
aim_test_data* aim_engine_v2_00_b::GenerateTestData(vector<vector<vector<uint8_t>>> Image, vector<vector<vector<vector<int16_t>>>> Kernel, string OutputFilepath)
{
    int NumScales = Kernel.size();
    vector<vector<vector<int>>> MinMax(NumScales);

    for (int i=0; i<NumScales; i++)
        MinMax[i] = GetMinMax(Kernel[i]);

    int ImageHeight = Image.size();
	int ImageWidth = Image[0].size();

    histogram_partition* HistogramGrid = new histogram_partition(1024, 1024, 8, 8);
	HistogramGrid->CreateRegularizedGrid(ImageWidth, ImageHeight, 104, 104);

    vector<vector<vector<vector<vector<int64_t>>>>> ConvolvedImages = GenerateConvolutionTestData(Image, Kernel, OutputFilepath);
    vector<vector<vector<vector<vector<int64_t>>>>> HistogramImages = GenerateHistogramTestData(ConvolvedImages, MinMax, HistogramGrid, OutputFilepath);
    vector<vector<vector<vector<vector<int64_t>>>>> LogLookupImages = GenerateLogLookupTestData(HistogramImages, OutputFilepath);
    vector<vector<vector<vector<vector<int64_t>>>>> NegLookupImages = GenerateNegateTestData(LogLookupImages, m_InterpolationEngines[0]->GetOutputWidth(), OutputFilepath);
    vector<vector<vector<vector<vector<int64_t>>>>> ConvertedImages = GenerateFormatConverterTestData(NegLookupImages, 5, 28, 5, 17, OutputFilepath);
    vector<vector<vector<vector<int64_t>>>> AdderImages = GenerateAdderTestData(ConvertedImages, 24, OutputFilepath);
    vector<vector<int64_t>> AccumulatorImages = GenerateAccumulatorTestData(AdderImages, 32, OutputFilepath);
}
*/

/*
vector<vector<int64_t>> aim_engine_v2_00_b::GenerateAccumulatorTestData(vector<vector<vector<vector<int64_t>>>> Image, int Bitwidth, string OutputFilepath)
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
*/

/*
vector<vector<vector<vector<vector<int64_t>>>>> aim_engine_v2_00_b::GenerateFormatConverterTestData(vector<vector<vector<vector<vector<int64_t>>>>> Image, int InputIntegerBits, int InputFractionBits, int OutputIntegerBits, int OutputFractionBits, string OutputFilepath)
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
*/

/*
vector<vector<vector<vector<int64_t>>>> aim_engine_v2_00_b::GenerateAdderTestData(ImageContainer* Image, int Bitwidth, string OutputFilepath)
{
    int NumEngines = Image.size();
    int NumChannels = Image[0].size();
    int NumIterations = Image[0][0].size();

    (int bytesPerChannel, int Dim0, int Dim1=0, int Dim2=0, int Dim3=0, int Dim4=0, int Dim5=0, int Dim6=0, int Dim7=0)

    ImageContainer* AdderResultContainer = new ImageContainer(8,NumChannels,NumIterations,)

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
*/

/*
vector<vector<vector<vector<vector<int64_t>>>>> aim_engine_v2_00_b::GenerateNegateTestData(vector<vector<vector<vector<vector<int64_t>>>>> Image, int BitWidth, string OutputFilepath)
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
*/

/*
vector<vector<vector<vector<vector<int64_t>>>>> aim_engine_v2_00_b::GenerateConvolutionTestData(vector<vector<vector<uint8_t>>> Image, vector<vector<vector<vector<int16_t>>>> Kernel, string OutputFilepath)
{
    int NumEngines = m_GaborEngines.size();
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

                ConvolvedResult[e][c][o] = m_GaborEngines[e]->Convolve(Image, Kernel[e][o], c);
                WriteToFile(ConvolvedResult[e][c][o], NumBits, sw);
                WriteToFile(ConvolvedResult[e][c][o], NumBits, sw);
            }
        }
        sw->close();
    }

    return ConvolvedResult;
}
*/

/*
vector<vector<vector<vector<vector<int64_t>>>>> aim_engine_v2_00_b::GenerateHistogramTestData(vector<vector<vector<vector<vector<int64_t>>>>> Image, vector<vector<vector<int>>> MinMax, histogram_partition* Partition, string OutputFilepath)
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
        m_HistogramEngines[e]->SetPartition(Partition);
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
*/

/*
vector<vector<vector<vector<vector<int64_t>>>>> aim_engine_v2_00_b::GenerateLogLookupTestData(vector<vector<vector<vector<vector<int64_t>>>>> Image, string OutputFilepath)
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
*/

void aim_engine_v2_00_b::WriteToFile(vector<vector<int64_t>> Image, int OutputDatawidth, ofstream* sw)
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

void aim_engine_v2_00_b::WriteToFile(vector<int64_t> Data, int OutputDatawidth, ofstream* sw)
{
    uint64_t Mask = 0xFFFFFFFFFFFFFFFF >> (64 - OutputDatawidth);
    int formatWidth = (int)ceil((float)OutputDatawidth / 4);

    for (int i = 0; i < Data.size(); i++)
    {
		(*sw) << std::ios_base::hex << std::setfill('0') << std::setw(formatWidth) << ((uint64_t)Data[i] & Mask) << endl;
    }
}

void aim_engine_v2_00_b::WriteKernelDataArray(vector<vector<int16_t>> Kernel, string OutputFile)
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

void aim_engine_v2_00_b::WriteInputDataArray(vector<vector<vector<uint8_t>>> Image, int Channel, string OutputFile)
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

double aim_engine_v2_00_b::Log2(double X)
{
	return log(X) / log(2.0);
}

double aim_engine_v2_00_b::Log(double X)
{
	return log(X);
}
