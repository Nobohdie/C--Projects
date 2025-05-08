#include "tile.h"
#include <string>
using namespace std;
//color code library
const string PPL = "\033[1;35m";
const string CLR = "\033[0m";
const string PPL_BCK = "\033[1;45m";
const string BLK = "\033[0;30m";
const string RED = "\033[1;31m";
const string RED_BCK = "\033[1;41m";
const string GRN = "\033[1;32m";

/* 0 = empty
 * 1 = wall
 * 2 = door
 * 3 = player
 * 4 = partially broken
 * 5 = shop
 * 6 = unbreakable
*/

//constructor
tile::tile() {
    tileType = 0;
    tileString = "    ";
    prevType = 0;
    isWalkable = true;
    isInteractable = false;
}

tile::tile(int type, int num) {
    number = num;
    tileType = type;
    prevType = type;
    isInteractable = false;
    if(tileType == 0) {
        tileString = "    ";
        isWalkable = true;
    }
    else if(tileType == 1) {
        tileString = PPL_BCK + "    " + CLR;
        isWalkable = false;
    }
}

//Sets the type of tile and provides their attributes
void tile::setType(int newType, bool updatePrevType) {
    tileType = newType;
    //walkable by default
    isWalkable = true;
    //can store the previous tileType for later use
    if (updatePrevType) {
        prevType = newType;
        isInteractable = false;
    }
    switch(tileType) {
        case 0: //empty
            tileString = "    ";
            break;
        case 1: //wall: not interactable, not walkable
            tileString = PPL_BCK + "    " + CLR;
            isWalkable = false;
            break;
        case 2: //door: interactable & walkable
            tileString = RED + " [] " + CLR;
            isInteractable = true;
            break;
        case 3: //player
            tileString = "(^^)";
            break;
        case 4: //broken wall: not interactable, not walkable
            tileString = PPL_BCK +"////" + CLR;
            isWalkable = false;
            break;
        case 5: //door: walkable & interactable
            tileString = GRN + " $$ " + CLR;
            isInteractable = true;
            break;
        case 6: //indestructible wall: not interactable, not walkable
            tileString = RED_BCK + "    " + CLR;
            isWalkable = false;
            break;
    }
}

//accessors
string tile::getTileString() {
    return tileString;
}
int tile::getType() {
    return tileType;
}
int tile::getPrevType() {
    return prevType;
}
bool tile::getIsWalkable() {
    return isWalkable;
}
int tile::getNum(){
    return number;
}
bool tile::getIsInteractable() {
    return isInteractable;
}