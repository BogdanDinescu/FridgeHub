#include <iostream>
#include "JsonClass.h"
#include "InputHandler.cpp"

// g++ main.cpp JsonClass.cpp -ljsoncpp -o main

int main(int argc, char** argv)
{
    JsonClass a;
    a.parseFile("../Fridge.json");
    a.printJson();
    Http::listenAndServe<InputHandler>(Pistache::Address("*:8080"));
    //return 0;
}
