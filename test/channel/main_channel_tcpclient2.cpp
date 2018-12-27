
#include <ccxx/ccxx.h>

using namespace std;

class Protocol : public CxIChannelSubject
{
public:
    Protocol()
    {
        _times = 0;
        _channel = new CxChannelTcpclient();

        _channel->addObserver(this);

        _channel->setRemoteIp("10.31.58.89");
        _channel->setRemotePort(5556);
        _channel->setAutoOpenInterval(3000);
        _channel->open();
    }

    ~Protocol()
    {
        delete _channel;
    }

    void sentTest() {
        if (_times % 50 == 0)
        {
            _channel->close();
            _channel->open();
            cxPrompt() << "sentTest reopen channel!";
        }
        else
        {
            int r = _channel->sendText(string("abcd i am tcp client.")+CxTime::currentSystemTimeString());
            cxPrompt() << "sentTest.return=" << r;
        }
        _times++;
    }

protected:
    void channel_receivedData(const uchar* pData, int iLength, void * oSource)
    {
        cxPrompt() << CxString::toHexstring(pData, iLength);
    }

private:
    CxChannelTcpclient * _channel;
    int _times;

};

Protocol * f_oProtocol = NULL;

 void fn_timer_timeout(int iInterval)
 {
     f_oProtocol->sentTest();
 }

int main(int argc, const char * argv[])
{
    CxApplication::init(argc, argv);

    f_oProtocol = new Protocol();

    CxTimerManager::startTimer(fn_timer_timeout, 100);

    int r = CxApplication::exec();

    delete f_oProtocol;
    f_oProtocol = NULL;

    return r;
}
