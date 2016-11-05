#include <string>
#include <sstream>
#include <vector>

#include "XmlObjectGetter.h"
#include "Annotation.h"

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		if (!item.empty())
			elems.push_back(item);
	}
	return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

XmlObjectGetter::XmlObjectGetter(std::string fileName)
{
	success = doc.LoadFile(fileName.c_str());
	if (success)
	{
		current = doc.FirstChild()->FirstChild();
		ImageFileName = current->FirstChild()->Value();
		current = current->NextSibling();
		ImageFilePath = current->FirstChild()->Value();
		advance();
	}
}

XmlObjectGetter::~XmlObjectGetter()
{
	delete current;
}

bool XmlObjectGetter::loadOk()
{
	return success;
}

void XmlObjectGetter::Initialize()
{
	id = -1;
	parent = -1;
	children.clear();
	isDeleted = false;
	isVerified = false;
	isOccluded = false;
	name = "";
	fullname = "";
	regionPolygon.clear();
	ImageFileName = "";
	ImageFilePath = "";
	Type = ItemType::UNSPECIFIED;
	Category = ItemCategory::UNSPECIFIED;
	Grouping = ItemGrouping::UNSPECIFIED;
	PackageType = ItemPackageType::UNSPECIFIED;
	Attributes.clear();
}

bool XmlObjectGetter::advance()
{
	Initialize();

	current = current->NextSibling();
	while (current && current->ValueTStr() != "object")
	{
		current = current->NextSibling();
	}
	if (!current)
	{
		return false;
	}

	return update();	
}
bool XmlObjectGetter::update()
{
	TiXmlNode* child = current->FirstChild();
	regionPolygon.clear();
	while (child)
	{
		if (child->ValueTStr() == "name")
		{
			TiXmlNode* objectData = child->FirstChild();
			if (objectData != NULL)
				name = objectData->Value();
		}
		else if (child->ValueTStr() == "polygon")
		{
			TiXmlNode* objectData = child->FirstChild();
			while (objectData)
			{
				if (objectData->ValueTStr() == "pt")
				{
					float X = atoi(objectData->FirstChild()->FirstChild()->Value());
					float Y = atoi(objectData->FirstChild()->NextSibling()->FirstChild()->Value());
					regionPolygon.push_back(new PointF(X,Y));
				}
				objectData = objectData->NextSibling();
			}
		}
		else if (child->ValueTStr() == "parts")
		{
			TiXmlNode* objectData = child->FirstChild();
			while (objectData)
			{
				if (objectData->ValueTStr() == "hasparts")
				{
					TiXmlNode* currentChild = objectData->FirstChild();
					if (currentChild != NULL)
					{
						std::vector<std::string> partsID = split(objectData->FirstChild()->Value(), ',');
						for (std::vector<std::string>::iterator it = partsID.begin(); it != partsID.end(); ++it) {
							int id = atoi((*it).c_str());
							children.push_back(id);
						}
					}
				}
				else if (objectData->ValueTStr() == "ispartof")
				{
					TiXmlNode* currentChild = objectData->FirstChild();
					if (currentChild != NULL)
						parent = atoi(objectData->FirstChild()->Value());
				}
				objectData = objectData->NextSibling();
			}
		}
		else if (child->ValueTStr() == "attributes")
		{
			TiXmlNode* objectData = child->FirstChild();
			if (objectData != NULL)
			{
				std::string currentChild = objectData->Value();
				std::vector<std::string> attributes = split(currentChild, ' ');
				parseAttributes(attributes);
			}
		}
		else if (child->ValueTStr() == "id")
		{
			TiXmlNode* objectData = child->FirstChild();
			if (objectData != NULL)
				id = atoi(objectData->ValueTStr().c_str());
		}
		else if (child->ValueTStr() == "deleted")
		{
			isDeleted = false;
			TiXmlNode* objectData = child->FirstChild();
			if (objectData != NULL)
				if (objectData->ValueTStr().c_str() != "0")
					isDeleted = true;
		}
		else if (child->ValueTStr() == "occluded")
		{
			isOccluded = false;
			TiXmlNode* objectData = child->FirstChild();
			if (objectData != NULL)
				if (objectData->ValueTStr().c_str() != "no")
					isOccluded = true;
		}
		else if (child->ValueTStr() == "verified")
		{
			isVerified = false;
			TiXmlNode* objectData = child->FirstChild();
			if (objectData != NULL)
				if (objectData->ValueTStr().c_str() != "0")
					isVerified = true;
		}
		child = child->NextSibling();
	}
	return true;
}

void XmlObjectGetter::parseAttributes(std::vector<std::string> attributes)
{
	Type = ItemType::UNSPECIFIED;
	Category = ItemCategory::UNSPECIFIED;
	Grouping = ItemGrouping::UNSPECIFIED;
	PackageType = ItemPackageType::UNSPECIFIED;

	for (std::vector<std::string>::iterator it = attributes.begin(); it != attributes.end(); ++it) {
		if ((*it) == "UNSPECIFIED")
			Type = ItemType::UNSPECIFIED;
		else if ((*it) == "AISLE")
			Type = ItemType::AISLE;
		else if ((*it) == "SHELF")
			Type = ItemType::SHELF;
		else if ((*it) == "PRODUCT_GROUP")
			Type = ItemType::PRODUCT_GROUP;
		else if ((*it) == "PRODUCT")
			Type = ItemType::PRODUCT;
		else if ((*it) == "LABEL")
			Type = ItemType::LABEL;
		else if ((*it) == "LABEL_TEXT")
			Type = ItemType::LABEL_TEXT;
		else if ((*it) == "LABEL_GRAPHIC")
			Type = ItemType::LABEL_GRAPHIC;
		else if ((*it) == "SODA")
			Category = ItemCategory::SODA;
		else if ((*it) == "CEREAL")
			Category = ItemCategory::CEREAL;
		else if ((*it) == "FLOUR")
			Category = ItemCategory::FLOUR;
		else if ((*it) == "CANNED_GOOD")
			Category = ItemCategory::CANNED_GOOD;
		else if ((*it) == "TWO")
			Grouping = ItemGrouping::TWO;
		else if ((*it) == "THREE")
			Grouping = ItemGrouping::THREE;
		else if ((*it) == "FOUR")
			Grouping = ItemGrouping::FOUR;
		else if ((*it) == "SEVERAL")
			Grouping = ItemGrouping::SEVERAL;
		else if ((*it) == "MANY")
			Grouping = ItemGrouping::MANY;
		else if ((*it) == "CAN_TALL")
			PackageType = ItemPackageType::CAN_TALL;
		else if ((*it) == "CAN_SHORT")
			PackageType = ItemPackageType::CAN_SHORT;
		else if ((*it) == "BOTTLE_TALL")
			PackageType = ItemPackageType::BOTTLE_TALL;
		else if ((*it) == "BOTTLE_MEDIUM")
			PackageType = ItemPackageType::BOTTLE_MEDIUM;
		else if ((*it) == "BOTTLE_SHORT")
			PackageType = ItemPackageType::BOTTLE_SHORT;
		else if ((*it) == "JAR_TALL")
			PackageType = ItemPackageType::JAR_TALL;
		else if ((*it) == "JAR_MEDIUM")
			PackageType = ItemPackageType::JAR_MEDIUM;
		else if ((*it) == "JAR_SHORT")
			PackageType = ItemPackageType::JAR_SHORT;
		else if ((*it) == "BOX_SQUARE")
			PackageType = ItemPackageType::BOX_SQUARE;
		else if ((*it) == "BOX_TALL")
			PackageType = ItemPackageType::BOX_TALL;
		else if ((*it) == "BOX_WIDE")
			PackageType = ItemPackageType::BOX_WIDE;
		else if ((*it) == "DIET")
			Attributes.push_back(ItemAttribute::DIET);
		else if ((*it) == "NO_CALORIE")
			Attributes.push_back(ItemAttribute::NO_CALORIE);
		else if ((*it) == "LOW_CALORIE")
			Attributes.push_back(ItemAttribute::LOW_CALORIE);
		else if ((*it) == "CAFFINE_FREE")
			Attributes.push_back(ItemAttribute::CAFFINE_FREE);
		else if ((*it) == "LOW_SODIUM")
			Attributes.push_back(ItemAttribute::LOW_SODIUM);
		else if ((*it) == "GLUTEN_FREE")
			Attributes.push_back(ItemAttribute::GLUTEN_FREE);
	}
}

int XmlObjectGetter::getID()
{
	return id;
}

bool XmlObjectGetter::getIsDeleted()
{
	return isDeleted;
}

bool XmlObjectGetter::getIsVerifed()
{
	return isVerified;
}

bool XmlObjectGetter::getIsOccluded()
{
	return isOccluded;
}

std::string XmlObjectGetter::getObjectFullname()
{
	return name;
}

std::string XmlObjectGetter::getFileName()
{
	return ImageFileName;
}

std::string XmlObjectGetter::getFilePath()
{
	return ImageFilePath;
}

int XmlObjectGetter::getParentPart()
{
	return parent;
}

std::vector<int> XmlObjectGetter::getChildrenID()
{
	return children;
}

std::string XmlObjectGetter::getObjectName()
{
	return name;
}

std::vector<PointF*> XmlObjectGetter::getRegionPolygon(bool RestrictBox)
{
	return regionPolygon;
}

ItemType XmlObjectGetter::getItemType()
{
	return Type;
}

ItemCategory XmlObjectGetter::getItemCategory()
{
	return Category;
}

ItemGrouping XmlObjectGetter::getItemGrouping()
{
	return Grouping;
}

ItemPackageType XmlObjectGetter::getItemPackageType()
{
	return PackageType;
}

std::vector<ItemAttribute> XmlObjectGetter::getItemAttributes()
{
	return Attributes;
}

/*std::vector<Point2DFloat*> regionPolygon;
TiXmlDocument doc;
std::string label;
TiXmlNode* current;*/
