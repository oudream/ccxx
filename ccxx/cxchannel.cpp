#include "cxchannel.h"

#include "cxapplication.h"


using namespace std;


static volatile bool f_bPacketInterinfoOut = true;

static std::vector<CxChannelRoad*> f_oChannelRoads;
static CxMutex * f_oChannelRoadsLock = new CxMutex();

std::vector<CxChannelBase*> f_oChannels;


CxChannelRoad::CxChannelRoad(const sockaddr *oRemoteSockAddr, CxChannelBase *oChannel):
    _localChannel(oChannel),_remoteIpAddress(oRemoteSockAddr),
    _localSocket(INVALID_SOCKET), _sourceId(0), _sourceIpAddress()
{
    if (_remoteIpAddress.port() > 1024 && _remoteIpAddress.port() < GM_SHORT_MAX)
    {
        _sourceIpAddress = _remoteIpAddress;
    }
    _createTime = CxTime::currentSystemTime();
    _lastTime = _createTime;
}

CxChannelRoad::CxChannelRoad(const string &sIp, ushort iPort, CxChannelBase *oChannel):
    _localChannel(oChannel),_remoteIpAddress(sIp, iPort),
    _localSocket(INVALID_SOCKET), _sourceId(0), _sourceIpAddress()
{
    if (_remoteIpAddress.port() > 1024 && _remoteIpAddress.port() < GM_SHORT_MAX)
    {
        _sourceIpAddress = _remoteIpAddress;
    }
    _createTime = CxTime::currentSystemTime();
    _lastTime = _createTime;
}

CxChannelRoad::CxChannelRoad(const CxChannelRoad &other) :
    _localChannel(other._localChannel),_remoteIpAddress(other._remoteIpAddress),
    _localSocket(other._localSocket), _sourceId(other._sourceId), _sourceIpAddress(other._sourceIpAddress),
    _createTime(other._createTime), _lastTime(other._lastTime)
{
}

string CxChannelRoad::reportSelf()
{
    const std::string & sLocalChannel = _localChannel->localName();
    string sCreateTime = CxTime::toString(_createTime);
    string sLastTime = CxTime::toString(_lastTime);
    std::string sResult = CxString::format(" remote_address: %s, remote_port: %d | %d, local_channel: %s, local_socket: %d, sourceIp: %s, sourcePort=%d "\
                                           "sourceid: 0x%02x, createTime: %s, lastTime: %s ",
                                           _remoteIpAddress.ip().c_str(), _remoteIpAddress.port(), _sourceIpAddress.port(), sLocalChannel.c_str(), _localSocket, _sourceIpAddress.ip().c_str(), _sourceIpAddress.port(),
                                           _sourceId, sCreateTime.c_str(), sLastTime.c_str());
    return sResult;
}

int CxChannelRoad::sendData(const char *pData, int iLength)
{
    if (_localChannel)
    {
        return _localChannel->sendData(pData, iLength, this);
    }
    else
    {
        return -1;
    }
}


CxChannelRoad * CxChannelRoadManager::allocate(const struct sockaddr * oRecSockAddr, CxChannelBase *oLocalChannel, socket_t iLocalSocket)
{
    CxChannelRoad * oChannelRoad = find(oRecSockAddr, oLocalChannel);
    if (!oChannelRoad)
    {
        CxChannelRoadManager::cleanUp();
        oChannelRoad = new CxChannelRoad(oRecSockAddr, oLocalChannel);
        oChannelRoad->setLocalSocket(iLocalSocket);
        f_oChannelRoads.push_back(oChannelRoad);
    }
    oChannelRoad->setLastTime(CxTime::currentSystemTime());
    return oChannelRoad;
}

CxChannelRoad *CxChannelRoadManager::allocate(const string &sRemoteIp, ushort iRemotePort, CxChannelBase *oLocalChannel, int iSourceId)
{
    CxChannelRoad * oChannelRoad = find(sRemoteIp, iRemotePort);
    if (!oChannelRoad)
    {
        CxChannelRoadManager::cleanUp();
        oChannelRoad = new CxChannelRoad(sRemoteIp, iRemotePort, oLocalChannel);
        oChannelRoad->setSourceId(iSourceId);
        f_oChannelRoads.push_back(oChannelRoad);
    }
    oChannelRoad->setLastTime(CxTime::currentSystemTime());
    return oChannelRoad;
}

CxChannelRoad *CxChannelRoadManager::find(const sockaddr *oRemoteSockAddr, CxChannelBase *oLocalChannel)
{
    for (size_t i = 0; i < f_oChannelRoads.size(); ++i)
    {
        CxChannelRoad * oChannelRoad = f_oChannelRoads.at(i);
        if (oChannelRoad->_localChannel == oLocalChannel && oChannelRoad->_remoteIpAddress.isSameIpAddress(oRemoteSockAddr))
        {
            return oChannelRoad;
        }
    }
    return NULL;
}

CxChannelRoad *CxChannelRoadManager::find(const string &sRemoteIp, ushort iRemotePort)
{
    for (size_t i = 0; i < f_oChannelRoads.size(); ++i)
    {
        CxChannelRoad * oChannelRoad = f_oChannelRoads.at(i);
        if (oChannelRoad->_remoteIpAddress.isSameIpAddress(sRemoteIp, iRemotePort))
        {
            return oChannelRoad;
        }
    }
    return NULL;
}

CxChannelRoad *CxChannelRoadManager::find(int iSourceId)
{
    for (size_t i = 0; i < f_oChannelRoads.size(); ++i)
    {
        CxChannelRoad * oChannelRoad = f_oChannelRoads.at(i);
        if (oChannelRoad->_sourceId == iSourceId)
        {
            return oChannelRoad;
        }
    }
    return NULL;
}

CxChannelRoad *CxChannelRoadManager::find(void *oSource)
{
    for (size_t i = 0; i < f_oChannelRoads.size(); ++i)
    {
        CxChannelRoad * oChannelRoad = f_oChannelRoads.at(i);
        if (oChannelRoad == oSource)
        {
            return oChannelRoad;
        }
    }
    return NULL;
}

void CxChannelRoadManager::erase(CxChannelRoad *oChannelRoad)
{
    CxContainer::remove(f_oChannelRoads, oChannelRoad);
    delete oChannelRoad;
}

std::vector<string> CxChannelRoadManager::reportSelf()
{
    std::vector<string> sReports;
    sReports.push_back("*** ChannelRoad Manager Report ***");
    sReports.push_back(CxString::format("ChannelRoad Count: %d", f_oChannelRoads.size()));
    for (size_t i = 0; i < f_oChannelRoads.size(); ++i)
    {
        CxChannelRoad * oChannelRoad = f_oChannelRoads.at(i);
        sReports.push_back(CxString::format("ChannelRoad %03d : { %s }", i, oChannelRoad->reportSelf().c_str()));
    }
    return sReports;
}

void CxChannelRoadManager::nullLocalSocket(socket_t iSocket)
{
    for (size_t i = 0; i < f_oChannelRoads.size(); ++i)
    {
        CxChannelRoad * oChannelRoad = f_oChannelRoads.at(i);
        if (oChannelRoad->_localSocket == iSocket)
        {
            oChannelRoad->setLocalSocket(INVALID_SOCKET);
        }
    }
}

void CxChannelRoadManager::nullLocalSocket(const std::vector<socket_t> &sockets)
{
    for (size_t i = 0; i < sockets.size(); ++i)
    {
        socket_t iSocket = sockets.at(i);
        nullLocalSocket(iSocket);
    }
}

int CxChannelRoadManager::sendData(int iSourceId, const char *pData, int iLength)
{
    CxChannelRoad * oChannelRoad = find(iSourceId);
    if (oChannelRoad)
    {
        return oChannelRoad->_localChannel->sendData(pData, iLength, oChannelRoad);
    }
    return 0;
}

int CxChannelRoadManager::updateRemote(int iSourceId, int iTargetPort)
{
    CxChannelRoad * oChannelRoad = find(iSourceId);
    if (oChannelRoad)
    {
        oChannelRoad->updateRemote(iTargetPort);
        return TRUE;
    }
    return FALSE;
}

void CxChannelRoadManager::cleanUp()
{
    msepoch_t dtDiff = f_oChannelRoads.size() > 255 ? (30 * 1000) : (30 * 60 * 1000);
    int iClean = 0;
    for(std::vector<CxChannelRoad*>::iterator it = f_oChannelRoads.begin(); it != f_oChannelRoads.end();)
    {
        CxChannelRoad * oChannelRoad = * it;
        msepoch_t dtNow = CxTime::currentSystemTime();
        if (dtNow - oChannelRoad->_lastTime > dtDiff)
        {
            it = f_oChannelRoads.erase(it);
            delete oChannelRoad;
            ++iClean;
        }
        else
        {
            ++it;
        }
    }
    if (iClean > 0)
    {
        cxPrompt() << "CxChannelRoadManager::cleanUp.Count=" << iClean << "; remain=" << f_oChannelRoads.size();
    }
}




//**************************************************************************
//--------------------------------------------------------------------------
//**************************************************************************

CxChannelPublisher::CxChannelPublisher()
{
    _subjects = new std::vector<CxIChannelSubject*>();
}

CxChannelPublisher::~CxChannelPublisher()
{
    delete _subjects;
}

bool CxChannelPublisher::canConnectChange(const CxChannelBase *oChannel)
{
    {
        bool bCan = true;
        for (size_t i = 0; i < _subjects->size(); ++i) {
            CxIChannelSubject* oSubject = _subjects->at(i);
            bCan = oSubject->channel_canChangeConnect(oChannel, oChannel->connected(), ! oChannel->connected());
            if ( ! bCan ){
                return false;
            }
        }
        return bCan;
    }
}

void CxChannelPublisher::connectChanged(const CxChannelBase *oChannel)
{
    for (size_t i = 0; i < _subjects->size(); ++i)
    {
        CxIChannelSubject * o = _subjects->at(i);
        o->channel_connectChanged(oChannel);
    }
}

void CxChannelPublisher::receivedData(const uchar *pData, int iLength, void *oSource)
{
    for (size_t i = 0; i < _subjects->size(); ++i)
    {
        CxIChannelSubject * o = _subjects->at(i);
        o->channel_receivedData(pData, iLength, oSource);
    }
}

void CxChannelPublisher::beforeDelete(const CxChannelBase *oChannel)
{
    for (size_t i = 0; i < _subjects->size(); ++i)
    {
        CxIChannelSubject * o = _subjects->at(i);
        o->channel_beforeDelete(oChannel);
    }
}



//**************************************************************************
//--------------------------------------------------------------------------
//**************************************************************************

CxChannelBase::CxChannelBase()
{
    _receivedByteCount = 0;
    _sentByteCount = 0;
    _lastReceivedTime = CxValueType::minValue<msepoch_t>();
    _lastSentTime = CxValueType::minValue<msepoch_t>();

    _publisher = new CxChannelPublisher();
    _checkChannelTm = new CxTimer();
    _checkChannelTm->init(this, 1000);
    _checkChannelTm->start();

    _timers = 0;

    _autoOpenInterval = 0;
    _channelId = 0;

    _isSendQueue = false;
    _sender = NULL;
    _senderStatus = ThreadStatus_Creating;

    _isProcessQueue = false;
    _processer = NULL;
    _processerStatus = ThreadStatus_Creating;

    _isMultiRoad = false;
}


CxChannelBase::~CxChannelBase()
{
    delete _checkChannelTm;
    delete _publisher;
    CxApplication::nullProcessCallBackTarget(this);
}


int CxChannelBase::sendData(const char* pData, int iLength, void *oTarget)
{
    if (! (connected() && iLength > 0 && pData) )
    {
        return -1;
    }

    int result = 0;
    if (_isSendQueue)
    {
        if (_sender)
        {
            result = _sender->push(pData, iLength, oTarget);
        }
        else
        {
            outChannelPrompt().reason(11) << " send fail ! _sender=NULL;";
        }
    }
    else
    {
        result = writeDataImpl(pData, iLength, oTarget);
    }
    if (result>0)
    {
        _sentByteCount += result;
        _lastSentTime = CxTime::currentSystemTime();
        if (f_bPacketInterinfoOut)
        {
            std::string s = CxString::toHexstring(pData, iLength);
            if (oTarget)
            {
                outChannelPrompt().reason(11) << " send to " << ((CxChannelRoad*)oTarget)->sourceIpAddress().getIpPort() << " : " << s;
            }
            else
            {
                outChannelPrompt().reason(11) << " send : " << s;
            }
        }
    }
    else
    {
        if (oTarget)
        {
            outChannelPrompt().reason(11) << " send fail ! IpAddress=" << iLength << ((CxChannelRoad*)oTarget)->sourceIpAddress().getIpPort() << "; DataLength=" << iLength;
        }
        else
        {
            outChannelPrompt().reason(11) << " send fail ! DataLength=" << iLength;
        }
    }
    return result;
}

int CxChannelBase::sendBytes(const std::vector<char>& bytes, void * oTarget)
{
    if ( bytes.size() > 0 )
        return sendData((char *)(& bytes.front()), bytes.size(), oTarget);
    else
        return 0;
}

int CxChannelBase::sendBytes(const std::vector<uchar>& bytes, void *oTarget)
{
    if ( bytes.size() > 0 )
        return sendData((char *)(& bytes.front()), bytes.size(), oTarget);
    else
        return 0;
}

int CxChannelBase::sendText(const std::string& sText, void *oTarget)
{
    return sendData(sText.data(), sText.size(), oTarget);;
}


int CxChannelBase::sendHexString(const std::string& sText, void *oTarget)
{
    return sendBytes(CxString::fromHexstring(sText), oTarget);;
}

void CxChannelBase::pushProcessData(const char *pData, int iLength, void *oSource, int iTag)
{
    if (_processer)
    {
        _processer->push(pData, iLength, oSource, iTag);
    }
    else
    {
        processReceivedData(pData, iLength, oSource, iTag);
    }
}


void CxChannelBase::open()
{
    if (! getConnectedImpl())
    {
        openChannelPrepare();
        openChannelImpl();

        stopAndDeleteSenderThread();
        if (_isSendQueue)
        {
            _sender = createSenderThread();
            _sender->init();
            _sender->setStatus(&_senderStatus);
            _sender->setChannel(this);
            _senderStatus = ThreadStatus_Start;
            _sender->start();
        }

        stopAndDeleteProcesserThread();
        if (_isProcessQueue)
        {
            _processer = createProcesserThread();
            _processer->init();
            _processer->setStatus(&_processerStatus);
            _processer->setChannel(this);
            _processerStatus = ThreadStatus_Start;
            _processer->start();
        }
    }
}

void CxChannelBase::close()
{
    closeChannelImpl();
    stopAndDeleteSenderThread();
    stopAndDeleteProcesserThread();
}

bool CxChannelBase::isOpen() const
{
    return getConnectedImpl();
}

bool CxChannelBase::connected() const
{
    return getConnectedImpl();
}

void CxChannelBase::setConnected(bool bConnected)
{
    if (bConnected)
    {
        open();
    }
    else
    {
        close();
    }
}

void CxChannelBase::stopAndDeleteSenderThread()
{
    if (! _sender) return;
    _senderStatus = ThreadStatus_Stop;
    _sender->stop();
    delete _sender;
    _sender = NULL;
}

void CxChannelBase::stopAndDeleteProcesserThread()
{
    if (! _processer) return;
    _processerStatus = ThreadStatus_Stop;
    _processer->stop();
    delete _processer;
    _processer = NULL;
}

void CxChannelBase::channelChanged()
{
    _publisher->connectChanged(this);
}

void CxChannelBase::threadEventNotify(CxChannelBase *oTarget, int iChannelEvent, int iTag, const void *pData, int iLength, void *oSource)
{
    GM_ASSER_OBJECT(oTarget);
    if (iChannelEvent == ChannelEvent_Received_Data && oTarget->isProcessQueue())
    {
        oTarget->pushProcessData((const char*)pData, iLength, oSource, iTag);
    }
    else
    {
        CxApplication::pushProcessCallBack(CxChannelBase::processThreadNofity, iChannelEvent, iTag, pData, iLength, oSource, oTarget);
        CxApplication::signalMainThread();
    }
}

void CxChannelBase::processReceivedData(const char *pData, int iLength, void *oSource, int iTag)
{
    if (oSource)
    {
        cxPromptCheck(iLength > CxSockAddrMaxSize, return);

        const char * pRecData = pData + CxSockAddrMaxSize;
        int iRecLength = iLength - CxSockAddrMaxSize;
        const sockaddr * oRecSockaddr = (const sockaddr*)pData;
        CxChannelRoad * oRoad = NULL;

        if (_isMultiRoad)
        {
            oRoad = CxChannelRoadManager::allocate(oRecSockaddr, this, iTag);
        }

        _receivedByteCount += iRecLength;
        _lastReceivedTime = CxTime::currentSystemTime();
        if (f_bPacketInterinfoOut)
        {
            std::string s = CxString::toHexstring(pRecData, iRecLength);
            if (oRoad)
            {
                outChannelPrompt().reason(12) << " from " << oRoad->remoteIpAddress().getIpPort() << " received : " << s;
            }
            else
            {
                outChannelPrompt().reason(12) << " received : " << s;
            }
        }
        else
        {
            _infoReceived += CxString::format(">%lld=%d", _lastReceivedTime, iRecLength);
            outReceiveInfo();
        }

        _publisher->receivedData((const uchar *)pRecData, iRecLength, oRoad);
    }
    else
    {
        _receivedByteCount += iLength;
        _lastReceivedTime = CxTime::currentSystemTime();
        if (f_bPacketInterinfoOut)
        {
            std::string s = CxString::toHexstring(pData, iLength);
            outChannelPrompt().reason(12) << " received : " << s << cxEndLine;
        }
        else
        {
            _infoReceived += CxString::format(">%lld=%d", _lastReceivedTime, iLength);
            outReceiveInfo();
        }

        _publisher->receivedData((const uchar *)pData, iLength, NULL);
    }
}

void CxChannelBase::processThreadNofity(int iChannelEvent, int iTag, const void * pData, int iLength, void * oSource, void * oTarget)
{
    GM_INVALID_RETURE(oTarget != 0);

    CxChannelBase * oChannel = (CxChannelBase *)oTarget;
    int iFirstEvent = iChannelEvent & 0xFF000000;
    switch (iFirstEvent)
    {
    case ChannelEvent_Platform:
    {
        if (iChannelEvent == ChannelEvent_Platform_Error)
        {
            string sError = (pData && iLength > 0) ? string((const char*)pData, iLength) : "";
            oChannel->outChannelPrompt() << "ChannelEvent_Platform_Error - processError : " << sError << cxEndLine;
            oChannel->processError(iChannelEvent, sError, iTag);
        }
    }
        break;
    case ChannelEvent_Connect:
    {
        oChannel->processConnect(iChannelEvent, iTag, string((const char*)pData, iLength));
    }
        break;
    case ChannelEvent_Receive:
    {
        if (iChannelEvent == ChannelEvent_Received_Data)
        {
            oChannel->processReceivedData((const char*)pData, iLength, oSource, iTag);
        }
        else if ((iChannelEvent == ChannelEvent_Receive_Error) || (iChannelEvent == ChannelEvent_Receive_ShutdownR))
        {
            string sError = (pData && iLength > 0) ? string((const char*)pData, iLength) : "";
            oChannel->outChannelPrompt() << "ChannelEvent_Receive_ShutdownR || ChannelEvent_Receive_Error - processError : " << sError << cxEndLine;
            oChannel->processError(iChannelEvent, sError, iTag);
        }
    }
        break;
    case ChannelEvent_Send:
    {
        if (iChannelEvent == ChannelEvent_Send_Error)
        {
            string sError = (pData && iLength > 0) ? string((const char*)pData, iLength) : "";
            oChannel->outChannelPrompt() << "ChannelEvent_Send_ShutdownR || ChannelEvent_Send_Error - processError : " << sError << cxEndLine;
            oChannel->processError(iChannelEvent, sError, iTag);
        }
        else
        {
            oChannel->processSentData(iChannelEvent, iTag, string((const char*)pData, iLength));
        }
    }
        break;
    }
}

std::vector<std::string> CxChannelBase::reportSelf() const
{
    std::vector<std::string> sReports;
    sReports.push_back("*** Channel Report ***");
    sReports.push_back(CxString::format("TypeSimpleName=%s", typeSimpleName().c_str()));
    sReports.push_back(CxString::format("LocalName=%s", _localName.c_str()));
    sReports.push_back(CxString::format("ChannelName=%s", _channelName.c_str()));
    sReports.push_back(CxString::format("ConnectState=%s", connected() ? "Connected" : "Disconnected"));
    sReports.push_back(CxString::format("AutoOpenInterval=%d", _autoOpenInterval));
    sReports.push_back(CxString::format("ObserverCount=%d", _publisher->Subjects()->size()));
    sReports.push_back(CxString::format("ReceivedByteCount=%d", _receivedByteCount));
    sReports.push_back(CxString::format("LastReceivedTime=%s", CxTime::toString(_lastReceivedTime).c_str()));
    sReports.push_back(CxString::format("SentByteCount=%d", _sentByteCount));
    sReports.push_back(CxString::format("LastSentTime=%s", CxTime::toString(_lastSentTime).c_str()));
    sReports.push_back(CxString::format("IsMultiRoad=%d", _isMultiRoad));
    sReports.push_back(CxString::format("IsSendQueue=%d",_isSendQueue));
    sReports.push_back(CxString::format("IsProcessQueue=%d", _isProcessQueue));
    toReport(sReports);
    return sReports;
}

bool CxChannelBase::isSameChannel(const std::map<std::string, std::string>& params) const
{
    return isSameChannelImpl(params);
}

void CxChannelBase::timer_timeOut(const CxTimer *oTimer)
{
    ++_timers;

    //3 second
    if (_timers % 3 == 0)
    {
        outReceiveInfo();
        checkChannel();
    }

    //_autoOpenInterval / 1000 second
    if (_autoOpenInterval > 1000 && (_timers % (_autoOpenInterval / 1000)) == 0)
    {
        open();
    }
}

void CxChannelBase::processError(int iError, const string &sError, int iTag)
{
    close();
}

void CxChannelBase::describeSelf(CxDescribeUnit & oDescribeUnit) const
{
    CxDescribeString* oChannelNameDescribe = oDescribeUnit.createAndRegister<CxDescribeString>(CS_EntryChannelName);
    oChannelNameDescribe->initValue(_channelName);
    oChannelNameDescribe->setIsReadOnly(true);
    toDescribe(oDescribeUnit);
}

void CxChannelBase::toDescribe(CxDescribeUnit & describeUnit) const
{
    CxDescribeBoolean* oSendQueueEnabledDescribe = describeUnit.createAndRegister<CxDescribeBoolean>(CS_EntryIsSendQueue);
    oSendQueueEnabledDescribe->initValue(_isSendQueue);
    CxDescribeBoolean* oProcessQueueEnabledDescribe = describeUnit.createAndRegister<CxDescribeBoolean>(CS_EntryIsProcessQueue);
    oProcessQueueEnabledDescribe->initValue(_isProcessQueue);
}

void CxChannelBase::toContext(CxISetSkv & context) const
{
    context.setValue(CS_EntryIsSendQueue , _isSendQueue);
    context.setValue(CS_EntryIsProcessQueue , _isProcessQueue);
}

void CxChannelBase::fromContext(const CxIGetSkv & context)
{
    setIsSendQueue(context.getValue(CS_EntryIsSendQueue, _isSendQueue));
    setIsProcessQueue(context.getValue(CS_EntryIsProcessQueue, _isProcessQueue));
}

void CxChannelBase::channelBeforeDelete()
{
    _publisher->beforeDelete(this);
}



//**************************************************************************
//--------------------------------------------------------------------------
//**************************************************************************

const std::vector<CxChannelBase *>* CxChannelManager::channels()
{
    return &f_oChannels;
}

void CxChannelManager::openAll()
{
    for (size_t i = 0; i < f_oChannels.size(); ++i)
    {
        CxChannelBase* oChannel = f_oChannels.at(i);
        oChannel->open();
    }
}

void CxChannelManager::closeAll()
{
    for (size_t i = 0; i < f_oChannels.size(); ++i)
    {
        CxChannelBase* oChannel = f_oChannels.at(i);
        oChannel->close();
    }
}

void CxChannelManager::deleteAll()
{
    for (size_t i = 0; i < f_oChannels.size(); ++i)
    {
        CxChannelBase* o = f_oChannels.at(i);
        recoveChanrnel(o);
    }
    f_oChannels.clear();
}

void CxChannelManager::setPacketInterinfoOut(bool value)
{
    f_bPacketInterinfoOut = value;
}

CxChannelBase * CxChannelManager::createChannel(const std::string &sTypeSimpleName)
{
    CxChannelBase* oChannel = CxChannelFactoryManager::createObject(sTypeSimpleName);
    if (oChannel)
    {
        f_oChannels.push_back(oChannel);
        oChannel->setChannelId(f_oChannels.size());
    }
    return oChannel;
}

void CxChannelManager::recoveChanrnel(CxChannelBase *oChannel)
{
    if (oChannel->publisher()->Subjects()->size()<=0)
    {
        if (oChannel->connected())
            oChannel->close();
        oChannel->channelBeforeDelete();
        CxContainer::remove(f_oChannels, oChannel);
        delete oChannel;
    }
}

CxChannelBase * CxChannelManager::channel(const CxChannelBase *oChanel)
{
    for (size_t i = 0; i < f_oChannels.size(); ++i)
    {
        CxChannelBase* oChannel1 = f_oChannels.at(i);
        if (oChannel1 == oChanel)
            return oChannel1;
    }
    return NULL;
}

CxChannelBase *CxChannelManager::channel(int iChannelId)
{
    for (size_t i = 0; i < f_oChannels.size(); ++i)
    {
        CxChannelBase* oChannel = f_oChannels.at(i);
        if (oChannel->channelId() == iChannelId)
            return oChannel;
    }
    return NULL;
}




int CxChannelBase::SenderThread::push(const char *pData, int iLength, void *oTarget)
{
    if (pData && iLength > 0 && iLength < 1024 * 32)
    {
        CxMutexScope lock(_lock);
        _pushBuffers->push_back(std::vector<char>(pData, pData + iLength));
        _pushTargets->push_back(oTarget);
    }
    else
    {
        return 0;
    }
    _singleWait.signal();
    return iLength;
}

void CxChannelBase::SenderThread::init()
{
    _pushBuffers = & _buffers1;
    _popBuffers = & _buffers2;
    _pushTargets = & _targets1;
    _popTargets = & _targets2;
}

void CxChannelBase::SenderThread::run()
{
    cxPromptCheck(_channel, return);
    * _status = ThreadStatus_Running;
    while (* _status == ThreadStatus_Running)
    {
        _singleWait.wait();
        {
            CxMutexScope lock(_lock);
            std::vector<std::vector<char> > * tmpBuffer = _pushBuffers;
            _pushBuffers = _popBuffers;
            _popBuffers = tmpBuffer;
            std::vector<void*> * tmpTargets = _pushTargets;
            _pushTargets = _popTargets;
            _popTargets = tmpTargets;
        }

        for (size_t i = 0; i < _popBuffers->size(); ++i)
        {
            std::vector<char> & cb = _popBuffers->at(i);
            void * oTarget = _popTargets->at(i);
            writeData(&cb.front(), cb.size(), oTarget);
        }
        _popBuffers->clear();
        _popTargets->clear();
    }
}










int CxChannelBase::ProcesserThread::push(const char *pData, int iLength, void *oSource, int iTag)
{
    if (pData && iLength > 0 && iLength < 1024 * 32)
    {
        CxMutexScope lock(_lock);
        _pushBuffers->push_back(std::vector<char>(pData, pData + iLength));
        _pushSources->push_back(oSource);
        _pushTags->push_back(iTag);
    }
    else
    {
        return 0;
    }
    _singleWait.signal();
    return iLength;
}

void CxChannelBase::ProcesserThread::init()
{
    _pushBuffers = & _buffers1;
    _popBuffers = & _buffers2;
    _pushSources = & _sources1;
    _popSources = & _sources2;
    _pushTags = & _tags1;
    _popTags = & _tags2;
}

void CxChannelBase::ProcesserThread::run()
{
    cxPromptCheck(_channel, return);
    * _status = ThreadStatus_Running;
    while (* _status == ThreadStatus_Running)
    {
        _singleWait.wait();
        {
            CxMutexScope lock(_lock);
            std::vector<std::vector<char> > * tmpBuffer = _pushBuffers;
            _pushBuffers = _popBuffers;
            _popBuffers = tmpBuffer;
            std::vector<void*> * tmpSources = _pushSources;
            _pushSources = _popSources;
            _popSources = tmpSources;
            std::vector<int> * tmpTags = _pushTags;
            _pushTags = _popTags;
            _popTags = tmpTags;
        }

        for (size_t i = 0; i < _popBuffers->size(); ++i)
        {
            std::vector<char> & cb = _popBuffers->at(i);
            void * oTarget = _popSources->at(i);
            int iTag = _popTags->at(i);
            processData(&cb.front(), cb.size(), oTarget, iTag);
        }
        _popBuffers->clear();
        _popSources->clear();
        _popTags->clear();
    }
}
