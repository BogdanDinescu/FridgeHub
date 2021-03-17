#include <pistache/net.h>
#include <pistache/http.h>
#include <pistache/peer.h>
#include <pistache/http_headers.h>
#include <pistache/cookie.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>
#include <pistache/common.h>
#include "Fridge.cpp"

using namespace Pistache;

class Endpoint {
public:
    explicit Endpoint(Address addr)
        : httpEndpoint(std::make_shared<Http::Endpoint>(addr))
    { }

    // Initialization of the server. Additional options can be provided here
    void init(size_t thr = 2) {
        auto opts = Http::Endpoint::options()
            .threads(static_cast<int>(thr));
        httpEndpoint->init(opts);
        // Server routes are loaded up
        setupRoutes();
    }

    // Server is started threaded.  
    void start() {
        httpEndpoint->setHandler(router.handler());
        httpEndpoint->serveThreaded();
    }

    // When signaled server shuts down
    void stop(){
        httpEndpoint->shutdown();
    }
private:
    void setupRoutes() {
        using namespace Rest;
        //Rest::Routes::Get(router, "/auth", Routes::bind(&MicrowaveEndpoint::doAuth, this));
        Routes::Post(router, "/settings/temp/:value", Routes::bind(&Endpoint::setTemp, this));
        Routes::Get(router, "/settings/temp", Routes::bind(&Endpoint::getTemp, this));
    }

    void setTemp(const Rest::Request& request, Http::ResponseWriter response) {
        int value = request.param(":value").as<int>();
        fridge.setTemp(value);
        response.send(Http::Code::Ok,  "Temp was set to " + request.param(":value").as<std::string>());
    }

    void getTemp(const Rest::Request& request, Http::ResponseWriter response){
        using namespace Http;
        int value = fridge.getTemp();
        std::string s_value = std::to_string(value);
        response.headers()
                .add<Header::Server>("pistache/0.1")
                .add<Header::ContentType>(MIME(Text, Plain));

        response.send(Http::Code::Ok, s_value);
    }

    Fridge fridge;
    std::shared_ptr<Http::Endpoint> httpEndpoint;
    Rest::Router router;
};