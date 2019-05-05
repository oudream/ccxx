#include "cxchannel_tcpserver.h"


#ifndef GM_OS_WIN
#include <net/if.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#endif


using namespace std;


//CxChannelTcpserverFactory* CxChannelTcpserverFactory::s_instance =
//        CxChannelFactoryManager::createAndRegister<CxChannelTcpserverFactory>();


int CxChannelTcpserver::sendDataBySockets(CxChannelBase * oChannel, const char *pData, int iLength, void *oTarget, const std::vector<socket_t> &mSockets)
{
    if (oTarget)
    {
        CxChannelRoad * oRoad = (CxChannelRoad *)oTarget;
        socket_t soTarget = oRoad->localSocket();
        if (soTarget > 0)
        {
            if (CxContainer::contain(mSockets, soTarget))
            {
                int iError = ::send( soTarget, pData, iLength, 0 );
                if (SOCKET_ERROR == iError)
                {
                    int iErrorCode = CxSocket::error();
                    if (iErrorCode != 0 && iErrorCode != EAGAIN)
                    {
                        std::string sError = CxString::format("tcpserver error by [send] remoteIpAddress[%s:%d] sock[%lld] errorCode[%d]",
                                                              oRoad->sourceIpAddress().ip().c_str(), oRoad->sourceIpAddress().port(),
                                                              int64(soTarget), iErrorCode) ;
                        threadEventNotify(oChannel, ChannelEvent_Send_Error, soTarget, sError.c_str(), sError.size());
                    }
                }
                return iError;
            }
        }
        return 0;
    }
    else
    {
        int r = 0;
        for (size_t i = 0; i < mSockets.size(); ++i)
        {
            socket_t so = mSockets.at(i);
            int iSend = ::send( so, pData, iLength, 0 );
            if (SOCKET_ERROR == iSend)
            {
                int iErrorCode = CxSocket::error();
                if (iErrorCode != 0 && iErrorCode != EAGAIN)
                {
                    std::string sError = CxString::format("tcpserver error by [send] sock[%lld] errorCode[%d]",
                                                          int64(so), iErrorCode) ;
                    threadEventNotify(oChannel, ChannelEvent_Send_Error, so, sError.c_str(), sError.size());
                    continue;
                }
            }
            else
            {
                r = iSend;
            }
        }
        return r;
    }
}

CxChannelTcpserver::CxChannelTcpserver()
{
    _localIp = "127.0.0.1";
    _localPort = 5555;
    _selecter = NULL;
    _selecterStatus = ThreadStatus_Creating;
    _accepter = NULL;
    _accepterStatus = ServerAcceptStatus_None;
}

CxChannelTcpserver::~CxChannelTcpserver()
{
    close();
}

void CxChannelTcpserver::toContext(CxISetSkv& context) const
{
    CxChannelBase::toContext(context);
    context.setValue(CS_EntryLocalIpAddress, _localIp);
    context.setValue(CS_EntryLocalPort, _localPort);
}

void CxChannelTcpserver::fromContext(const CxIGetSkv& context)
{
    CxChannelBase::fromContext(context);

    setLocalIp(context.getValue(CS_EntryLocalIpAddress, _localIp));
    setLocalPort(context.getValue(CS_EntryLocalPort, _localPort));
}

void CxChannelTcpserver::toReport(std::vector<std::string> &sReports) const
{
    sReports.push_back(CS_EntryLocalIpAddress + "=" + _localIp);
    sReports.push_back(CS_EntryLocalPort + "=" + CxString::toString(_localPort));
}

void CxChannelTcpserver::toDescribe(CxDescribeUnit & describeUnit) const
{
    CxDescribeString* oLocalIpAddressDescribe = describeUnit.createAndRegister<CxDescribeString>(CS_EntryLocalIpAddress); oLocalIpAddressDescribe->initValue(_localIp);
    CxDescribeInteger* oLocalPortDescribe      = describeUnit.createAndRegister<CxDescribeInteger>(CS_EntryLocalPort);      oLocalPortDescribe->initValue(_localPort);
}

void CxChannelTcpserver::doSetLocalName()
{
    _localName = CS_TypeSimpleName_Tcpserver + "@" + _localIp + ":" + CxString::toString(_localPort);
}

bool CxChannelTcpserver::isSameChannelImpl(const std::map<std::string, std::string> &params) const
{
    std::string sLocalIpAddress = CxContainer::value( params, CS_EntryLocalIpAddress );
    std::string sLocalPort = CxContainer::value( params, CS_EntryLocalPort );
    int iPort = CxString::fromString(sLocalPort, ci_int_zero);
    if (sLocalIpAddress.size()>0 && _localIp == sLocalIpAddress && _localPort == iPort )
    {
        return true;
    }
    return false;
}

int CxChannelTcpserver::writeDataImpl(const char *pData, int iLength, void * oTarget)
{
    return sendDataBySockets(this, pData, iLength, oTarget, _sockets);
}

void CxChannelTcpserver::processConnect(int iEvent , int iTag , const std::string &sInfo)
{
    if (iEvent == ChannelEvent_Connect_Success)
    {
        if (iTag > 0)
        {
            appendSocket(iTag);
            outChannelDebug() << "processConnect -> appendSocket(" << iTag << ")!!!" << cxEndLine;
        }
        else
        {
            createAndStartSelecterThread();
            outChannelDebug() << "processConnect -> createAndStartSelecterThread - listen success !!!" << cxEndLine;
        }
    }
    else
    {
        _accepterStatus = ServerAcceptStatus_Error;
        close();
        outChannelDebug() << "processConnect -> ChannelEvent_Connect_Fail : closeChannelImpl" << cxEndLine;
    }
}

void CxChannelTcpserver::processError(int iError, const std::string &sError , int iTag)
{
    if (iError == ChannelEvent_Platform_Error && iTag == -1)
    {
        close();
        outChannelDebug() << "processError -> ChannelEvent_Platform_Error : closeChannelImpl" << cxEndLine;
    }

    switch (iError)
    {
    case ChannelEvent_Platform_Error:
    case ChannelEvent_Receive_Error:
    case ChannelEvent_Receive_ShutdownR:
    case ChannelEvent_Send_Error:
    {
        cxPrompt() << "TCP Server : remote client stop 002";
        socket_t so = iTag;
        if (so > 0)
        {
            stopAndDeleteSocket(so);
            outChannelDebug() << "processError -> [ErrorId]=" << iError << " : stopAndDeleteSocket [socket]=" << so << cxEndLine;
        }
    }
        break;
    default:
        break;
    }
}

void CxChannelTcpserver::createAndStartSelecterThread()
{
    stopAndDeleteSelecterThread();
    _selecter = new SelecterThread();
    _selecter->setStatus(&_selecterStatus);
    _selecter->setChannel(this);
    _selecterStatus = ThreadStatus_Start;
    _selecter->start();
}

void CxChannelTcpserver::stopAndDeleteSelecterThread()
{
    if (! _selecter) return;
    _selecterStatus = ThreadStatus_Stop;
    _selecter->waitExit();
    delete _selecter;
    _selecter = NULL;
}

void CxChannelTcpserver::createAndStartAccepterThread()
{
    stopAndDeleteAccepterThread();
    _accepter = new AccepterThread();
    _accepter->setChannel(this);
    _accepter->start();
}

void CxChannelTcpserver::stopAndDeleteAccepterThread()
{
    if (! _accepter) return;
    _accepter->close();
    _accepter->waitExit();
    delete _accepter;
    _accepter = NULL;
}

void CxChannelTcpserver::appendSocket(socket_t so)
{
    vector<socket_t>::iterator it = std::find(_sockets.begin(), _sockets.end(), so);
    if (it == _sockets.end())
    {
        _sockets.push_back(so);
    }

    //sync to thread select(reader) and sender
    if (_selecter) _selecter->pushAddSock(so);
    if (_sender) ((SenderThread2*)_sender)->pushAddSock(so);
}

void CxChannelTcpserver::stopAndDeleteSocket(socket_t so)
{
    cxPrompt() << "TCP Server : remote client stop 003";
    //sync to thread select(reader) and sender
    if (_selecter) _selecter->pushRemoveSock(so);
    if (_sender) ((SenderThread2*)_sender)->pushRemove(so);

    bool bIsExist = false;
    vector<socket_t>::iterator it = std::find(_sockets.begin(), _sockets.end(), so);
    if (it != _sockets.end())
    {
        _sockets.erase(it);
        bIsExist = true;
    }
    if (bIsExist)
    {
        CxSocket::cancel(so);
        CxSocket::release(so);
    }
    cxPrompt() << "TCP Server : remote client stop 004";
    CxChannelRoadManager::nullLocalSocket(so);
}

void CxChannelTcpserver::stopAndClearSockets()
{
    vector<socket_t> sockets = _sockets;

    //sync to thread select(reader) and sender
    if (_selecter) _selecter->pushClearSocks(sockets);
    if (_sender) ((SenderThread2*)_sender)->pushClear(sockets);

    _sockets.clear();
    for (size_t i = 0; i < sockets.size(); ++i)
    {
        socket_t so = sockets.at(i);
        CxSocket::cancel(so);
        CxSocket::release(so);
    }

    CxChannelRoadManager::nullLocalSocket(sockets);
}

void CxChannelTcpserver::openChannelImpl()
{
    if (_accepterStatus == ServerAcceptStatus_None)
    {
        _accepterStatus = ServerAcceptStatus_Preparing;
        createAndStartAccepterThread();
    }
}

void CxChannelTcpserver::closeChannelImpl()
{
    _selecterStatus = ThreadStatus_Stop;
    stopAndClearSockets();
    stopAndDeleteAccepterThread();
    stopAndDeleteSelecterThread();
    _accepterStatus = ServerAcceptStatus_None;
}

bool CxChannelTcpserver::getConnectedImpl() const
{
    return _accepterStatus == ServerAcceptStatus_Accepting/* && _receiver != NULL*/;
}
