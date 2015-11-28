
#include <cxmain.h>

using namespace std;

class Protocol : public CxIChannelSubject
{
public:
    Protocol()
    {
        _channel = new CxChannelSerial();

        _channel->addObserver(this);

        _channel->setPortName("COM1");
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
        _channel->sendData(pData, iLength);
        cout <<  _channel->connected() << endl;
    }

private:
    CxChannelSerial * _channel;

};


int main(int argc, char * argv[])
{
    cout << "begin test seral" << endl;

    CxApplication::init(argc, argv);

    Protocol pl;

    return CxApplication::exec();
}
