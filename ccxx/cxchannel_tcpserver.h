#ifndef CXCHANNEL_TCPSERVER_H
#define CXCHANNEL_TCPSERVER_H


#include "cxchannel.h"

#define ServerAcceptStatus_None         (0)
#define ServerAcceptStatus_Preparing    (1)
#define ServerAcceptStatus_Accepting    (2)
#define ServerAcceptStatus_Append       (3)
#define ServerAcceptStatus_Error        (4)
#define ServerAcceptStatus_Closing      (5)


class GM_CCXX_CORE_API CxChannelTcpserver : public CxChannelBase
{
public:
    static int sendDataBySockets(CxChannelBase * oChannel, const char* pData, int iLength, void * oTarget, const std::vector<socket_t> & mSockets);

public:
    CxChannelTcpserver();

    ~CxChannelTcpserver();

    inline const std::string& typeSimpleName() const { return CS_TypeSimpleName_Tcpserver; }

    inline int typeSimpleId() const { return CxInterinfo::Type_IO_Tcpserver; }

    void toContext(CxISetSkv& context) const ;

    void fromContext(const CxIGetSkv& context);

    inline const std::string& localIp() const { return _localIp; }
    inline void setLocalIp(const std::string& value) {
        if (!connected())
        {
            _localIp = value;
            doSetLocalName();
        }
    }

    inline int localPort() const { return _localPort; }
    inline void setLocalPort(ushort value) {
        if (!connected())
        {
            _localPort = value;
            doSetLocalName();
        }
    }

protected:
    ///----------Abstruct Realization --- begin ---
    bool getConnectedImpl() const;

    void openChannelImpl();

    void closeChannelImpl();

    int writeDataImpl(const char* pData, int iLength, void * oTarget);

    void processConnect( int iEvent , int iTag , const std::string& sInfo );

    void processSentData( int iEvent , int iTag , const std::string& sInfo ) {}

    void processError( int iError , const std::string& sError , int iTag );

    bool isSameChannelImpl(const std::map<std::string, std::string> &params) const;

    void toReport(std::vector<std::string>& sReports) const;

    void toDescribe( CxDescribeUnit & describeUnit) const;
    ///----------Abstruct Realization --- end ---

    //connect thread
    friend class AccepterThread;
    class AccepterThread : public CxJoinableThread
    {
    public:
        inline void setChannel(CxChannelTcpserver * v) { _channel = v; }

        inline void waitExit() { join(); }

        inline void close()
        {
            _listenStatus = ThreadStatus_Stop;
            CxSocket::cancel(_listenSocket);
            CxSocket::release(_listenSocket);
        }

    protected:
        inline void run()
        {
            int iError = 0;
            _listenSocket = CxSocket::create(PF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (INVALID_SOCKET == _listenSocket)
            {
                return;
            }

            ushort iLocalPost = _channel->_localPort;
            std::string sLocalIp = _channel->_localIp;
            CxIpAddress stSockAddr(sLocalIp, iLocalPost, true);

            iError = CxSocket::bindto(_listenSocket, stSockAddr.getSockAddr());
            if (iError != 0)
            {
                std::string sError = CxString::format("[ip]=%s [port]=%d bind error : %s\n", sLocalIp.c_str(), iLocalPost, strerror(iError));
                threadEventNotify(_channel, ChannelEvent_Connect_Fail, 0, sError.c_str(), sError.size());
                CxSocket::release(_listenSocket);
                return;
            }

            iError = CxSocket::listento(_listenSocket);
            if (iError != 0)
            {
                std::string sError = CxString::format("[ip]=%s [port]=%d listen error : %s\n", sLocalIp.c_str(), iLocalPost, strerror(iError));
                threadEventNotify(_channel, ChannelEvent_Connect_Fail, 0, sError.c_str(), sError.size());
                CxSocket::release(_listenSocket);
                return;
            }
            else
            {
                _channel->_accepterStatus = ServerAcceptStatus_Accepting;
                threadEventNotify(_channel, ChannelEvent_Connect_Success, 0);
            }

            _listenStatus = ThreadStatus_Running;
            while (1)
            {
                socket_t iConnectFD = CxSocket::acceptfrom(_listenSocket);
                if(INVALID_SOCKET == iConnectFD)
                {
                    if (_listenStatus != ThreadStatus_Stop)
                    {
                        std::string sError = CxString::format("[ip]=%s [port]=%d accept error : %s\n", sLocalIp.c_str(), iLocalPost, strerror(iError));
                        threadEventNotify(_channel, ChannelEvent_Connect_Fail, 0, sError.c_str(), sError.size());
                        CxSocket::release(_listenSocket);
                    }
                    return;
                }

                CxSocket::sendTimeout(iConnectFD, 1000);
                threadEventNotify(_channel, ChannelEvent_Connect_Success, iConnectFD);
            }
        }

    private:
        CxChannelTcpserver * _channel;
        socket_t _listenSocket;
        volatile int _listenStatus;

    };

    friend class SenderThread2;
    class SenderThread2 : public SenderThread
    {
    public:
        SenderThread2()
        {
            _socketsChanngeState = 0;
        }

        inline void pushAddSock(socket_t so)
        {
            CxMutexScope lock(_socketsChanngeLock);
            _socketsAdd.push_back(so);
            _socketsChanngeState |= 0x01;
        }

        inline void pushRemove(socket_t so)
        {
            CxMutexScope lock(_socketsChanngeLock);
            _socketsRemove.push_back(so);
            _socketsChanngeState |= 0x02;
        }

        inline void pushClear(const std::vector<socket_t> sockets)
        {
            CxMutexScope lock(_socketsChanngeLock);
            CxContainer::append(_socketsRemove, sockets);
            _socketsChanngeState |= 0x02;
        }

    protected:
        inline int writeDataThread(const char* pData, int iLength, void * oTarget)
        {
            if (_socketsChanngeState != 0)
            {
                CxMutexScope lock(_socketsChanngeLock);
                if ((_socketsChanngeState & 0x01) == 0x01)
                {
                    CxContainer::append(_sockets, _socketsAdd);
                    _socketsAdd.clear();
                }
                else if ((_socketsChanngeState & 0x02) == 0x02)
                {
                    CxContainer::remove(_sockets, _socketsRemove);
                    _socketsRemove.clear();
                }
                _socketsChanngeState = 0;
            }

            return CxChannelTcpserver::sendDataBySockets(_channel, pData, iLength, oTarget, _sockets);
        }

    private:
        volatile int _socketsChanngeState;
        CxMutex _socketsChanngeLock;
        std::vector<socket_t> _sockets;
        std::vector<socket_t> _socketsAdd;
        std::vector<socket_t> _socketsRemove;

    };

    inline SenderThread * createSenderThread() { return new SenderThread2(); }

    //read thread
    friend class SelecterThread;
    class SelecterThread : public CxJoinableThread
    {
    public:
        SelecterThread()
        {
            _socketsChanngeState = 0;
        }

        inline void setChannel(CxChannelTcpserver * v) { _channel = v; }

        inline void setStatus(int * volatile v) { _status = v; }

        inline void pushAddSock(socket_t so)
        {
            CxMutexScope lock(_socketsChanngeLock);
            _socketsAdd.push_back(so);
            _socketsChanngeState |= 0x01;
        }

        inline void pushRemoveSock(socket_t so)
        {
            CxMutexScope lock(_socketsChanngeLock);
            _socketsRemove.push_back(so);
            _socketsChanngeState |= 0x02;
        }

        inline void pushClearSocks(const std::vector<socket_t> sockets)
        {
            CxMutexScope lock(_socketsChanngeLock);
            CxContainer::append(_socketsRemove, sockets);
            _socketsChanngeState |= 0x02;
        }

        inline void waitExit() { join(); }

    protected:
        inline void run()
        {
            fd_set inRecvs;
            FD_ZERO(&inRecvs);
            fd_set inSends;
            FD_ZERO(&inSends);
            fd_set inErrors;
            FD_ZERO(&inErrors);
            socket_t soMax = INVALID_SOCKET;

            int rSelect;
            struct timeval tv;
            struct timeval *tvp = &tv;

            fd_set outRecvs = inRecvs;
//                    fd_set outSends = inSends;
            fd_set outErrors = inErrors;


            _recSockAddr = (sockaddr*)_buffer;
            _recBuffer = _buffer + CxSockAddrMaxSize;

            while (* _status == ThreadStatus_Start)
            {
                if (_socketsChanngeState != 0)
                {
                    {
                        CxMutexScope lock(_socketsChanngeLock);
                        if ((_socketsChanngeState & 0x01) == 0x01)
                        {
                            CxContainer::append(_sockets, _socketsAdd);
                            for (int i = 0; i < _socketsAdd.size(); ++i)
                            {
                                socket_t so = _socketsAdd[i];
                                struct sockaddr addr;
                                if (CxSocket::remote(so, (struct sockaddr_storage *)&addr) == 0)
                                {
                                    _socketAddres[_socketsAdd[i]] = addr;
                                }
                            }
                            _socketsAdd.clear();
                        }
                        else if ((_socketsChanngeState & 0x02) == 0x02)
                        {
                            CxContainer::remove(_sockets, _socketsRemove);
                            CxContainer::remove(_socketAddres, _socketsRemove);
                            _socketsRemove.clear();
                        }
                        _socketsChanngeState = 0;
                    }

                    FD_ZERO(&inRecvs);
                    socket_t soMax = INVALID_SOCKET;
                    for (size_t i = 0; i < _sockets.size(); ++i)
                    {
                        if (i >= FD_SETSIZE) break;
                        socket_t so = _sockets.at(i);
                        FD_SET(so, &inRecvs);
                        if (so > soMax) soMax = so;
                    }
                    inSends = inRecvs;
                    inErrors = inRecvs;
                }

                if (_sockets.size() > 0)
                {
                    outRecvs = inRecvs;
//                    outSends = inSends;
                    outErrors = inErrors;

                    tv.tv_usec = 0;
                    tv.tv_sec = 1;

                    rSelect = ::select((int)(soMax + 1), &outRecvs, NULL, &outErrors, tvp);
                    if (rSelect == 0)
                    {
                        continue;
                    }
                    else if (rSelect > 0)
                    {
                        raiseSelectRecv(&outRecvs);
                        raiseSelectError(&outErrors);
                    }
                    else
                    {
                        cxPrompt() << "TCP Server : remote client stop 001 Error By Select";
                        threadEventNotify(_channel, ChannelEvent_Platform_Error, -1);
                        break;
                    }
                }
                else
                {
                    CxThread::sleep(100);
                }
            }
        }

        void exit()
        {
            * _status = ThreadStatus_Exit;
            CxJoinableThread::exit();
        }

    private:
        inline void raiseSelectRecv(fd_set * oFds)
        {
            for (size_t i = 0; i < _sockets.size(); ++i)
            {
                socket_t so = _sockets.at(i);
                if (FD_ISSET(so, oFds))
                {
                    std::map<socket_t, struct sockaddr>::const_iterator it = _socketAddres.find(so);
                    if (it != _socketAddres.end())
                    {
                        * _recSockAddr = it->second;
                    }
                    ssize_t iSize = CxSocket::recvfrom(so,
                                                       _recBuffer, sizeof(_buffer)-CxSockAddrMaxSize,
                                                       0,
                                                       (struct sockaddr_storage *)_recSockAddr);
                    if ((iSize > 0) && (iSize <= (sizeof(_buffer)-CxSockAddrMaxSize)))
                    {
                        threadEventNotify(_channel, ChannelEvent_Received_Data, so, _buffer, iSize+CxSockAddrMaxSize,
                                          this);
                    }
                    else if (iSize == 0)
                    {
                        cxPrompt() << "TCP Server : remote client stop 001 Shutdown";
                        pushRemoveSock(so);
                        threadEventNotify(_channel, ChannelEvent_Receive_ShutdownR, so);
                        break;
                    }
                    else
                    {
                        if (* _status != ThreadStatus_Stop)
                        {
                            int iErrorCode = CxSocket::error();
                            if (iErrorCode != 0 && iErrorCode != EAGAIN)
                            {
                                cxPrompt() << "TCP Server : remote client stop 001 Error By Rec - iErrorCode: " << iErrorCode;
                                pushRemoveSock(so);
                                threadEventNotify(_channel, ChannelEvent_Receive_Error, so);
                                continue;
                            }
                        }
                    }
                }
            }
        }

        inline void raiseSelectError(fd_set * oFds)
        {
            for (size_t i = 0; i < _sockets.size(); ++i)
            {
                socket_t so = _sockets.at(i);
                if (FD_ISSET(so, oFds))
                {
                    int iErrorCode = CxSocket::error(so);
                    if (iErrorCode != 0 && iErrorCode != EAGAIN)
                    {
                        cxPrompt() << "TCP Server : remote client stop 001 Error By Error - iErrorCode: " << iErrorCode;
                        pushRemoveSock(so);
                        threadEventNotify(_channel, ChannelEvent_Platform_Error, so);
                        continue;
                    }
                }
            }
        }

    private:
        CxChannelTcpserver * _channel;
        int * volatile _status;
        volatile int _socketsChanngeState;
        CxMutex _socketsChanngeLock;
        std::vector<socket_t> _sockets;
        std::map<socket_t, struct sockaddr> _socketAddres;
        std::vector<socket_t> _socketsAdd;
        std::vector<socket_t> _socketsRemove;
        char _buffer[ChannelBuffer_Size];
        sockaddr * _recSockAddr;
        char * _recBuffer;

    };

private:
    std::string _localIp;
    ushort _localPort;
    SelecterThread * _selecter;
    int _selecterStatus;
    AccepterThread * _accepter;
    volatile int _accepterStatus;
    std::vector<socket_t> _sockets;
    CxMutex _socketsLocker;

private:
    void doSetLocalName();

    void createAndStartSelecterThread();

    void stopAndDeleteSelecterThread();

    void createAndStartAccepterThread();

    void stopAndDeleteAccepterThread();

    void appendSocket(socket_t so);

    void stopAndDeleteSocket(socket_t so);

    void stopAndClearSockets();

};

class GM_CCXX_CORE_API CxChannelTcpserverFactory : public CxChannelFactoryBase
{
//    GM_SINGLETON_DECLARATION( CxChannelTcpserverFactory )
public:
    std::string factoryName(){
        return CS_TypeSimpleName_Tcpserver;
    }
    CxChannelBase* createObject(){
        return new CxChannelTcpserver;
    }
};

#endif // CXCHANNEL_TCPSERVER_H
