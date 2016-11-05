/*
 * SURFWorkConsumer.cpp
 *
 *  Created on: Nov 28, 2015
 *      Author: peterazientara
 */

#include "HOGWorkConsumer.h"


#ifdef __linux__
// to_string not being recognized here
namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}
#endif

HOGWorkConsumer::HOGWorkConsumer() {
	// TODO Auto-generated constructor stub

}
HOGWorkConsumer::HOGWorkConsumer(IVortexNIFSAP * accel){
	AcceleratorInterface = accel;
}

HOGWorkConsumer::~HOGWorkConsumer() {
	// TODO Auto-generated destructor stub
}

void HOGWorkConsumer::init(){
	//_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//AcceleratorInterface = new nifsap_capi_v2_00_b(false,0,"",7,0,0);
	//if (!AcceleratorInterface->Initialize(0))
//	{
//		fprintf(stderr, "Failed to init accelerator\n");
//		delete(AcceleratorInterface);
//		return;
//	}
//	AcceleratorInterface->AddMemoryInterface(0,0,0);

	RHA = new hog_engine_v1_00_a("RHA", 0, 0, 1, AcceleratorInterface);
	RHA->AcceleratorAddNotificationTarget(0, 0, 0);

	ImageHandle	= AcceleratorInterface->AllocateMemory(currentWidth * currentHeight * currentDepth);
}
void HOGWorkConsumer::runWork(WorkItem * w){
	//TODO:: implement man logic
	parseParams((uint8_t *)w->params, w->params_length);
	memcpy((void *)ImageHandle->GetMemoryOffset(), (const void *)w->data, (unsigned long long)w->data_length);
	cout << "HOG: " << currentWidth << endl;
	cout << "HOG: " << currentHeight << endl;
	fprintf(stderr, "Starting Process\n");
	int num_histograms = (currentWidth*currentHeight)/(CELLSIZE*CELLSIZE);
	auto t1 = std::chrono::system_clock::now();
	uint32_t* Bins = RHA->Process(ImageHandle, currentWidth, currentHeight, KERNELWIDTH, KERNELWIDTH, DETECTIONTHRESHOLD);
	uint32_t* SortedBins = (uint32_t*)malloc(sizeof(uint32_t)*num_histograms*NUMRHABIN);
	reArrangeRHAOuput2(Bins, SortedBins, num_histograms, RHABATCHSIZE, NUMRHABIN);
	w->setData((uint8_t *)SortedBins, sizeof(uint32_t)*num_histograms*NUMRHABIN, false, true, false, true);
	auto t2 = std::chrono::system_clock::now();
		    	std::cout << "HOG PIPE: "
		    	  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
		    	  << " milliseconds" << std::endl;
	//AcceleratorInterface->DeallocateMemory(ImageHandle);
}
void HOGWorkConsumer::deInit(){
	//TODO:: implement cleanup
	AcceleratorInterface->DeallocateMemory(ImageHandle);
	delete(AcceleratorInterface);
}
void HOGWorkConsumer::parseParams(uint8_t * params, uint32_t paramsLength){
	if(paramsLength > 16){
		newWidth = (int)((uint32_t)((((((params[3] << 8) | params[2]) << 8) | params[1]) << 8) | params[0]));
		newHeight = (int)((uint32_t)((((((params[7] << 8) | params[6]) << 8) | params[5]) << 8) | params[4]));
		newDepth = (int)((uint32_t)((((((params[11] << 8) | params[10]) << 8) | params[9]) << 8) | params[8]));
		newThreshold = (int)((uint32_t)((((((params[15] << 8) | params[14]) << 8) | params[13]) << 8) | params[12]));
	}
	configure();
}
void HOGWorkConsumer::configure(){
	if((newWidth != currentWidth) || (newHeight != currentHeight) || (newDepth != currentDepth)){
		currentWidth = newWidth;
		currentHeight = newHeight;
		currentDepth = newDepth;
		dataSize = currentWidth * currentHeight * currentDepth;
		AcceleratorInterface->DeallocateMemory(ImageHandle);
		ImageHandle = AcceleratorInterface->AllocateMemory(dataSize);
	}
}
/**************************************************************/
/*							FUNCTIONS			    		  */
/**************************************************************/
/**
* Loads the given descriptor vector from a file
* @param descriptorVector the descriptor vector to load
* @param fileName
*/
void HOGWorkConsumer::loadDescriptorVectorFromFile(const char* fileName, cv::Mat& descriptor)
{
	printf("Loading descriptor vector from file '%s'\n", fileName);
	CvMLData* mlData = new CvMLData;
	int status = mlData->read_csv(fileName);
	if (status != 0)
		printf("Error reading svm data file\n");
	const cv::Mat tmpdescriptor = mlData->get_values();
	// Need to do physical copy to new location
	tmpdescriptor.copyTo(descriptor);
	mlData->~CvMLData();
	return;
}
/**
* Convert SVM data from floating point (LibSVM / OpenCV) to fixed point for hardware SVM
* Used Yasuki's mat2bin tool as reference
* @param svmData the trained SVM weights
*/
cv::Mat HOGWorkConsumer::convertSVM(cv::Mat& svmData)
{
	// Normalize svmData Globally
	double svmMinVal;
	double svmMaxVal;
	minMaxLoc(svmData, &svmMinVal, &svmMaxVal, NULL, NULL);
	double svmAbsMaxVal = mymax(abs(svmMinVal), abs(svmMaxVal));
	cv::Mat normsvmData;
	svmData.convertTo(normsvmData, CV_64FC1, 1 / svmAbsMaxVal, 0);
	int org_roi_height = normsvmData.rows;
	int org_roi_width = normsvmData.cols;
	int org_roi_bin = normsvmData.channels();
	// Check if dimensions is Pedro or Dalal
	assert((org_roi_bin == 31) || (org_roi_bin == 36));
	int tgt_roi_width = ((org_roi_width + 7) / 8) * 8; // bigger nearest multiply of 8 value
	int tgt_roi_height = ((org_roi_height + 7) / 8) * 8; // bigger nearest multiply of 8 value
	int tgt_roi_bin = 36;
	cv::Mat svmHW(tgt_roi_height, tgt_roi_width, CV_16SC(tgt_roi_bin));
	int space_h = (tgt_roi_width != org_roi_width) ? 1 : 0;
	int space_v = (tgt_roi_height != org_roi_height) ? 1 : 0;
	double maxVal;
	double minVal;
	double *buf = (double *)malloc(sizeof(double) * org_roi_width * org_roi_height * org_roi_bin);
	double *ptr = (double *)normsvmData.ptr<double>(0, 0);
#define REF_BUF(V,H,B) (buf[V*(org_roi_width*org_roi_bin) + H * (org_roi_bin) + B])
	for (int v = 0; v < org_roi_height; v++)
	{
		for (int h = 0; h < org_roi_width; h++)
		{
			for (int bin = 0; bin < org_roi_bin; bin++)
			{
				if (v == 0 && h == 0 && bin == 0)
				{
					maxVal = *ptr;
					minVal = *ptr;
				}
				else
				{
					maxVal = mymax(*ptr, maxVal);
					minVal = mymin(*ptr, minVal);
				}
				REF_BUF(v, h, bin) = *ptr;
				*ptr++;
			}
		}
	}
	FILE *fp = fopen("svm.bin", "wb");
	FILE *txt_fp = fopen("svm.txt", "w");
	FILE *txt_flt_fp = fopen("svm.flt.txt", "w");
	ptr = (double *)normsvmData.ptr<double>(0);
	short int maxSVal = -32768;
	short int minSVal = 32767;
	for (int v = 0; v < tgt_roi_height; v++)
	{
		for (int h = 0; h < tgt_roi_width; h++)
		{
			for (int bin = 0; bin < tgt_roi_bin; bin++)
			{
				int org_v = v - space_v;
				int org_h = h - space_h;
				bool valid = (0 <= org_v && org_v < org_roi_height
					&&
					0 <= org_h && org_h < org_roi_width
					&&
					0 <= bin && bin < org_roi_bin);
				//short int val;
				if (valid)
				{
					double tmp = REF_BUF(org_v, org_h, bin) * 16384.0;
					if (tmp < -16384.0f && 16384.0f<tmp)
					{
						printf("tmp was %f\n", tmp);
						printf("Error: overflow occured value It must be from -1.0 to 1.0\n");
					}
					//val = (short int)tmp;
					// SVM in fixed point for hardware
					svmHW.ptr<short int>(v)[svmHW.channels()*h + bin] = (short int)tmp;
				}
				else
				{
					svmHW.ptr<short int>(v)[svmHW.channels()*h + bin] = 0;
					//val = 0;
				}
				fwrite(&(svmHW.ptr<short int>(v)[svmHW.channels()*h + bin]), sizeof(short int), 1, fp);
				fprintf(txt_fp, "%10d, ", svmHW.ptr<short int>(v)[svmHW.channels()*h + bin]);
				fprintf(txt_flt_fp, "%10f, ", (double)((double)(svmHW.ptr<short int>(v)[svmHW.channels()*h + bin]) / 16384));
				maxSVal = mymax(maxSVal, svmHW.ptr<short int>(v)[svmHW.channels()*h + bin]);
				minSVal = mymin(minSVal, svmHW.ptr<short int>(v)[svmHW.channels()*h + bin]);
			}
			fprintf(txt_fp, "\n");
			fprintf(txt_flt_fp, "\n");
		}
	}
	fclose(txt_fp);
	fclose(txt_flt_fp);
	fclose(fp);
	printf("MaxFixPVal = %d MinSVal = %d\n", maxSVal, minSVal);
	printf("Info: Use (%d * CELL_WIDTH, %d * CELL_HEIGHT) pix  ROI_Size for this SVM\n", tgt_roi_width, tgt_roi_height);
	printf("Info: svm.bin dumped\n");
	return svmHW;
}
/**
* Use OpenCV groupRectangles
* @param rectList
* @param weights
* @param groupThreshold
* @params epsilon
*/
void HOGWorkConsumer::nonMaxSurpression(std::vector<cv::Rect>& rectList, std::vector<double>& weights, int groupThreshold, double eps)
{
	if (groupThreshold <= 0 || rectList.empty())
	{
		return;
	}
	CV_Assert(rectList.size() == weights.size());
	std::vector<int> labels;
	int nclasses = partition(rectList, labels, cv::SimilarRects(eps));
	std::vector<cv::Rect_<double> > rrects(nclasses);
	std::vector<int> numInClass(nclasses, 0);
	std::vector<double> foundWeights(nclasses, -std::numeric_limits<double>::max());
	int i, j, nlabels = (int)labels.size();
	for (i = 0; i < nlabels; i++)
	{
		int cls = labels[i];
		rrects[cls].x += rectList[i].x;
		rrects[cls].y += rectList[i].y;
		rrects[cls].width += rectList[i].width;
		rrects[cls].height += rectList[i].height;
		foundWeights[cls] = mymax(foundWeights[cls], weights[i]);
		numInClass[cls]++;
	}
	for (i = 0; i < nclasses; i++)
	{
		// find the average of all ROI in the cluster
		cv::Rect_<double> r = rrects[i];
		double s = 1.0 / numInClass[i];
		rrects[i] = cv::Rect_<double>(cv::saturate_cast<double>(r.x*s),
		cv::saturate_cast<double>(r.y*s),
		cv::saturate_cast<double>(r.width*s),
		cv::saturate_cast<double>(r.height*s));
	}
	rectList.clear();
	weights.clear();
	for (i = 0; i < nclasses; i++)
	{
		cv::Rect r1 = rrects[i];
		int n1 = numInClass[i];
		double w1 = foundWeights[i];
		if (n1 <= groupThreshold)
			continue;
		// filter out small rectangles inside large rectangles
		for (j = 0; j < nclasses; j++)
		{
			int n2 = numInClass[j];
			if (j == i || n2 <= groupThreshold)
				continue;
			cv::Rect r2 = rrects[j];
			int dx = cv::saturate_cast<int>(r2.width * eps);
			int dy = cv::saturate_cast<int>(r2.height * eps);
			if (r1.x >= r2.x - dx &&
				r1.y >= r2.y - dy &&
				r1.x + r1.width <= r2.x + r2.width + dx &&
				r1.y + r1.height <= r2.y + r2.height + dy &&
				(n2 > std::max(3, n1) || n1 < 3))
				break;
		}

		if (j == nclasses)
		{
			rectList.push_back(r1);
			weights.push_back(w1);
		}
	}
}
/**
* Generate Image for Hardware
* @param InputImg
* @param OutputFilename
*/
void generateHWImage(cv::Mat InputImg, std::string OutputFilename)
{
	int numHWChannels = 4; // BGRA
	bool toPad = false;
	assert((InputImg.channels() == 3) || (InputImg.channels() == numHWChannels));
	if (InputImg.channels() == 3)
		toPad = true;
	FILE *fp = NULL;
	// Open the file to write
	// This also checks if the file exists and/or can be opened for reading correctly
	fp = fopen(OutputFilename.c_str(), "w");

	if (fp == NULL)
	{
		cout << "Could not open specified file" << endl;
		return;
	}
	else
	{
		cout << "File opened successfully" << endl;
	}
	cout << "Writing pixel values to file" << endl;

	for (int y = 0; y < InputImg.rows; y++)
	{
		for (int x = 0; x < InputImg.cols; x++)
		{
			cv::Vec3b pixel = InputImg.at<cv::Vec3b>(y, x);

			for (int c = 0; c < InputImg.channels(); c++)
			{
				fprintf(fp, "%02X\n", pixel.val[c]);

				if ((c == 2) && toPad)
				{
					fprintf(fp, "%02X\n", 255);
				}
			}
		}
	}
	fclose(fp);
}
/**
* Generate Histograms for Hardware
* @param Histograms
* @param OutputFilename
*/
void generateHWBins(cv::Mat Histograms, std::string OutputFilename)
{
	int numHWChannels = 1; // Gray
	assert(Histograms.channels() == numHWChannels);
	FILE *fp = NULL;
	// Open the file to write
	// This also checks if the file exists and/or can be opened for reading correctly
	fp = fopen(OutputFilename.c_str(), "w");
	if (fp == NULL)
	{
		cout << "Could not open specified file" << endl;
		return;
	}
	else
	{
		cout << "File opened successfully" << endl;
	}
	cout << "Writing bin values to file" << endl;
	for (int y = 0; y < Histograms.rows; y++)
	{
		int32_t binvalue = Histograms.at<int32_t>(y);
		fprintf(fp, "%02X\n", (int32_t)(binvalue & 0x000000FF) >> 0);
		fprintf(fp, "%02X\n", (int32_t)(binvalue & 0x0000FF00) >> 8);
		fprintf(fp, "%02X\n", (int32_t)(binvalue & 0x00FF0000) >> 16);
		fprintf(fp, "%02X\n", (int32_t)(binvalue & 0xFF000000) >> 24);
	}

	fclose(fp);
}
/*
* Get the size of a file
* Ref: aim_test_v2_00_b.cpp
*/
long getFileSize(FILE *file)
{
	long lCurPos, lEndPos;
	lCurPos = ftell(file);
	fseek(file, 0, 2);
	lEndPos = ftell(file);
	fseek(file, lCurPos, 0);
	return lEndPos;
}
/*
* Read binary data for Hardware
* Ref: aim_test_v2_00_b.cpp
*/
void ReadImageData(std::string filename, void* dataPtr)
{
	FILE *file = NULL;      // File pointer
	// Open the file in binary mode using the "rb" format string
	// This also checks if the file exists and/or can be opened for reading correctly
	file = fopen(filename.c_str(), "rb");
	if (file == NULL)
	{
		cout << "Could not open specified file" << endl;
		return;
	}
	else
	{
		cout << "File opened successfully" << endl;
	}
	// Get the size of the file in bytes
	long fileSize = getFileSize(file);
	// Read the file in to the buffer
	fread(dataPtr, fileSize, 1, file);
}

void HOGWorkConsumer::reArrangeRHAOuput2(uint32_t* input_bins, uint32_t* output_bins, int num_histograms, int histogram_batch_size, int num_bins_per_histogram)
{
	bool dump_files = false;
	if (dump_files)
	{
		   FILE* fp_in = NULL;


		   fp_in = fopen("rha_output_unsorted_bins.dat", "w");

		   if (fp_in == NULL)
		   {
			   cerr << "Could not open file to write rha output" << endl;
			   exit(1);
		   }

		   cout << __func__ << ": File opened successfully" << endl;

		   for (int i = 0; i < num_histograms; i++)
		   {
			   for (int j = 0; j < num_bins_per_histogram; j++)
			   {
				   fprintf(fp_in, "%d\n", input_bins[i*num_bins_per_histogram+j]);
			   }
		   }

		   fclose(fp_in);
	}

	int numBatch = num_histograms/ histogram_batch_size;
	int numBatchRemainder = num_histograms % histogram_batch_size;

	int first_hist = 0;
	int last_hist = 0;

	int out_offset = 0; // this is where it should be
	int in_offset = 0;

	for (int nb = 0; nb < numBatch; nb++)
	{
		first_hist = nb*histogram_batch_size;
		last_hist = ((nb+1)*histogram_batch_size)-1;

		out_offset = first_hist*num_bins_per_histogram; // this is where it should be
		in_offset = last_hist*num_bins_per_histogram;  // this is where it was

		while (first_hist <= last_hist)
		{
			for (int b = 0; b < num_bins_per_histogram; b++)
			{
				output_bins[out_offset+b] = input_bins[in_offset+b];
			}

			out_offset += num_bins_per_histogram;
			in_offset -= num_bins_per_histogram;
			first_hist++;
		}

	}

	// Handle the last batch which may not contain 'histogram_batch_size' histograms
	if (numBatchRemainder != 0)
	{
		first_hist = numBatch*histogram_batch_size;
		last_hist = num_histograms-1;

		out_offset = first_hist*num_bins_per_histogram; // this is where it should be
		in_offset = last_hist*num_bins_per_histogram;  // this is where it was

		while (first_hist <= last_hist)
		{
			for (int b = 0; b < num_bins_per_histogram; b++)
			{
				output_bins[out_offset+b] = input_bins[in_offset+b];
			}

			out_offset += num_bins_per_histogram;
			in_offset -= num_bins_per_histogram;
			first_hist++;
		}
	}

if (dump_files)
{
	 	   FILE* fp_out = fopen("rha_output_sorted_bins.dat", "w");

           if (fp_out == NULL)
           {
        	   cerr << "Could not open file to write sorted rha output" << endl;
           	   exit(1);
           }

           cout << __func__ << ": File opened successfully" << endl;

		   for (int i = 0; i < num_histograms; i++)
		   {
			   for (int j = 0; j < num_bins_per_histogram; j++)
			   {
				   fprintf(fp_out, "%d\n", output_bins[i*num_bins_per_histogram+j]);
			   }
		   }

		   fclose(fp_out);
}
}
