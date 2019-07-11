#ifndef CXCHANNEL_TCPCLIENT_H
#define CXCHANNEL_TCPCLIENT_H


#include "cxchannel.h"


class GM_CCXX_CORE_API CxChannelTcpclient : public CxChannelBase
{
public:
    CxChannelTcpclient();

    ~CxChannelTcpclient();

    inline const std::string& typeSimpleName() const { return CS_TypeSimpleName_Tcpclient; }

    inline int typeSimpleId() const { return CxInterinfo::Type_IO_Tcpclient; }

    void toContext(CxISetSkv& context) const ;

    void fromContext(const CxIGetSkv& context);

    inline const std::string& remoteIp() const { return _remoteIp; }
    inline void setRemoteIp(const std::string& value) {
        if (!connected()) {
            _remoteIp = value;
            doSetLocalName();
        }
    }

    inline int remotePort() const { return _remotePort; }
    inline void setRemotePort(ushort value) {
        if (!connected()) {
            _remotePort = value;
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

    void processSentData( int iEvent , int iTag, const std::string& sInfo ) {}

    bool isSameChannelImpl(const std::map<std::string, std::string> &params) const;

    void toReport(std::vector<std::string>& sReports) const;

    void toDescribe( CxDescribeUnit & describeUnit) const;
    ///----------Abstruct Realization --- end ---

    //connect thread
    friend class ConnecterThread;
    class ConnecterThread : public CxJoinableThread
    {
    public:
        inline void setChannel(CxChannelTcpclient * v) { _channel = v; }

        void waitExit() { join(); }

    protected:
        void run();

    private:
        CxChannelTcpclient * _channel;
    };

    //read thread
    friend class ReceiverThread;
    class ReceiverThread : public CxJoinableThread
    {
    public:
        inline void setChannel(CxChannelTcpclient * v) { _channel = v; }

        inline void setSocket(socket_t v) { _so = v; }

        inline void setStatus(int * volatile v) { _status = v; }

        void waitExit() { join(); }

    protected:
        void run();

        void exit();

    private:
        CxChannelTcpclient * _channel;
        socket_t _so;
        int * volatile _status;
        char _buffer[ChannelBuffer_Size];

    };

private:
    std::string _remoteIp;
    ushort _remotePort;
    socket_t _socket;
    ReceiverThread * _receiver;
    int _receiverStatus;
    ConnecterThread * _connecter;
    volatile int _connectStatus;

private:
    void doSetLocalName();

    void shutdownAndClose();

    void createAndStartReceiverThread();

    void stopAndDeleteReceiverThread();

    void createAndStartConnecterThread();

    void stopAndDeleteConnecterThread();

};

class GM_CCXX_CORE_API CxChannelTcpclientFactory : public CxChannelFactoryBase
{
//    GM_SINGLETON_DECLARATION( CxChannelTcpclientFactory )
public:
    std::string factoryName(){
        return CS_TypeSimpleName_Tcpclient;
    }
    CxChannelBase* createObject(){
        return new CxChannelTcpclient;
    }
};

#endif // CXCHANNEL_TCPCLIENT_H
