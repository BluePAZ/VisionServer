#ifndef __MULTICHANNEL_HISTOGRAM_V1_00_A_PARTITION_H__
#define __MULTICHANNEL_HISTOGRAM_V1_00_A_PARTITION_H__

#include <stdint.h>
#include <vector>
#include <tuple>

using namespace std;

class histogram_partition
{
public:
	histogram_partition(int MaxImageWidth, int MaxImageHeight, int CellWidth, int CellHeight);
	~histogram_partition(void);

	uint16_t GetMapping(int y, int x);
	int GetCellTableCount();
	void AddMapping(vector<tuple<int,int,uint16_t > > CellMappings);
	void AddMapping(tuple<int,int,uint16_t > CellMapping);
	void CreateDefaultMapping();
	void CreateRegularizedGrid(int ImageWidth, int ImageHeight, int BlockWidth, int BlockHeight);
	tuple<int,int > GetCellCoordinates(int y, int x);
	vector<uint16_t> GetMappings();
private:
	int m_MaxImageWidth;
	int m_MaxImageHeight;

	vector<vector<uint16_t> > m_CellTable;

	int GetRowCount();
	int GetColCount();
	int m_CellWidth;
	int m_CellHeight;
};

#endif

