#include "block_norm_v1_00_a.h"
#include "VortexImageDescriptor.h"
#include "VortexMessage.h"
#include <cmath>
#include <string>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

block_norm_v1_00_a::block_norm_v1_00_a(string Name, uint8_t BusID, uint8_t SwitchID, uint8_t PortID, IVortexNIFSAP* NIFSAP)
{
  m_Name = Name;

  m_IsConfigured = false;

  m_SlaveBaseAddress[0] = 0x0000000000000000;   // sap-controller
  m_SlaveBaseAddress[1] = 0x0000000400000000;   // sap-datapath
  m_SlaveBaseAddress[2] = 0x0000000800000000;   // sap-output
  m_SlaveBaseAddress[3] = 0x0000000C00000000;

  //Datapath Address
  m_DatapathBaseAddress = m_SlaveBaseAddress[1];
  //Pixel Distribution Address

  /////////// Pipeline Base Address //////////////////////////////////////////////
  m_PipelineBaseAddress = m_DatapathBaseAddress + 0x1000000;
  /////////// Pipeline Base Address //////////////////////////////////////////////

  m_ImageConfig = new block_norm_v1_00_a_image_configuration();
  m_ModeConfig = new block_norm_v1_00_a_mode_descriptor();
  m_MemConfig = new block_norm_v1_00_a_mem_descriptor();
  m_OutAddrConfig = new block_norm_v1_00_a_out_addr_descriptor();

  SetNetworkInterface(NIFSAP);
  SetSAPAddress(BusID, SwitchID, PortID);

  
}

block_norm_v1_00_a::~block_norm_v1_00_a(void)
{
  delete(m_SAPAddress);

  for(std::vector<block_norm_v1_00_a_notification_config* >::iterator it = m_NotificationConfig.begin(); 
      it != m_NotificationConfig.end(); 
      ++it){
    delete (*it);
  }
  delete(m_ImageConfig);
  delete(m_ModeConfig);
  delete(m_MemConfig);
  delete(m_OutAddrConfig);
  //GetNetworkInterface()->DeallocateFlowID(GetControllerConfiguration()->GetResultStoreFlowID());
  GetNetworkInterface()->DeallocateFlowID(GetMemDescriptor()->GetSvmFlowID());
  GetNetworkInterface()->DeallocateMemory(m_ScratchMemory);

}
 
void block_norm_v1_00_a::Reset()
{
  GetNetworkInterface()->ResetSAP(m_SAPAddress);
  m_IsConfigured = false;
}

string block_norm_v1_00_a::GetName()
{
  return m_Name;
}

void block_norm_v1_00_a::SetNetworkInterface(IVortexNIFSAP* value)
{
  m_NetworkInterface = value;
}

IVortexNIFSAP* block_norm_v1_00_a::GetNetworkInterface()
{
  return m_NetworkInterface;
}

void block_norm_v1_00_a::SetSAPAddress(uint8_t BusID, uint8_t SwitchID, uint8_t PortID)
{
  m_SAPAddress = new VortexDeviceAddress(BusID,SwitchID,PortID);
}

VortexDeviceAddress* block_norm_v1_00_a::GetSAPAddress()
{
  return m_SAPAddress;
}

 
block_norm_v1_00_a_image_configuration* block_norm_v1_00_a::GetImageConfiguraiton(){
  return m_ImageConfig;
}
block_norm_v1_00_a_mode_descriptor* block_norm_v1_00_a::GetModeDescriptor(){
  return m_ModeConfig;
}
block_norm_v1_00_a_mem_descriptor* block_norm_v1_00_a::GetMemDescriptor(){
  return m_MemConfig;
}
block_norm_v1_00_a_out_addr_descriptor* block_norm_v1_00_a::GetOutAddrDescriptor(){
  return m_OutAddrConfig;
}
 
void block_norm_v1_00_a::AcceleratorAddNotificationTarget(uint8_t BusID, uint8_t SwitchID, uint8_t PortID)
{
  m_NotificationConfig.push_back(new block_norm_v1_00_a_notification_config(BusID,SwitchID,PortID));
}

void block_norm_v1_00_a::AcceleratorSetImageConfiguration(uint16_t ImageWidth, 
							  uint16_t ImageHeight, 
							  IMAGE_DESCRIPTOR_FORMAT_COLOR FormatColor, 
							  IMAGE_DESCRIPTOR_FORMAT_DEPTH FormatDepth, 
							  IMAGE_DESCRIPTOR_TYPE FormatType){
  //  GetImageConfiguraiton()->SetIndex(m_SlaveBaseAddress[0]);
  GetImageConfiguraiton()->SetWidth(ImageWidth);
  GetImageConfiguraiton()->SetHeight(ImageHeight);	

  uint8_t skip_svm_load = 0; // make this argument
  GetImageConfiguraiton()->SetFormatColor(FormatColor);
  GetImageConfiguraiton()->SetFormatDepth(FormatDepth);
  GetImageConfiguraiton()->SetType(FormatType);

}

void block_norm_v1_00_a::SetDescriptors(uint16_t ImageWidth, 
							  uint16_t ImageHeight, 
							  IMAGE_DESCRIPTOR_FORMAT_COLOR FormatColor, 
							  IMAGE_DESCRIPTOR_FORMAT_DEPTH FormatDepth, 
							  IMAGE_DESCRIPTOR_TYPE FormatType,
							  
							  // uint16_t hog_ld_flow_id, // this is not required for C++ version
							  //uint8_t hog_ld_mem_id, // now this is in ImageHandle
							  //uint64_t _hog_ld_mem_address,// now this is in ImageHandle
							  // uint8_t block_norm_device_id,
							  uint8_t nmlz_out_sel, // 0(default)=outputNmlzdBin, 1=outputDotProduct
							  uint8_t feature_pedro_sel, // 0=Dalal, 1=FeaturePedro,
							  uint8_t CellPixWidth,
							  uint8_t CellPixHeight,
							  uint8_t RoiPixWidth,
							  uint8_t RoiPixHeight,
							  uint8_t BinNum,
							  uint16_t ScaleAdj,
							  uint8_t ScaleCst,
							  float ProjShift,
							  float TextScale,
							  uint8_t NormMax,
							  uint8_t NormAdjust,
							  // uint16_t svm_ld_flow_id, // this is not required 
							  uint64_t SvmAddr,
							  uint16_t SvmMemID)
{
  AcceleratorSetImageConfiguration(ImageWidth, ImageHeight,
				   FormatColor, FormatDepth, FormatType);


  GetModeDescriptor()->SetCellPixWidth(CellPixWidth);
  GetModeDescriptor()->SetCellPixHeight(CellPixHeight);
  uint8_t skip_svm_load = 0; // make this argument

  // ModeConfig.Mode -------------------------------
  uint8_t mode = 0;
  if(nmlz_out_sel > 0){
    mode |= 0x1; // output normalization result
  } else {
    mode |= 0x2; // output dot product
  }
  if(skip_svm_load > 0) {
    mode |= 0x4; // load svm
  }
  if(feature_pedro_sel > 0){
    mode |= ((0x4|0x2|0x1)<<4);
  }

  GetModeDescriptor()->SetMode(mode); 
  // ModeConfig.FlipEn -------------------------------
  uint8_t FlipEn = 0;
  if(BinNum>9 && feature_pedro_sel == 0){
    BinNum=BinNum/2;
    FlipEn = 1;
  } else {
    //BinNum=BinNum;
    FlipEn = 0;
  }
  GetModeDescriptor()->SetFlipEn(FlipEn);
  // ----------------------------------------
  GetModeDescriptor()->SetBinNum(BinNum);

  GetModeDescriptor()->SetScaleAdj(ScaleAdj);
  GetModeDescriptor()->SetScaleCst(ScaleCst);
  // ProjShift --------------------------
  GetModeDescriptor()->SetProjShift((ProjShift == 0.5)? 1 : (ProjShift==0.25)? 2 : 0);

  // 
  int text_scale_decode = 0;
  {
    float tmp = TextScale;
    if (tmp - 0.25 >= 0){   // text_scale =     1  * 0.25 + {0or1} * 0.125 + {0or1} * 0.0625 + {0or1)*0.03125
      text_scale_decode |= 0x8;
      tmp -= 0.25f;
    }
    if (tmp - 0.125 >= 0){  // text_scale = {0or1} * 0.25 +     1 * 0.125 + {0or1} * 0.0625 + {0or1}*0.03125
      text_scale_decode |= 0x4;
      tmp -= 0.125f;
    }
    if (tmp - 0.0625 >= 0){ // text_scale = {0or1} * 0.25 + {0or1} * 0.125 +    1 * 0.0625 + {0or1}*0.03125
      text_scale_decode |= 0x2;
      tmp -= 0.0625f;
    }
    if (tmp - 0.03125 >= 0){ // text_scale = {0or1} * 0.25 + {0or1} * 0.125 + {0or1} * 0.0625 +   1}*0.03125
      text_scale_decode |= 0x1;
      //tmp -= 0.03125f;
    }
  }

  GetModeDescriptor()->SetTextScale(text_scale_decode);
  
  GetModeDescriptor()->SetNormMax(NormMax);
  GetModeDescriptor()->SetNormAdjust((NormAdjust<=1? 1 : NormAdjust==8? 2 : NormAdjust==32? 4 :NormAdjust==128? 8 : 1));

  GetModeDescriptor()->SetRoiWidthInCell(RoiPixWidth/CellPixWidth);
  GetModeDescriptor()->SetRoiHeightInCell(RoiPixHeight/CellPixHeight);

  GetMemDescriptor()->SetSvmAddr(SvmAddr);
  GetMemDescriptor()->SetSvmMemID(SvmMemID);
}

void block_norm_v1_00_a::AcceleratorAllocateScratchMemory(uint64_t ScratchSize)
{
  m_ScratchMemory = GetNetworkInterface()->AllocateMemory(ScratchSize);
  VortexDeviceAddress* MemoryDevice = m_ScratchMemory->GetMemoryDeviceID();

  //  GetImageConfiguraiton()->SetResultDeviceID(MemoryDevice->GetBusID(),MemoryDevice->GetSwitchID(),MemoryDevice->GetPortID());
  //  GetImageConfiguraiton()->SetResultAddress((uint64_t)m_ScratchMemory->GetMemoryOffset());

  // 
  //Not supported in CAPI - 05/24/2016 - ska
  /*
  GetOutAddrDescriptor()->SetNormAddr((uint64_t)m_ScratchMemory->GetMemoryOffset());
  GetOutAddrDescriptor()->SetNormMemID(m_ScratchMemory->GetMemoryDeviceID()->GetDeviceID());
  */
  GetOutAddrDescriptor()->SetDotpAddr((uint64_t)m_ScratchMemory->GetMemoryOffset());
  GetOutAddrDescriptor()->SetDotpMemID(m_ScratchMemory->GetMemoryDeviceID()->GetDeviceID());
}

void block_norm_v1_00_a::AcceleratorAllocateFlowID()
{
  uint16_t BaseFlowID = GetNetworkInterface()->AllocateFlowID(3); // +0:SVM +1:HistLoad +2:Output

  if (BaseFlowID != 0){
    GetMemDescriptor()->SetSvmFlowID(BaseFlowID);
    GetMemDescriptor()->SetHogFlowID(BaseFlowID+1);
    GetMemDescriptor()->SetDotpFlowID(BaseFlowID+2);
  }
}

bool block_norm_v1_00_a::Configure()
{
  ///////////// Retrieve Variables ////////////////////////////////////
  //int ImageWidth  = GetImageConfiguraiton()->GetWidth();
  //int ImageHeight = GetImageConfiguraiton()->GetHeight();
  //int ImageSize   = GetImageConfiguraiton()->GetImageSizeBytes();

  AcceleratorAllocateFlowID();  // This is the SVM Fetch Flow ID. Histogram Fetch Flow ID will be +1 of this and DotP Store Flow ID will be +2 of this
  AcceleratorAllocateScratchMemory(this->GetOutputSize()); // This is the allocation for results 

  return (m_IsConfigured = true);
}

void block_norm_v1_00_a::ConfigureDeviceID(VortexDeviceAddress* SAPDevice, uint64_t SAPAddress)
{
  uint8_t* DeviceIDBytes = SAPDevice->GetBytes();
  VortexConfigurationGroup* vcgDeviceID = new VortexConfigurationGroup(SAPDevice, new VortexConfigurationPacket(DeviceIDBytes, 2, SAPAddress), VortexEntityType::SAP);

  //Configure
  GetNetworkInterface()->Configure(vcgDeviceID);
  delete(vcgDeviceID);
}

int32_t* block_norm_v1_00_a::Process(VortexMemoryAllocation* ImageHandle, 
				     uint16_t ImageWidth, 
				     uint16_t ImageHeight,
				     // uint16_t hog_ld_flow_id, // this is not required for C++ version
				     // uint8_t hog_ld_mem_id, // now this is in ImageHandle
				     //uint64_t _hog_ld_mem_address,// now this is in ImageHandle
				     // uint8_t block_norm_device_id,
				     uint8_t nmlz_out_sel, // 0(default)=outputNmlzdBin, 1=outputDotProduct
				     uint8_t feature_pedro_sel, // 0=Dalal, 1=FeaturePedro,
				     uint8_t CellPixWidth,
				     uint8_t CellPixHeight,
				     uint8_t RoiPixWidth,
				     uint8_t RoiPixHeight,
				     uint8_t BinNum,
				     uint16_t ScaleAdj,
				     uint8_t ScaleCst,
				     float ProjShift,
				     float TextScale,
				     uint8_t NormMax,
				     uint8_t NormAdjust,
				     // uint16_t svm_ld_flow_id, // this is not required 
				     uint64_t SvmAddr,
				     uint16_t SvmMemID)
{
  int ConfiguredImageWidth = GetImageConfiguraiton()->GetWidth();
  int ConfiguredImageHeight = GetImageConfiguraiton()->GetHeight();
  //int ConfiguredImageSize = GetImageConfiguraiton()->GetImageSizeBytes();
  //printf("NCA: Set Descriptors\n");
  SetDescriptors(ImageWidth,
	  ImageHeight,
	  IMAGE_DESCRIPTOR_FORMAT_COLOR::NONE,
	  IMAGE_DESCRIPTOR_FORMAT_DEPTH::UINT32,
	  IMAGE_DESCRIPTOR_TYPE::RAW,
	  nmlz_out_sel, // 0(default)=outputNmlzdBin, 1=outputDotProduct
	  feature_pedro_sel, // 0=Dalal, 1=FeaturePedro,
	  CellPixWidth,
	  CellPixHeight,
	  RoiPixWidth,
	  RoiPixHeight,
	  BinNum,
	  ScaleAdj,
	  ScaleCst,
	  ProjShift,
	  TextScale,
	  NormMax,
	  NormAdjust,
	  SvmAddr,
	  SvmMemID);
  if (!m_IsConfigured || (ConfiguredImageWidth != ImageWidth) || (ConfiguredImageHeight != ImageHeight) )
    {
      //Reset();

      AcceleratorSetImageConfiguration(
				       ImageWidth,
				       ImageHeight,
				       IMAGE_DESCRIPTOR_FORMAT_COLOR::NONE,
				       IMAGE_DESCRIPTOR_FORMAT_DEPTH::UINT32,
				       IMAGE_DESCRIPTOR_TYPE::RAW
				       );
	  ConfiguredImageWidth = GetImageConfiguraiton()->GetWidth();
	  ConfiguredImageHeight = GetImageConfiguraiton()->GetHeight();
      if (!Configure())
	return NULL;

    }
  //printf("NCA: Set Descriptors\n");
  // Not sure why it is done twice - ska - 06/25/2016
  SetDescriptors(ImageWidth, 
		 ImageHeight,
				       IMAGE_DESCRIPTOR_FORMAT_COLOR::NONE,
				       IMAGE_DESCRIPTOR_FORMAT_DEPTH::UINT32,
		 IMAGE_DESCRIPTOR_TYPE::RAW,
		 nmlz_out_sel, // 0(default)=outputNmlzdBin, 1=outputDotProduct
		 feature_pedro_sel, // 0=Dalal, 1=FeaturePedro,
		 CellPixWidth,
		 CellPixHeight,
		 RoiPixWidth,
		 RoiPixHeight,
		 BinNum,
		 ScaleAdj,
		 ScaleCst,
		 ProjShift,
		 TextScale,
		 NormMax,
		 NormAdjust,
		 SvmAddr,
		 SvmMemID);
  
  VortexImageDescriptor* ImageDescriptor = new VortexImageDescriptor();
  ImageDescriptor->SetIdentifier(0xEEFF); // this is now 10 bit not 16 bit .. need to figure out what impact this has 
  ImageDescriptor->SetWidth(ConfiguredImageWidth);
  ImageDescriptor->SetHeight(ConfiguredImageHeight);
  ImageDescriptor->SetType(IMAGE_DESCRIPTOR_TYPE::RAW);
  ImageDescriptor->SetFormatColor(IMAGE_DESCRIPTOR_FORMAT_COLOR::NONE);
  ImageDescriptor->SetFormatDepth(IMAGE_DESCRIPTOR_FORMAT_DEPTH::UINT32);
  ImageDescriptor->SetImageHandle(ImageHandle); // Give Image address & id
  
  VortexMessage* ProcessMessage = new VortexMessage(
						    VortexMessageType::EXECUTE_REQUEST,
						    NULL,
						    GetSAPAddress(),
						    true);

  ProcessMessage->AddBytes(ImageDescriptor->GetBytes(),16);
  ProcessMessage->AddBytes(m_ModeConfig->GetBytes(), 16);
  ProcessMessage->AddBytes(m_MemConfig->GetBytes(), 16);
  ProcessMessage->AddBytes(m_OutAddrConfig->GetBytes(), 16);
  GetNetworkInterface()->SendMessage(ProcessMessage);

  printf("NCA: Message Sent\n");

  delete(ProcessMessage);
  delete(ImageDescriptor);
  
  VortexMessage* ProcessMessageResponse = GetNetworkInterface()->WaitMessage();
  if ((ProcessMessageResponse == NULL) || ProcessMessageResponse->GetType() != VortexMessageType::EXECUTE_COMPLETE)
    {
      delete(ProcessMessageResponse);
      return NULL;
    }

  delete(ProcessMessageResponse);

  if(GetModeDescriptor()->IsNormalizeOut()!=0){
    /* Not tested (take care for return type */
    //uint16_t *memAddr = (uint16_t*)(GetOutAddrDescriptor()->GetNormAddr());
    // Not supported in CAPI - ska - 04/24/2016
	  uint16_t *memAddr = NULL;
    return (int32_t*) memAddr; // cast to uint16_t* to access
  } else if(GetModeDescriptor()->IsDotpOut()!=0){
    int32_t *memAddr = (int32_t*)(GetOutAddrDescriptor()->GetDotpAddr());
    // Filling Zero to invalid area === if this is not required, comment out this 
    {
      uint8_t cellPixWidth = GetModeDescriptor()->GetCellPixWidth();
      uint8_t cellPixHeight = GetModeDescriptor()->GetCellPixHeight();
      //uint16_t imageWidth = GetImageConfiguraiton()->GetWidth();
      //uint16_t imageHeight = GetImageConfiguraiton()->GetHeight();
      uint16_t roiCellWidth = RoiPixWidth / cellPixWidth;
      uint16_t roiCellHeight = RoiPixHeight / cellPixHeight;
      uint16_t imageCellWidth = GetImageConfiguraiton()->GetWidth()/cellPixWidth;
      uint16_t imageCellHeight = GetImageConfiguraiton()->GetHeight()/cellPixHeight;
      
	  bool check_output = false;
	  FILE* fp1 = NULL;
	  //FILE* fp2 = NULL;

	  if (check_output)
	  {
		  // Open the file to write
		  // This also checks if the file exists and/or can be opened for reading correctly
		  fp1 = fopen("dotp_output1.dat", "w");
		  if (fp1 == NULL)
		  {
			  cout << "Could not open specified file" << endl;
			  return NULL;
		  }
		  else
		  {
			  cout << "File 1 opened successfully" << endl;
		  }

		  // Open the file to write
		  // This also checks if the file exists and/or can be opened for reading correctly
		  //fp2 = fopen("dotp_output2.dat", "w");
		  //if (fp2 == NULL)
		  //{
			//  cout << "Could not open specified file" << endl;
			//  return NULL ;
		  //}
		  //else
		  //{
			//  cout << "File 2 opened successfully" << endl;
		  //}

		  /* Too Slow
		  ofstream fp;
		  fp.open(OutputFilename);
		  */

		  cout << "Writing dotp values to file" << endl;

	  }

	  // NOTE: In CAPI, we use val not re_val - ska - 03/10/2016
	  for(int v = 0; v < imageCellHeight; v += 1){
	for(int h = 0; h < imageCellWidth; h += 1){
	  if((0<v && (v + roiCellHeight) < imageCellHeight)
	     &&
	     (0<h && (h + roiCellWidth) < imageCellWidth)){
	    // Endian Change ( is this work?)
	    //uint32_t re_val = (((val&&0xFF) << 24) | ((val&&0xFF00) << (16-8)) | ((val&&0xFF0000) >> (16-8)) | ((val&&0xFF000000) >> (24)) );
		if (check_output)
		{
			uint32_t val = (uint32_t)memAddr[v*imageCellWidth+h];
			fprintf(fp1, "%08X\n", val);
			//fprintf(fp2, "%08X\n", re_val);
		}
	    //memAddr[v*imageCellWidth+h] = (int32_t)re_val; // re_val needed in plda
		//memAddr[v*imageCellWidth + h] = (int32_t)val;
          } else {
        	  // invalid area
	    memAddr[v*imageCellWidth+h] = (int32_t)((uint32_t)0x80000000);
	  }
	}
      }
    
	  if (check_output)
	  {
		  fclose(fp1);
		  //fclose(fp2);
	  }
	
	}

    return memAddr;
  }
}



uint64_t block_norm_v1_00_a::GetOutputSize(void)
{
  uint16_t imageWidth = GetImageConfiguraiton()->GetWidth();
  uint16_t imageHeight = GetImageConfiguraiton()->GetHeight();
  uint8_t cellPixWidth = GetModeDescriptor()->GetCellPixWidth();
  uint8_t cellPixHeight = GetModeDescriptor()->GetCellPixHeight();
  uint8_t binNum = GetModeDescriptor()->GetBinNum();
  uint8_t nmlzOut = GetModeDescriptor()->IsNormalizeOut();
  uint8_t dotpOut = GetModeDescriptor()->IsDotpOut();
  uint16_t imageCellWidth = imageWidth/cellPixWidth;
  uint16_t imageCellHeight = imageHeight/cellPixHeight;
  if(dotpOut){
    return 4/*byte per roi*/ * imageCellWidth * imageCellHeight;
  } 
  else if(nmlzOut){
    return 4/*block per cell*/ * binNum /*bin per cell*/ * imageCellWidth * imageCellHeight;
  }
  else {
    cout << "Error" << __FILE__ << " " << __LINE__ << endl;
    exit(1);
    return 0;
  }
}




