#ifndef TEST_CHANNEL_WIN_H
#define TEST_CHANNEL_WIN_H

#include <QMainWindow>
#include <QTimer>
#include <ccxx/ccxx.h>

namespace Ui {
class TestChannelWin;
}

class TestQEvent : public QObject
{

    Q_OBJECT

public:
    explicit TestQEvent(QObject *parent=0);

    ~TestQEvent();

    bool event(QEvent *);

};


class TimerThread : public CxJoinableThread
{
public:
    void waitExit();

protected:
    void run();

};



class TestChannelWin : public QMainWindow , public CxIChannelSubject , CxInterinfoOut_I
{

    Q_OBJECT

public:
    explicit TestChannelWin(QWidget *parent = 0);
    ~TestChannelWin();

    void channel_receivedData(const uchar* pData, int iLength);

    void interinfo_out( const std::string& sInfo, const std::string& sTitle, int type = 0, int reason = 0, int source = 0, int target = 0, int iTag = 0 );

    CxInterinfo::PlatformEnum platformValue() { return CxInterinfo::Platform_Desktop; }

    void testPostEvent();

    void outInfo(const QString& sInfo);

    void outInfo(const std::string& sInfo);

private slots:
    void on_sendBn_clicked();

    void on_clearInfoOutBn_clicked();

    void on_connectBn_clicked();

    void on_closeBn_clicked();

    void on_isSendQueueCb_stateChanged(int arg1);

    void on_timer_timeout();

    void on_channelTypeCb_currentIndexChanged(int index);

    void on_testBn_clicked();

    void on_xmlExplainBn1_clicked();

    void on_xmlExplainBn2_clicked();

private:
    Ui::TestChannelWin *ui;
    QTimer * _timer;
    TimerThread _timerThread;

    std::string _portName;
    int _baudRateEnum;
    int _flowControl;
    int _parity;
    int _stopBits;
    int _characterSize;

    CxChannelBase * _channel;

};

#endif // TEST_CHANNEL_WIN_H
