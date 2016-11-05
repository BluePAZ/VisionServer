#include "block_norm_v1_00_a_mode_descriptor.h"


block_norm_v1_00_a_mode_descriptor::block_norm_v1_00_a_mode_descriptor(void)
{
  m_payload.Mode = 0;
//  m_payload.NormOutMode = 0;
  m_payload.CellPixWidth = 0;
  m_payload.CellPixHeight = 0;
  m_payload.BinNum = 0;
  m_payload.ScaleAdj = 0;
  m_payload.ScaleCst = 0;
  m_payload.ProjShift = 0;
  m_payload.TextScale = 0;
	
  m_payload.NormMax = 0;
  m_payload.NormAdjust = 0;
  m_payload.FlipEn = 0;
  m_payload.RoiWidthInCell = 0;
  m_payload.RoiHeightInCell = 0;
}

block_norm_v1_00_a_mode_descriptor::~block_norm_v1_00_a_mode_descriptor(void)
{
}

uint8_t block_norm_v1_00_a_mode_descriptor::GetMode(){
	return m_payload.Mode;
}

void block_norm_v1_00_a_mode_descriptor::SetMode(uint8_t value){
	m_payload.Mode = value;
}

/*
uint8_t block_norm_v1_00_a_mode_descriptor::GetNormOutMode(){
  return m_payload.NormOutMode;
}

void block_norm_v1_00_a_mode_descriptor::SetNormOutMode(uint8_t value){
  m_payload.NormOutMode = value;
}
*/

uint8_t block_norm_v1_00_a_mode_descriptor::GetCellPixWidth(){
  return m_payload.CellPixWidth;
}

void block_norm_v1_00_a_mode_descriptor::SetCellPixWidth(uint8_t value){
  m_payload.CellPixWidth = value;
}

uint8_t block_norm_v1_00_a_mode_descriptor::GetCellPixHeight(){
  return m_payload.CellPixHeight;
}

void block_norm_v1_00_a_mode_descriptor::SetCellPixHeight(uint8_t value){
  m_payload.CellPixHeight = value;
}

uint8_t block_norm_v1_00_a_mode_descriptor::GetBinNum(){
  return m_payload.BinNum;
}

void block_norm_v1_00_a_mode_descriptor::SetBinNum(uint8_t value){
  m_payload.BinNum = value;
}

uint16_t block_norm_v1_00_a_mode_descriptor::GetScaleAdj(){
  return m_payload.ScaleAdj;
}

void block_norm_v1_00_a_mode_descriptor::SetScaleAdj(uint16_t value){
  m_payload.ScaleAdj = value;
}

uint8_t block_norm_v1_00_a_mode_descriptor::GetScaleCst(){
  return m_payload.ScaleCst;
}

void block_norm_v1_00_a_mode_descriptor::SetScaleCst(uint8_t value){
  m_payload.ScaleCst = value;
}

uint8_t block_norm_v1_00_a_mode_descriptor::GetProjShift(){
  return m_payload.ProjShift;
}

void block_norm_v1_00_a_mode_descriptor::SetProjShift(uint8_t value){
  m_payload.ProjShift = value;
}

uint8_t block_norm_v1_00_a_mode_descriptor::GetTextScale(){
  return m_payload.TextScale;
}

void block_norm_v1_00_a_mode_descriptor::SetTextScale(uint8_t value){
  m_payload.TextScale = value;
}

uint8_t block_norm_v1_00_a_mode_descriptor::GetNormMax(){
  return m_payload.NormMax;
}

void block_norm_v1_00_a_mode_descriptor::SetNormMax(uint8_t value){
  m_payload.NormMax = value;
}

uint8_t block_norm_v1_00_a_mode_descriptor::GetNormAdjust(){
  return m_payload.NormAdjust;
}

void block_norm_v1_00_a_mode_descriptor::SetNormAdjust(uint8_t value){
  m_payload.NormAdjust = value;
}

uint8_t block_norm_v1_00_a_mode_descriptor::GetFlipEn(){
  return m_payload.FlipEn;
}

void block_norm_v1_00_a_mode_descriptor::SetFlipEn(uint8_t value){
  m_payload.FlipEn = value;
}

uint8_t block_norm_v1_00_a_mode_descriptor::GetRoiWidthInCell(){
  return m_payload.RoiWidthInCell;
}

void block_norm_v1_00_a_mode_descriptor::SetRoiWidthInCell(uint8_t value){
  m_payload.RoiWidthInCell = value;
}

uint8_t block_norm_v1_00_a_mode_descriptor::GetRoiHeightInCell(){
  return m_payload.RoiHeightInCell;
}

void block_norm_v1_00_a_mode_descriptor::SetRoiHeightInCell(uint8_t value){
  m_payload.RoiHeightInCell = value;
}

uint8_t* block_norm_v1_00_a_mode_descriptor::GetBytes()
{
  return m_payload.GetBytes();
}
