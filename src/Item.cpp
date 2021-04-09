#include "Item.hpp"

Item::Item()
{}
Item::~Item()
{}
Item::Item(std::string name, ItemDate itemExpDate, float weight, float calories)
{
    this->name = name;
    this->itemExpDate = itemExpDate;
    this->weight = weight;
    this->calories = calories;
}

std::string Item::getName()
{
    return this->name;
}
void Item::setName(std::string nume)
{
    this->name = nume;
}

ItemDate Item::getItemDate()
{
    return this->itemExpDate;
}
void Item::setItemDate(ItemDate x)
{
    this->itemExpDate.day = x.day;
    this->itemExpDate.month = x.month;
    this->itemExpDate.year = x.year;
}

std::string Item::getStringItemDate()
{
    return itemExpDate.getString();
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
    this->calories = calories;
}

bool Item::isExpired()
{
    // get current time
    time_t theTime = time(NULL);
    struct tm *aTime = localtime(&theTime);
    int curr_day = aTime->tm_mday;
    int curr_month = aTime->tm_mon + 1;
    int curr_year = aTime->tm_year + 1900;
    ItemDate now(curr_day, curr_month, curr_year);

    if (this->itemExpDate < now)
    {
        return true;
    }

    return false;

}