#ifndef __PRODUCTGROUP_H__
#define __PRODUCTGROUP_H__
//#define _GLIBCXX_USE_CXX11_ABI 0

#include "RegionOfInterest.h"

class ProductGroup : public RegionOfInterest
{
public:
	ProductGroup(std::string Name, std::string Fullname, int id, int parent_id, std::vector<int> children_id, std::vector<PointF*> Polygon, ItemCategory Category, ItemGrouping Grouping, std::vector<ItemAttribute> Attributes);
	~ProductGroup();

	std::vector<RegionOfInterest*> GetProducts();
	void AddChildItem(RegionOfInterest* item) override;
};
#endif
