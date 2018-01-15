
#include <ccxx/ccxx.h>

using namespace std;

class UdpSubject : public CxIChannelSubject
{
protected:
    void channel_receivedData(const uchar* pData, int iLength, void * oSource)
    {
        // 接收数据
        cout << "Recv " << iLength << " Byte" << endl;
    }

};

CxChannelUdp f_mUdp;
UdpSubject f_mUdpSubject;

void fn_timer_timeout1(int iInterval)
{
    cout << CxTime::currentSystemTimeString() << endl;
    if (!f_mUdp.isOpen())
    {
        cout << "Udp Client Is Not Open!";
        return;
    }
    f_mUdp.sendText("test send text1.");
    char data[10] = "hellodata";
    int iResult = f_mUdp.sendData(data, sizeof(data));
    cout << "Udp Client sendData Result=" << iResult << endl;
}

int main(int argc, const char * argv[])
{
    cout << "Udp Client Test Begin: " << endl;

    CxApplication::init(argc, argv);

    int iLocalPort = CxString::toInt32(CxAppEnv::findArgument("lp"));
    if (! CxString::isValidPort(iLocalPort)) iLocalPort = 5556;
    // 新建，配置
    // 实现CxIChannelSubject，并注册处理
    f_mUdp.addObserver(&f_mUdpSubject);
    f_mUdp.setLocalIp("127.0.0.1");
    f_mUdp.setLocalPort(iLocalPort);
    f_mUdp.setRemoteIp("127.0.0.1");
    f_mUdp.setRemotePort(5555);
    f_mUdp.open();

    cout << "Udp Client Open. port=" << iLocalPort << ", status=" << f_mUdp.isOpen()<< endl;

    CxTimerManager::startTimer(fn_timer_timeout1, 2000);

    return CxApplication::exec();
}
