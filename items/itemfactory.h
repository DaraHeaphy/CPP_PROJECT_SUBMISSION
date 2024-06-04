#ifndef ITEMFACTORY_H
#define ITEMFACTORY_H
#include "item.h"

class ItemFactory
{
public:
    ItemFactory();

    Item* create(ItemJSON);
};

#endif // ITEMFACTORY_H
