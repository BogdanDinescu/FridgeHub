#include "EndpointClass.hpp"
#include "temp.hpp"
#include <fstream>

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
    Routes::Get(router, "/getAudit", Routes::bind(&EndpointClass::getAudit, this));
}

/* Functia primeste ca parametru un string care este parsat ca float, 
 * iar daca valoarea parsata indeplineste conditiile de temperatura corecta
 * ea va fi setata ca temperatura noua altfel se va trimite un mesaj conform 
 * caruia temperatura trimisa este gresita */
void EndpointClass::setTemp(const Rest::Request& request, Http::ResponseWriter response) {
    time_t theTime = time(NULL);
    struct tm *aTime = localtime(&theTime);

    auto audit = fridge.getAudit();
    audit.push_back("Set temperature " + std::to_string(aTime->tm_hour) + ":" + std::to_string(aTime->tm_min) + ":" + std::to_string(aTime->tm_sec));
    fridge.setAudit(audit);
    
    appendAudit(audit);

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
    time_t theTime = time(NULL);
    struct tm *aTime = localtime(&theTime);

    auto audit = fridge.getAudit();
    audit.push_back("Get temperature " + std::to_string(aTime->tm_hour) + ":" + std::to_string(aTime->tm_min) + ":" + std::to_string(aTime->tm_sec));
    fridge.setAudit(audit);
    
    appendAudit(audit);

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
    time_t theTime = time(NULL);
    struct tm *aTime = localtime(&theTime);

    auto audit = fridge.getAudit();
    audit.push_back("Add item " + std::to_string(aTime->tm_hour) + ":" + std::to_string(aTime->tm_min) + ":" + std::to_string(aTime->tm_sec));
    fridge.setAudit(audit);
    
    appendAudit(audit);

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
    time_t theTime = time(NULL);
    struct tm *aTime = localtime(&theTime);

    auto audit = fridge.getAudit();
    audit.push_back("Get specific item " + std::to_string(aTime->tm_hour) + ":" + std::to_string(aTime->tm_min) + ":" + std::to_string(aTime->tm_sec));
    fridge.setAudit(audit);
    
    appendAudit(audit);

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
    time_t theTime = time(NULL);
    struct tm *aTime = localtime(&theTime);
    
    auto audit = fridge.getAudit();
    audit.push_back("Get expired items " + std::to_string(aTime->tm_hour) + ":" + std::to_string(aTime->tm_min) + ":" + std::to_string(aTime->tm_sec));
    fridge.setAudit(audit);

    appendAudit(audit);

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
    time_t theTime = time(NULL);
    struct tm *aTime = localtime(&theTime);
    
    auto audit = fridge.getAudit();
    audit.push_back("Get items " + std::to_string(aTime->tm_hour) + ":" + std::to_string(aTime->tm_min) + ":" + std::to_string(aTime->tm_sec));
    fridge.setAudit(audit);

    appendAudit(audit);

    using namespace Http;
    std::string value = fridge.getItemsAsString();

    response.headers()
           .add<Header::Server>("pistache/0.1")
            .add<Header::ContentType>(MIME(Text, Plain));

    response.send(Http::Code::Ok, value);
}

/* Functia sterge un produs folosind numele acestuia dat ca parametru in request body */
void EndpointClass::removeItem(const Rest::Request& request, Http::ResponseWriter response) {
    time_t theTime = time(NULL);
    struct tm *aTime = localtime(&theTime);
    
    auto audit = fridge.getAudit();
    audit.push_back("Remove item " + std::to_string(aTime->tm_hour) + ":" + std::to_string(aTime->tm_min) + ":" + std::to_string(aTime->tm_sec));
    fridge.setAudit(audit);

    appendAudit(audit);

    JsonClass json;
    Json::Value root = json.parseString(request.body());

    std::string value = root["name"].asString();
    fridge.removeItemByName(value);

    response.send(Http::Code::Ok, "Item removed");
}

/* functia primeste un json cu un item si modifica caloriile si cantitatea de produs daca exista */
void EndpointClass::updateItem(const Rest::Request& request, Http::ResponseWriter response)
{
    time_t theTime = time(NULL);
    struct tm *aTime = localtime(&theTime);
    
    auto audit = fridge.getAudit();
    audit.push_back("Update item " + std::to_string(aTime->tm_hour) + ":" + std::to_string(aTime->tm_min) + ":" + std::to_string(aTime->tm_sec));
    fridge.setAudit(audit);

    appendAudit(audit);

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
    time_t theTime = time(NULL);
    struct tm *aTime = localtime(&theTime);
    
    auto audit = fridge.getAudit();
    audit.push_back("Get calories for specific product " + std::to_string(aTime->tm_hour) + ":" + std::to_string(aTime->tm_min) + ":" + std::to_string(aTime->tm_sec));
    fridge.setAudit(audit);
    
    appendAudit(audit);

    JsonClass json;
    Json::Value root = json.parseString(request.body());

    float calories = fridge.calculateCalories(root);

    response.send(Http::Code::Ok, std::to_string(calories));
}

/* Functia trimite un raspuns care contine numarul total de calorii al produselor care nu au expirat
 * din frigider. */
void EndpointClass::getTotalCalories(const Rest::Request& request, Http::ResponseWriter response) {
    time_t theTime = time(NULL);
    struct tm *aTime = localtime(&theTime);
    
    auto audit = fridge.getAudit();
    audit.push_back("Get total calories " + std::to_string(aTime->tm_hour) + ":" + std::to_string(aTime->tm_min) + ":" + std::to_string(aTime->tm_sec));
    fridge.setAudit(audit);

    appendAudit(audit);

    float total_calories = fridge.getTotalCalories();

    response.send(Http::Code::Ok, std::to_string(total_calories));
}

void EndpointClass::getAudit(const Rest::Request& request, Http::ResponseWriter response) 
{
    std::string res = fridge.concatenateString();

    response.send(Http::Code::Ok, res);
}

void EndpointClass::appendAudit(std::vector<std::string> audits)
{
    std::ofstream f;
    f.open("Audit_Fridge.log", std::ios_base::app);

    f << audits.back() << '\n';

    // for (auto audit : audits) 
    // {
    //     f << audit << '\n';
    // }

    f.close();
}