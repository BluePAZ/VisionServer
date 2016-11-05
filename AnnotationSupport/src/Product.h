#ifndef __PRODUCT_H__
#define __PRODUCT_H__

//#define _GLIBCXX_USE_CXX11_ABI 0

#include "RegionOfInterest.h"

class Product : public RegionOfInterest
{
public:
	Product(std::string Name, std::string Fullname, int id, int parent_id, std::vector<int> children_id, std::vector<PointF*> Polygon, ItemCategory Category, ItemPackageType PackageType, std::vector<ItemAttribute> Attributes);
	~Product();

	std::vector<RegionOfInterest*> GetLabels();
	void AddChildItem(RegionOfInterest* item) override;
};
#endif

