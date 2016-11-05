#ifndef __AISLE_H__
#define __AISLE_H__

//#define _GLIBCXX_USE_CXX11_ABI 0

#include "RegionOfInterest.h"
#include <vector>

class Aisle : public RegionOfInterest
{
public:
	Aisle(std::string Name, std::string Fullname, int id, int parent_id, std::vector<int> children_id, std::vector<PointF*> Polygon, ItemCategory Category, std::vector<ItemAttribute> Attributes);
	~Aisle();

	std::vector<RegionOfInterest*> GetShelves();
	void AddChildItem(RegionOfInterest* item) override;
};

#endif
