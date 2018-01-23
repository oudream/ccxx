
#include <ccxx/ccxx.h>

using namespace std;

class SerialSubject : public CxIChannelSubject
{
protected:
    void channel_receivedData(const uchar* pData, int iLength, void * oSource)
    {
        // 接收数据
        cxPrompt() << "Recv " << iLength << " Byte";
    }

};

CxChannelSerial f_mSerial;
SerialSubject f_mSerialSubject;

void fn_timer_timeout1(int iInterval)
{
    cout << CxTime::currentSystemTimeString() << endl;
    if (!f_mSerial.isOpen())
    {
        cout << "Serial Client Is Not Open!";
        return;
    }
    int iResult = f_mSerial.sendText("123abc999888777666555444333222111000");
    cout << "Serial sendData Result=" << iResult << endl;
}

int main(int argc, const char * argv[])
{
    cout << "Serial Client Test Begin: " << endl;

    CxApplication::init(argc, argv);

    string sPort = CxAppEnv::findArgument("port");
    if (sPort.empty()) sPort = "COM5";
    // 新建，配置
    // 实现CxIChannelSubject，并注册处理
    f_mSerial.addObserver(&f_mSerialSubject);
//PortName      = COM1
//BaudRate      = 6
//CharacterSize = 3
//StopBits      = 1
//Parity        = 0
//FlowControl   = 0
    f_mSerial.setPortName(sPort);
    f_mSerial.setBaudRate(6);
    f_mSerial.setCharacterSize((CxChannelSerial::CharacterSize)3);
    f_mSerial.setStopBits((CxChannelSerial::StopBits)1);
    f_mSerial.setParity((CxChannelSerial::Parity)0);
    f_mSerial.setFlowControl((CxChannelSerial::Flow)0);

    f_mSerial.open();

    cxPrompt() << f_mSerial.reportSelf();

    cout << "Serial Client Open. port=" << sPort << ", status=" << f_mSerial.isOpen()<< endl;

    CxTimerManager::startTimer(fn_timer_timeout1, 2000);

    return CxApplication::exec();
}
