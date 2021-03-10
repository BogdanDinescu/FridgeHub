#include <pistache/endpoint.h>
#include <iostream>
#include <cstring>
#include <jsoncpp/json/value.h>
#include "JsonClass.h"

using namespace Pistache;

struct InputHandler : public Http::Handler {
  HTTP_PROTOTYPE(InputHandler)
  void onRequest(const Http::Request& request, Http::ResponseWriter writer) override{

    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse( request.body(), root );
    if ( !parsingSuccessful )
    {
        std::cout  << "Failed to parse configuration\n"
                   << reader.getFormattedErrorMessages();
        return;
    }

    std::string encoding = root.get("encoding", "UTF-8" ).asString();

    const Json::Value Items = root["Booting Parameters"]["Main Fridge"]["Items"];

    std::cout << Items[0];

    writer.send(Http::Code::Ok, Items[0]["Name"].asString());
  }
};