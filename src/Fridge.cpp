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

std::string Fridge::getItem(std::string name)
{
    for (auto i=items.begin(); i!=items.end(); ++i)
    {
        if((*i).getName().compare(name) == 0) {
            return (*i).getName() + std::to_string((*i).getWeight()) + std::to_string((*i).getWeight());
        }
    }
    return "";
}

bool Fridge::removeItemByName(std::string s)
{
    for (auto i=items.begin(); i!=items.end(); ++i)
    {
        if((*i).getName().compare(s) == 0) {
            items.erase(i);
            return true;
        }
    }
    return false;
}