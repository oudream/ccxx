#include "test_channel_win.h"
#include "ui_test_channel_win.h"


#include <ccxx/ccxx.h>
#include <ccxx/qtcommon.h>


using namespace std;


TestChannelWin * f_win = NULL;
QEvent f_event = QEvent(QEvent::User);



TestQEvent f_testEvent;



TestChannelWin::TestChannelWin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TestChannelWin)
{
    f_win = this;
    ui->setupUi(this);

    _timer = new QTimer();
    _timer->setInterval(1000);
    connect(_timer, SIGNAL(timeout()), SLOT(on_timer_timeout()));
    _timer->start();

    _channel = NULL;
    CxInterinfoOut::addObserver(this);

    std::vector<std::string> sLocalIps = CxNetwork::getLocalIps();
    for (size_t i = 0; i < sLocalIps.size(); ++i)
    {
        string sIp = sLocalIps.at(i);
        if (CxString::isValidIp(sIp))
        {
            ui->connectIp1Ed->addItem(QtCommonString::gbkToQString(sIp));
        }
    }
}

TestChannelWin::~TestChannelWin()
{
    CxInterinfoOut::removeObserver(this);
    _timerThread.waitExit();
    delete _timer;
    if (_channel) delete _channel;
    delete ui;
}

void TestChannelWin::testPostEvent()
{

}

void TestChannelWin::on_sendBn_clicked()
{
    GM_INVALID_RETURE(_channel);

    string sInfo = QtCommonString::gbkToStdString(ui->infoInEd->toPlainText());
    vector<uchar> data = CxString::fromHexstring(sInfo);

    string sIp = QtCommonString::gbkToStdString(ui->remoteIpEd->text());
    string sPort = QtCommonString::gbkToStdString(ui->remotePortEd->text());
    if (CxString::isValidIp(sIp) && CxString::isValidPort(CxString::toInt32(sPort)) && ui->channelTypeCb->currentIndex() == 3 && ui->channelTypeCb->currentIndex() == 4)
    {
        ((CxChannelUdp *)_channel)->writeTo((const char *)data.data(), data.size(), sIp, CxString::toInt32(sPort));
    }
    else
    {
        _channel->sendBytes(data);
    }
}

int iCount = 0;
void TestChannelWin::outInfo(const QString &sInfo)
{
    ui->infoOutEd->append(sInfo);
    ui->infoOutEd->append(QString::number(++iCount));
}

void TestChannelWin::outInfo(const string &sInfo)
{
    outInfo(QtCommonString::gbkToQString(sInfo));
}

void TestChannelWin::channel_receivedData(const uchar *pData, int iLength)
{
    outInfo(CxString::toHexstring(pData, iLength));
}

void TestChannelWin::interinfo_out(const string &sInfo, const string &sTitle, int type, int reason, int source, int target, int iTag)
{
    stringstream sOutInfo;
    if (sTitle.size()>0)
        sOutInfo << "[title]=" << sTitle;
    if (type)
        sOutInfo << "[type]=" << type;
    if (reason)
        sOutInfo << "[reason]=" << reason;
    if (source)
        sOutInfo << "[source]=" << source;
    if (target)
        sOutInfo << "[target]=" << target;
    if (iTag)
        sOutInfo << "[tag]=" << iTag;
    if (sOutInfo.tellp() > 0)
        sOutInfo << CxGlobal::lineString;
    sOutInfo << sInfo;

    outInfo(sOutInfo.str());
}

void TestChannelWin::on_clearInfoOutBn_clicked()
{
    ui->infoOutEd->clear();
}


TestQEvent::TestQEvent(QObject *parent)
  : QObject(parent)
{
    
}

TestQEvent::~TestQEvent()
{
}

bool TestQEvent::event(QEvent *)
{
    f_win->outInfo(CxTime::currentSystemTimeString());
}


volatile bool _canRun = true;

QEvent f_receivedDataEvent(QEvent::User);

void TimerThread::waitExit()
{
    _canRun = false;
    join();
}

void TimerThread::run()
{
    while (_canRun)
    {
        CxThread::sleep(10);
        QApplication::postEvent(&f_testEvent, &f_receivedDataEvent);
    }
}

void TestChannelWin::on_connectBn_clicked()
{
    if (_channel && _channel->connected())
    {
        _channel->close();
        delete _channel;
        _channel = NULL;
        outInfo(QString("disconnect success!!!"));
        return;
    }

    string sIp1 = QtCommonString::gbkToStdString(ui->connectIp1Ed->currentText());
    string sIp2 = QtCommonString::gbkToStdString(ui->connectIp2Ed->text());
    int iPort1 = ui->connectPort1Ed->value();
    int iPort2 = ui->connectPort2Ed->value();
    if (! CxString::isValidIp(sIp1) || ! CxString::isValidIp(sIp2)
            || ! CxString::isValidPort(iPort1)|| ! CxString::isValidPort(iPort2))
        return;

    if (! _channel)
    {
        //udp
        //tcp client
        //tcp server
        //udp (multi road)
        //tcp (multi road)
        int iChannelType = ui->channelTypeCb->currentIndex();
        switch (iChannelType)
        {
        case 1:
        {
            CxChannelTcpclient * oTcpclient = new CxChannelTcpclient();
            _channel = oTcpclient;
            oTcpclient->setRemoteIp(sIp2);
            oTcpclient->setRemotePort(iPort2);
        }
            break;
        case 0:
        case 3:
        {
            CxChannelUdp * oUdp = new CxChannelUdp();
            _channel = oUdp;
            oUdp->setLocalIp(sIp1);
            oUdp->setLocalPort(iPort1);
            oUdp->setRemoteIp(sIp2);
            oUdp->setRemotePort(iPort2);
        }
            break;
        case 2:
        case 4:
        {
            CxChannelTcpserver * oTcpServer = new CxChannelTcpserver();
            _channel = oTcpServer;
            oTcpServer->setLocalIp(sIp1);
            oTcpServer->setLocalPort(iPort1);
        }
            break;
        case 5:
        {
            CxChannelSerial * oSerial = new CxChannelSerial();
            _channel = oSerial;
            oSerial->setPortName(_portName);
            oSerial->setBaudRate(_baudRateEnum);
            oSerial->setFlowControl((CxChannelSerial::Flow)_flowControl);
            oSerial->setParity((CxChannelSerial::Parity)_parity);
            oSerial->setStopBits((CxChannelSerial::StopBits)_stopBits);
            oSerial->setCharacterSize((CxChannelSerial::CharacterSize)_characterSize);
        }
            break;
        default:
            break;
        }
        _channel->addObserver(this);
        _channel->setAutoOpenInterval(3000);
        if (iChannelType == 3 || iChannelType == 4)
        {
            _channel->setIsMultiRoad(true);
        }
        _channel->open();
    }


    int iCount = 0;
    while (! _channel->connected())
    {
        if (iCount > 3000)
            break;
        CxApplication::waiting(1);
        ++iCount;
    }

    if (!_channel->connected())
    {
        outInfo(QString("connect fail!!!"));
        GM_DELETEANDNULL_OBJECT(_channel);
    }
    else
    {
        outInfo(QString("connect success!!!"));
    }
}

void TestChannelWin::on_closeBn_clicked()
{
    if (_channel && _channel->connected())
    {
        _channel->close();
        delete _channel;
        _channel = NULL;
    }
    this->close();
    CxApplication::exit();
}

void TestChannelWin::on_isSendQueueCb_stateChanged(int arg1)
{
    GM_INVALID_RETURE(_channel);

    _channel->setIsSendQueue(ui->isSendQueueCb->checkState() == Qt::Checked);
}

void TestChannelWin::on_timer_timeout()
{
    if (_channel && _channel->connected())
    {
        ui->connectBn->setText("disconnect");
    }
    else
    {
        ui->connectBn->setText("connect");
    }
}

#include "serial_config_win.h"

void TestChannelWin::on_channelTypeCb_currentIndexChanged(int index)
{
    if (index == 5) {
        serial_config_win::inputConfig(_portName,_baudRateEnum,_flowControl,_parity,_stopBits,_characterSize);
    }
}

void TestChannelWin::on_testBn_clicked()
{
    if (_channel) _channel->close();
}
