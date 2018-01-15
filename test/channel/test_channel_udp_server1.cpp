
#include <ccxx/ccxx.h>

using namespace std;

map<ushort, string> f_mPortIp;

class UdpSubject : public CxIChannelSubject
{
protected:
    void channel_receivedData(const uchar* pData, int iLength, void * oSource)
    {
        // 接收数据
        cout << "Recv " << iLength << " Byte" << endl;
        if (oSource)
        {
            const CxChannelRoad * oChannelRoad = CxChannelRoadManager::find(oSource);
            string sIp = oChannelRoad->remoteIpAddress().ip();
            ushort iPort = oChannelRoad->remoteIpAddress().port();
            cout << "From Remote Ip=" << sIp << ", Port=" << iPort;
            f_mPortIp[iPort] = sIp;
        }
    }

};

CxChannelUdp f_mUdp;
UdpSubject f_mUdpSubject;

void fn_timer_timeout1(int iInterval)
{
    cout << CxTime::currentSystemTimeString() << endl;
    if (!f_mUdp.isOpen())
    {
        cout << "Udp Server Is Not Open!";
        return;
    }
    char data[10] = "hellodata";
    for (std::map<ushort, string>::const_iterator it = f_mPortIp.begin(); it != f_mPortIp.end(); ++it)
    {
        ushort iPort = it->first;
        string sIp = it->second;
        int iResult = f_mUdp.writeTo(data, sizeof(data), sIp, iPort);
        cout << "Udp Server writeTo Ip=" << sIp << ", Post=" << iPort << ", Result=" << iResult << endl;
    }
}

int main(int argc, const char * argv[])
{
    cout << "Udp Server Test Begin: " << endl;

    CxApplication::init(argc, argv);

    // 新建，配置
    // 实现CxIChannelSubject，并注册处理
    f_mUdp.addObserver(&f_mUdpSubject);
    f_mUdp.setIsServer(true);
    f_mUdp.setIsMultiRoad(true);
    f_mUdp.setLocalIp("127.0.0.1");
    f_mUdp.setLocalPort(5555);
    f_mUdp.open();

    CxTimerManager::startTimer(fn_timer_timeout1, 3000);

    return CxApplication::exec();
}
