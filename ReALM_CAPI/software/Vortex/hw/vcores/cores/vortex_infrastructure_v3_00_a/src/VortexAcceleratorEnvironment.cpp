#include "VortexSAPTypes.h"

int VortexAcceleratorEnvironment::m_ID = 0;

VortexAcceleratorEnvironment::VortexAcceleratorEnvironment(void)
{
	m_Identifier = m_ID++;
}

VortexAcceleratorEnvironment::~VortexAcceleratorEnvironment(void)
{
	for(std::map<string,VortexVariable*>::iterator it = m_VariableTable.begin(); it != m_VariableTable.end(); ++it)
		delete(it->second);
}

int VortexAcceleratorEnvironment::GetIdentifier()
{
	return m_Identifier;
}

void VortexAcceleratorEnvironment::Insert(string Name, void* Variable)
{
	VortexVariable* existingVar = Retrieve(Name);
	if (existingVar == NULL)
		m_VariableTable[Name] = new VortexVariable(Name,Variable);
	else
		existingVar->SetVariable(Variable);
}

VortexVariable* VortexAcceleratorEnvironment::Retrieve(string Name)
{
	if (m_VariableTable.find(Name) == m_VariableTable.end())
		return NULL;
	else
		return m_VariableTable[Name];
}