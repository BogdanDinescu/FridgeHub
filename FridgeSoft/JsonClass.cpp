#include <iostream>
#include <fstream>
#include "JsonClass.h"

JsonClass::JsonClass() {
    file_path = "../Fridge.json";
    std::ifstream file_input(file_path);

    reader.parse(file_input, jsonInformation);

    file_input.close();
}

void JsonClass::printJson() {
    std::cout << jsonInformation << std::endl;
}
