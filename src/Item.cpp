#include "Item.h"

Item::Item()
{}
Item::~Item()
{}

std::string Item::getName()
{
    return this->name;
}
void Item::setName(std::string nume)
{
    this->name = nume;
}

itemDate Item::getItemDate()
{
    return this->itemExpDate;
}
void Item::setItemDate(itemDate x)
{
    this->itemExpDate.day = x.day;
    this->itemExpDate.month = x.month;
    this->itemExpDate.year = x.year;
}

float Item::getWeight()
{
    return this->weight;
}
void Item::setWeight(float weigth)
{
    this->weight = weight;
}

float Item::getCalories()
{
    return this->calories;
}
void Item::setCalories(float calories)
{
    this->calories =  calories;
}
