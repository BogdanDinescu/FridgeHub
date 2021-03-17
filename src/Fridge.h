#include <iostream>
#include <list>
#include "Item.h"

class Fridge {
public:
    Fridge();
    ~Fridge();
    int getTemp();
    void setTemp(int);
    void addItem(Item);
    void addItem(std::string, ItemDate, float, float);
    bool removeItemByName(std::string);
private:
    int temp;
    std::list<Item> items;
};