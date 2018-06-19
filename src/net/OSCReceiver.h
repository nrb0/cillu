#pragma once

#include "ip/UdpSocket.h"

#include "osc/OscReceivedElements.h"
#include "osc/OscPacketListener.h"

#include <functional>
#include <thread>

namespace cillu
{

//----------------------------------------------------------------------------------------------------------------------

class OSCReceiver : public osc::OscPacketListener
{
public:
    OSCReceiver(int port, std::function<void(const osc::ReceivedMessage&)> callback);
    ~OSCReceiver();

    void start();
    void stop();

    void ProcessMessage(const osc::ReceivedMessage& m, const IpEndpointName& remoteEndpoint) override;

private:
    bool m_running = false;

    IpEndpointName m_endpoint;
    std::unique_ptr<UdpListeningReceiveSocket> m_socket;
    std::thread m_thread;
    std::function<void(const osc::ReceivedMessage&)> m_callback;
};

//----------------------------------------------------------------------------------------------------------------------


}
