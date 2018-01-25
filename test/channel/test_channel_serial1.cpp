
#include <ccxx/ccxx.h>

using namespace std;

static long long f_iSerialReceivedTotal = 0;

class SerialSubject : public CxIChannelSubject
{
protected:
    void channel_receivedData(const uchar* pData, int iLength, void * oSource)
    {
        // 接收数据
        f_iSerialReceivedTotal += iLength;
        cxPrompt() << "Recv " << f_iSerialReceivedTotal << " Byte";
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
    if (sPort.empty()) sPort = "COM1";
    // 新建，配置
    // 实现CxIChannelSubject，并注册处理
    f_mSerial.addObserver(&f_mSerialSubject);
    f_mSerial.setPortName(sPort);                                   //PortName      = COM1
    f_mSerial.setBaudRate(CxChannelSerial::BaudRate_56000);         //BaudRate      = 10
    f_mSerial.setCharacterSize(CxChannelSerial::CharacterSizeEight);//CharacterSize = 3
    f_mSerial.setStopBits(CxChannelSerial::stopBitsOne);            //StopBits      = 0
    f_mSerial.setParity(CxChannelSerial::parityNone);               //Parity        = 0
    f_mSerial.setFlowControl(CxChannelSerial::flowNone);            //FlowControl   = 0

    f_mSerial.open();

    cxPrompt() << f_mSerial.reportSelf();

    cout << "Serial Client Open. port=" << sPort << ", status=" << f_mSerial.isOpen()<< endl;

    CxTimerManager::startTimer(fn_timer_timeout1, 2000);

    return CxApplication::exec();
}
