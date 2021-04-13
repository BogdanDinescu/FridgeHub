#include <pistache/router.h>
#include "Fridge.hpp"
#include "JsonClass.hpp"
#include <pistache/endpoint.h>

using namespace Pistache;

class EndpointClass 
{
public:
	explicit EndpointClass(Address);
	void init(size_t);
	void start();
	void stop();
	Fridge getFridge();

private:
	void setupRoutes();
	void setTemp(const Rest::Request& request, Http::ResponseWriter response);
	void getTemp(const Rest::Request& request, Http::ResponseWriter response);
	void addItem(const Rest::Request& request, Http::ResponseWriter response);
	void getItem(const Rest::Request& request, Http::ResponseWriter response);
	void getExpired(const Rest::Request& request, Http::ResponseWriter response);
	void getItems(const Rest::Request& request, Http::ResponseWriter response);
	void removeItem(const Rest::Request& request, Http::ResponseWriter response);
	void updateItem(const Rest::Request& request, Http::ResponseWriter response);
	void calories(const Rest::Request& request, Http::ResponseWriter response);
    void getTotalCalories(const Rest::Request& request, Http::ResponseWriter response);
	void getAudit(const Rest::Request& request, Http::ResponseWriter response);

	Fridge fridge;
    std::shared_ptr<Http::Endpoint> httpEndpoint;
    Rest::Router router;
};