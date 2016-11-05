#include "nifsap_logger_v1_00_a.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <iostream>


using namespace std;

nifsap_logger_v1_00_a::nifsap_logger_v1_00_a(bool SimulationMode, int BoardID, string ConfigFilename)
{
	m_ConfigFilename = ConfigFilename;

	remove(m_ConfigFilename.c_str());

	for (uint16_t flowID = 64; flowID < 1023; flowID++)
		m_FlowIDList.push_back(flowID);
}

nifsap_logger_v1_00_a::~nifsap_logger_v1_00_a(void)
{
	for(std::vector<VortexDeviceAddress*>::iterator it = m_MemoryInterfaces.begin(); it != m_MemoryInterfaces.end(); ++it)
		delete (*it);
}

void nifsap_logger_v1_00_a::AddMemoryInterface(uint8_t BusID, uint8_t SwitchID, uint8_t PortID)
{
	m_MemoryInterfaces.push_back(new VortexDeviceAddress(BusID,SwitchID,PortID));
}

void nifsap_logger_v1_00_a::ResetSAP(VortexDeviceAddress* SAP)
{
	uint8_t ResetKeyValue[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF };

	VortexConfigurationGroup* vcg = new VortexConfigurationGroup(SAP, new VortexConfigurationPacket(ResetKeyValue,16,0xF0),VortexEntityType::NIFSAP);

	Configure(vcg);

	delete(vcg);
}

bool nifsap_logger_v1_00_a::Initialize(int BoardID)
{
	return false;
}

void nifsap_logger_v1_00_a::Deinitialize()
{
}

void nifsap_logger_v1_00_a::Configure(VortexConfigurationGroup* ConfigGroup)
{
	if (ConfigGroup == NULL)
		return;

	vector<string> Lines;
	ifstream ifile(m_ConfigFilename);
	std::string line;

	if (ifile.is_open())
	{
		while (std::getline(ifile, line))
		{
			Lines.push_back(line);
		}
		ifile.close();
	}

	if (Lines.size() == 0)
	{
		stringstream s;
		s << std::uppercase << std::hex << std::setfill('0') << std::setw(32) << 0;
		Lines.push_back(s.str());
	}

	int num_configs;
	string::size_type pos1 = Lines[0].find_first_not_of('0');
	string::size_type pos2 = Lines[0].length()-1;
	string strNumConfigs = Lines[0].substr((pos1 == string::npos) ? Lines[0].length()-1 : pos1, pos2);
	istringstream (strNumConfigs) >> std::hex >> num_configs;
	num_configs += ConfigGroup->GetLength();

	stringstream s;
	s << std::uppercase << std::hex << std::setfill('0') << std::setw(32) << (int)num_configs;

	Lines[0] = s.str();

	IVortexConfiguration** Configurations = ConfigGroup->GetConfigurations();
	for (int i = 0; i < ConfigGroup->GetLength(); i++)
    {
        uint16_t ConfigFlow;

		switch (Configurations[i]->GetConfigurationType())
        {
		case VortexEntityType::NIFSAP: ConfigFlow = 20; break;
		case VortexEntityType::NIFSOP: ConfigFlow = 21; break;
		case VortexEntityType::SOP: ConfigFlow = 22; break;
		case VortexEntityType::SAP: ConfigFlow = 12; break;
		default: ConfigFlow = 12; break;
        }

		int ConfigBytesLength;
        uint8_t* ConfigBytes = Configurations[i]->GetConfiguration()->GetDataBytes(ConfigBytesLength);

		stringstream strConfigLength;
        stringstream strConfigAddress;
        stringstream strConfigDevice;
        stringstream strConfigFlow;
        stringstream strConfigRSVD;
		/*
        strConfigLength << std::hex << std::setfill('0') << std::setw(4) << (ConfigBytesLength / 16);
		strConfigAddress << std::hex << std::setfill('0') << std::setw(16) << Configurations[i]->GetConfiguration()->GetAddress();
        strConfigDevice << std::hex << std::setfill('0') << std::setw(4) << ConfigGroup->GetDeviceID()->GetDeviceID();
        strConfigFlow << std::hex << std::setfill('0') << std::setw(4) << ConfigFlow;
        strConfigRSVD << std::hex << std::setfill('0') << std::setw(4) << 0;

		stringstream configLine;

		configLine << strConfigRSVD << strConfigFlow << strConfigDevice << strConfigAddress << strConfigLength;
		*/

		stringstream configLine;

		configLine << std::uppercase << std::hex << std::setfill('0')
			<< std::setw(4 ) << (int)0
			<< std::setw(4 ) << (int)ConfigFlow
			<< std::setw(4 ) << (int)ConfigGroup->GetDeviceID()->GetDeviceID()
			<< std::setw(16) << (unsigned long long)Configurations[i]->GetConfiguration()->GetAddress()
			<< std::setw(4 ) << (int)(ConfigBytesLength / 16);


		Lines.push_back(configLine.str());

        for (int f = 0; f < ConfigBytesLength / 16; f++)
        {
			stringstream strDataBytes;
			/*
			strDataBytes << std::uppercase << std::hex << std::setfill('0')
							<< std::setw(2) << (int)ConfigBytes[f * 16 + 15]
							<< std::setw(2) << (int)ConfigBytes[f * 16 + 14]
							<< std::setw(2) << (int)ConfigBytes[f * 16 + 13]
							<< std::setw(2) << (int)ConfigBytes[f * 16 + 12]
							<< std::setw(2) << (int)ConfigBytes[f * 16 + 11]
							<< std::setw(2) << (int)ConfigBytes[f * 16 + 10]
							<< std::setw(2) << (int)ConfigBytes[f * 16 + 9]
							<< std::setw(2) << (int)ConfigBytes[f * 16 + 8]
							<< std::setw(2) << (int)ConfigBytes[f * 16 + 7]
							<< std::setw(2) << (int)ConfigBytes[f * 16 + 6]
							<< std::setw(2) << (int)ConfigBytes[f * 16 + 5]
							<< std::setw(2) << (int)ConfigBytes[f * 16 + 4]
							<< std::setw(2) << (int)ConfigBytes[f * 16 + 3]
							<< std::setw(2) << (int)ConfigBytes[f * 16 + 2]
							<< std::setw(2) << (int)ConfigBytes[f * 16 + 1]
							<< std::setw(2) << (int)ConfigBytes[f * 16 + 0];
			*/

			strDataBytes << std::uppercase << std::hex << std::setfill('0')
							//<< std::setw(8) << ((int)*((int*)&ConfigBytes[f * 16 + 12]))
							<< std::setw(16) << ((unsigned long long)*((unsigned long long*)&ConfigBytes[f * 16 + 8]))
							//<< std::setw(8) << ((int)*((int*)&ConfigBytes[f * 16 + 4]))
							<< std::setw(16) << ((unsigned long long)*((unsigned long long*)&ConfigBytes[f * 16 + 0]));


			Lines.push_back(strDataBytes.str());
        }
    }

	ofstream ofile(m_ConfigFilename,std::ios::trunc);

	if (ofile.is_open())
	{
		for (int i=0; i<Lines.size(); i++)
			ofile << Lines[i] << endl;

		ofile.close();
	}
}

void nifsap_logger_v1_00_a::Configure(VortexConfigurationGroup* Configurations[], int NumConfigurations)
{
	for (int i=0; i<NumConfigurations; i++)
		Configure(Configurations[i]);
}

void nifsap_logger_v1_00_a::Configure(uint16_t FlowID, uint64_t Length, VortexRemoteDescriptorConfiguration* Descriptors[], int NumDescriptors)
{
}

bool nifsap_logger_v1_00_a::WriteStream(uint8_t* SourceBuffer, uint64_t Length, uint16_t FlowID)
{
	return false;
}

bool nifsap_logger_v1_00_a::WriteStream(uint8_t* SourceBuffer, uint64_t Length, uint16_t FlowID, VortexRemoteDescriptorConfiguration* Descriptors[], int NumDescriptors)
{
	return false;
}

bool nifsap_logger_v1_00_a::WriteStream(uint8_t* SourceBuffer, uint64_t Length, uint16_t FlowID, VortexDeviceAddress* TargetDevice, uint64_t TargetAddress)
{
	return false;
}

bool nifsap_logger_v1_00_a::ReadStream(uint8_t* TargetBuffer, uint64_t Length, uint16_t FlowID)
{
	return false;
}

bool nifsap_logger_v1_00_a::ReadStream(uint8_t* TargetBuffer, uint64_t Length, uint16_t FlowID, VortexRemoteDescriptorConfiguration* Descriptors[], int NumDescriptors)
{
	return false;
}

bool nifsap_logger_v1_00_a::ReadStream(uint8_t* TargetBuffer, uint64_t Length, uint16_t FlowID, VortexDeviceAddress* SourceDevice, uint64_t SourceAddress)
{
	return false;
}

bool nifsap_logger_v1_00_a::DMA(VortexTransferDescriptor* Descriptor)
{
	return false;
}

bool nifsap_logger_v1_00_a::DMA(VortexTransferDescriptor* Descriptor, int TimeoutMilliseconds)
{
	return false;
}

void nifsap_logger_v1_00_a::SendMessage(VortexMessage* Message)
{
}

VortexMessage* nifsap_logger_v1_00_a::WaitMessage(int TimeoutMilliseconds)
{
	return NULL;
}

VortexMemoryAllocation* nifsap_logger_v1_00_a::AllocateMemory(uint64_t Size)
{
	if (m_MemoryInterfaces.size() == 0)
		return NULL;

	int MemoryInterfaceIndex = 0;
	void* ptr = Allocate(Size,CAPI_CACHELINE_SIZE);

	VortexMemoryAllocation* Allocation = new VortexMemoryAllocation(m_MemoryInterfaces[MemoryInterfaceIndex],(uint64_t)ptr,Size);

	return Allocation;
}

void nifsap_logger_v1_00_a::DeallocateMemory(VortexMemoryAllocation* Hndl)
{
	Deallocate((void*)Hndl->GetMemoryOffset());
	delete(Hndl);
}

uint16_t nifsap_logger_v1_00_a::AllocateFlowID()
{
	uint16_t flowID = 0;
	if (m_FlowIDList.size() > 0)
	{
		flowID = m_FlowIDList.front();
		m_FlowIDList.erase(m_FlowIDList.begin());
	}
	return flowID;
}

void nifsap_logger_v1_00_a::DeallocateFlowID(uint16_t FlowID)
{
	//No checks to see if it exists already
	m_FlowIDList.push_back(FlowID);
}


void* nifsap_logger_v1_00_a::Allocate(int Size, int Alignment)
{
	void *p;
	#ifndef _WIN32
		if (posix_memalign(&p, Alignment, Size) != 0)
			p = NULL;
	#else
		p = _aligned_malloc(Size, Alignment);
	#endif
        if (!p)
			throw std::bad_alloc();
		return p;
	}

void nifsap_logger_v1_00_a::Deallocate(void* p)
{
	#ifndef _WIN32
		free(p);
	#else
		_aligned_free(p);
	#endif
}
