#include <iostream>

class Fridge {
public:
    Fridge();
    ~Fridge();
    int getTemp();
    void setTemp(int);
private:
    int temp;
};