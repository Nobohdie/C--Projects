/**
 * BUGS:
 *  - It is possible to get stuck in a fight by entering an empty string for choice
 * TO DO:
 *  - add shop
 *  - compact
 */
//libraries:
#include "tile.h"
#include "entity.h"
#include <iostream>
#include <string>
#include <vector>
#include <conio.h> //for getch
#include <thread> //for sleep function
using namespace std;

//color code library
const string PPL = "\033[1;35m";
const string YLW = "\033[1;33m";
const string RED = "\033[1;31m";
const string CLR = "\033[0m";
const string PPL_BCK = "\033[1;45m";

//variables used throughout main
int playerLocation;
entity player;
int lvl;

//generates a new cave
void generateCave(vector<tile> &cave) {
    playerLocation = 0;         //resets player location
    system("cls");              //clears console
    srand(time(NULL));          //reset random seed
    int rows = rand() % 7 + 3;  //determines cave size
    //empties cave of previous tiles
    while (!cave.empty()) {
        cave.pop_back();
    }
    //populates cave with empty & wall tiles
    for(int i = 0; i < rows * 5; i ++) {
        int type = rand() % 2;
        int chance = rand() % 4;
        tile nextTile = tile(type, i);
        cave.push_back(nextTile);
    }

    //chooses random tile and creates a spread of unbreakable tiles
    int blobSize = rand() % 5 + 1;
    int pos = rand() % (cave.size()-8) + 7;
    //for the size of the spread, choose random direction and generate unbreakable
    for(int i = 0; i <= blobSize; i++) {
        cave[pos].setType(6, true);
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
        if(pos + modify > 0 && pos + modify < cave.size()-1) pos += modify;
    }

    //chance of creating shop
    if(rand() % 10 + 1 <= 4) cave.at(rand() % (cave.size()-1) + 1).setType(5, true);
    srand(time(NULL));
    //creating door and player
    cave[rand() % (cave.size()-4) + 4].setType(2, true);
    cave[0].setType(3, false);
}

//prints the cave
void printCave(vector<tile> &cave, int waitTime) {
    //printing general information & top line
    printf("\nLevel: %i", lvl);
    printf("\n%s______________________%s\n", PPL.c_str(), CLR.c_str());
    printf("%s %s", PPL_BCK.c_str(), CLR.c_str());
    //prints each tile in sequence with optional wait time
    for(tile t : cave)
    {
        if(t.getNum() % 5 == 0 && t.getNum() != 0) {
            printf("%s %s\n%s %s", PPL_BCK.c_str(), CLR.c_str(),PPL_BCK.c_str(), CLR.c_str());
        }
        printf(t.getTileString().c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
    }
    printf("%s %s\n", PPL_BCK.c_str(), CLR.c_str());
}

//function to loop for a fight
void fight(){
    //starting info & variable creation
    cout << RED << "A FIGHT BEGINS!!" << CLR << endl;
    int mana = 4;
    //lists for enemies
    string enemyNames[4] = {"silverfish", "spider", "skeleton", "zombie"};
    string enemWep[4] = {"bite", "bite", "bow", "fists"};
    int enemStr[4] = {5, 11, 12, 14};
    int enemDef[4] = {6, 12, 12, 14};
    int enemHP[4] = {10, 14, 15, 17};
    //chooses random enemytype & creates enemy entity object
    srand(time(NULL));
    int enem = rand() % 4;
    entity enemy = entity(enemyNames[enem].c_str(), enemHP[enem], enemDef[enem], enemStr[enem]);
    enemy.giveItem(enemWep[enem]);
    //fight loop continues while player & enemy are alive
    while (player.getHealth() > 0 && 0 < enemy.getHealth()){
        //clears screen & provides info
        system("cls");
        cout << endl;
        enemy.printStats();
        player.printStats();
        printf("Mana: %i", mana);
        //takes attack option
        cout << "\nDo you attack (1) debuff (2) or heal (3)?\n" << endl;
        int atk;
        cin >> atk;
        switch(atk) {
            case 1: //if player chooses to attack, has player choose weapon and runs attack against enemy
                player.printInventory();
                int egg;
                printf("enter the number for the item you attack with: ");
                cin >> egg;
                player.attack(enemy, player.getInventory()[egg]);
                mana ++;
                break;
            case 2: //if chooses to inflict debuff, player chooses attribute to affect, runs debuff function. subtracts 1 mana
                if(mana >= 1) {
                    printf("Do you target their defense (1) or strength (2)?\n");
                    int chc;
                    cin >> chc;
                    if(chc == 1) enemy.debuff("defense", player.getStr() / 4);
                    else if(chc == 2) enemy.debuff("strength", player.getStr() / 4);
                    mana--;
                }
                else printf("Not enough MANA!\n");
                break;
            case 3: //if chooses to heal, sets player health to increase by strength / 4 + level / 5. Subtracts 2 mana
                if(mana >=2) {
                    int healing = player.getStr() / 4 + (lvl / 5);
                    printf("You regain %i hp\n", healing);
                    player.setHealth(player.getHealth() + healing);
                    mana -=2;
                }
                else printf("Not enough MANA!\n");
                break;
        }
        if(enemy.getHealth() > 0) enemy.attack(player, "bite"); //default enemy attack
        getch();
    }
    //displays outcome & clears system
    if(player.getHealth() > 0) cout << YLW << "YOU WIN!" << CLR << endl;
    else cout << RED << "\tYOU DIED" << CLR << endl;
    getch();
    system("cls");
}

//controls player movement
void move(string direction, vector<tile> &cave) {
    system("cls");
    int goalPos = playerLocation;
    char dirChar = direction[0];
    //generates a goal position based on user input and ensures it is within bounds & reachable
    switch(dirChar) {
        case 'd' : // right
            if(playerLocation + 1 < cave.size() && ((playerLocation + 1) % 5 !=0)) goalPos ++;
            break;
        case 'w' : //up
            if(playerLocation - 5 >= 0) goalPos -=5;
            break;
        case 's': //down
            if(playerLocation + 5 < cave.size()) goalPos += 5;
            break;
        case 'a': //left
            if((playerLocation - 1 >= 0) && ((playerLocation) % 5 !=0)) goalPos--;
            break;
        default:
            printf("%sYou cannot do that here...%s", RED.c_str(), CLR.c_str());
        }  
    //if the goal position is not walkable, begin to break the tile if possible
    if(!cave.at(goalPos).getIsWalkable()) {
        if(cave.at(goalPos).getType() == 1) cave.at(goalPos).setType(4, true);
        else if(cave.at(goalPos).getType() == 4){
            cave.at(goalPos).setType(0, true);
            if(rand() % 10 == 1) fight(); //slight chance of beginning a fight after fully breaking a wall
        }
    }
    //sets the current location of the player back to its original state, then moves the player
    else{
        cave.at(playerLocation).setType(cave.at(playerLocation).getPrevType(), true);
        playerLocation = goalPos;
    }
    cave.at(playerLocation).setType(3, false);
}

//when a player interacts with a tile: currently door or shop
void interact(vector<tile> &cave, tile &location) {
    system("cls");
    switch(location.getPrevType()) {
        case 2: //door: enter new level: regenerate cave & increase level
            generateCave(cave);
            lvl ++;
            cout << YLW << "Entering next level..." << CLR;
            break;
        case 5: //shop: will be implemented later
            printf("%sTHE SHOP IS CLOSED RIGHT NOW!!! COME BACK SOON%s", RED.c_str(), CLR.c_str());
            getch();
            break;
    }
}

//primary game loop
int main() {
    //initialize variables
    string action = "";
    vector<tile> cave;
    player = entity("player", 20, 10, 12);
    player.giveItem("pickaxe");
    lvl = 0;
    system("cls");
    //display starting information
    printf("Welcome! Press%s W A S %sor%s D %sto move.\nPress \"%sspace%s\" to interact with a tile.\n", YLW.c_str(), CLR.c_str(), YLW.c_str(), CLR.c_str(), YLW.c_str(), CLR.c_str());
    printf("%s[]%s represents a door to the next level. Interact with it to continue.\n", YLW.c_str(), CLR.c_str());
    printf("You can break a wall by attempting to walk into it. %sRED%s walls cannot be broken.\n", RED.c_str(), CLR.c_str());
    cout << endl;
    player.printStats();
    player.printInventory();
    getch();
    //generate first cave
    generateCave(cave);
    int wait = 20;
    //while the player is alive and doesn't quit, run the core game loop
    while (action != "q" && player.getHealth() > 0) {
        printCave(cave, wait);
        wait = 0;
        action = getch();
        if (action == " " && cave.at(playerLocation).getIsInteractable()) {
            wait = 20;
            interact(cave, cave.at(playerLocation));
        }
        else if(action == "/") generateCave(cave); //dev tool to continually generate cave
        else move(action, cave);
    }
    return 0;
}