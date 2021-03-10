#include <pistache/endpoint.h>
#include <iostream>
#include <jsoncpp/json/value.h>
#include "JsonClass.h"

using namespace Pistache;

struct InputHandler : public Http::Handler {
  HTTP_PROTOTYPE(InputHandler)
  void onRequest(const Http::Request& request, Http::ResponseWriter writer) override{
    JsonClass json;
    Json::Value jsonValue = json.parseString(request.body());
    std::cout<<jsonValue.asString();
    const Json::Value items = jsonValue["Booting Parameters"]["Main Fridge"]["Items"];
    
    //for (int i = 0; i < items.size(); ++i )
    //  std::cout<<items[i].get("Name","Nu s-a gasit nume").asString()<<std::endl;
    writer.send(Http::Code::Ok, items[0]["Name"].asString());
  }
};