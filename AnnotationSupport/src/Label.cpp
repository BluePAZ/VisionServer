#include "Label.h"


Label::Label(std::string Name, std::string Fullname, int id, int parent_id, std::vector<int> children_id, std::vector<PointF*> Polygon, std::vector<ItemAttribute> Attributes)
	: RegionOfInterest(Name, Fullname, id, parent_id, children_id, Polygon, ItemType::LABEL, ItemCategory::UNSPECIFIED, ItemGrouping::UNSPECIFIED, ItemPackageType::UNSPECIFIED, Attributes)
{
}

Label::~Label()
{
}

std::vector<RegionOfInterest*> Label::GetLabels()
{
	std::vector<RegionOfInterest*> labels;

	for (std::vector<RegionOfInterest*>::iterator it = m_ChildItems.begin(); it != m_ChildItems.end(); ++it) {
		if ((*it)->Type == ItemType::LABEL_TEXT || (*it)->Type == ItemType::LABEL_GRAPHIC)
			labels.push_back(*it);
	}

	return labels;
}

void Label::AddChildItem(RegionOfInterest * item)
{
	if (item->Type == ItemType::LABEL_TEXT || item->Type == ItemType::LABEL_GRAPHIC)
		RegionOfInterest::AddChildItem(item);
}
