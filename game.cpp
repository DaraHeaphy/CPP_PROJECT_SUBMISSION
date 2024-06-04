#include <iostream>
#include <vector>
#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>
#include <QIODevice>
#include <QDebug>
#include <QObject>
#include <unordered_map>
#include "game.h"
#include "items/itemfactory.h"
#include "container.h"

using namespace std;

//initializer list
Game::Game() : currentLocation(nullptr), JsonFilePath(":/Locations.json"), File(JsonFilePath) {

    if (!File.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file:" << JsonFilePath;
        return;
    }

    QByteArray Bytes = File.readAll();
    File.close();

    QJsonParseError JsonError;
    Document = QJsonDocument::fromJson(Bytes, &JsonError);
    if (JsonError.error != QJsonParseError::NoError) {
        qDebug() << "Error In Json Data :" << JsonError.errorString();
        return;
    }

    if (!Document.isObject()) {
        qDebug() << "Invalid JSON format";
        return;
    }

    QJsonObject locationsObj = Document.object()["locations"].toObject();
    ItemFactory itemFactory;
    for(const QString& locationKey : locationsObj.keys()) {
        QJsonObject locationObj = locationsObj.find(locationKey).value().toObject();

        Location location;
        location.description_of_area = locationObj.value("description-of-area").toString().toStdString();
        location.location_image = locationObj.value("location-image").toString().toStdString();

        // parse exits
        QJsonObject exitsObj = locationObj["exits"].toObject();
        if(!exitsObj.isEmpty()) {
            for (const QString& exitKey : exitsObj.keys()) {
                QJsonObject exitDetailsObj = exitsObj[exitKey].toObject();
                if(exitDetailsObj.isEmpty()) {
                    continue;
                }
                Exit exit;
                if(exitDetailsObj.contains("id") && exitDetailsObj.find("id").value() != NULL){
                    exit.id = exitDetailsObj.find("id").value().toString().toStdString();
                }
                if(exitDetailsObj.contains("description") && exitDetailsObj.find("description").value() != NULL){
                    exit.description = exitDetailsObj.find("description").value().toString().toStdString();
                }
                if(exitDetailsObj.contains("accessible") && exitDetailsObj.find("accessible").value() != NULL){
                    exit.accessible = exitDetailsObj.find("accessible").value().toBool();
                }
                if(exitDetailsObj.contains("actionText") && exitDetailsObj.find("actionText").value() != NULL){
                    exit.actionText = exitDetailsObj.find("actionText").value().toString().toStdString();
                }
                location.exits[exitKey.toStdString()] = exit;
            }
        }
        // parse items
        QJsonArray itemsObj = locationObj["items"].toArray();
        for(const QJsonValueRef& valueRef : itemsObj) {
            QJsonObject itemObj = valueRef.toObject();
            ItemJSON item;
            item.name = itemObj.value("name").toString().toStdString();
            item.action = itemObj.value("action").toString().toStdString();
            item.type = itemObj.value("type").toString().toStdString();
            item.location = itemObj.value("location").toInt();
            item.item_image = itemObj.value("item-image").toString().toStdString();

            Item* abstractItem = itemFactory.create(item);
            location.items.push_back(abstractItem);
            location.itemMap[item.name] = abstractItem;
        }

        // parse obstacle
        if (locationObj.contains("obstacle")) {
            QJsonObject obstacleObj = locationObj["obstacle"].toObject();
            if (!obstacleObj.isEmpty()) {
                Obstacle obstacle;
                obstacle.description = obstacleObj.value("obstacleDescription").toString().toStdString();
                obstacle.requiredAction = obstacleObj.value("requiredAction").toString().toStdString();
                obstacle.active = obstacleObj.value("active").toBool();
                obstacle.actionSuccess = obstacleObj.value("actionSuccess").toString().toStdString();
                obstacle.actionFailure = obstacleObj.value("actionFailure").toString().toStdString();
                location.obstacle = obstacle;
            }
        }
        locations[locationKey.toStdString()] = location;
    }

    if(locations.find("start") != locations.end()) {
        currentLocation = &locations.find("start")->second;
    } else {
        throw runtime_error("No location with key 'start' found");
    }
}

unordered_map<string, Location>& Game::getLocations(){
    return this->locations;
}

void Game::fillInMap(const unordered_map<string, Location>& locationsObj) {
    locations = locationsObj;
}

void Game::setCurrentLocationToStart() {
    if (locations.find("start") != locations.end()) {
        currentLocation = &locations["start"];
        printAreaDescription();
        emit locationImageChanged(QString::fromStdString(currentLocation->location_image));
    } else {
        cout << "Start location not found." << endl;
    }
}

void Game::moveToLocation(const string& direction) {
    QString actionText;
    if (currentLocation->exits.find(direction) != currentLocation->exits.end()) {
        Exit exit = currentLocation->exits.find(direction)->second;
        if (!exit.accessible) {
            actionText += exit.actionText;
            emit invalidDirection(actionText);
            return;
        }
        if (locations.find(exit.id) != locations.end()) {
            if(getCurrentLocationObstacles() == true && currentLocation->obstacle.active == true) {
                emit invalidDirection(QString::fromStdString(currentLocation->obstacle.description));
            } else {
                currentLocation = &locations.at(exit.id);
                printAreaDescription();
                emit locationChanged(QString::fromStdString(exit.id));
                emit locationImageChanged(QString::fromStdString(currentLocation->location_image));

                vector<Item*> items = currentLocation->items;
                if(!items.empty()){
                    for(int i = 0; i < items.size(); i++) {
                        emit itemImageUpdated(*items[i]);
                    }
                }
            }
        }
    } else {
        emit invalidDirection(actionText);
    }
}

void Game::interactWithObject(const string& objectId) {
    if (currentLocation->objects.find(objectId) != currentLocation->objects.end()) {
        if (currentLocation->objects[objectId]) {
            cout << "Interacted with object: " << objectId << endl;
        } else {
            cout << "Object not accessible at this location!" << endl;
        }
    } else {
        cout << "Object not found!" << endl;
    }
};

void Game::getCurrentLocationExits() {
    cout << "Exits: ";
    for (const auto& exit : currentLocation->exits) {
        cout << exit.first << ", ";
    }
    cout << endl;
}

bool Game::getCurrentLocationObstacles() {
    if(currentLocation != nullptr) {
        return !currentLocation->obstacle.description.empty();
    }
    return false;
}

void Game::printAreaDescription() {
    QString output;
    output += currentLocation->description_of_area + "\n";
    if(getCurrentLocationObstacles()) {
    output += currentLocation->obstacle.description;
    }
    for (const auto& exit : currentLocation->exits) {
        output += exit.second.description + "\n";
    }
    emit areaDescriptionChanged(output);
}

// using overloaded "+" operator
void Game::updateInventory(Item* item) {
    inventory + item;
    emit refreshUiInventory(inventory);
}

Container<Item*> Game::getInventory() {
    return inventory;
}

void Game::handleAction(Item* item) {
    item->use();
}

void Game::handleItemUsed(string actionString) {
    string output;
    if(getCurrentLocationObstacles() == true) {
        if(actionString == currentLocation->obstacle.requiredAction) {
            currentLocation->obstacle.active = false;
            output = currentLocation->obstacle.actionSuccess;
            emit areaDescriptionChanged(QString::fromStdString(output));
       }
        else
        {
            output = currentLocation->obstacle.actionFailure;
            emit areaDescriptionChanged(QString::fromStdString(output));
        }
    }
}


