#ifndef __REGIONOFINTEREST_H__
#define __REGIONOFINTEREST_H__
//#define _GLIBCXX_USE_CXX11_ABI 0


#include <vector>
#include "Annotation.h"
#include "PointF.h"

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/nonfree/nonfree.hpp"

class RegionOfInterest
{
public:
	RegionOfInterest(std::string name, std::string fullname, int id, int parent_id, std::vector<int> children_id, std::vector<PointF*> polygon, ItemType type, ItemCategory category, ItemGrouping grouping, ItemPackageType packageType, std::vector<ItemAttribute> attributes);
	~RegionOfInterest();

	std::string Name;
	std::string Fullname;
	int ID;
	ItemType Type;
	ItemCategory Category;
	ItemGrouping Grouping;
	ItemPackageType PackageType;
	std::vector<ItemAttribute> Attributes;
	std::vector<PointF*> Polygon;
	float XScale;
	float YScale;
	
	virtual void SetScale(float xscale, float yscale);
	virtual void UpdatePolygon();
	virtual std::vector<RegionOfInterest*> GetChildItems();
	virtual void AddChildItem(RegionOfInterest* item);
	virtual void AddChildItem(std::vector<RegionOfInterest*> items);
	virtual void AddChildID(int id);
	virtual void AddChildID(std::vector<int> id);
	virtual void SetID(int id);
	virtual int GetID();
	virtual void SetParentID(int id);
	virtual void SetParent(RegionOfInterest* parent);
	virtual RegionOfInterest* GetParent();
	virtual int GetParentID();
	virtual std::vector<int> GetChildID();
	virtual std::vector<PointF*>GetPolygon(cv::Mat * transformMatrix);
	virtual std::vector<PointF*>GetPolygon();
protected:
	std::vector<RegionOfInterest*> m_ChildItems;
	std::vector<int> m_ChildrenID;
	RegionOfInterest* m_Parent;
	int m_ParentID;

};
#endif

