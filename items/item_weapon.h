#ifndef ITEM_WEAPON_H
#define ITEM_WEAPON_H
#include "item.h"
#include <QObject>

// union example of efficient memory use - same memory used for iDam and fDam
union damage {
    int iDam;
    float fDam;
};

class ItemWeapon : public Item {
    Q_OBJECT

public:
    ItemWeapon(ItemJSON itemJSON);

    void take();
    void use();

signals:
    void updateInventory(Item* item);
    void actionPerformed(Item* item);

private:
    string actionString;
};

#endif // ITEM_WEAPON_H
