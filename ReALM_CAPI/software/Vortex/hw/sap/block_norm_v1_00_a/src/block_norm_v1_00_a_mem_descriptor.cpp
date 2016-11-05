#include "block_norm_v1_00_a_mem_descriptor.h"


block_norm_v1_00_a_mem_descriptor::block_norm_v1_00_a_mem_descriptor(void)
{
  m_payload.SvmAddr = 0;
  m_payload.SvmMemID = 0;
  
  m_payload.SvmFlowID = 0;
  m_payload.HogFlowID = 0;
  m_payload.DotpFlowID = 0;
}

block_norm_v1_00_a_mem_descriptor::~block_norm_v1_00_a_mem_descriptor(void)
{
}

uint64_t block_norm_v1_00_a_mem_descriptor::GetSvmAddr(){
  return m_payload.SvmAddr;
}
void block_norm_v1_00_a_mem_descriptor::SetSvmAddr(uint64_t value){
  m_payload.SvmAddr = value;
}

uint16_t block_norm_v1_00_a_mem_descriptor::GetSvmMemID(){
  return m_payload.SvmMemID;
}
void block_norm_v1_00_a_mem_descriptor::SetSvmMemID(uint16_t value){
  m_payload.SvmMemID = value;
}


uint16_t block_norm_v1_00_a_mem_descriptor::GetSvmFlowID(){
  return m_payload.SvmFlowID;
}
void block_norm_v1_00_a_mem_descriptor::SetSvmFlowID(uint16_t value){
  m_payload.SvmFlowID = value;
}

uint16_t block_norm_v1_00_a_mem_descriptor::GetHogFlowID(){
  return m_payload.HogFlowID;
}
void block_norm_v1_00_a_mem_descriptor::SetHogFlowID(uint16_t value){
  m_payload.HogFlowID = value;
}

uint16_t block_norm_v1_00_a_mem_descriptor::GetDotpFlowID(){
  return m_payload.DotpFlowID;
}
void block_norm_v1_00_a_mem_descriptor::SetDotpFlowID(uint16_t value){
  m_payload.DotpFlowID = value;
}

uint8_t* block_norm_v1_00_a_mem_descriptor::GetBytes()
{
  return m_payload.GetBytes();
}
