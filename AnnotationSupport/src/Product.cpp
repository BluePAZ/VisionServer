#include "Product.h"

Product::Product(std::string Name, std::string Fullname, int id, int parent_id, std::vector<int> children_id, std::vector<PointF*> Polygon, ItemCategory Category, ItemPackageType PackageType, std::vector<ItemAttribute> Attributes)
	: RegionOfInterest(Name, Fullname, id, parent_id, children_id, Polygon, ItemType::PRODUCT, Category, ItemGrouping::UNSPECIFIED, PackageType, Attributes)
{
}

Product::~Product()
{
}

std::vector<RegionOfInterest*> Product::GetLabels()
{
	std::vector<RegionOfInterest*> labels;

	for (std::vector<RegionOfInterest*>::iterator it = m_ChildItems.begin(); it != m_ChildItems.end(); ++it) {
		if ((*it)->Type == ItemType::LABEL_TEXT || (*it)->Type == ItemType::LABEL_GRAPHIC || (*it)->Type == ItemType::LABEL)
			labels.push_back(*it);
	}

	return labels;
}

void Product::AddChildItem(RegionOfInterest * item)
{
	if (item->Type == ItemType::LABEL_TEXT || item->Type == ItemType::LABEL_GRAPHIC || item->Type == ItemType::LABEL)
		RegionOfInterest::AddChildItem(item);
}

