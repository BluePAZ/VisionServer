#ifndef __AIM_ENGINE_V1_00_A_H__
#define __AIM_ENGINE_V1_00_A_H__

#include <VortexTypes.h>
#include <VortexSAPTypes.h>
#include <VortexSOPTypes.h>
#include "convolution_v1_00_a.h"
#include "histogram_v3_00_a.h"
#include "linear_interpolator_v1_00_a.h"
#include "AIMConfigurationList.h"
#include "AIMTestData.h"
#include "AIMOpcodeConfig.h"
#include "AIMProcessorConfig.h"
#include "AIMNotificationConfig.h"

enum class PIPELINE_ADDRESS_RANGE_INDEX : uint64_t { CONVOLUTION = 0x0000000000000000, HISTOGRAM = 0x0000000000400000, LOG_LOOKUP = 0x0000000000800000};

class aim_engine_v1_00_a
{
public:
	aim_engine_v1_00_a(string Name, uint8_t BusID, uint8_t SwitchID, uint8_t PortID, IVortexNIFSAP* NIFSAP);
	~aim_engine_v1_00_a(void);

	string GetName();

	IVortexNIFSAP* GetNetworkInterface();
	VortexDeviceAddress* GetSAPAddress();

	AIMProcessorConfig* GetAcceleratorConfiguration();
	void AcceleratorSetNumOrientations(int NumOrientations);
	void AcceleratorAddKernel(int KernelWidth, int KernelHeight);
	void AcceleratorAddNotificationTarget(uint8_t BusID, uint8_t SwitchID, uint8_t PortID);
	void AcceleratorSetImageConfiguration(uint16_t ImageWidth, uint16_t ImageHeight, IMAGE_DESCRIPTOR_FORMAT_COLOR FormatColor, IMAGE_DESCRIPTOR_FORMAT_DEPTH FormatDepth, IMAGE_DESCRIPTOR_TYPE FormatType);

	void Reset();
	bool Configure();
	uint8_t* Process(VortexMemoryAllocation* ImageHandle, int ImageWidth, int ImageHeight);
	AIMTestData* GenerateTestData(vector<vector<vector<uint8_t>>> Image, vector<vector<vector<vector<int16_t>>>> Kernel, string OutputFilepath);

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

	vector<convolution_v1_00_a*> m_ConvolutionEngines;
	vector<histogram_v3_00_a*> m_HistogramEngines;
	vector<linear_interpolator_v1_00_a*> m_InterpolationEngines;
	vector<VortexOperatorLink*> m_PipelineLinks;

	uint64_t m_SlaveBaseAddress[4];
	uint64_t m_DatapathBaseAddress;
	uint64_t m_PixelDistributionAddress;

	vector<uint64_t> m_PipelineBaseAddress;
	vector<uint64_t> m_ConvolutionEngineAddress;
	vector<uint64_t> m_HistogramEngineAddress;
	vector<uint64_t> m_InterpolationEngineAddress;

	AIMProcessorConfig* m_AcceleratorConfig;
	vector<AIMNotificationConfig*> m_NotificationConfig;
	vector<AIMOpcodeConfig*> m_OpcodeConfig;

	void SetNetworkInterface(IVortexNIFSAP* value);
	void SetSAPAddress(uint8_t BusID, uint8_t SwitchID, uint8_t PortID);
	void AcceleratorAllocateFlowID();
	void AcceleratorAllocateScratchMemory(uint64_t ScratchSize);
	void AcceleratorSetNumChannels(int NumChannels);

	void ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, IVortexConfiguration** Configurations, int NumConfigurations);
	void ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, vector<AIMProcessorConfig*> Configs);
	void ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, AIMProcessorConfig* Config);
	void ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, vector<AIMOpcodeConfig*> Configs);
	void ConfigureList(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress, vector<AIMNotificationConfig*> Configs);
	void ConfigureDeviceID(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress);

	vector<AIMOpcodeConfig*> GenerateOpcodes(int NumOrientations, int NumChannels);

	static double Log2(double X);
	static double Log(double X);

	vector<vector<vector<int>>> GetMinMaxMultiple(vector<vector<vector<vector<int16_t>>>> KernelValues);
	vector<vector<int>> GetMinMax(vector<vector<vector<int16_t>>> KernelValues);
	vector<vector<int64_t>> GenerateAccumulatorTestData(vector<vector<vector<vector<int64_t>>>> Image, int BitWidth, string OutputFilepath);
	vector<vector<vector<vector<vector<int64_t>>>>> GenerateFormatConverterTestData(vector<vector<vector<vector<vector<int64_t>>>>> Image, int InputIntegerBits, int InputFractionBits, int OutputIntegerBits, int OutputFractionBits, string OutputFilepath);
	vector<vector<vector<vector<int64_t>>>> GenerateAdderTestData(vector<vector<vector<vector<vector<int64_t>>>>> Image, int Bitwidth, string OutputFilepath);
	vector<vector<vector<vector<vector<int64_t>>>>> GenerateNegateTestData(vector<vector<vector<vector<vector<int64_t>>>>> Image, int BitWidth, string OutputFilepath);
	vector<vector<vector<vector<vector<int64_t>>>>> GenerateConvolutionTestData(vector<vector<vector<uint8_t>>> Image, vector<vector<vector<vector<int16_t>>>> Kernel, string OutputFilepath);
	vector<vector<vector<vector<vector<int64_t>>>>> GenerateHistogramTestData(vector<vector<vector<vector<vector<int64_t>>>>> Image, vector<vector<vector<int>>> MinMax, string OutputFilepath);
	vector<vector<vector<vector<vector<int64_t>>>>> GenerateLogLookupTestData(vector<vector<vector<vector<vector<int64_t>>>>> Image, string OutputFilepath);
	void WriteToFile(vector<vector<int64_t>> Image, int OutputDatawidth, ofstream* sw);
	void WriteToFile(vector<int64_t> Data, int OutputDatawidth, ofstream* sw);
	void WriteKernelDataArray(vector<vector<int16_t>> Kernel, string OutputFilepath);
	void WriteInputDataArray(vector<vector<vector<uint8_t>>>, int Channel, string OutputFilepath);
};

#endif
