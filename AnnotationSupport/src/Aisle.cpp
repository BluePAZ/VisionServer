#include "Aisle.h"

Aisle::Aisle(std::string Name, std::string Fullname, int id, int parent_id, std::vector<int> children_id, std::vector<PointF*> Polygon, ItemCategory Category, std::vector<ItemAttribute> Attributes)
	:RegionOfInterest(Name, Fullname, id, parent_id, children_id, Polygon, ItemType::AISLE, Category, ItemGrouping::UNSPECIFIED, ItemPackageType::UNSPECIFIED, Attributes)
{
}

Aisle::~Aisle()
{
}

std::vector<RegionOfInterest*> Aisle::GetShelves()
{
	std::vector<RegionOfInterest*> shelves;

	for (std::vector<RegionOfInterest*>::iterator it = m_ChildItems.begin(); it != m_ChildItems.end(); ++it) {
		if ((*it)->Type == ItemType::SHELF)
			shelves.push_back(*it);
	}

	return shelves;
}

void Aisle::AddChildItem(RegionOfInterest * item)
{
	if (item->Type == ItemType::SHELF)
		RegionOfInterest::AddChildItem(item);
}

