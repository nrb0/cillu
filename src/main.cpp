#include "KontrolS49.h"

#include <net/OSCReceiver.h>

#include "cabl/util/Color.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <signal.h>

//----------------------------------------------------------------------------------------------------------------------

volatile bool running = true;

void intHandler(int)
{
    running = false;
}

//----------------------------------------------------------------------------------------------------------------------

int main(int argc, const char* argv[])
{
    (void) argc; // suppress unused parameter warnings
    (void) argv; // suppress unused parameter warnings

    signal(SIGINT, intHandler);

    cillu::KontrolS49 s49;

    while(running)
    {
        s49.onTimer();
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    std::cout << "Stopping" << std::endl;

    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
