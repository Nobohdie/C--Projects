//cave.h
#include <string>
#include <vector>
#include "tile.h"
using namespace std;
#ifndef CAVE_H
#define CAVE_H

class cave {
    public:
        //constructor
        cave();
        cave(int lvl, int playerLocation);
        //
        void generateCave();
        void printCave(int waitTime);
        bool move(string direction);
        void interact();
        //
        vector<tile> getTiles();
        bool getTileInteractable();
        int getlvl();
    private:
        vector<tile> tiles;
        int lvl;
        int playerLocation;
};
#endif