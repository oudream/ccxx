#ifndef CXAPPLICATION_QT_H
#define CXAPPLICATION_QT_H


#include "cxapplication.h"

#include <QApplication>



class CxApplicationReceiver : public QObject
{

    Q_OBJECT

public:
    explicit CxApplicationReceiver(QObject *parent=0);

    ~CxApplicationReceiver();

    bool event(QEvent *);

};


class CxApplicationQt : public CxApplication
{
public:
    static void init(int argc, const char * argv[], int iProjectType = GM_PROJECT_TYPE);

    static int exec(int iTag = 0);

};

#endif // CXAPPLICATION_QT_H
