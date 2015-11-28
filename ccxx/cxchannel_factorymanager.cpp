
#include "cxchannel_tcpclient.h"
#include "cxchannel_tcpserver.h"
#include "cxchannel_udp.h"
#include "cxchannel_serial.h"

std::vector<CxFactoryTemplate<CxChannelBase> *> *CxChannelBase::factoriesContainer()
{
    static std::vector<CxFactoryTemplate<CxChannelBase> *> m;
    return &m;
}

void CxChannelBase::factoriesCreateAndRegister()
{
    static bool bNotInit = true;

    if (bNotInit)
    {
        CxChannelFactoryManager::createAndRegister<CxChannelTcpclientFactory>();
        CxChannelFactoryManager::createAndRegister<CxChannelTcpserverFactory>();
        CxChannelFactoryManager::createAndRegister<CxChannelUdpFactory>();
        CxChannelFactoryManager::createAndRegister<CxChannelSerialFactory>();
        bNotInit = false;
    }
}

