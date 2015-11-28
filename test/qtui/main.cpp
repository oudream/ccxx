
//#include <cxapplication.h>
//#include <cxtime.h>
//#include <cxstring.h>
//#include <cxfile.h>

//using namespace std;


//int main(int argc, char * argv[])
//{
//    CxApplication::init(argc, argv);



//    return 0;
//}


#include "test_channel_win.h"

#include <QApplication>
#include <ccxx/ccxx.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CxApplication::init(argc, argv);

    CxLogThread::startLog();

    TestChannelWin w;
    w.show();

    return CxApplication::exec(1);
}
