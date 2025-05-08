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
#include "cave.h"
#include <iostream>
#include <string>
#include <vector>
#include <conio.h> //for getch
//#include <thread> //for sleep function
using namespace std;

//color code library
const string YLW = "\033[1;33m";
const string RED = "\033[1;31m";
const string CLR = "\033[0m";

//variables used throughout main
entity player;

//function to loop for a fight
void fight(int lvl){
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
        if(enemy.getHealth() > 0) enemy.attack(player, enemy.getInventory()[0]); //default enemy attack
        getch();
    }
    //displays outcome & clears system
    if(player.getHealth() > 0) cout << YLW << "YOU WIN!" << CLR << endl;
    else cout << RED << "\tYOU DIED" << CLR << endl;
    getch();
    system("cls");
}

//primary game loop
int main() {
    //initialize variables
    string action = "";
    cave cv = cave();
    player = entity("player", 20, 10, 12);
    player.giveItem("pickaxe");
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
    cv.generateCave();
    int wait = 20;
    //while the player is alive and doesn't quit, run the core game loop
    while (action != "q" && player.getHealth() > 0) {
        cv.printCave(wait);
        wait = 0;
        action = getch();
        if (action == " " && cv.getTileInteractable()) {
            wait = 20;
            cv.interact();
        }
        else if(action == "/") cv.generateCave(); //dev tool to continually generate cave
        else {
            if(cv.move(action)) fight(cv.getlvl());
        }
    }
    return 0;
}