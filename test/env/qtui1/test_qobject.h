#ifndef TEST_QOBJECT_H
#define TEST_QOBJECT_H

#include <QApplication>


class CxApplicationReceiver : public QObject
{

Q_OBJECT

public:
    explicit CxApplicationReceiver(QObject *parent=0);

    ~CxApplicationReceiver();

    bool event(QEvent *);

};

#endif // TEST_QOBJECT_H
