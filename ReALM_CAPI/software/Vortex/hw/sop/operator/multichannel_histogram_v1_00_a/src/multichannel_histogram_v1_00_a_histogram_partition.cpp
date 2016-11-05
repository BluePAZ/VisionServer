#include "multichannel_histogram_v1_00_a_histogram_partition.h"
#include <math.h>

histogram_partition::histogram_partition(int MaxImageWidth, int MaxImageHeight, int CellWidth, int CellHeight)
{
	m_MaxImageWidth = MaxImageWidth;
    m_MaxImageHeight = MaxImageHeight;

    int NumRows = m_MaxImageHeight / CellHeight;
    int NumCols = m_MaxImageWidth / CellWidth;

    m_CellWidth = CellWidth;
    m_CellHeight = CellHeight;

    // Default mappings
	m_CellTable.resize(NumRows);
    for (int r = 0; r < NumRows; r++)
    {
		m_CellTable[r].resize(NumCols);
        for (int c = 0; c < NumCols; c++)
        {
            AddMapping(tuple<int, int, uint16_t>(r, c, 0));
        }
    }
}

histogram_partition::~histogram_partition(void)
{
}

uint16_t histogram_partition::GetMapping(int y, int x)
{
	tuple<int,int> coordinates = GetCellCoordinates(y, x);
    int r = std::get<0>(coordinates);
    int c = std::get<1>(coordinates);

    if (r >= GetRowCount() || c >= GetColCount())
        return 0xFFFF;

    return m_CellTable[r][c];
}

void histogram_partition::AddMapping(vector<tuple<int,int,uint16_t>> CellMappings)
{
	for (std::vector<tuple<int,int,uint16_t>>::iterator it = CellMappings.begin() ; it != CellMappings.end(); ++it)
		AddMapping(*it);
}

void histogram_partition::AddMapping(tuple<int,int,uint16_t> CellMapping)
{
	int r = std::get<0>(CellMapping);
    int c = std::get<1>(CellMapping);

	if (r < GetRowCount() && c < GetColCount())
		m_CellTable[r][c] = std::get<2>(CellMapping);
}

void histogram_partition::CreateDefaultMapping()
{
	uint16_t m = 0;
    for (int r = 0; r < GetRowCount(); r++)
        for (int c = 0; c < GetColCount(); c++)
            AddMapping(tuple<int,int,uint16_t>(r, c, m++));
}

void histogram_partition::CreateRegularizedGrid(int ImageWidth, int ImageHeight, int BlockWidth, int BlockHeight)
{
	if (ImageWidth > m_MaxImageWidth || ImageHeight > m_MaxImageHeight)
	    return;

	int NumBlocksPerRow = ImageWidth / BlockWidth;

	for (int y = 0; y < ImageHeight; y++)
	{
	    for (int x = 0; x < ImageWidth; x++)
	    {
	        tuple<int,int> CellCoordinates = GetCellCoordinates(y, x);
	        int cellY = std::get<0>(CellCoordinates);
	        int cellX = std::get<1>(CellCoordinates);
	        int blockY = (cellY * m_CellHeight) / BlockHeight;
	        int blockX = (cellX * m_CellWidth) / BlockWidth;
	        int blockIndex = (blockY * NumBlocksPerRow) + blockX;

	        AddMapping(tuple<int,int,uint16_t>(cellY, cellX, (uint16_t)blockIndex));
	    }
	}
}

tuple<int,int> histogram_partition::GetCellCoordinates(int y, int x)
{
	return tuple<int,int>((int)floor((float)y / (float)m_CellHeight), (int)floor((float)x / (float)m_CellWidth) );
}

vector<uint16_t> histogram_partition::GetMappings()
{
	vector<uint16_t> Mappings;
	for (std::vector<vector<uint16_t>>::iterator r = m_CellTable.begin() ; r != m_CellTable.end(); ++r)
		for (std::vector<uint16_t>::iterator c = (*r).begin() ; c != (*r).end(); ++c)
			Mappings.push_back(*c);

	return Mappings;
}

int histogram_partition::GetRowCount()
{
	return m_CellTable.size();
}

int histogram_partition::GetColCount()
{
	return m_CellTable[0].size();
}

int histogram_partition::GetCellTableCount()
{
	return GetColCount() * GetRowCount();
}
