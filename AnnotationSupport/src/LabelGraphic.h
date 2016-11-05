#ifndef __LABELGRAPHIC_H__
#define __LABELGRAPHIC_H__

//#define _GLIBCXX_USE_CXX11_ABI 0

#include "RegionOfInterest.h"

class LabelGraphic : public RegionOfInterest
{
public:
	LabelGraphic(std::string Name, std::string Fullname, int id, int parent_id, std::vector<int> children_id, std::vector<PointF*> Polygon, std::vector<ItemAttribute> Attributes);
	LabelGraphic(RegionOfInterest* roi);
	~LabelGraphic();
};
#endif

