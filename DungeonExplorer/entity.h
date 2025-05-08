#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>
using namespace std;

/*parent class entity:
 * Currently both player and enemies are entities
 * will update to separate the two into subclasses of entities
 * this will allow more abilities for the player & more engaging enemies
 */
class entity {
    public: 
        //constructors
        entity();
        entity(string name, int hp, int def, int str);
            //mutators
        virtual void setHealth(int health);
        virtual void attack(entity& target, string item);
        virtual void debuff(string attribute, int debuff);
        virtual void giveItem(string item);
            //accessors
        virtual int getHealth();
        virtual int getDefense();
        virtual string getName();
        virtual int getStr();
        virtual vector<string> getInventory();
        virtual void printStats();
        virtual void printInventory();
    protected:
        string name;
        int health;
        int defense;
        int strength;
        vector<string> inventory;
};
#endif