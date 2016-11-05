#ifndef __LABEL_H__
#define __LABEL_H__
//#define _GLIBCXX_USE_CXX11_ABI 0


#include "RegionOfInterest.h"

class Label : public RegionOfInterest
{
public:
	Label(std::string Name, std::string Fullname, int id, int parent_id, std::vector<int> children_id, std::vector<PointF*> Polygon, std::vector<ItemAttribute> Attributes);
	~Label();

	std::vector<RegionOfInterest*> GetLabels();
	void AddChildItem(RegionOfInterest* item) override;
};
#endif

