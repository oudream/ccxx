/*
cxtest_channel_udp1 -lip 10.32.50.57 -lport 5555 -rip 10.32.50.57 -rport 5556
cxtest_channel_udp1 -lip 10.32.50.57 -lport 5556 -rip 10.32.50.57 -rport 5555
/**/
#include <ccxx/ccxx.h>

using namespace std;

class UdpSubject : public CxIChannelSubject
{
public:
    UdpSubject(){ 
        mRecvTotalTimes = 0;
        mRecvTotalLen = 0;
    }
    
    ~UdpSubject(){ 
        
    }
    
    int mRecvTotalTimes;
    int mRecvTotalLen;

protected:
    void channel_receivedData(const uchar* pData, int iLength, void * oSource)
    {
        mRecvTotalTimes++;
        mRecvTotalLen += iLength;
        cout << "Recv " << iLength << " Byte, mRecvTotalTimes " << mRecvTotalTimes << " Byte, mRecvTotalLen " << mRecvTotalLen << endl;
    }

};

CxChannelUdp f_mUdp;
UdpSubject f_mUdpSubject;
#define GM_TEST_UDP_DATA_LEN 14000
int mSendTotalTimes = 0;
int mSendTotalLen = 0;

void fn_timer_timeout1(int iInterval)
{
    cout << CxTime::currentSystemTimeString() << endl;
    if (!f_mUdp.isOpen())
    {
        cout << "Udp Client Is Not Open!";
        return;
    }
    char data[GM_TEST_UDP_DATA_LEN];
    f_mUdp.sendData(data, GM_TEST_UDP_DATA_LEN);
    int iResult = f_mUdp.sendData(data, sizeof(data));
    mSendTotalTimes ++;
    mSendTotalLen += iResult;
    cout << "Udp Client sendData Result=" << iResult << " Byte, mSendTotalTimes " << mSendTotalTimes << " Byte, mSendTotalLen " << mSendTotalLen << endl;
}

int main(int argc, const char * argv[])
{
    cout << "Udp Client Test Begin: " << endl;

    CxApplication::init(argc, argv);

    string sLocalIp = CxAppEnv::findArgument("lip");
    if (! CxString::isValidIp(sLocalIp)) sLocalIp = "127.0.0.1";
    int iLocalPort = CxString::toInt32(CxAppEnv::findArgument("lport"));
    if (! CxString::isValidPort(iLocalPort)) iLocalPort = 5555;
    string sRemoteIp = CxAppEnv::findArgument("rip");
    if (! CxString::isValidIp(sRemoteIp)) sRemoteIp = "127.0.0.1";
    int iRemotePort = CxString::toInt32(CxAppEnv::findArgument("rport"));
    if (! CxString::isValidPort(iRemotePort)) iRemotePort = 5556;

    f_mUdp.addObserver(&f_mUdpSubject);
    f_mUdp.setLocalIp(sLocalIp);
    f_mUdp.setLocalPort(iLocalPort);
    f_mUdp.setRemoteIp(sRemoteIp);
    f_mUdp.setRemotePort(iRemotePort);
    f_mUdp.open();
    
    cout << "Udp Client Open. port=" << iLocalPort << ", status=" << f_mUdp.isOpen()<< endl;

    CxThread::sleep(5000);

    CxTimerManager::startTimer(fn_timer_timeout1, 100);

    return CxApplication::exec();
}
