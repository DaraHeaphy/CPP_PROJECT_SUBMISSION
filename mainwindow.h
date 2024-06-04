#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Qstring>
#include <QObject>
#include <QPushButton>
#include "items/item.h"
#include "container.h"

using namespace std;

class InvalidItemLocation{
    // friendship *****
    friend class Game;
};



QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
     explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
     void itemUsed(int inventorySlot);

signals:
    void updateInventory(Item* item);
    void handleItemUsed(string actionString);

public slots:
    void updateLocationImage(const QString& location_image);
    void update_gui_text(const QString& description);
    void updateLocationItems(Item& item);
    void handleInventoryRefresh(Container<Item*> inventory);

private slots:

    void on_northButton_clicked();

    void on_southButton_clicked();

    void on_eastButton_clicked();

    void on_westButton_clicked();

    void on_areaDescriptionButton_clicked();

    void on_itemButton_clicked();

    void on_itemButton2_clicked();

    void on_inventorySlot1_clicked();

    void on_inventorySlot2_clicked();

    void on_inventorySlot3_clicked();

    void on_interactButton_clicked();

private:
    Ui::MainWindow *ui;
    Item* leftItem;
    Item* rightItem;

    const int ICON_WIDTH = 81;
    const int ICON_HEIGHT = 71;

    void updateButtonIcon(QPushButton* button, string item_image);
    void clearButton(QPushButton* button);
    void itemTakeSequence(Item* item, QPushButton* button);
};
#endif // MAINWINDOW_H
