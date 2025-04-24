#include "entity.h"
#include <string>
#include <vector>
using namespace std;

//default constructor
entity :: entity(){
    name = "Default";
    health = 10;
    defense = 10;
    strength = 10;
}
//overloaded constructor
entity :: entity(string name, int hp, int def, int str){
    this-> name = name;
    health = hp;
    defense = def;
    strength = str;
}

//accessors
int entity::getHealth(){
    return health;
}
vector<string> entity::getInventory(){
    return inventory;
}
int entity::getDefense(){
    return defense;
}
string entity::getName(){
    return name;
}
int entity::getStr() {
    return strength;
}

//supplies atk modifier based on weapon choice
int getMod(string item) {
    if (item == "pickaxe") {
        return 1;
    }
    else if(item == "sword") {
        return 4;
    }
    else return 0;
}

//mutators
void entity::setHealth(int health){
    this-> health = health;
}

void entity::giveItem(string item){
    inventory.push_back(item);
}

//attacks a target entity with a chosen item
void entity::attack(entity &target, string item) {
    int dmg = strength + getMod(item) - target.getDefense();
    if(dmg < 0) dmg = 0;
    printf("%s attacks with %s dealing %i damage\n", name.c_str(), item.c_str(), dmg);
    target.setHealth(target.getHealth()-dmg);
}

//debuffs a certain attribute of the entity (defense / strength)
void entity::debuff(string attribute, int debuff) {
    if(attribute == "strength") strength -= debuff;
    else defense -= debuff;
}

//prints all entity stats
void entity::printStats() {
    printf("%s:\nstrength: %i\tdefense: %i\tHealth:%i\n", name.c_str(), strength, defense, health);
}

//prints entity's inventory
void entity::printInventory() {
    printf("Inventory:\n");
    for(int i = 0; i < inventory.size(); i ++) {
        printf("\t%i:(%s)\t", i,inventory[i].c_str());
    }
    printf("\n");
}