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
  void channel_receivedData(const uchar *pData, int iLength)
  {
  }

 private:
  CxChannelTcpclient *_channel;

};

