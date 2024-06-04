 #include "gamemanager.h"
#include "game.h"
#include <QObject>

using namespace std;

GameManager::GameManager() {}

bool GameManager::setUpNewGame() {    
        game.setCurrentLocationToStart();
        return true;
    }






