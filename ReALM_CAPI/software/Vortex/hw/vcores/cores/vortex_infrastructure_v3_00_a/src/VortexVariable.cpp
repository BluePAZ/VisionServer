#include "VortexTypes.h"

VortexVariable::VortexVariable(string Name, void* Variable)
{
	SetName(Name);
	SetVariable(Variable);
}

VortexVariable::~VortexVariable(void)
{
}

string VortexVariable::GetName()
{
	return m_Name;
}

void VortexVariable::SetName(string value)
{
	m_Name = value;
}

void* VortexVariable::GetVariable()
{
	return m_Variable;
}

void VortexVariable::SetVariable(void* value)
{
	m_Variable = value;
}