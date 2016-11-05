#include "Shelf.h"



Shelf::Shelf(std::string Name, std::string Fullname, int id, int parent_id, std::vector<int> children_id, std::vector<PointF*> Polygon, ItemCategory Category, std::vector<ItemAttribute> Attributes)
	:RegionOfInterest(Name, Fullname, id, parent_id, children_id, Polygon, ItemType::SHELF, Category, ItemGrouping::UNSPECIFIED, ItemPackageType::UNSPECIFIED, Attributes)
{
}

Shelf::~Shelf()
{
}

std::vector<RegionOfInterest*> Shelf::GetProducts()
{
	std::vector<RegionOfInterest*> products;

	for (std::vector<RegionOfInterest*>::iterator it = m_ChildItems.begin(); it != m_ChildItems.end(); ++it) {
		if ((*it)->Type == ItemType::PRODUCT || (*it)->Type == ItemType::PRODUCT_GROUP)
			products.push_back(*it);
	}

	return products;
}

void Shelf::AddChildItem(RegionOfInterest * item)
{
	if (item->Type == ItemType::PRODUCT || item->Type == ItemType::PRODUCT_GROUP)
		RegionOfInterest::AddChildItem(item);
}

