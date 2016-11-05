#ifndef __AIM_ENGINE_V2_00_B_H__
#define __AIM_ENGINE_V2_00_B_H__

#include <VortexTypes.h>
#include <VortexSAPTypes.h>
#include <VortexSOPTypes.h>
#include "gabor_extractor_v1_00_a.h"
#include "multichannel_histogram_v1_00_a.h"
#include "multichannel_linear_interpolator_v1_00_a.h"
#include "aim_engine_v2_00_b_configuration_list.h"
#include "aim_engine_v2_00_b_test_data.h"
#include "aim_engine_v2_00_b_opcode_config.h"
#include "aim_engine_v2_00_b_processor_config.h"
#include "aim_engine_v2_00_b_notification_config.h"

#ifndef PIPELINEADDR_PIPELINEADDR__
#define PIPELINEADDR_PIPELINEADDR__
enum class PIPELINE_ADDRESS_RANGE_INDEX : uint64_t { CONVOLUTION = 0x0000000000000000, HISTOGRAM = 0x0000000000400000, LOG_LOOKUP = 0x0000000000800000};
#endif
class aim_engine_v2_00_b
{
public:
	aim_engine_v2_00_b(string Name, uint8_t BusID, uint8_t SwitchID, uint8_t PortID, IVortexNIFSAP* NIFSAP);
	~aim_engine_v2_00_b(void);

	string GetName();

	IVortexNIFSAP* GetNetworkInterface();
	VortexDeviceAddress* GetSAPAddress();

	aim_processor_config* GetAcceleratorConfiguration();
	void AcceleratorSetNumOrientations(int NumOrientations);
	void AcceleratorAddKernel(int KernelWidth, int KernelHeight);
	void AcceleratorAddNotificationTarget(uint8_t BusID, uint8_t SwitchID, uint8_t PortID);
	void AcceleratorSetImageConfiguration(uint16_t ImageWidth, uint16_t ImageHeight, IMAGE_DESCRIPTOR_FORMAT_COLOR FormatColor, IMAGE_DESCRIPTOR_FORMAT_DEPTH FormatDepth, IMAGE_DESCRIPTOR_TYPE FormatType);

	void Reset();
	bool Configure();
	uint8_t* Process(VortexMemoryAllocation* ImageHandle, int ImageWidth, int ImageHeight);
	//aim_test_data* GenerateTestData(ImageContainer* Image, vector<vector<vector<vector<int16_t> > > > Kernel, string OutputFilepath);

private:
	string m_Name;

	bool m_IsConfigured;
	int m_NumOrientations;
	int m_NumChannels;
	int m_NumPipelines;
	vector<int> m_KernelScales;

	VortexMemoryAllocation* m_ScratchMemory;
	IVortexNIFSAP* m_NetworkInterface;
	VortexDeviceAddress* m_SAPAddress;

	gabor_extractor_v1_00_a* m_GaborEngine;
	multichannel_histogram_v1_00_a* m_HistogramEngine;
	multichannel_linear_interpolator_v1_00_a* m_InterpolationEngine;
	vector<VortexOperatorLink*> m_PipelineLinks;

	uint64_t m_SlaveBaseAddress[4];
	uint64_t m_DatapathBaseAddress;
	uint64_t m_PixelDistributionAddress;

	uint64_t m_PipelineBaseAddress;
	uint64_t m_GaborEngineAddress;
	uint64_t m_HistogramEngineAddress;
	uint64_t m_InterpolationEngineAddress;

	aim_processor_config* m_AcceleratorConfig;
	vector<aim_notification_config*> m_NotificationConfig;
	vector<aim_opcode_config*> m_OpcodeConfig;

	void SetNetworkInterface(IVortexNIFSAP* value);
	void SetSAPAddress(uint8_t BusID, uint8_t SwitchID, uint8_t PortID);
	void AcceleratorAllocateFlowID();
	void AcceleratorAllocateScratchMemory(uint64_t ScratchSize);
	void AcceleratorSetNumChannels(int NumChannels);

	void ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, IVortexConfiguration** Configurations, int NumConfigurations);
	void ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, vector<aim_processor_config*> Configs);
	void ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, aim_processor_config* Config);
	void ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, vector<aim_opcode_config*> Configs);
	void ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, vector<aim_notification_config*> Configs);
	void ConfigureDeviceID(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress);

	vector<aim_opcode_config*> GenerateOpcodes(int NumOrientations, int NumChannels);

	static double Log2(double X);
	static double Log(double X);

	vector<vector<vector<int> > > GetMinMaxMultiple(vector<vector<vector<vector<int16_t> > > > KernelValues);
	vector<vector<int> > GetMinMax(vector<vector<vector<int16_t> > > KernelValues);
	/*
	ImageContainer* GenerateAccumulatorTestData(ImageContainer* Image, int BitWidth, string OutputFilepath);
	ImageContainer* GenerateFormatConverterTestData(ImageContainer* Image, int InputIntegerBits, int InputFractionBits, int OutputIntegerBits, int OutputFractionBits, string OutputFilepath);
	ImageContainer* GenerateAdderTestData(ImageContainer* Image, int Bitwidth, string OutputFilepath);
	ImageContainer* GenerateNegateTestData(ImageContainer* Image, int BitWidth, string OutputFilepath);
	ImageContainer* GenerateConvolutionTestData(ImageContainer* Image, vector<vector<vector<vector<int16_t> > > > Kernel, string OutputFilepath);
	ImageContainer* GenerateHistogramTestData(ImageContainer* Image, vector<vector<vector<int> > > MinMax, histogram_partition* Partition, string OutputFilepath);
	ImageContainer* GenerateLogLookupTestData(ImageContainer*, string OutputFilepath);
	*/
	void WriteToFile(vector<vector<int64_t> > Image, int OutputDatawidth, ofstream* sw);
	void WriteToFile(vector<int64_t> Data, int OutputDatawidth, ofstream* sw);
	void WriteKernelDataArray(vector<vector<int16_t> > Kernel, string OutputFilepath);
	void WriteInputDataArray(vector<vector<vector<uint8_t> > >, int Channel, string OutputFilepath);
};

#endif
