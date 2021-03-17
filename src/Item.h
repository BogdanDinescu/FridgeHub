#include <iostream>
#include <string>
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

    float getWeight();
    void setWeight(float);

    float getCalories();
    void setCalories(float);
private:
    std::string name;
    ItemDate itemExpDate;
    float weight;
    float calories;
};