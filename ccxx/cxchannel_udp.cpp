#include "cxchannel_udp.h"

#include "cxapplication.h"

#ifndef GM_OS_WIN
#include <net/if.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#endif


//CxChannelUdpFactory* CxChannelUdpFactory::s_instance =
//        CxChannelFactoryManager::createAndRegister<CxChannelUdpFactory>();


CxChannelUdp::CxChannelUdp()
{
    _socketBind = INVALID_SOCKET;
    _socketSend = INVALID_SOCKET;
    _receiver = NULL;
    _statusReceiver = ThreadStatus_Creating;
    _isServer = false;
}

CxChannelUdp::~CxChannelUdp()
{
    close();
}


void CxChannelUdp::toContext(CxISetSkv& context) const
{
    CxChannelBase::toContext(context);
    context.setValue(CS_EntryLocalIpAddress, _localIpAddress.ip());
    context.setValue(CS_EntryLocalPort, _localIpAddress.port());
    context.setValue(CS_EntryRemoteIpAddress, _remoteIpAddress.ip());
    context.setValue(CS_EntryRemotePort, _remoteIpAddress.port());
}


void CxChannelUdp::fromContext(const CxIGetSkv& context)
{
    CxChannelBase::fromContext(context);
    setLocalIp(context.getValue(CS_EntryLocalIpAddress, std::string("")));
    setLocalPort(context.getValue(CS_EntryLocalPort, 0));
    setRemoteIp(context.getValue(CS_EntryRemoteIpAddress, std::string("")));
    setRemotePort(context.getValue(CS_EntryRemotePort, 0));
}

int CxChannelUdp::writeTo(const char *pData, int iLength, std::string szRemoteIp, int nRemotePort)
{
    cxPromptCheck(_socketSend != INVALID_SOCKET, return -1);

    CxIpAddress sockAddrSend(szRemoteIp, nRemotePort);

    return writeTo(pData, iLength, sockAddrSend);
}

int CxChannelUdp::writeTo(const char *pData, int iLength, const CxIpAddress & mSendIp)
{
    cxPromptCheck(_socketSend != INVALID_SOCKET, return -1);
    cxPromptCheck(mSendIp.isValid(), return -1);

    msepoch_t tmStart = CxTime::currentMsepoch();

    ssize_t r = ::sendto( _socketSend, pData, iLength, 0, mSendIp.getSockAddr(), mSendIp.getSockAddrSize());

    if (SOCKET_ERROR == r)
    {
       int iErrorCode = CxSocket::error();
       if (iErrorCode != EAGAIN )// &&  iErrorCode!= EINTR) //&& iErrorCode != EDEADLOCK
       {
           outChannelPrompt() << "writeTo fail : " << mSendIp.ip() << mSendIp.port() << cxEndLine;
       }
    }

    //判断发送超时
    if((CxTime::currentMsepoch()-tmStart)>100) r = -2;

    return r;
}

void CxChannelUdp::setRemoteIpAddress(const std::string &sIp, ushort iPort)
{
    if (! isSendQueue() && CxApplication::isApplicationThread())
    {
        _remoteIpAddress.setIp(sIp);
        _remoteIpAddress.setPort(iPort);
    }
}


void CxChannelUdp::toReport(std::vector<std::string> &sReports) const
{
    sReports.push_back(CS_EntryLocalIpAddress + "=" + _localIpAddress.ip());
    sReports.push_back(CS_EntryLocalPort + "=" + CxString::toString(_localIpAddress.port()));
    sReports.push_back(CS_EntryRemoteIpAddress + "=" + _remoteIpAddress.ip());
    sReports.push_back(CS_EntryRemotePort + "=" + CxString::toString(_remoteIpAddress.port()));
}

void CxChannelUdp::toDescribe(CxDescribeUnit & describeUnit) const
{
    CxDescribeString* oLocalIpAddressDescribe = describeUnit.createAndRegister<CxDescribeString>(CS_EntryLocalIpAddress); oLocalIpAddressDescribe->initValue(_localIpAddress.ip());
    CxDescribeInteger* oLocalPortDescribe      = describeUnit.createAndRegister<CxDescribeInteger>(CS_EntryLocalPort);      oLocalPortDescribe->initValue(_localIpAddress.port());
    CxDescribeString* oRemoteIpAddressDescribe = describeUnit.createAndRegister<CxDescribeString>(CS_EntryRemoteIpAddress); oRemoteIpAddressDescribe->initValue(_remoteIpAddress.ip());
    CxDescribeInteger* oRemotePortDescribe      = describeUnit.createAndRegister<CxDescribeInteger>(CS_EntryRemotePort);      oRemotePortDescribe->initValue(_remoteIpAddress.port());
}

void CxChannelUdp::doSetLocalName()
{
    _localName = CS_TypeSimpleName_Udp + "@" + _localIpAddress.ip() + ":" + CxString::toString(_localIpAddress.port());
}

bool CxChannelUdp::hasRemote() const
{
    return _remoteIpAddress.isValid();
}

bool CxChannelUdp::hasLocal() const
{
    return _localIpAddress.isValid();
}

bool CxChannelUdp::isSameChannelImpl(const std::map<std::string, std::string> &params) const
{
    std::string sLocalIpAddress = CxContainer::value( params, CS_EntryLocalIpAddress );
    std::string sLocalPort = CxContainer::value( params, CS_EntryLocalPort );
    std::string sRemoteIpAddress = CxContainer::value( params, CS_EntryRemoteIpAddress );
    std::string sRemotePort = CxContainer::value( params, CS_EntryRemotePort );
    int iRemotePort = CxString::fromString(sRemotePort, ci_int_zero);
    int iLocalPort = CxString::fromString(sLocalPort, ci_int_zero);
    if (sRemoteIpAddress.size()>0 && _remoteIpAddress.ip() == sRemoteIpAddress && _remoteIpAddress.port() == iRemotePort )
    {
        if (sLocalIpAddress.size()>0 && _localIpAddress.ip() == sLocalIpAddress && _localIpAddress.port() == iLocalPort )
        {
            return true;
        }
    }
    return false;
}

int CxChannelUdp::writeDataImpl(const char *pData, int iLength, void * oTarget)
{
    cxPromptCheck(_socketSend != INVALID_SOCKET, return -1);
    if (oTarget)
    {
        CxChannelRoad * oRoad = (CxChannelRoad *)oTarget;
        const CxIpAddress & ipAddress = oRoad->sourceIpAddress();

        cxPromptCheck(ipAddress.isValid(), return -1);

        ssize_t r = ::sendto(_socketSend, pData, iLength, 0, ipAddress.getSockAddr(), ipAddress.getSockAddrSize());

        if (SOCKET_ERROR == r)
        {
            int iErrorCode = CxSocket::error();
            if (iErrorCode != EAGAIN )// &&  iErrorCode!= EINTR) //&& iErrorCode != EDEADLOCK
            {
                threadEventNotify(this, ChannelEvent_Send_Error);
            }
        }

        return r;
    }
    else
    {
        cxPromptCheck(_remoteIpAddress.isValid(), return -1);

        ssize_t r = ::sendto(_socketSend, pData, iLength, 0, _remoteIpAddress.getSockAddr(), _remoteIpAddress.getSockAddrSize());

        if (SOCKET_ERROR == r)
        {
            int iErrorCode = CxSocket::error();
            if (iErrorCode != EAGAIN )// &&  iErrorCode!= EINTR) //&& iErrorCode != EDEADLOCK
            {
                threadEventNotify(this, ChannelEvent_Send_Error);
            }
        }

        return r;
    }
}

void CxChannelUdp::processError(int iError, const std::string &sError, int iTag)
{
    if (_isServer && iError == ChannelEvent_Receive_ShutdownR)
    {
        outChannelPrompt() << " isServer=" << _isServer << " return";
        return;
    }
    else
    {
        CxChannelBase::processError(iError, sError, iTag);
    }
}

void CxChannelUdp::createAndStartThread()
{
    stopAndDeleteThread();
    _receiver = new ReceiverThread();
    _receiver->setStatus(&_statusReceiver);
    _receiver->setChannel(this);
    _statusReceiver = ThreadStatus_Start;
    _receiver->start();
}

void CxChannelUdp::stopAndDeleteThread()
{
    if (! _receiver) return;
    _statusReceiver = ThreadStatus_Stop;
    _receiver->waitExit();
    delete _receiver;
    _receiver = NULL;
}

void CxChannelUdp::shutdownAndClose()
{
    if (_socketSend != _socketBind && _socketSend != INVALID_SOCKET)
    {
        CxSocket::cancel(_socketSend);
        CxSocket::release(_socketSend);
        _socketSend = INVALID_SOCKET;
    }
    if (_socketBind != INVALID_SOCKET)
    {
        CxSocket::cancel(_socketBind);
        CxSocket::release(_socketBind);
        _socketBind = INVALID_SOCKET;
    }
}

void CxChannelUdp::openChannelImpl()
{
    cxPromptCheck(_socketBind == INVALID_SOCKET && _receiver == NULL, return);

    _socketSend = INVALID_SOCKET;
    if (_localIpAddress.isValid())
    {
        _socketBind = CxSocket::create(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
        cxPromptCheck(_socketBind != INVALID_SOCKET, return);

        //CxSocket return 0 : success
        int iError = CxSocket::bindto(_socketBind, _localIpAddress.getSockAddr());
        if (iError != 0)
        {
            std::string sError = CxString::format("[ip]=%s [port]=%d bind error : %s\n", _localIpAddress.ip().c_str(), _localIpAddress.port(), strerror(iError));
            outChannelPrompt() << sError;
            CxSocket::release(_socketBind);
            _socketBind = INVALID_SOCKET;
            return;
        }

        //关于UDP socket 的10054错误解决办法
        //http://support.microsoft.com/kb/263823/
#ifdef GM_OS_WIN
        DWORD dwBytesReturned = 0;
        BOOL bNewBehavior = FALSE;

        // disable  new behavior using
        // IOCTL: SIO_UDP_CONNRESET
#define SIO_UDP_CONNRESET _WSAIOW(IOC_VENDOR,12)
        iError = ::WSAIoctl(_socketBind, SIO_UDP_CONNRESET,
                            &bNewBehavior, sizeof(bNewBehavior),
                            NULL, 0, &dwBytesReturned,
                            NULL, NULL);

        if (SOCKET_ERROR == iError)
        {
            DWORD dwErr = WSAGetLastError();
            std::string sError = CxString::format("[ip]=%s [port]=%d WSAIoctl error : %d\n", _localIpAddress.ip().c_str(), _localIpAddress.port(), dwErr);
            outChannelPrompt() << sError;
            CxSocket::release(_socketBind);
            _socketBind = INVALID_SOCKET;
            return;
        }
#endif

        _socketSend = _socketBind;

        outChannelDebug() << "openChannelImpl -> createAndStartThread - bind success !!!";
    }
    if (_socketSend == INVALID_SOCKET)
    {
        _socketSend = CxSocket::create(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    }

    createAndStartThread();
}


void CxChannelUdp::closeChannelImpl()
{
    shutdownAndClose();
    stopAndDeleteThread();
}

bool CxChannelUdp::getConnectedImpl() const
{
    return (_socketBind != INVALID_SOCKET && _statusReceiver == ThreadStatus_Running) || (_socketSend != INVALID_SOCKET);
}

void CxChannelUdp::ReceiverThread::run()
{
    socket_t iRecvSocket = _channel->_socketBind;
    cxPromptCheck(iRecvSocket != INVALID_SOCKET, return);

    char mBuffer[4096];
    sockaddr * pSenderAddr = (sockaddr *)mBuffer;
    char * pRecvBuf = mBuffer + CxSockAddrMaxSize;
    size_t iBufLen = sizeof(mBuffer) - CxSockAddrMaxSize;

    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(iRecvSocket, &fds);

    fd_set inRecvs = fds;
    fd_set inSends = inRecvs;
    fd_set inErrors = inRecvs;

    int rSelect;
    struct timeval tv;
    struct timeval *tvp = &tv;
    tv.tv_usec = 100;
    tv.tv_sec = 0;

    fd_set outRecvs = inRecvs;
    fd_set outSends = inSends;
    fd_set outErrors = inErrors;

    * _status = ThreadStatus_Running;
    while (* _status == ThreadStatus_Running)
    {
        outRecvs = inRecvs;
//        outSends = inSends;
        outErrors = inErrors;

        rSelect = ::select((int)(iRecvSocket + 1), &outRecvs, NULL, &outErrors, tvp);
        if (rSelect == 0)
        {
            continue;
        }
        else if (rSelect > 0)
        {
            if (FD_ISSET(iRecvSocket, &outRecvs))
            {
                socklen_t iSenderAddrSize = CxSockAddrMaxSize;

                size_t iSize = ::recvfrom(iRecvSocket,
                         pRecvBuf,
                         iBufLen,
                         0,
                         pSenderAddr,
                         &iSenderAddrSize);

                if (iSize > 0 && iSize <= iBufLen)
                {
                    threadEventNotify(_channel, ChannelEvent_Received_Data, 0, mBuffer, iSize+CxSockAddrMaxSize, this);
                }
                else if (iSize == 0)
                {
                    threadEventNotify(_channel, ChannelEvent_Receive_ShutdownR);
                    break;
                }
                else
                {
                    if (* _status != ThreadStatus_Stop)
                    {
                        int iErrorCode = CxSocket::error(iRecvSocket);
                        if (iErrorCode != EAGAIN)
                        {
                            threadEventNotify(_channel, ChannelEvent_Receive_Error);
                            break;
                        }
                    }
                }
            }

            if (FD_ISSET(iRecvSocket, &outErrors))
            {
                if (* _status != ThreadStatus_Stop)
                {
                    int iErrorCode = CxSocket::error(iRecvSocket);
                    if (iErrorCode != EAGAIN)
                    {
                        threadEventNotify(_channel, ChannelEvent_Receive_Error);
                        break;
                    }
                }
            }
        }
        else
        {
            threadEventNotify(_channel, ChannelEvent_Platform_Error, -1);
            break;
        }
    }
    * _status = ThreadStatus_Stop;


// don't delete : use sync
//
//    * _status = ThreadStatus_Running;
//    while (* _status == ThreadStatus_Running)
//    {
//        size_t iSize = ::recvfrom(iRecvSocket,
//                 pRecvBuf,
//                 iBufLen,
//                 0,
//                 (sockaddr *)&senderAddr,
//                 &iSenderAddrSize);

//        if (iSize > 0 && iSize <= iBufLen)
//        {
//            threadEventNotify(_channel, ChannelEvent_Received_Data, 0, pRecvBuf, iSize);
//        }
//        else if (iSize == 0)
//        {
//            threadEventNotify(_channel, ChannelEvent_Receive_ShutdownR);
//            break;
//        }
//        else
//        {
//            if (* _status != ThreadStatus_Stop)
//            {
//                int iErrorCode = CxSocket::error();
//                if (iErrorCode != EAGAIN)
//                {
//                    threadEventNotify(_channel, ChannelEvent_Receive_Error);
//                    break;
//                }
//            }
//        }
//    }
//    * _status = ThreadStatus_Stop;
}

void CxChannelUdp::ReceiverThread::exit()
{
    * _status = ThreadStatus_Exit;
    CxJoinableThread::exit();
}
