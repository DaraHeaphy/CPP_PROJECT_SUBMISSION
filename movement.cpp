#include "movement.h"
#include "game.h"

Movement::Movement() {}

Game game;

void Movement::goNorth() {
    game.moveToLocation("north");
}

void Movement::goSouth() {
    game.moveToLocation("south");
}

void Movement::goEast() {
    game.moveToLocation("east");
}

void Movement::goWest() {
    game.moveToLocation("west");
}

