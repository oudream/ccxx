//
// Created by oudream on 2017/3/17.
//

#include <ccxx/ccxx.h>

class Protocol : public CxIChannelSubject {
 public:
  Protocol()
  {
    _channel = new CxChannelTcpclient();
    _channel->setRemoteIp("127.0.0.1");
    _channel->setAutoOpenInterval(5000);
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
  void channel_receivedData(const uchar *pData, int iLength)
  {
  }

 private:
  CxChannelTcpclient *_channel;

};

Protocol * f_oProtocol = NULL;

void fn_timer_timeout(int iInterval)
{
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
