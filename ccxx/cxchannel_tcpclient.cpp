#include "cxchannel_tcpclient.h"

#ifndef GM_OS_WIN
#include <net/if.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#endif


//CxChannelTcpclientFactory* CxChannelTcpclientFactory::s_instance =
//        CxChannelFactoryManager::createAndRegister<CxChannelTcpclientFactory>();


#define ClientConnectStatus_None            (0)
#define ClientConnectStatus_Connecting      (1)
#define ClientConnectStatus_Connected       (2)
#define ClientConnectStatus_Disconnecting   (3)
#define ClientConnectStatus_Disconnected    (4)


CxChannelTcpclient::CxChannelTcpclient()
{
    _remoteIp = "127.0.0.1";
    _remotePort = 5555;
    _socket = INVALID_SOCKET;
    _receiver = NULL;
    _receiverStatus = ThreadStatus_Creating;
    _connecter = NULL;
    _connectStatus = ClientConnectStatus_None;
}

CxChannelTcpclient::~CxChannelTcpclient()
{
    close();
}


void CxChannelTcpclient::toContext(CxISetSkv& context) const
{
    CxChannelBase::toContext(context);
    context.setValue(CS_EntryRemoteIpAddress, _remoteIp);
    context.setValue(CS_EntryRemotePort, _remotePort);
}


void CxChannelTcpclient::fromContext(const CxIGetSkv& context)
{
    CxChannelBase::fromContext(context);
    setRemoteIp(context.getValue(CS_EntryRemoteIpAddress, std::string("")));
    setRemotePort(context.getValue(CS_EntryRemotePort, 0));
}


void CxChannelTcpclient::toReport(std::vector<std::string> &sReports) const
{
    sReports.push_back(CS_EntryRemoteIpAddress + "=" + _remoteIp);
    sReports.push_back(CS_EntryRemotePort + "=" + CxString::toString(_remotePort));
}

void CxChannelTcpclient::toDescribe(CxDescribeUnit & describeUnit) const
{
    CxDescribeString* oRemoteIpAddressDescribe = describeUnit.createAndRegister<CxDescribeString>(CS_EntryRemoteIpAddress); oRemoteIpAddressDescribe->initValue(_remoteIp);
    CxDescribeInteger* oRemotePortDescribe      = describeUnit.createAndRegister<CxDescribeInteger>(CS_EntryRemotePort);      oRemotePortDescribe->initValue(_remotePort);
}

void CxChannelTcpclient::doSetLocalName()
{
    _localName = CS_TypeSimpleName_Tcpclient + "@" + _remoteIp + ":" + CxString::toString(_remotePort);
}

bool CxChannelTcpclient::isSameChannelImpl(const std::map<std::string, std::string> &params) const
{
    std::string sRemoteIpAddress = CxContainer::value( params, CS_EntryRemoteIpAddress );
    std::string sRemotePort = CxContainer::value( params, CS_EntryRemotePort );
    int iPort = CxString::fromString(sRemotePort, ci_int_zero);
    if (sRemoteIpAddress.size()>0 && _remoteIp == sRemoteIpAddress && _remotePort == iPort )
    {
        return true;
    }
    return false;
}

int CxChannelTcpclient::writeDataImpl(const char *pData, int iLength, void * oTarget)
{
    if (_socket == INVALID_SOCKET)
        return 0;

    ssize_t r = ::send( _socket, pData, iLength, 0 );
    if (SOCKET_ERROR == r)
    {
        int iErrorCode = CxSocket::error();
        if (iErrorCode != EAGAIN)
        {
            threadEventNotify(this, ChannelEvent_Send_Error);
        }
    }
    return r;
}

void CxChannelTcpclient::processConnect(int iEvent, int iTag, const std::string &sInfo)
{
    if (_connecter)
    {
        delete _connecter;
        _connecter = NULL;
    }
    if (iEvent == ChannelEvent_Connect_Success)
    {
        _connectStatus = ClientConnectStatus_Connected;
        createAndStartReceiverThread();
        outChannelDebug() << "processConnect -> ChannelEvent_Connect_Success " << cxEndLine;
    }
    else
    {
        _socket = INVALID_SOCKET;
        _connectStatus = ClientConnectStatus_Disconnected;
        outChannelDebug() << "processConnect -> ChannelEvent_Connect_Fail " << cxEndLine;
    }
}

void CxChannelTcpclient::createAndStartReceiverThread()
{
    stopAndDeleteReceiverThread();
    _receiver = new ReceiverThread();
    _receiver->setSocket(_socket);
    _receiver->setStatus(&_receiverStatus);
    _receiver->setChannel(this);
    _receiverStatus = ThreadStatus_Start;
    _receiver->start();
}

void CxChannelTcpclient::stopAndDeleteReceiverThread()
{
    if (! _receiver) return;
    _receiverStatus = ThreadStatus_Stop;
    _receiver->waitExit();
    delete _receiver;
    _receiver = NULL;
}

void CxChannelTcpclient::createAndStartConnecterThread()
{
    stopAndDeleteConnecterThread();
    _connecter = new ConnecterThread();
    _connecter->setChannel(this);
    _connecter->start();
}

void CxChannelTcpclient::stopAndDeleteConnecterThread()
{
    if (! _connecter) return;
    _connecter->waitExit();
    delete _connecter;
    _connecter = NULL;
}

void CxChannelTcpclient::shutdownAndClose()
{
    if (_socket == INVALID_SOCKET)
        return;
    CxSocket::cancel(_socket);
    CxSocket::release(_socket);
    _socket = INVALID_SOCKET;
}

void CxChannelTcpclient::openChannelImpl()
{
    if (_socket == INVALID_SOCKET && _connectStatus != ClientConnectStatus_Connecting)
    {
        _connectStatus = ClientConnectStatus_Connecting;
        createAndStartConnecterThread();
    }
}

void CxChannelTcpclient::closeChannelImpl()
{
    shutdownAndClose();
    stopAndDeleteConnecterThread();
    stopAndDeleteReceiverThread();
}

bool CxChannelTcpclient::getConnectedImpl() const
{
    return _connectStatus == ClientConnectStatus_Connected && _receiver != NULL;
//    return _socket != INVALID_SOCKET && _reader != NULL;
}

void CxChannelTcpclient::ReceiverThread::run()
{
    while (* _status == ThreadStatus_Start)
    {
        ssize_t iSize = ::recv(_so, _buffer, sizeof(_buffer), 0);
        if (SOCKET_ERROR == iSize)
        {
            int iErrorCode = CxSocket::error();
            if (iErrorCode != EAGAIN)
            {
                threadEventNotify(_channel, ChannelEvent_Receive_Error);
                break;
            }
        }
        else if (0 == iSize)
        {
            threadEventNotify(_channel, ChannelEvent_Receive_ShutdownR);
            break;
        }
        else
        {
            threadEventNotify(_channel, ChannelEvent_Received_Data, 0, _buffer, iSize);
        }
    }
}

void CxChannelTcpclient::ReceiverThread::exit()
{
    * _status = ThreadStatus_Exit;
    CxJoinableThread::exit();
}



void CxChannelTcpclient::ConnecterThread::run()
{
    //create socket
    socket_t SocketFD = CxSocket::create(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == SocketFD)
    {
        return;
    }

    ushort iRemotePort = _channel->_remotePort;
    std::string sRemoteIp = _channel->_remoteIp;
    CxIpAddress stSockAddr(sRemoteIp, iRemotePort);

//    if (sRemoteIp.empty() || sRemoteIp == "0.0.0.0" || CxString::equalCase(sRemoteIp, "localhost"))
//        sRemoteIp = "127.0.0.1";
//    //connect socket
//    struct sockaddr_in stSockAddr;
//    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));
//    stSockAddr.sin_family = AF_INET;
//    stSockAddr.sin_port = ::htons(iRemotePort);
//    stSockAddr.sin_addr.s_addr = ::inet_addr(_channel->_remoteIp.c_str());

    if (SOCKET_ERROR == CxSocket::connectto(SocketFD, stSockAddr.getSockAddr()))
    {
        CxSocket::release(SocketFD);
        threadEventNotify(_channel, ChannelEvent_Connect_Fail);
    }
    else
    {
        _channel->_socket = SocketFD;
        threadEventNotify(_channel, ChannelEvent_Connect_Success);
    }
}
