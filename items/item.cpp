#include "item.h"

Item::Item(ItemJSON itemJSON) {
    this->name = itemJSON.name;
    this->type = itemJSON.type;
    this->action = itemJSON.action;

    // bit structure implementation
    if(itemJSON.location == 1) {
        this->location.location = 1;
    } else if(itemJSON.location == 2) {
        this->location.location = 2;
    } else {
        this->location.location = 0;
    }

    this->item_image = itemJSON.item_image;
}

Item::Item(Item* item, bool shallow) {
    // shallow copy constructor -> points to same location in memory as original item used as the copy
    if(shallow == true) {
        this->name = item->name;
        this->type = item->type;
        this->item_image = item->item_image;
        this->action = item->action;
        this->location = item->location;
    }
    // deep copy constructor -> assigns new memory locations to store the copied values in
    this->name = item->name.c_str();
    this->type = item->type.c_str();
    this->item_image = item->item_image.c_str();
    this->action = item->action.c_str();
    this->location = item->location;
}

void Item::use() {}
