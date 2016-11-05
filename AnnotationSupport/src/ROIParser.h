#ifndef __ROIPARSER_H__
#define __ROIPARSER_H__

//#define _GLIBCXX_USE_CXX11_ABI 0

#include "Annotation.h"
#include "RegionOfInterest.h"
#include "Aisle.h"
#include "Product.h"
#include "ProductGroup.h"
#include "Shelf.h"
#include "Label.h"
#include "LabelGraphic.h"
#include "LabelText.h"

#include <set>
#include <map>

class ROIParser
{
public:
	ROIParser();
	~ROIParser();
	
	std::vector<RegionOfInterest*> GetROI();
	std::vector<Aisle*> GetAisles();
	std::vector<Shelf*> GetShelves();
	std::vector<Product*> GetProducts();
	std::vector<ProductGroup*> GetProductGroups();
	std::vector<Label*> GetLabels();
	std::vector<LabelText*> GetLabelTexts();
	std::vector<LabelGraphic*> GetLabelGrapics();

	static ROIParser * CreateFromFile(std::string Path);
	void Parse(std::string xmlFilepath);
	
private:
	bool success;
	std::map<int,RegionOfInterest*> m_RegionsOfInterest;
	std::set<Aisle*> m_Aisles;
	std::set<Shelf*> m_Shelves;
	std::set<ProductGroup*> m_ProductGroups;
	std::set<Product*> m_Product;
	std::set<Label*> m_Labels;
	std::set<LabelText*> m_LabelTexts;
	std::set<LabelGraphic*> m_LabelGraphics;

	void BuildHierarchies(std::vector<RegionOfInterest*> regions);
	void CreateObjectLinks(std::vector<RegionOfInterest*> regions);
	void CreateObjects(std::vector<RegionOfInterest*> regions);
	bool loadOk();
};
#endif
