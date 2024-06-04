#ifndef ITEM_H
#define ITEM_H
#include "takeable.h"
#include "useable.h"
#include "itemjson.h"
#include <string>
#include <vector>
#include <QObject>

using namespace std;

// inheritance and multiple inheritance
class Item : public Takeable, public Useable, public QObject
{
    // friendship
    friend class MainWindow;
public:
    Item();
    Item(ItemJSON itemJSON);
    Item(Item* item, bool shallow);

    virtual void use();

protected:
    string action;
    string name;
    string type;
    string item_image;

    // bit structure for location
    struct {
        unsigned int location : 2;
    } location;

private:
    vector<string> deepCopyActions();
};

#endif // ITEM_H
