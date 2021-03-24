#include <iostream>
#include <list>
#include "Item.h"
#include "JsonClass.h"

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
    std::string getItemAsString(std::string);
    std::string getExpiredItems();
    bool updateItem(std::string, float);
    float calculateCalories(Json::Value);
private:
    int temp;
    std::list<Item> items;
    Item* getItem(std::string);
};