#ifndef CXCHANNEL_UDP_H
#define CXCHANNEL_UDP_H


#include "cxchannel.h"


class CxChannelUdp : public CxChannelBase
{
public:
    CxChannelUdp();

    ~CxChannelUdp();

    inline const std::string& typeSimpleName() const { return CS_TypeSimpleName_Udp; }

    inline int typeSimpleId() const { return CxInterinfo::Type_IO_Udp; }

    void toContext(CxISetSkv& context) const ;

    void fromContext(const CxIGetSkv& context);

    inline const std::string& localIp() const { return _localIpAddress.ip(); }
    inline void setLocalIp(const std::string& value) {
        if (!connected())
        {
            _localIpAddress.setIp(value);
            doSetLocalName();
        }
    }

    inline int localPort() const { return _localIpAddress.port(); }
    inline void setLocalPort(ushort value) {
        if (!connected())
        {
            _localIpAddress.setPort(value);
            doSetLocalName();
        }
    }

    inline const std::string& remoteIp() const { return _remoteIpAddress.ip(); }
    inline void setRemoteIp(const std::string& value) {
        if (!connected())
        {
            _remoteIpAddress.setIp(value);
            doSetLocalName();
        }
    }

    inline int remotePort() const { return _remoteIpAddress.port(); }
    inline void setRemotePort(ushort value) {
        if (!connected())
        {
            _remoteIpAddress.setPort(value);
            doSetLocalName();
        }
    }

    inline bool isServer() const { return _isServer; }
    inline void setIsServer(bool value) {
        if (!connected())
        {
            _isServer = value;
        }
    }

    int writeTo(const char *pData, int iLength, std::string szRemoteIp, int nRemotePort);

    int writeTo(const char *pData, int iLength, const CxIpAddress & mSendIp);

    void setRemoteIpAddress(const std::string& sIp, ushort iPort);

protected:
    ///----------Abstruct Realization --- begin ---
    bool getConnectedImpl() const;

    void openChannelImpl();

    void closeChannelImpl();

    int writeDataImpl(const char* pData, int iLength, void * oTarget);

    void processError( int iError , const std::string& sError , int iTag );

    bool isSameChannelImpl(const std::map<std::string, std::string> &params) const;

    void toReport(std::vector<std::string>& sReports) const;

    void toDescribe( CxDescribeUnit & describeUnit) const;

    ///----------Abstruct Realization --- end ---

    friend class ReceiverThread;
    //read thread
    class ReceiverThread : public CxJoinableThread
    {
    public:
        inline void setChannel(CxChannelUdp * v) { _channel = v; }

        inline void setStatus(int * volatile v) { _status = v; }

        void waitExit() { join(); }

    protected:
        void run();

        void exit();

    private:
        CxChannelUdp * _channel;
        int * volatile _status;

    };

private:
    CxIpAddress _localIpAddress;
    CxIpAddress _remoteIpAddress;

    //risk : send and recv share one socket , maybe send or recv fail;
    //solve : use lock this risk
    socket_t _socketBind;
    socket_t _socketSend;

    ReceiverThread * _receiver;
    int _statusReceiver;

    bool _isServer;

private:
    void doSetLocalName();

    bool hasRemote() const;

    bool hasLocal() const;

    void shutdownAndClose();

    void createAndStartThread();

    void stopAndDeleteThread();

    friend class ReceiverThread;

};

class CxChannelUdpFactory : public CxChannelFactoryBase
{
//    GM_SINGLETON_DECLARATION( CxChannelUdpFactory )
public:
    std::string factoryName() {
        return CS_TypeSimpleName_Udp;
    }

    CxChannelBase* createObject() {
        return new CxChannelUdp;
    }

};

#endif // CXCHANNEL_UDP_H
