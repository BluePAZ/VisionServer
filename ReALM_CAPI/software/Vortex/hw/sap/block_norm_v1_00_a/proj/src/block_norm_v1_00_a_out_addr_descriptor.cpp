#include "block_norm_v1_00_a_out_addr_descriptor.h"


block_norm_v1_00_a_out_addr_descriptor::block_norm_v1_00_a_out_addr_descriptor(void)
{
  m_payload.NormAddr = 0;
  m_payload.NormMemID = 0;
  m_payload.DotpAddr = 0;
  m_payload.DotpMemID = 0;
}

block_norm_v1_00_a_out_addr_descriptor::~block_norm_v1_00_a_out_addr_descriptor(void)
{
}

uint64_t block_norm_v1_00_a_out_addr_descriptor::GetNormAddr(){
  return m_payload.NormAddr;
}
void block_norm_v1_00_a_out_addr_descriptor::SetNormAddr(uint64_t value){
  m_payload.NormAddr = value;
}

uint16_t block_norm_v1_00_a_out_addr_descriptor::GetNormMemID(){
  return m_payload.NormMemID;
}
void block_norm_v1_00_a_out_addr_descriptor::SetNormMemID(uint16_t value){
  m_payload.NormMemID = value;
}

uint64_t block_norm_v1_00_a_out_addr_descriptor::GetDotpAddr(){
  return m_payload.DotpAddr;
}
void block_norm_v1_00_a_out_addr_descriptor::SetDotpAddr(uint64_t value){
  m_payload.DotpAddr = value;
}

uint16_t block_norm_v1_00_a_out_addr_descriptor::GetDotpMemID(){
  return m_payload.DotpMemID;
}
void block_norm_v1_00_a_out_addr_descriptor::SetDotpMemID(uint16_t value){
  m_payload.DotpMemID = value;
}

uint8_t* block_norm_v1_00_a_out_addr_descriptor::GetBytes()
{
  return m_payload.GetBytes();
}
