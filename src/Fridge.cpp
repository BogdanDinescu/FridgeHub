#include <iostream>

class Fridge {
public:
    explicit Fridge(){ }
    int getTemp() {
        return temp;
    }
    void setTemp(int t) {
        this->temp = t;
    }
private:
    int temp;
};