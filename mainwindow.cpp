#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QPlainTextEdit>
#include <Qstring>
#include <string>
#include <QTextStream>
#include <QObject>
#include "movement.h"
#include "game.h"
#include <iostream>

using namespace std;

Movement movement;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&game, &Game::areaDescriptionChanged, this, &MainWindow::update_gui_text);
    connect(&game, &Game::invalidDirection, this, &MainWindow::update_gui_text);
    connect(&game, &Game::locationImageChanged, this, &MainWindow::updateLocationImage);
    connect(&game, &Game::itemImageUpdated, this, &MainWindow::updateLocationItems);
    connect(&game, &Game::refreshUiInventory, this, &MainWindow::handleInventoryRefresh);
    connect(this, &MainWindow::handleItemUsed, &game, &Game::handleItemUsed);

}

// destructor for the ui
MainWindow::~MainWindow()
{
    delete ui;
    cout << "ui deleted" << endl;
}

void MainWindow::update_gui_text(const QString& description)
{
    qApp->processEvents();
    ui->textEdit->setText(description);
    ui->textEdit->update();
}

void MainWindow::updateLocationImage(const QString& location_image) {
    QPixmap image(location_image);
    ui->locationLabel->setPixmap(image);
}

void MainWindow::updateLocationItems(Item& item) {
    if(item.location.location == 1) {
        leftItem = &item;
        updateButtonIcon(ui->itemButton, item.item_image);
    } else if(item.location.location == 2) {
        rightItem = &item;
        updateButtonIcon(ui->itemButton2, item.item_image);
    } else {
        throw InvalidItemLocation();
    }
}

void MainWindow::on_northButton_clicked()
{
    movement.goNorth();
}

void MainWindow::on_southButton_clicked()
{
    movement.goSouth();
}

void MainWindow::on_eastButton_clicked()
{
    movement.goEast();
}

void MainWindow::on_westButton_clicked()
{
    movement.goWest();
}

void MainWindow::itemTakeSequence(Item* item, QPushButton* button) {
    item->take();
    update_gui_text(QString::fromStdString("You take a " + item->name + ". This could come in handy..."));
    clearButton(button);
}

void MainWindow::on_areaDescriptionButton_clicked()
{
    game.printAreaDescription();
}

void MainWindow::on_itemButton_clicked()
{
    if(leftItem != NULL) {
        itemTakeSequence(leftItem, ui->itemButton);
        leftItem = NULL;
    }
}

void MainWindow::on_itemButton2_clicked()
{
    if(rightItem != NULL) {
        itemTakeSequence(rightItem, ui->itemButton2);
        rightItem = NULL;
    }
}

void MainWindow::handleInventoryRefresh(Container<Item*> inventory) {
    for(int i = 0; i < inventory.size(); i++) {
        string itemImage = inventory.get(i)->item_image;
        if(i == 0) {
            updateButtonIcon(ui->inventorySlot1, itemImage);
        } else if(i == 1) {
            updateButtonIcon(ui->inventorySlot2, itemImage);
        } else if(i == 2) {
            updateButtonIcon(ui->inventorySlot3, itemImage);
        } else {
            update_gui_text("Inventory is full!");
        }
    }
}

void MainWindow::updateButtonIcon(QPushButton* button, string item_image) {
    QPixmap itemImage(QString::fromStdString(item_image));
    button->setIconSize(QSize(ICON_WIDTH, ICON_HEIGHT));
    button->setIcon(itemImage);
}

void MainWindow::clearButton(QPushButton* button) {
    button->setIcon(QIcon());
}

void MainWindow::itemUsed(int inventorySlot) {
    Container<Item*> inventory = game.getInventory();
    if (0 <= inventorySlot && inventorySlot < inventory.size()) {
        Item* item = inventory.get(inventorySlot);
        if (item != NULL) {
            string actionString = item->action;
            emit handleItemUsed(actionString);
        }
    }
}


void MainWindow::on_inventorySlot1_clicked()
{
    itemUsed(0);
}


void MainWindow::on_inventorySlot2_clicked()
{
    itemUsed(1);
}


void MainWindow::on_inventorySlot3_clicked()
{
    itemUsed(2);
}


void MainWindow::on_interactButton_clicked()
{
    string actionString = "interact";
    emit handleItemUsed(actionString);
}

