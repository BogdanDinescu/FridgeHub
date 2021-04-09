#include <iostream>
#include <list>
#include "Item.hpp"
#include "JsonClass.hpp"

class Fridge {
public:
    Fridge();
    ~Fridge();
    
    float getTemp();
    void setTemp(float);
    
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
    float getTotalCalories();
private:
    float temp;
    std::list<Item> items;
    Item* getItem(std::string);
};
