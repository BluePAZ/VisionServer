#include "RegionOfInterest.h"

RegionOfInterest::RegionOfInterest(std::string name, std::string fullname, int id, int parent_id, std::vector<int> children_id, std::vector<PointF*> polygon, ItemType type, ItemCategory category, ItemGrouping grouping, ItemPackageType packageType, std::vector<ItemAttribute> attributes)
{
	Name = name;
	Fullname = fullname;
	Polygon = polygon;
	Type = type;
	Category = category;
	Grouping = grouping;
	PackageType = packageType;
	Attributes = attributes;
	
	SetID(id);
	SetParentID(parent_id);
	AddChildID(children_id);
	SetParent(NULL);
	
	XScale = 1;
	YScale = 1;
}

RegionOfInterest::~RegionOfInterest()
{
	for (std::vector<PointF*>::iterator it = Polygon.begin(); it != Polygon.end(); ++it) {
		delete(*it);
	}
}

void RegionOfInterest::SetScale(float xscale, float yscale)
{
	XScale = xscale;
	YScale = yscale;
	UpdatePolygon();
}

void RegionOfInterest::UpdatePolygon()
{
	if (XScale != 1 || YScale != 1) {
		for (int i = 0; i < Polygon.size(); i++) {
			Polygon.at(i)->X *= XScale;
			Polygon.at(i)->Y *= YScale;
		}
	}
}

std::vector<PointF*> RegionOfInterest::GetPolygon(cv::Mat * homographyMat)
{
	//Do perspectiveTransform
	std::vector<cv::Point2f> obj_corners(4);
	obj_corners[0] = cvPoint(Polygon[0]->X,Polygon[0]->Y);
	obj_corners[1] = cvPoint(Polygon[1]->X,Polygon[1]->Y);
	obj_corners[2] = cvPoint(Polygon[2]->X,Polygon[2]->Y);
	obj_corners[3] = cvPoint(Polygon[3]->X,Polygon[3]->Y);
	std::vector<cv::Point2f> scene_corners(4);
	cv::perspectiveTransform( obj_corners, scene_corners, *homographyMat);

	std::vector<PointF*> transformed_polygon;
	transformed_polygon.push_back(new PointF(scene_corners[0].x, scene_corners[0].y));
	transformed_polygon.push_back(new PointF(scene_corners[1].x, scene_corners[1].y));
	transformed_polygon.push_back(new PointF(scene_corners[2].x, scene_corners[2].y));
	transformed_polygon.push_back(new PointF(scene_corners[3].x, scene_corners[3].y));
	return transformed_polygon;
}

std::vector<PointF*> RegionOfInterest::GetPolygon()
{
	return Polygon;
}

std::vector<RegionOfInterest*> RegionOfInterest::GetChildItems()
{
	return m_ChildItems;
}

void RegionOfInterest::AddChildItem(RegionOfInterest* item)
{
	m_ChildItems.push_back(item);
}

void RegionOfInterest::AddChildItem(std::vector<RegionOfInterest*> items)
{
	for (std::vector<RegionOfInterest*>::iterator it = items.begin(); it != items.end(); ++it) {
		m_ChildItems.push_back(*it);
	}
}

void RegionOfInterest::AddChildID(int id)
{
	m_ChildrenID.push_back(id);
}

void RegionOfInterest::AddChildID(std::vector<int> id)
{
	for (std::vector<int>::iterator it = id.begin(); it != id.end(); ++it) {
		m_ChildrenID.push_back(*it);
	}
}

void RegionOfInterest::SetID(int id)
{
	ID = id;
}

int RegionOfInterest::GetID()
{
	return ID;
}

void RegionOfInterest::SetParent(RegionOfInterest * parent)
{
	m_Parent = parent;
}

RegionOfInterest * RegionOfInterest::GetParent()
{
	return m_Parent;
}

std::vector<int> RegionOfInterest::GetChildID()
{
	return m_ChildrenID;
}

void RegionOfInterest::SetParentID(int id)
{
	m_ParentID = id;
}

int RegionOfInterest::GetParentID()
{
	return m_ParentID;
}
