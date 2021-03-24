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
    void loadItems(std::string);
    std::list<Item> getItems();
    std::string getItemsAsString();
    void addItem(std::string, ItemDate, float, float);
    bool removeItemByName(std::string);
    std::string getItem(std::string);
    std::string getExpiredItems();
private:
    int temp;
    std::list<Item> items;
};