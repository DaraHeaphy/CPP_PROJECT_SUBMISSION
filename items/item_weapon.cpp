#include "item_weapon.h"
#include "item.h"
#include "game.h"

ItemWeapon::ItemWeapon(ItemJSON itemJSON) : Item(itemJSON) {
    connect(this, &ItemWeapon::updateInventory, &game, &Game::updateInventory);
    connect(this, &ItemWeapon::actionPerformed, &game, &Game::handleAction);
}

void ItemWeapon::use() {
    if(name == "sword") {
        // use damage.iDam
        emit actionPerformed(this);
    }
    else
    {
        // use damage.fDam
    }
}

void ItemWeapon::take() {
    emit updateInventory(this);
}

