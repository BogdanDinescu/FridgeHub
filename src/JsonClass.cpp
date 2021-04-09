#include <iostream>
#include <fstream>
#include "JsonClass.hpp"

JsonClass::JsonClass(){}

void JsonClass::printJson() {
    std::cout << jsonInformation << std::endl;
}

void JsonClass::parseFile(std::string str) {
    this->file_path = str;
    std::ifstream file_input(file_path);

    bool ok = reader.parse(file_input, jsonInformation);
    if ( !ok )
    {
        std::cout<<"Failed to parse configuration\n"<< reader.getFormattedErrorMessages();
    }

    file_input.close();
}

Json::Value JsonClass::getJsonInformation()
{
    return jsonInformation;
}

Json::Value JsonClass::parseString(std::string str) {
    bool ok = reader.parse(str.c_str(), jsonInformation);
    if ( !ok )
    {
        std::cout<<"Failed to parse configuration\n"<< reader.getFormattedErrorMessages();
    }
    return jsonInformation;
}
