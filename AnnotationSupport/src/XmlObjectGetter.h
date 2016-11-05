#ifndef __XMLOBJECT_GETTER_H__
#define __XMLOBJECT_GETTER_H__

#include "Annotation.h"
#include "PointF.h"

class XmlObjectGetter
{
public:
	XmlObjectGetter(std::string fileName);
	~XmlObjectGetter();

	std::string getObjectName();
	std::string getObjectFullname();
	bool advance();
	std::vector<PointF*> getRegionPolygon(bool RestrictBox = false);
	std::string getFileName();
	std::string getFilePath();
	int getID();
	int getParentPart();
	std::vector<int> getChildrenID();
	bool getIsDeleted();
	bool getIsVerifed();
	bool getIsOccluded();
	ItemType getItemType();
	ItemCategory getItemCategory();
	ItemGrouping getItemGrouping();
	ItemPackageType getItemPackageType();
	std::vector<ItemAttribute> getItemAttributes();

	bool loadOk();

private:
	bool update();
	bool success;
	TiXmlDocument doc;
	std::string name;
	std::string fullname;
	std::vector<int> children;
	int parent;
	int id;
	bool isDeleted;
	bool isVerified;
	bool isOccluded;
	std::vector<PointF*> regionPolygon;
	TiXmlNode* current;
	std::string ImageFileName;
	std::string ImageFilePath;
	ItemType Type;
	ItemCategory Category;
	ItemGrouping Grouping;
	ItemPackageType PackageType;
	std::vector<ItemAttribute> Attributes;

	void Initialize();
	void parseAttributes(std::vector<std::string> Attributes);
};
#endif
