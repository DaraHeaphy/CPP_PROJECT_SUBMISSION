#include "itemfactory.h"
#include "item_weapon.h"

ItemFactory::ItemFactory() {}

Item* ItemFactory::create(ItemJSON itemJSON) {
    if(itemJSON.type == "weapon") {
        return new ItemWeapon(itemJSON);
    } else if(itemJSON.type == "key") {
        // return new ItemKey(itemJSON);
        return NULL;
    }
}
