#include <iostream>
#include <string>

typedef struct itemDate{
    int day, month, year;
}; 

class Item
{
public:
    Item();
    ~Item();

    std::string getName();
    void setName(std::string);

    itemDate getItemDate();
    void setItemDate(itemDate);

    float getWeight();
    void setWeight(float);

    float getCalories();
    void setCalories(float);
private:
    std::string name;
    itemDate itemExpDate;
    float weight;
    float calories;
};