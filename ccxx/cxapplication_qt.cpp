#include "cxapplication_qt.h"


CxApplicationReceiver f_applicationReceiver;


CxApplicationReceiver::CxApplicationReceiver(QObject *parent)
    : QObject(parent)
{
}

CxApplicationReceiver::~CxApplicationReceiver()
{
}

bool CxApplicationReceiver::event(QEvent * oEvent)
{
    CxApplication::runProcessCallBacks();
    return true;
}


//*CxApplicationQt
int fn_doLoopEvents_qt(int iTag)
{
    return QApplication::exec();
}

void fn_signalMainThread_qt()
{
    QApplication::postEvent(& f_applicationReceiver, new QEvent(QEvent::User));
}

void fn_doProcessEvents_qt()
{
    QApplication::processEvents();
}

void fn_doExit_qt()
{
    QApplication::exit();
}

void fn_reset_qt()
{
}


void CxApplicationQt::init(int argc, const char *argv[], int iProjectType)
{
    CxApplication::setFnCore(fn_doLoopEvents_qt,
                             fn_signalMainThread_qt,
                             fn_doProcessEvents_qt,
                             fn_doExit_qt,
                             fn_reset_qt);
    CxApplication::init(argc, argv, iProjectType);
}

int CxApplicationQt::exec(int iTag)
{
    return CxApplication::exec(iTag);
}













