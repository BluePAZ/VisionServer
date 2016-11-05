#include "VortexTypes.h"
#include <stack>
#include <sstream>
#include <iomanip>
#include <iostream>

VortexConfigurationPacket::VortexConfigurationPacket(void)
{
	m_Address = 0;
}

VortexConfigurationPacket::VortexConfigurationPacket(uint64_t Address)
{
	m_Address = Address;
}

VortexConfigurationPacket::VortexConfigurationPacket(uint8_t Data[], int Length, uint64_t Address)
{
	m_Address = Address;
	Add(Data, Length);
}

VortexConfigurationPacket::~VortexConfigurationPacket(void)
{
}

void VortexConfigurationPacket::Add(uint8_t Data[], int Length)
{
	int currentSize = m_Data.size();
	m_Data.resize(currentSize + Length);

	for (int i=0; i<Length; i++)
		m_Data[i+currentSize] = Data[i];

	
}

void VortexConfigurationPacket::Add(uint8_t Data)
{
	m_Data.push_back(Data);
}

void VortexConfigurationPacket::Clear()
{
	m_Data.clear();
}

uint8_t* VortexConfigurationPacket::GetDataBytes(int& Length)
{
	MakeMultiple(16);
	Length = m_Data.size();
	/*
	uint8_t* dataCopy = new uint8_t[*Length];
	for (int i=0; i<*Length; i++)
		dataCopy[i] = m_Data[i];
	*/
	printf("Data Pointer address is 0x%016X\n",m_Data.data());

	return m_Data.data();
}

uint64_t VortexConfigurationPacket::GetAddress()
{
	return m_Address;
}

void VortexConfigurationPacket::SetAddress(uint64_t value)
{
	m_Address = value;
}

string* VortexConfigurationPacket::GetConfigString(int& Length)
{
	stack<uint8_t> ConfigByteStack;
	vector<string> ConfigLines;
	int ConfigByteLength;
	char hexBytes1[16];
	char hexBytes2[16];
	
	uint64_t CurrentAddress = m_Address;
	uint8_t* ConfigBytes = GetDataBytes(ConfigByteLength);

	for (int b=0; b<ConfigByteLength; b++)
	{	
		std::ostringstream ConfigLine;
		//ConfigLine << std::hex;
		
		ConfigByteStack.push(ConfigBytes[b]);

		if (b == (ConfigByteLength-1))
			while (ConfigByteStack.size() != 16)
				ConfigByteStack.push(0);

		if (ConfigByteStack.size() == 16)
		{
			//ConfigLine << std::uppercase << std::setw(2) << std::setfill('0');

			while (ConfigByteStack.size() != 0)
			{
				sprintf(hexBytes1, "%02X",(uint8_t)ConfigByteStack.top());
				ConfigLine << hexBytes1;
				ConfigByteStack.pop();
			}

			sprintf(hexBytes2, "%09X",CurrentAddress);
			ConfigLine << hexBytes2;//std::uppercase << std::setw(9) << std::setfill('0') << CurrentAddress;
			CurrentAddress += 16;
			ConfigLines.push_back(ConfigLine.str());
		}
	}

	Length = ConfigLines.size();

	string* ConfigLinesCopy = new string[Length];
	for (int i=0; i<Length; i++)
		ConfigLinesCopy[i] = ConfigLines[i];

	return ConfigLinesCopy;
}

void VortexConfigurationPacket::MakeMultiple(int Size)
{
	if (m_Data.size() == 0)
		m_Data.push_back(0);

	while((m_Data.size() % 16) != 0)
		m_Data.push_back(0);
}