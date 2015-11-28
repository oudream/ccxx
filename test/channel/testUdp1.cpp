
#include <cxmain.h>

using namespace std;

class Protocol : public CxIChannelSubject
{
public:
    Protocol()
    {
        _channel = new CxChannelUdp();

        _channel->addObserver(this);

        _channel->setLocalIp("127.0.0.1");
        _channel->setLocalPort(5556);
        _channel->setRemoteIp("127.0.0.1");
        _channel->setRemotePort(5555);
        _channel->setAutoOpen(true);
        _channel->open();
//        _channel->close();
//        _channel->open();
//        _channel->close();
//        _channel->open();
    }

    ~Protocol()
    {
        delete _channel;
    }

protected:
    void channel_receivedData(const uchar* pData, int iLength)
    {
        bool b = _channel->connected();
        if (b)
        {
            _channel->sendText("aaa!!!");
        }
    }

private:
    CxChannelUdp * _channel;

};


int main(int argc, char * argv[])
{
    cout << "begin test uapd" << endl;

    CxApplication::init(argc, argv);

    Protocol pl;

    return CxApplication::exec();
}
