
#include <cxmain.h>


class Protocol : public CxIChannelSubject
{
public:
    Protocol()
    {
        _channel = new CxChannelTcpclient();
        _channel->setRemoteIp("127.0.0.1");
        _channel->setAutoOpen(true);
        _channel->open();
        _channel->close();
        _channel->open();
        _channel->close();
        _channel->open();
    }

    ~Protocol()
    {
        delete _channel;
    }

protected:
    void channel_receivedData(const uchar* pData, int iLength)
    {
    }

private:
    CxChannelTcpclient * _channel;

};


int main(int argc, char * argv[])
{
    CxApplication::init(argc, argv);

    Protocol pl;

    return CxApplication::exec();
}
