#ifndef JSONCLASS_H
#define JSONCLASS_H

#include <string>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/json.h>

class JsonClass {
    public:
        JsonClass();
        void printJson();
    private:
        std::string file_path;
        Json::Value jsonInformation;
        Json::Reader reader;
};

#endif //JSONCLASS_H
