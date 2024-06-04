#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include <QObject>

using namespace std;

class GameManager : public QObject
{
    Q_OBJECT

public:
    GameManager();
    bool setUpNewGame();
};

#endif // GAMEMANAGER_H
