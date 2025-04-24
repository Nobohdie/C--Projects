//tile.h
#include <string>
using namespace std;
#ifndef TILE_H
#define TILE_H

class tile {
    public:
        //constructor
        tile();
        tile(int type, int num);
        //mutator
        void setType(int newType, bool updatePrevType);          //sets the current tile type
        void clearOldType();      //resets type and prevType to empty
        //accessor
        string getTileString();
        int getType();
        bool getIsWalkable();
        int getPrevType();
        int getNum();
        bool getIsInteractable();
    private:
        int tileType;
        int number;
        string tileString;
        int prevType;
        bool isWalkable;
        bool isInteractable;

};
#endif