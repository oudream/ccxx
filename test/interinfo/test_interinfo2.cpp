
#include <ccxx.h>

using namespace std;


int iCount = 0;

CxChannelTcpserver f_tcpServer;

class TimerPrintFunction : public CxInterinfoIn_I
{
public:
    static void timer_timeOut(int iInterval)
    {
        ++ iCount;
        if (iInterval == 1000)
        {
            cout << "TimerPrintFunction" << iInterval << endl;
        }
        else if (iInterval == 3000)
        {
            cout << "TimerPrintFunction" << iInterval << endl;
        }
        if (iCount>5)
            CxTimerManager::stopTimer(timer_timeOut, s_interval_3);
        if (iCount>10)
            CxTimerManager::stopTimer(timer_timeOut, s_interval_1);
    }

    static void start()
    {
        CxTimerManager::startTimer(timer_timeOut, s_interval_1);
        CxTimerManager::startTimer(timer_timeOut, s_interval_3);
    }

    static void stop()
    {
        CxTimerManager::stopTimer(timer_timeOut, s_interval_1);
        CxTimerManager::stopTimer(timer_timeOut, s_interval_3);
    }

protected:
    void interinfo_in_line( const std::string& sInfo, const msepoch_t & dtIn, int iSource = 0, int eInType = 0, int iTag = 0 )
    {

        vector<std::string> sArgvs = CxString::split(sInfo, ' ');
        if (sArgvs.size() < 2) return;
        const std::string & sCommand = sArgvs[0];
        const std::string & sPurpose = sArgvs[1];
        std::string sParam = (sArgvs.size() > 2) ? sArgvs[2] : "";

        if (sCommand == "system")
        {
            if (sPurpose == "exit")
            {
                CxApplication::exit();
            }
        }
        else if (sCommand == "msobject")
        {
            if (sPurpose == "report")
            {
                if (sParam.empty() || sParam == "all")
                {
                    std::string sFilePath = CxFileSystem::mergeFilePath( CxApplication::applicationConfigPath(), "msObjects.txt" );
                    int iCount = CxMsObjectDataManager::saveMsObjects(sFilePath);
                    cxPrompt() << "save MsObjects to " << sFilePath << " , file line count " << iCount << cxEndLine;
                }
            }
        }
        else if (sCommand == "interinfo")
        {

        }
        else if (sCommand == "enable")
        {
            if (sPurpose == "channel.packet.out")
            {
                CxChannelManager::setPacketInterinfoOut(true);
            }
            else if (0)
            {

            }
            else
            {
            }
        }
        else if (sCommand == "disable")
        {
            if (sPurpose == "channel.packet.out")
            {
                CxChannelManager::setPacketInterinfoOut(false);
            }
            else if (0)
            {

            }
            else
            {
            }
        }
    }


private:
    static int s_interval_1;
    static int s_interval_3;

};

int TimerPrintFunction::s_interval_1 = 1000;
int TimerPrintFunction::s_interval_3 = 3000;

static TimerPrintFunction f_serverMult;

int main(int argc, char * argv[])
{
    cout << "begin test interinfo : " << endl;

    CxApplication::init(argc, argv);

    f_tcpServer.setLocalPort(5555);
    CxConsoleInterinfo::addObserver(& f_serverMult);

    TimerPrintFunction::start();

    cout << "end test interinfo !!!" << endl;

    return CxApplication::exec();
}
