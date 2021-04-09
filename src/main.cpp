#include <iostream>
#include <signal.h>
#include <thread>
#include "JsonClass.hpp"
#include "EndpointClass.hpp"
#include "mqttClient.hpp"

/* g++ main.cpp JsonClass.cpp Fridge.cpp Item.cpp EndpointClass.cpp -ljsoncpp -lpistache -lpthread -lmosquitto -o main
 * mosquitto_pub -h test.mosquitto.org -t 'fridgeHub/temperature' -m -60
 * mosquitto_sub -h test.mosquitto.org -t 'fridgeHub/temperature' */

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
    EndpointClass stats(addr);

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

	/* Required before calling other mosquitto functions */
	mosquitto_lib_init();

	/* Create a new client instance.
	 * id = NULL -> ask the broker to generate a client id for us
	 * clean session = true -> the broker should remove old sessions when we connect
	 * obj = NULL -> we aren't passing any of our private data for callbacks
	 */
	mosq = mosquitto_new(NULL, true, NULL);
	if(mosq == NULL){
		fprintf(stderr, "Error: Out of memory.\n");
		return;
	}

	/* Configure callbacks. This should be done before connecting ideally. */
	mosquitto_connect_callback_set(mosq, on_connect);
	mosquitto_publish_callback_set(mosq, on_publish);

	/* Connect to test.mosquitto.org on port 1883, with a keepalive of 60 seconds.
	 * This call makes the socket connection only, it does not complete the MQTT
	 * CONNECT/CONNACK flow, you should use mosquitto_loop_start() or
	 * mosquitto_loop_forever() for processing net traffic. */
	rc = mosquitto_connect(mosq, "test.mosquitto.org", 1883, 60);
	if(rc != MOSQ_ERR_SUCCESS){
		mosquitto_destroy(mosq);
		fprintf(stderr, "Error: %s\n", mosquitto_strerror(rc));
		return;
	}

	/* Run the network loop in a background thread, this call returns quickly. */
	rc = mosquitto_loop_start(mosq);
	if(rc != MOSQ_ERR_SUCCESS){
		mosquitto_destroy(mosq);
		fprintf(stderr, "Error: %s\n", mosquitto_strerror(rc));
		return;
	}

	while(1){
		publish_sensor_data(mosq);
	}

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
