#include <iostream>
#include "mainwindow.h"
#include <QApplication>
#include "gamemanager.h"
#include <QFileInfo>
using namespace std;

// advanced pre processor directive to check which OS is currently in use
#ifdef _WIN32
void platformSpecificFunction() {
    cout << "Running on Windows" << endl;
}
#else
void platformSpecificFunction() {
    cout << "Running on Linux" << endl;
}
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.setFixedSize(910, 520);
    w.show();

    GameManager gameManager;
    gameManager.setUpNewGame();

    return a.exec();
}




