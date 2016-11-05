#include "ProductGroup.h"


ProductGroup::ProductGroup(std::string Name, std::string Fullname, int id, int parent_id, std::vector<int> children_id, std::vector<PointF*> Polygon, ItemCategory Category, ItemGrouping Grouping, std::vector<ItemAttribute> Attributes)
	:RegionOfInterest(Name, Fullname, id, parent_id, children_id, Polygon, ItemType::PRODUCT_GROUP, Category, ItemGrouping::UNSPECIFIED, ItemPackageType::UNSPECIFIED, Attributes)
{
}

ProductGroup::~ProductGroup()
{
}

std::vector<RegionOfInterest*> ProductGroup::GetProducts()
{
	std::vector<RegionOfInterest*> products;

	for (std::vector<RegionOfInterest*>::iterator it = m_ChildItems.begin(); it != m_ChildItems.end(); ++it) {
		if ((*it)->Type == ItemType::PRODUCT)
			products.push_back(*it);
	}

	return products;
}

void ProductGroup::AddChildItem(RegionOfInterest * item)
{
	if (item->Type == ItemType::PRODUCT)
		RegionOfInterest::AddChildItem(item);
}
