
//#include <cxapplication.h>
//#include <cxtime.h>
//#include <cxstring.h>
//#include <cxfile.h>

//using namespace std;


//int main(int argc,const char * argv[])
//{
//    CxApplication::init(argc, argv);



//    return 0;
//}


#include "test_channel_win.h"

#include <QApplication>
#include <ccxx/ccxx.h>

#undef main

int main(int argc,const char *argv[])
{
    QApplication a(argc, (char **)argv);
    CxApplication::init(argc, argv);

    TestChannelWin w;
    w.show();

    return CxApplication::exec();
}
