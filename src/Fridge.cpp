#include "Fridge.h"
#include "JsonClass.h"
#include <list>
#include <time.h>

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

std::list<Item> Fridge::getItems()
{
    return this->items;
}

std::string Fridge::getItemsAsString()
{
    std::string res = "";

    for (auto i = this->items.begin(); i != this->items.end(); ++i)
    {
        res += (*i).getName();
        res += ",";
    }

    if (res.size() != 0)
    {
        res.pop_back();
    }

    return res;
}

void Fridge::loadItems(std::string filePath)
{
    JsonClass js;
    js.parseFile(filePath);
    static Json::Value  jsonValues = js.getJsonInformation();

    if (jsonValues.isMember("Items"))
    {
        const Json::Value itemsFromJson = jsonValues["Items"];
        for ( auto itemFromJson : itemsFromJson )
        {
            ItemDate data;
            data.stringToItemDate(itemFromJson["itemExpDate"].asString());

            Item item(itemFromJson["name"].asString(), data, itemFromJson["weight"].asFloat(), itemFromJson["calories"].asFloat());

            this->items.push_back(item);
        }
    }
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
            return "{ \"name\" : \"" + (*i).getName() + "\",\"itemExpDate\":" + (*i).getStringItemDate() +
                    + ",\"weight\" :" + std::to_string((*i).getWeight())
                    + ",\"calories\" :" + std::to_string((*i).getCalories()) + "}";
        }
    }
    return "";
}

bool Fridge::removeItemByName(std::string s)
{
    for (auto i = items.begin(); i != items.end(); ++i)
    {
        if((*i).getName().compare(s) == 0) {
            items.erase(i);
            return true;
        }
    }
    return false;
}

std::string Fridge::getExpiredItems()
{
    /* partea asta este pentru a obtine data curenta */
    time_t theTime = time(NULL);
    struct tm *aTime = localtime(&theTime);
    int curr_day = aTime->tm_mday;
    int curr_month = aTime->tm_mon + 1;
    int curr_year = aTime->tm_year + 1900;
    ItemDate now(curr_day, curr_month, curr_year);

    std::string res = "";
    for (auto i=items.begin(); i!=items.end(); ++i)
    {
        if ((*i).getItemDate() < now)
        {
            res += (*i).getName();
            res += ",";
        }
    }
    if (res.size() != 0)
        res.pop_back();
    return res;
}

/* 
    Functia asta intoarce true daca a fost updatat produsul cu succes,
    intoarce false daca am mancat toata cantitatea(din acel produs),
    sau daca am dat un nume de produs care nu exista sau altceva
*/
bool Fridge::updateItem(std::string name, float wvalue)
{
    for (auto i = items.begin(); i != items.end(); ++i)
    {
        if ((*i).getName().compare(name) == 0)
        {
            /* in change_weight am valoarea ramasa dupa ce am mancat wvalue unitati de masa, din produsul cu numele dat 'name' */
            float change_weight = (*i).getWeight() - wvalue;
            if (change_weight < 0) 
            {
                items.erase(i);
                return false;
            }
            (*i).setWeight(change_weight);
            return true;
        }
    }

    return false;
}