#include <iostream>
#include <signal.h>
#include <thread>
#include "JsonClass.h"
#include "Endpoint.cpp"
#include "mqttClient.hpp"

// g++ main.cpp JsonClass.cpp Fridge.cpp Item.cpp -ljsoncpp -lpistache -lpthread -lmosquitto -o main

/*
    {
        "name" : "Lapte",
        "itemExpDate" : "01/05/2010",
        "weight" : 42.30,
        "calories" : 300
    }
*/

void pistache_thread(int argc, char** argv)
{
    sigset_t signals;
    if (sigemptyset(&signals) != 0
            || sigaddset(&signals, SIGTERM) != 0
            || sigaddset(&signals, SIGINT) != 0
            || sigaddset(&signals, SIGHUP) != 0
            || pthread_sigmask(SIG_BLOCK, &signals, nullptr) != 0) {
        perror("install signal handler failed");
        return;
    }

    Port port(8080);
    int thr = 2;

    if (argc >= 2) {
        port = static_cast<uint16_t>(std::stol(argv[1]));

        if (argc == 3)
            thr = std::stoi(argv[2]);
    }

    Address addr(Ipv4::any(), port);

    std::cout << "Cores = " << hardware_concurrency() << std::endl;
    std::cout << "Using " << thr << " threads" << std::endl;

    // Instance of the class that defines what the server can do.
    Endpoint stats(addr);

    // Initialize and start the server
    stats.init(thr);
    stats.start();

    // Code that waits for the shutdown signal for the server
    int signal = 0;
    int status = sigwait(&signals, &signal);
    if (status == 0)
    {
        std::cout << "received signal " << signal << std::endl;
    }
    else
    {
        std::cerr << "sigwait returns " << status << std::endl;
    }

    stats.stop();
}

void mosquitto_thread(int argc, char** argv)
{
    struct mosquitto *mosq;
	int rc;

    /* initializare server/lib mosquitto */
	mosquitto_lib_init();

	mosq = mosquitto_new(NULL, true, NULL); // creaza un obiect mosquitto
	
	/* vrifica daca sa creat obiectul mosq cu succes */
	if(mosq == NULL){
		std::cout<<"Could not start mousquitto!";
		return;
	}

	/* Configure callbacks */
	mosquitto_connect_callback_set(mosq, on_connect);
	mosquitto_subscribe_callback_set(mosq, on_subscribe);
	mosquitto_message_callback_set(mosq, on_message);

    /* seteaza serverul si portul la care sa se conecteze */
	rc = mosquitto_connect(mosq, "test.mosquitto.org", 1883, 60);
	if(rc != MOSQ_ERR_SUCCESS)
	{
		mosquitto_destroy(mosq);
		std::cout<<mosquitto_strerror(rc);
		return;
	}
    
    /* 
        -1 pentru a functiona la infinit/pana e oprit
        1 pentru ca asa trebuie(nu au terminat cei de la mosquitto de implementat)
    */
	mosquitto_loop_forever(mosq, -1, 1);

	mosquitto_lib_cleanup();
}

int main(int argc, char** argv)
{
    std::thread thread1(pistache_thread, argc, argv);
    std::thread thread2(mosquitto_thread, argc, argv);

    thread1.join();
    thread2.join();
    return 0;
}
