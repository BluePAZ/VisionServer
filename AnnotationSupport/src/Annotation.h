#ifndef __ANNOTATION_H__
#define __ANNOTATION_H__

#include <iostream>
#include <vector>
#include "tinyxml.h"

enum class ItemType { UNSPECIFIED, AISLE, SHELF, PRODUCT_GROUP, PRODUCT, LABEL, LABEL_TEXT, LABEL_GRAPHIC };
enum class ItemCategory { UNSPECIFIED, SODA, CEREAL, FLOUR, CANNED_GOOD };
enum class ItemGrouping { UNSPECIFIED, TWO, THREE, FOUR, SEVERAL, MANY };
enum class ItemPackageType { UNSPECIFIED, CAN_TALL, CAN_SHORT, BOTTLE_TALL, BOTTLE_MEDIUM, BOTTLE_SHORT, JAR_TALL, JAR_MEDIUM, JAR_SHORT, BOX_SQUARE, BOX_TALL, BOX_WIDE };
enum class ItemAttribute { UNSPECIFIED, DIET, NO_CALORIE, LOW_CALORIE, CAFFINE_FREE, LOW_SODIUM, GLUTEN_FREE };

#endif
