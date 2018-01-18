#include "OSCReceiver.h"

namespace cillu
{

//----------------------------------------------------------------------------------------------------------------------

OSCReceiver::OSCReceiver(int port, std::function<void(const osc::ReceivedMessage&)> callback)
: m_socket(std::make_unique<UdpListeningReceiveSocket>(IpEndpointName(IpEndpointName::ANY_ADDRESS, port), this))
, m_callback(callback)
{
}

//----------------------------------------------------------------------------------------------------------------------

OSCReceiver::~OSCReceiver()
{
    stop();
}

//----------------------------------------------------------------------------------------------------------------------

void OSCReceiver::start()
{
    m_thread = std::thread([&]()
    {
        m_socket->Run();
    });
}

//----------------------------------------------------------------------------------------------------------------------

void OSCReceiver::stop()
{
    m_socket->AsynchronousBreak();
    m_thread.join();
}

//----------------------------------------------------------------------------------------------------------------------

void OSCReceiver::ProcessMessage(const osc::ReceivedMessage& m, const IpEndpointName&)
{
    m_callback(m);
}

//----------------------------------------------------------------------------------------------------------------------

}
