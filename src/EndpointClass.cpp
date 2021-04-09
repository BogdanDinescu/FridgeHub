#include "EndpointClass.hpp"
#include "temp.hpp"

float tempGlobal;

EndpointClass::EndpointClass(Address addr)
    : httpEndpoint(std::make_shared<Http::Endpoint>(addr))
{
    fridge.loadItems("../Inventory.json");
    fridge.setTemp(0.0);  // setam ca temperatura initiala sa fie 0 grade Celsius
}

/* Initialization of the server. Additional options can be provided here */
void EndpointClass::init(size_t thr = 2) {
    auto opts = Http::Endpoint::options()
        .threads(static_cast<int>(thr));
    httpEndpoint->init(opts);

    setupRoutes();  // Server routes are loaded up
}

// Server is started threaded.  
void EndpointClass::start() {
    httpEndpoint->setHandler(router.handler());
    httpEndpoint->serveThreaded();
}

// When signaled server shuts down
void EndpointClass::stop(){
    httpEndpoint->shutdown();
}

/* Un simplu getter pentru a obtine obiectul de tip fridge */
Fridge EndpointClass::getFridge()
{
    return fridge;
}

void EndpointClass::setupRoutes() 
{
    using namespace Rest;
    Routes::Post(router, "/settings/temp/:value", Routes::bind(&EndpointClass::setTemp, this));
    Routes::Get(router, "/settings/temp", Routes::bind(&EndpointClass::getTemp, this));
    Routes::Post(router, "/items/add", Routes::bind(&EndpointClass::addItem, this));
    Routes::Get(router, "/items/:name", Routes::bind(&EndpointClass::getItem, this));
    Routes::Get(router, "/items/expired", Routes::bind(&EndpointClass::getExpired, this));
    Routes::Get(router, "/items/getAll", Routes::bind(&EndpointClass::getItems, this));
    Routes::Post(router, "/items/remove", Routes::bind(&EndpointClass::removeItem, this));
    Routes::Put(router, "/update", Routes::bind(&EndpointClass::updateItem, this));
    Routes::Post(router, "/items/calories", Routes::bind(&EndpointClass::calories, this));
    Routes::Get(router, "/totalCalories", Routes::bind(&EndpointClass::getTotalCalories, this));
}

/* Functia primeste ca parametru un string care este parsat ca float, 
 * iar daca valoarea parsata indeplineste conditiile de temperatura corecta
 * ea va fi setata ca temperatura noua altfel se va trimite un mesaj conform 
 * caruia temperatura trimisa este gresita */
void EndpointClass::setTemp(const Rest::Request& request, Http::ResponseWriter response) {
    float value = request.param(":value").as<float>();
    
    if (value >= 1 && value <= 4)
    {
        fridge.setTemp(value);
        tempGlobal = value;
        response.send(Http::Code::Ok,  "Temp was set to " + request.param(":value").as<std::string>());
    }
    else
        response.send(Http::Code::Ok, "Temperatura aleasa nu este potrivita");
}

/* Functia trimite un raspuns care contine valoarea temperaturii din frigider */
void EndpointClass::getTemp(const Rest::Request& request, Http::ResponseWriter response) {
    using namespace Http;
    float value = fridge.getTemp();
    std::string s_value = std::to_string(value);
    response.headers()
           .add<Header::Server>("pistache/0.1")
            .add<Header::ContentType>(MIME(Text, Plain));

    response.send(Http::Code::Ok, s_value);
}

/* Functia parseaza datele primite ca string Json si le va introduce in fridge */
void EndpointClass::addItem(const Rest::Request& request, Http::ResponseWriter response) {
    JsonClass json;
    Json::Value root = json.parseString(request.body());
    ItemDate date;
    fridge.addItem(
        root["name"].asString(),
        date.stringToItemDate(root["itemExpDate"].asString()),
        root["weight"].asFloat(),
        root["calories"].asFloat()
        );
    response.send(Http::Code::Ok, "Item added");
}

/* Functia trimite un raspuns care contine datele produsului cerut altfel un string
 * cu mesajul ca nu s-a gasit produsul */
void EndpointClass::getItem(const Rest::Request& request, Http::ResponseWriter response) {
    using namespace Http;
    std::string name = request.param(":name").as<std::string>();
    std::string value = fridge.getItemAsString(name);
    response.headers()
           .add<Header::Server>("pistache/0.1")
            .add<Header::ContentType>(MIME(Text, Plain));

    response.send(Http::Code::Ok, value);
}

/* functie care intoarce numele itemelor care au expirat */
void EndpointClass::getExpired(const Rest::Request& request, Http::ResponseWriter response)
{
    using namespace Http;
    std::string value = fridge.getExpiredItems();
    response.headers()
           .add<Header::Server>("pistache/0.1")
            .add<Header::ContentType>(MIME(Text, Plain));
    if (value.compare("") != 0)
        response.send(Http::Code::Ok, value);
    else
        response.send(Http::Code::Ok, "Nu au fost gasite produse expirate");
}

/* functie care trimite toate itemele */
void EndpointClass::getItems(const Rest::Request& request, Http::ResponseWriter response)
{
    using namespace Http;
    std::string value = fridge.getItemsAsString();

    response.headers()
           .add<Header::Server>("pistache/0.1")
            .add<Header::ContentType>(MIME(Text, Plain));

    response.send(Http::Code::Ok, value);
}

/* Functia sterge un produs folosind numele acestuia dat ca parametru in request body */
void EndpointClass::removeItem(const Rest::Request& request, Http::ResponseWriter response) {
    JsonClass json;
    Json::Value root = json.parseString(request.body());

    std::string value = root["name"].asString();
    fridge.removeItemByName(value);

    response.send(Http::Code::Ok, "Item removed");
}

/* functia primeste un json cu un item si modifica caloriile si cantitatea de produs daca exista */
void EndpointClass::updateItem(const Rest::Request& request, Http::ResponseWriter response)
{
    JsonClass json;
    Json::Value root = json.parseString(request.body());

    std::string value = root["name"].asString();
    float wval = root["weight"].asFloat();
    bool res = fridge.updateItem(value, wval);

    if (res)
        response.send(Http::Code::Ok, "Items updated!");
    else
        response.send(Http::Code::Ok, "The item could not be updated");
}

/* Functia calculeaza caloriile pentru o cantitate si un nume de produs date in body-ul requestului */
void EndpointClass::calories(const Rest::Request& request, Http::ResponseWriter response) {
    JsonClass json;
    Json::Value root = json.parseString(request.body());

    float calories = fridge.calculateCalories(root);

    response.send(Http::Code::Ok, std::to_string(calories));
}

/* Functia trimite un raspuns care contine numarul total de calorii al produselor care nu au expirat
 * din frigider. */
void EndpointClass::getTotalCalories(const Rest::Request& request, Http::ResponseWriter response) {
    float total_calories = fridge.getTotalCalories();

    response.send(Http::Code::Ok, std::to_string(total_calories));
}