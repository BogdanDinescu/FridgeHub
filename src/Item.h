#include <iostream>
#include <string>
#include <time.h>
#include "ItemDate.h"

class Item
{
public:
    Item();
    Item(std::string, ItemDate, float, float);
    ~Item();

    std::string getName();
    void setName(std::string);

    ItemDate getItemDate();
    void setItemDate(ItemDate);

    std::string getStringItemDate();

    float getWeight();
    void setWeight(float);

    float getCalories();
    void setCalories(float);

    bool isExpired();
private:
    std::string name;
    ItemDate itemExpDate;
    float weight;
    float calories;
};