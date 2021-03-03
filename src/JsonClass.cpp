#include <iostream>
#include <fstream>
#include "JsonClass.h"

JsonClass::JsonClass(){}

void JsonClass::printJson() {
    std::cout << jsonInformation << std::endl;
}

void JsonClass::parseFile(std::string str) {
    this->file_path = str;
    std::ifstream file_input(file_path);

    reader.parse(file_input, jsonInformation);

    file_input.close();
}

Json::Value JsonClass::parseString(std::string str) {
    return reader.parse(str.c_str(), jsonInformation);
}
