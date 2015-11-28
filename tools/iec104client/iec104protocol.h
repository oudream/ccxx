#ifndef IEC104PROTOCOL_H
#define IEC104PROTOCOL_H

const int IEC104BUFLEN = 255;

class SendSocket
{
public:
    int sendData(const char * data, int iLength) = 0;

};

class IEC104Protocol
{
public:

    IEC104Protocol(SendSocket * oSendSocket);

    ~IEC104Protocol();

    void ProtocolAfterStart();

    void ProtocolAfterStop();

    void ProtocolReceivedData(const unsigned char * pData, int iLength);

    void ProtocolTimerTimeOut(const long long & dt);

private:

    static const long _callUpSecond = 10;
    static const long _callUpMicrosecond = 0;
    static const long _checkSecond = 5;
    static const long _checkMicrosecond = 0;
    long long _lastReceiveTime;
    long long _lastSendTime;
    long long _lastDataTime;
    long long _lastCallUpTime;

    int FRecBufState;
    int FRecBufLen;
    unsigned char FRecBuf[IEC104BUFLEN];
    int FRecBufIndex;
    unsigned char FRecTypeFlag;
    unsigned int FCalcerS;
    unsigned int FCalcerR;

    bool _started;
    bool _tested;

    SendSocket * m_oSendSocket;

private:
    int WriteData(unsigned char *data, int length);

    void StartCallUp();

    void StopCallUp();

    void ChannelCheck();

    int SendCallUp(unsigned short reason, unsigned short stationId, int address = 0);

    int SendFrameI(unsigned char *data, int length);

    int SendBuffer(unsigned char *data, int length);

    void DealData();

    void DealFrameI();

    void ResetCalcer();

    void DealYX();

    void DealYC();

    void DealTotal();

    void DealYK();

    void SendFrameS();

    void SendStart();

    void SendStop();

    void SendTest();

    void SendTestConfirmed();

};

#endif // IEC104PROTOCOL_H
