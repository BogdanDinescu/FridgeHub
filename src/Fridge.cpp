#include "Fridge.h"
#include <list>

Fridge::Fridge()
{
}

Fridge::~Fridge(){}

int Fridge::getTemp() 
{
    return temp;
}

void Fridge::setTemp(int t) 
{
    this->temp = t;
}

void Fridge::addItem(Item item)
{
    this->items.push_back(item);
}

void Fridge::addItem(std::string name, ItemDate itemExpDate, float weight, float calories)
{
    Item i(name, itemExpDate, weight, calories);
    this->items.push_back(i);
}

bool Fridge::removeItemByName(std::string s)
{
    for (auto i:items)
    {
        if(i.getName().compare(s) == 0) {
            items.remove(i);
            return true;
        }
    }
    return false;
}