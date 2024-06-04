 #ifndef GAME_H
#define GAME_H

#include <string>
#include <unordered_map>
#include <QString>
#include <QFile>
#include <QObject>
#include <vector>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QIODevice>
#include <QDebug>
#include "items/item.h"
#include "container.h"

using namespace std;

struct Exit {
    string description;
    string actionText;
    string id;
    bool accessible;
};

struct Obstacle {
    string description;
    string requiredAction;
    bool active;
    string actionSuccess;
    string actionFailure;
};

struct Location {
    string description_of_area;
    string location_image;
    string actionText;
    Obstacle obstacle;
    unordered_map<string, Exit> exits;
    unordered_map<string, bool> objects;
    unordered_map<string, Item*> itemMap; // Item pointer -> abstract classes cant be instantiated directly but you can have pointers to them where their values are concrete subclasses (itemweapon/itemkey)
    vector<Item*> items;
};

class Game  : public QObject
{
    Q_OBJECT

public:
    explicit Game();

    unordered_map<string, Location>& getLocations();

    void setCurrentLocationToStart();
    void moveToLocation(const string& direction);
    void interactWithObject(const string& objectId);
    void printAreaDescription();
    void fillInMap(const unordered_map<string, Location>& locationsObj);
    void getCurrentLocationExits();
    bool getCurrentLocationObstacles();
    void endGame();
    Container<Item*> getInventory();

signals:
    void transmit_to_gui(QString output);
    void areaDescriptionChanged(const QString& description);
    void locationChanged(const QString& locationID);
    void locationImageChanged(const QString& imageUrl);
    void itemImageUpdated(Item& item);
    void invalidDirection(const QString& actionText);
    void refreshUiInventory(Container<Item*> inventory);

public slots:
    void updateInventory(Item* item);
    void handleItemUsed(string actionString);
    void handleAction(Item* item);

private:
    Location* currentLocation;
    QString JsonFilePath;
    QFile File;
    QJsonDocument Document;
    unordered_map<string, Location> locations;
    bool state;
    // *arrays and pointers* vector is dynamically resizeable.
    Container<Item*> inventory;
};

// example of global variable
extern Game game;

#endif // GAME_H
