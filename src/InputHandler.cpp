#include <pistache/endpoint.h>
#include <iostream>
#include "JsonClass.h"

using namespace Pistache;

struct InputHandler : public Http::Handler {
  HTTP_PROTOTYPE(InputHandler)
  void onRequest(const Http::Request& request, Http::ResponseWriter writer) override{
    JsonClass json;
    Json::Value root = json.parseString(request.body());
    const Json::Value items = root["Booting Parameters"]["Main Fridge"]["Items"];
    writer.send(Http::Code::Ok, items[0]["Name"].asString());
  }
};
