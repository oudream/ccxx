#ifndef CXCHANNEL_H
#define CXCHANNEL_H


#include "cxchannel_global.h"
#include "cxcontext.h"
#include "cxfactory.h"
#include "cxtimer.h"
#include "cxcontainer.h"
#include "cxinterinfo.h"
#include "cxdescribe.h"
#include "cxsocket.h"

#define ChannelEvent_Platform   (0x01000000)
#define ChannelEvent_Connect    (0x05000000)
#define ChannelEvent_Receive    (0x06000000)
#define ChannelEvent_Send       (0x07000000)

#define ChannelEvent_Platform_Error     (ChannelEvent_Platform + 0x00050000)

#define ChannelEvent_Connect_Success    (ChannelEvent_Connect + 0x00050000)
#define ChannelEvent_Connect_Fail       (ChannelEvent_Connect + 0x00060000)

#define ChannelEvent_Received_Data      (ChannelEvent_Receive + 0x00050000)
#define ChannelEvent_Receive_Error      (ChannelEvent_Receive + 0x00060000)
#define ChannelEvent_Receive_ShutdownR  (ChannelEvent_Receive + 0x00070000)

#define ChannelEvent_Send_Success       (ChannelEvent_Send    + 0x00050000)
#define ChannelEvent_Send_Fail          (ChannelEvent_Send    + 0x00060000)
#define ChannelEvent_Send_Error         (ChannelEvent_Send    + 0x00070000)



class CxChannelBase;


class CxChannelRoad
{
public:
    CxChannelRoad(const struct sockaddr * oRemoteSockAddr, CxChannelBase * oChannel);

    CxChannelRoad(const std::string & sIp, ushort iPort, CxChannelBase * oChannel);

    CxChannelRoad(const CxChannelRoad &other);

    inline CxChannelRoad &operator=(const CxChannelRoad &other) {
        if (this != &other) {
            _remoteIpAddress = other._remoteIpAddress;
            _localChannel = other._localChannel;
            _localSocket = other._localSocket;
            _sourceId = other._sourceId;
            _sourceIpAddress = other._sourceIpAddress;
            _createTime = other._createTime;
            _lastTime = other._lastTime;
        }
        return *this;
    }

    inline const CxIpAddress & remoteIpAddress() const { return _remoteIpAddress; }

    inline CxChannelBase * localChannel() const { return _localChannel; }

    inline socket_t localSocket() const { return _localSocket; }
    inline void setLocalSocket(int value) {
        if (CxSocket::isValidSocket(value))
            _localSocket = value;
        else
            _localSocket = INVALID_SOCKET;
    }

    inline int sourceId() const { return _sourceId; }
    inline void setSourceId(int value) { _sourceId = value; }

    inline const CxIpAddress & sourceIpAddress() const { return _sourceIpAddress; }

    inline msepoch_t createTime() const { return _createTime; }

    inline msepoch_t lastTime() const { return _lastTime; }
    inline void setLastTime(msepoch_t value) { _lastTime = value; }

    inline void updateRemote(int iPort = 0) {
        if (iPort != _sourceIpAddress.port())
        {
            _sourceIpAddress.init(_remoteIpAddress.ip(), iPort);
        }
    }

    inline bool isInline() {
        return CxSocket::isValidSocket(_localSocket) || (CxTime::milliSecondDifferToNow(_lastTime) < 6000);
    }

    std::string reportSelf();

    int sendData(const char *pData, int iLength);

private:
    CxChannelBase * _localChannel;
    CxIpAddress _remoteIpAddress; // udp 用到的，从 recv 来的 socket 从获得
    socket_t _localSocket; //tcpserver 用到的
    int _sourceId; //来源 == remote
    CxIpAddress _sourceIpAddress; //来源 == remote
    msepoch_t _createTime;
    msepoch_t _lastTime;

    friend class CxChannelRoadManager;

};


class CxChannelRoadManager
{
public:
    static CxChannelRoad * find(const struct sockaddr * oRemoteSockAddr, CxChannelBase *oLocalChannel);

    static CxChannelRoad * find(const std::string & sRemoteIp, ushort iRemotePort);

    static CxChannelRoad * find(int iSourceId);

    static CxChannelRoad * allocate(const struct sockaddr * oRecSockAddr, CxChannelBase * oLocalChannel, socket_t iLocalSocket);

    static CxChannelRoad * allocate(const std::string & sRemoteIp, ushort iRemotePort, CxChannelBase * oLocalChannel, int iSourceId);

    static void erase(CxChannelRoad * oChannelRoad);

    static std::vector<std::string> reportSelf();

    static void nullLocalSocket(socket_t iSocket);

    static void nullLocalSocket(const std::vector<socket_t> & sockets);

    static int sendData(int iSourceId, const char * pData, int iLength);

    static int updateRemote(int iSourceId, int iTargetPort);

};


class CxIChannelSubject
{
protected:
    virtual bool channel_canChangeConnect(const CxChannelBase* oChannel, bool bOldConnect, bool bNewConnect) { return true; }

    virtual void channel_connectChanged(const CxChannelBase* oChannel) {}

    virtual void channel_beforeDelete(const CxChannelBase* oChannel) {}

    virtual void channel_receivedData(const uchar* pData, int iLength, void * oSource) {}

    friend class CxChannelPublisher;

};


class CxChannelPublisher
{
public:
    CxChannelPublisher();

    ~CxChannelPublisher();

    void addObserver(CxIChannelSubject* oSubject){ if (! CxContainer::contain(_subjects, oSubject)) _subjects->push_back(oSubject); }

    void removeObserver(CxIChannelSubject* oSubject){ std::remove(_subjects->begin(), _subjects->end(), oSubject); }

    inline const std::vector<CxIChannelSubject*>* Subjects() const { return _subjects; }

private:
    bool canConnectChange(const CxChannelBase* oChannel);

    void connectChanged(const CxChannelBase* oChannel);

    void receivedData(const uchar* pData, int iLength, void * oSource);

    void beforeDelete(const CxChannelBase* oChannel);

    std::vector<CxIChannelSubject*>* _subjects;

    friend class CxChannelBase;
};


class CxIChannelSend
{
public:
    virtual int send_data(const char* pData, int iLength, int iTargetId) =0;

};


class CxChannelFactoryBase : public CxFactoryTemplate<CxChannelBase>
{

};


class CxChannelBase : public ICxTimerNotify
{
public:
    static std::vector<CxFactoryTemplate<CxChannelBase>*>* factoriesContainer();

    static void factoriesCreateAndRegister();

    //sub thread notify other thread (main thread , processer thread)
    static void threadEventNotify(CxChannelBase * oTarget, int iEvent, int iTag = 0, const void * pData = 0, int iLength = 0, void * oSource = 0);

private:
    //main thread CallUp
    static void processThreadNofity(int, int, const void *, int, void *, void *);

public:
    CxChannelBase();

    virtual ~CxChannelBase();

    virtual const std::string& typeSimpleName() const = 0;

    virtual int typeSimpleId() const = 0;

    virtual void toContext(CxISetSkv& context) const;

    virtual void fromContext(const CxIGetSkv& context);

    void describeSelf(CxDescribeUnit & oDescribeUnit) const;

    std::vector<std::string> reportSelf() const;

    inline const std::string & localName() const { return _localName; }

    inline const CxChannelPublisher* publisher() const { return _publisher; }

    inline void addObserver(CxIChannelSubject* oSubject) { _publisher->addObserver(oSubject); }

    inline void removeObserver(CxIChannelSubject* oSubject) { _publisher->removeObserver(oSubject); }

    inline int sendData(const uchar * pData, int iLength, void * oTarget = NULL) { return sendData((const char *)(pData), iLength, oTarget); }

    int sendData(const char * pData, int iLength, void * oTarget = NULL);

    int sendBytes(const std::vector<char>& bytes, void * oTarget = NULL);

    int sendBytes(const std::vector<uchar>& bytes, void * oTarget = NULL);

    int sendText(const std::string& sText, void * oTarget = NULL);

    int sendHexString(const std::string& sText, void * oTarget = NULL);

    void pushProcessData(const char * pData, int iLength, void * oSource = NULL, int iTag = 0);

    inline int64 sentByteCount() const { return _sentByteCount; }

    inline int64 receivedByteCount() const { return  _receivedByteCount; }

    inline const msepoch_t & lastSentTime() const { return _lastSentTime; }

    inline const msepoch_t & lastReceivedTime() const { return _lastReceivedTime; }

    void open();

    void close();

    bool connected() const;

    void setConnected(bool bConnected);

    inline int channelId() const { return _channelId; }
    inline void setChannelId(int value) { _channelId = value; }

    inline const std::string& channelName() const { return _channelName; }
    inline void setChannelName(const std::string& value) { _channelName = value; }

    inline int autoOpenInterval() const { return _autoOpenInterval; }
    inline void setAutoOpenInterval(int value) { _autoOpenInterval = value; }

    inline bool isSendQueue() const { return _isSendQueue; }
    inline void setIsSendQueue(bool value) { _isSendQueue = value; }

    //attention : multiRoad and processQueue can not all ture
    inline bool isProcessQueue() const { return _isProcessQueue; }
    inline void setIsProcessQueue(bool value) { _isProcessQueue = value; if (value) _isMultiRoad = false; }

    //attention : multiRoad and processQueue can not all ture
    inline bool isMultiRoad() const { return _isMultiRoad; }
    inline void setIsMultiRoad(bool value) { _isMultiRoad = value; if (value) _isProcessQueue = false; }

    bool isSameChannel(const std::map<std::string, std::string>& params) const;

protected:
    void timer_timeOut(const CxTimer* oTimer);

    ///------Abstruct Declaration --- begin ---
    virtual bool getConnectedImpl() const = 0;

    virtual void openChannelImpl() = 0;

    virtual void closeChannelImpl() = 0;

    virtual int writeDataImpl(const char* pData, int iLength, void * oTarget) =0;

    virtual void openChannelPrepare() {}

    virtual void processConnect( int iEvent , int iTag, const std::string& sInfo ) {}

    virtual void processSentData( int iEvent , int iTag, const std::string& sInfo ) {}

    virtual void processError( int iError , const std::string& sError , int iTag );

    virtual bool isSameChannelImpl(const std::map<std::string, std::string>& params) const = 0;

    virtual void checkChannel() {}

    virtual void toReport(std::vector<std::string>& sInfos) const {}

    virtual void toDescribe(CxDescribeUnit& describeUnit) const;
    ///------Abstruct Declaration --- end ---


    ///------Sub class---CallUp---begin---
    void channelChanged();

    void channelBeforeDelete();

    //attention : may be main thread or processer thread run
    void processReceivedData(const char *, int, void * oSource, int iTag);

    inline CxOutStream outChannelPrompt() {
        CxOutStream r; r.noend(); r.type(CxInterinfo::Type_IO_Channel); r.source(_channelId); r.title(_localName); r << CxGlobal::promptString; return r;
    }

#if defined(GM_DEBUG)
    inline CxOutStream outChannelDebug() {
        CxOutStream r; r.noend(); r.type(CxInterinfo::Type_IO_Channel); r.source(_channelId); r.title(_localName); r << CxGlobal::debugString; return r;
    }
#else
#undef outChannelDebug
    inline CxNoOutStream outChannelDebug() { return CxNoOutStream(); }
#define CX_NO_outChannelDebug_MACRO while (false) outChannelDebug
#define outChannelDebug CX_NO_outChannelDebug_MACRO
#endif

    ///------Inherited---CallUp---end---

    inline void outReceiveInfo() {
        if (_infoReceived.size() > 1024 * 2)
        {
            outChannelPrompt().reason(12) << " received : " << _infoReceived << cxEndLine;
            _infoReceived.clear();
        }
    }

    enum ThreadStatusEnum {
        ThreadStatus_Creating = 0,
        ThreadStatus_Created,
        ThreadStatus_Start,
        ThreadStatus_Running,
        ThreadStatus_Stop,
        ThreadStatus_Exit
    };
    typedef enum ThreadStatusEnum ThreadStatusEnum;

    //*send thread
    class SenderThread : public CxJoinableThread
    {
    public:
        inline void setChannel(CxChannelBase * v) { _channel = v; }

        inline void setStatus(int * volatile v) { _status = v; }

        void waitExit() { _singleWait.signal(); join(); }

        int push(const char * pData, int iLength, void * oTarget);

        void init();

    protected:
        void run();

        virtual int writeData(const char* pData, int iLength, void * oTarget) {
            return _channel->writeDataImpl(pData, iLength, oTarget);
        }

        void exit() {
            * _status = ThreadStatus_Exit;
            CxJoinableThread::exit();
        }

    protected:
        CxChannelBase * _channel;

    private:
        int * volatile _status;
        CxMutex _lock;
        CxSingleWait _singleWait;
        std::vector<std::vector<char> > _buffers1;
        std::vector<std::vector<char> > _buffers2;
        std::vector<void*> _targets1;
        std::vector<void*> _targets2;
        std::vector<std::vector<char> > * volatile _pushBuffers;
        std::vector<std::vector<char> > * volatile _popBuffers;
        std::vector<void*> * volatile _pushTargets;
        std::vector<void*> * volatile _popTargets;

    };
    virtual SenderThread * createSenderThread() { return new SenderThread(); }

    //*send thread
    class ProcesserThread : public CxJoinableThread
    {
    public:
        inline void setChannel(CxChannelBase * v) { _channel = v; }

        inline void setStatus(int * volatile v) { _status = v; }

        void waitExit() { _singleWait.signal(); join(); }

        int push(const char * pData, int iLength, void * oSource, int iTag);

        void init();

    protected:
        void run();

        virtual void processData(const char* pData, int iLength, void * oTarget, int iTag) {
            _channel->processReceivedData(pData, iLength, oTarget, iTag);
        }

        void exit() {
            * _status = ThreadStatus_Exit;
            CxJoinableThread::exit();
        }

    protected:
        CxChannelBase * _channel;

    private:
        int * volatile _status;
        CxMutex _lock;
        CxSingleWait _singleWait;
        std::vector<std::vector<char> > _buffers1;
        std::vector<std::vector<char> > _buffers2;
        std::vector<void*> _sources1;
        std::vector<void*> _sources2;
        std::vector<int> _tags1;
        std::vector<int> _tags2;
        std::vector<std::vector<char> > * volatile _pushBuffers;
        std::vector<std::vector<char> > * volatile _popBuffers;
        std::vector<void*> * volatile _pushSources;
        std::vector<void*> * volatile _popSources;
        std::vector<int> * volatile _pushTags;
        std::vector<int> * volatile _popTags;

    };
    virtual ProcesserThread * createProcesserThread() { return new ProcesserThread(); }

private:
    void stopAndDeleteSenderThread();

    void stopAndDeleteProcesserThread();

private:
    int _channelId;
    int64 _receivedByteCount;
    int64 _sentByteCount;
    msepoch_t _lastReceivedTime;
    msepoch_t _lastSentTime;
    CxChannelPublisher* _publisher;
    CxTimer * _checkChannelTm;
    int _autoOpenInterval;
    std::string _channelName;
    std::string _infoReceived;

protected:
    std::string _localName;
    bool _isMultiRoad;
    //single thread to send data
    bool _isSendQueue;
    SenderThread * _sender;
    //single thread to process data
    bool _isProcessQueue;
    ProcesserThread * _processer;

private:
    int _senderStatus;
    int _processerStatus;

    friend class CxChannelManager;

};


class CxChannelFactoryManager : public CxFactoryManagerTemplate<CxChannelBase>
{
};


class CxChannelManager
{
public:
    static CxChannelBase* createChannel(const std::string& sTypeSimpleName);

    static void recoveChanrnel(CxChannelBase* oChannel);

    static const std::vector<CxChannelBase*>* channels();

    static CxChannelBase* channel(const CxChannelBase* oChanel);

    static CxChannelBase* channel(int iChannelId);

    static void deleteAll();

    static void setPacketInterinfoOut(bool value);

private:
    static void openAll();

    static void closeAll();

};

#endif // CXCHANNEL_H
