#include "cave.h"
//#include <vector>
//#include <string>
#include <thread>
#include <iostream>
#include <conio.h>
using namespace std;
//color code library
const string PPL = "\033[1;35m";
const string CLR = "\033[0m";
const string PPL_BCK = "\033[1;45m";
const string RED = "\033[1;31m";
const string YLW = "\033[1;33m";

cave::cave(){
    lvl = 0;
    playerLocation = 0;
    generateCave();
}

cave::cave(int lvl, int playerLocation) {
    this->lvl = lvl;
    this->tiles = tiles;
    this->playerLocation = playerLocation;
    generateCave();
}

//regenerates the tile vector
void cave::generateCave(){
    playerLocation = 0;         //resets player location
    system("cls");              //clears console
    srand(time(NULL));          //reset random seed
    int rows = rand() % 7 + 3;  //determines cave size
    //empties cave of previous tiles
    while (!tiles.empty()) {
        tiles.pop_back();
    }
    tile nextTile = tile(0, 0);
    tiles.push_back(nextTile);
    //populates cave with empty & wall tiles
    for(int i = 1; i < rows * 5; i ++) {
        int type = rand() % 2;
        int chance = rand() % 4;
        nextTile = tile(type, i);
        tiles.push_back(nextTile);
    }

    //chooses random tile and creates a spread of unbreakable tiles
    int blobSize = rand() % 5 + lvl / 2;
    int pos = rand() % (tiles.size()-8) + 7;
    //for the size of the spread, choose random direction and generate unbreakable
    for(int i = 0; i <= blobSize; i++) {
        tiles[pos].setType(6, true);
        int direction = rand() % 4;
        int modify =0;
        switch(direction){
            case 0: //left
                modify = -1;
                break;
            case 1: //right
                modify = 1;
                break;
            case 2: //up
                modify = -5;
                break;
            case 3: //down
                modify = 5;
                break;
        }
        if(pos + modify > 0 && pos + modify < tiles.size()-1) pos += modify;
    }

    //chance of creating shop
    if(rand() % 10 + 1 <= 4) tiles.at(rand() % (tiles.size()-1) + 1).setType(5, true);
    srand(time(NULL));
    //creating door and player
    tiles[rand() % (tiles.size()-4) + 4].setType(2, true);
    tiles[0].setType(3, false);
}

//prints the cave
void cave::printCave(int waitTime){
   //printing general information & top line
    printf("\nLevel: %i", lvl);
    printf("\n%s______________________%s\n", PPL.c_str(), CLR.c_str());
    printf("%s %s", PPL_BCK.c_str(), CLR.c_str());
    //prints each tile in sequence with optional wait time
    for(tile t : tiles)
    {
        if(t.getNum() % 5 == 0 && t.getNum() != 0) {
            printf("%s %s\n%s %s", PPL_BCK.c_str(), CLR.c_str(),PPL_BCK.c_str(), CLR.c_str());
        }
        printf(t.getTileString().c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
    }
    printf("%s %s\n", PPL_BCK.c_str(), CLR.c_str()); 
}

//allows for player movement in a specified direction
//will return true if a fight should start. main uses this to activate fight() method 
bool cave::move(string direction){
    system("cls");
    bool strtfight = false;
    int goalPos = playerLocation;
    char dirChar = direction[0];
    //generates a goal position based on user input and ensures it is within bounds & reachable
    switch(dirChar) {
        case 'd' : // right
            if(playerLocation + 1 < tiles.size() && ((playerLocation + 1) % 5 !=0)) goalPos ++;
            break;
        case 'w' : //up
            if(playerLocation - 5 >= 0) goalPos -=5;
            break;
        case 's': //down
            if(playerLocation + 5 < tiles.size()) goalPos += 5;
            break;
        case 'a': //left
            if((playerLocation - 1 >= 0) && ((playerLocation) % 5 !=0)) goalPos--;
            break;
        default:
            printf("%sYou cannot do that here...%s", RED.c_str(), CLR.c_str());
        }  
    //if the goal position is not walkable, begin to break the tile if possible
    if(!tiles.at(goalPos).getIsWalkable()) {
        if(tiles.at(goalPos).getType() == 1) tiles.at(goalPos).setType(4, true);
        else if(tiles.at(goalPos).getType() == 4){
            tiles.at(goalPos).setType(0, true);
            if(rand() % 10 == 1) strtfight = true; //slight chance of beginning a fight after fully breaking a wall
        }
    }
    //sets the current location of the player back to its original state, then moves the player
    else{
        tiles.at(playerLocation).setType(tiles.at(playerLocation).getPrevType(), true);
        playerLocation = goalPos;
    }
    tiles.at(playerLocation).setType(3, false);
    return strtfight; //will return -1 only if a tile is broken and a fight should start
}

void cave::interact() {
    system("cls");
    switch(tiles.at(playerLocation).getPrevType()) {
        case 2: //door: enter new level: regenerate cave & increase level
            generateCave();
            lvl ++;
            cout << YLW << "Entering next level..." << CLR;
            break;
        case 5: //shop: will be implemented later
            printf("%sTHE SHOP IS CLOSED RIGHT NOW!!! COME BACK SOON%s", RED.c_str(), CLR.c_str());
            getch();
            break;
    }
}

vector<tile> cave::getTiles(){
    return tiles;
}

bool cave::getTileInteractable(){
    return tiles.at(playerLocation).getIsInteractable();
}

int cave::getlvl(){
    return lvl;
}