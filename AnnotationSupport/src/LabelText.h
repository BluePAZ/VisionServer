#ifndef __LABELTEXT_H__
#define __LABELTEXT_H__
//#define _GLIBCXX_USE_CXX11_ABI 0

#include "RegionOfInterest.h"

class LabelText : public RegionOfInterest
{
public:
	LabelText(std::string Name, std::string Fullname, int id, int parent_id, std::vector<int> children_id, std::vector<PointF*> Polygon, std::vector<ItemAttribute> Attributes);
	~LabelText();
};
#endif
