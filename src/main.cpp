#include "OSCReceiver.h"

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

    auto callback = [](const osc::ReceivedMessage&)
    {
        std::cout << "Hello" << std::endl;
    };

    cillu::OSCReceiver receiver (15243, callback);

    receiver.start();

    while(running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
    }

    std::cout << "Stopping" << std::endl;
    receiver.stop();

    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
