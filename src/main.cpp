#include <iostream>
#include <signal.h>
#include <thread>
#include "JsonClass.h"
#include "InputHandler.cpp"
#include "Endpoint.cpp"

// g++ main.cpp JsonClass.cpp Fridge.cpp Item.cpp -ljsoncpp -lpistache -lpthread -o main
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

void foo()
{
    sleep(5);
    std::cout << "honey pot!\n";
}

int main(int argc, char** argv)
{
    std::thread thread1(pistache_thread, argc, argv);
    std::thread thread2(foo);

    thread1.join();
    thread2.join();
    //pistache_thread(argc, argv);
    std::cout << (int)(1000/60);
    return 0;
}
