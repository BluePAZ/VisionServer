#include "ROIParser.h"
#include "XmlObjectGetter.h"

ROIParser::ROIParser()
{
}

ROIParser::~ROIParser()
{
}

void ROIParser::Parse(std::string xmlFilepath)
{
	XmlObjectGetter* Ogetter = new XmlObjectGetter(xmlFilepath);
	if (!Ogetter->loadOk())
		return;
	
	std::vector<RegionOfInterest*> regions;

	bool end = false;
	while (!end)
	{
		RegionOfInterest* region = new RegionOfInterest(
			Ogetter->getObjectName(),
			Ogetter->getObjectFullname(),
			Ogetter->getID(),
			Ogetter->getParentPart(),
			Ogetter->getChildrenID(),
			Ogetter->getRegionPolygon(),
			Ogetter->getItemType(),
			Ogetter->getItemCategory(),
			Ogetter->getItemGrouping(),
			Ogetter->getItemPackageType(),
			Ogetter->getItemAttributes());
				
		regions.push_back(region);

		end = !Ogetter->advance();
	}

	BuildHierarchies(regions);
}

bool ROIParser::loadOk()
{
	return success;
}

ROIParser * ROIParser::CreateFromFile(std::string Path)
{
	ROIParser * p = new	ROIParser();
	p->Parse(Path);
	return p;
}

void ROIParser::BuildHierarchies(std::vector<RegionOfInterest*> regions)
{
	CreateObjects(regions);
	CreateObjectLinks(regions);
}

void ROIParser::CreateObjectLinks(std::vector<RegionOfInterest*> regions)
{
	for (std::vector<RegionOfInterest*>::iterator it = regions.begin(); it != regions.end(); ++it) {
		auto parent = m_RegionsOfInterest.find((*it)->ID);
		if (parent != m_RegionsOfInterest.end()){
			std::vector<int> childrenID = parent->second->GetChildID();
			for (std::vector<int>::iterator cID = childrenID.begin(); cID != childrenID.end(); ++cID) {
				auto child = m_RegionsOfInterest.find(*cID);
				if (child != m_RegionsOfInterest.end()) {
					parent->second->AddChildItem(child->second);
					child->second->SetParent(parent->second);
				}
			}
		}
	}
}

void ROIParser::CreateObjects(std::vector<RegionOfInterest*> regions)
{
	for (std::vector<RegionOfInterest*>::iterator it = regions.begin(); it != regions.end(); ++it) {
		RegionOfInterest* item = *it;
		switch (item->Type)
		{
		case ItemType::AISLE:
			{
				Aisle* aisle = new Aisle(item->Name, item->Fullname, item->GetID(), item->GetParentID(), item->GetChildID(), item->GetPolygon(), item->Category, item->Attributes);
				m_Aisles.insert(aisle);
				m_RegionsOfInterest.insert(std::pair<int, RegionOfInterest*>(item->GetID(), aisle));
			}
			break;
		case ItemType::SHELF:
			{
				Shelf* shelf = new Shelf(item->Name, item->Fullname, item->GetID(), item->GetParentID(), item->GetChildID(), item->GetPolygon(), item->Category, item->Attributes);
				m_Shelves.insert(shelf);
				m_RegionsOfInterest.insert(std::pair<int, RegionOfInterest*>(item->GetID(), shelf));
			}
			break;
		case ItemType::PRODUCT_GROUP:
			{
				ProductGroup* productGroup = new ProductGroup(item->Name, item->Fullname, item->GetID(), item->GetParentID(), item->GetChildID(), item->GetPolygon(), item->Category, item->Grouping, item->Attributes);
				m_ProductGroups.insert(productGroup);
				m_RegionsOfInterest.insert(std::pair<int, RegionOfInterest*>(item->GetID(), productGroup));
			}
			break;
		case ItemType::PRODUCT:
			{
				Product* product = new Product(item->Name, item->Fullname, item->GetID(), item->GetParentID(), item->GetChildID(), item->GetPolygon(), item->Category, item->PackageType, item->Attributes);
				m_Product.insert(product);
				m_RegionsOfInterest.insert(std::pair<int, RegionOfInterest*>(item->GetID(), product));
			}
			break;
		case ItemType::LABEL:
			{
				Label* label = new Label(item->Name, item->Fullname, item->GetID(), item->GetParentID(), item->GetChildID(), item->GetPolygon(), item->Attributes);
				m_Labels.insert(label);
				m_RegionsOfInterest.insert(std::pair<int, RegionOfInterest*>(item->GetID(), label));
			}
			break;
		case ItemType::LABEL_TEXT:
			{
				LabelText* labelText = new LabelText(item->Name, item->Fullname, item->GetID(), item->GetParentID(), item->GetChildID(), item->GetPolygon(), item->Attributes);
				m_LabelTexts.insert(labelText);
				m_RegionsOfInterest.insert(std::pair<int, RegionOfInterest*>(item->GetID(), labelText));
			}
			break;
		case ItemType::LABEL_GRAPHIC:
			{
				LabelGraphic* labelGraphic = new LabelGraphic(item->Name, item->Fullname, item->GetID(), item->GetParentID(), item->GetChildID(), item->GetPolygon(), item->Attributes);
				m_LabelGraphics.insert(labelGraphic);
				m_RegionsOfInterest.insert(std::pair<int, RegionOfInterest*>(item->GetID(), labelGraphic));
			}
			break;
		}
	}
}


std::vector<RegionOfInterest*> ROIParser::GetROI()
{
	std::vector<RegionOfInterest*> values;

	for (std::map<int, RegionOfInterest*>::iterator it = m_RegionsOfInterest.begin(); it != m_RegionsOfInterest.end(); ++it) {
		values.push_back(it->second);
	}

	return values;
}

std::vector<Aisle*> ROIParser::GetAisles()
{
	std::vector<Aisle*> values;

	for (std::set<Aisle*>::iterator it = m_Aisles.begin(); it != m_Aisles.end(); ++it) {
		values.push_back(*it);
	}

	return values;
}

std::vector<Shelf*> ROIParser::GetShelves()
{
	std::vector<Shelf*> values;

	for (std::set<Shelf*>::iterator it = m_Shelves.begin(); it != m_Shelves.end(); ++it) {
		values.push_back(*it);
	}

	return values;
}

std::vector<Product*> ROIParser::GetProducts()
{
	std::vector<Product*> values;

	for (std::set<Product*>::iterator it = m_Product.begin(); it != m_Product.end(); ++it) {
		values.push_back(*it);
	}

	return values;
}

std::vector<ProductGroup*> ROIParser::GetProductGroups()
{
	std::vector<ProductGroup*> values;

	for (std::set<ProductGroup*>::iterator it = m_ProductGroups.begin(); it != m_ProductGroups.end(); ++it) {
		values.push_back(*it);
	}

	return values;
}

std::vector<Label*> ROIParser::GetLabels()
{
	std::vector<Label*> values;

	for (std::set<Label*>::iterator it = m_Labels.begin(); it != m_Labels.end(); ++it) {
		values.push_back(*it);
	}

	return values;
}

std::vector<LabelText*> ROIParser::GetLabelTexts()
{
	std::vector<LabelText*> values;

	for (std::set<LabelText*>::iterator it = m_LabelTexts.begin(); it != m_LabelTexts.end(); ++it) {
		values.push_back(*it);
	}

	return values;
}

std::vector<LabelGraphic*> ROIParser::GetLabelGrapics()
{
	std::vector<LabelGraphic*> values;

	for (std::set<LabelGraphic*>::iterator it = m_LabelGraphics.begin(); it != m_LabelGraphics.end(); ++it) {
		values.push_back(*it);
	}

	return values;
}

