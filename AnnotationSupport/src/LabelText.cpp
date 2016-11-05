#include "LabelText.h"



LabelText::LabelText(std::string Name, std::string Fullname, int id, int parent_id, std::vector<int> children_id, std::vector<PointF*> Polygon, std::vector<ItemAttribute> Attributes)
	: RegionOfInterest(Name, Fullname, id, parent_id, children_id, Polygon, ItemType::LABEL_TEXT, ItemCategory::UNSPECIFIED, ItemGrouping::UNSPECIFIED, ItemPackageType::UNSPECIFIED, Attributes)
{
}

LabelText::~LabelText()
{
}
