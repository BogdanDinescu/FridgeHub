#include "Fridge.h"

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
