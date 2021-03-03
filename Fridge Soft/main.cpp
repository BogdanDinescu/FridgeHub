#include <iostream>
#include "JsonClass.hpp"

// g++ main.cpp JsonClass.cpp -ljsoncpp -o main

int main(int argc, char **argv)
{
    JsonClass a;
    a.printJson();
    return 0;
}
