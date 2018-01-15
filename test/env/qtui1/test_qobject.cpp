#include "test_qobject.h"

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
    return true;
}

