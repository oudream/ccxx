
#include <cxmain.h>

using namespace std;

class Protocol : public CxIChannelSubject
{
public:
    Protocol()
    {
        _channel = new CxChannelTcpserver();

        _channel->addObserver(this);

        _channel->setLocalIp("127.0.0.1");
        _channel->setLocalPort(5556);
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
    CxChannelTcpserver * _channel;

};


int main(int argc, char * argv[])
{
    cout << "begin test tcp server" << endl;

    CxApplication::init(argc, argv);

    Protocol pl;

    return CxApplication::exec();
}

