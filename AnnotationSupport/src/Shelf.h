#ifndef __SHELF_H__
#define __SHELF_H__
//#define _GLIBCXX_USE_CXX11_ABI 0

#include "RegionOfInterest.h"

class Shelf : public RegionOfInterest
{
public:
	Shelf(std::string Name, std::string Fullname, int id, int parent_id, std::vector<int> children_id, std::vector<PointF*> Polygon, ItemCategory Category, std::vector<ItemAttribute> Attributes);
	~Shelf();

	std::vector<RegionOfInterest*> GetProducts();
	void AddChildItem(RegionOfInterest* item) override;
};
#endif

