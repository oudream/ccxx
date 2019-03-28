#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <ccxx/ccxx.h>
#include <ccxx/cxqtutil.h>
#include <QSystemTrayIcon>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow , public ICxTimerNotify
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void timer_timeOut(const CxTimer* oTimer);

private slots:
    void on_pushButton_1_clicked();

    void on_pushButton_2_clicked();

    void on_checkBox_1_clicked();

    void on_pushButton_3_clicked();

private:
    void outInfo(const std::string & sInfo);

    void outInfo(const QString & sInfo);

private:
    Ui::MainWindow *ui;
    CxTimer _timer1;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QAction *quitAction;

};

#endif // MAINWINDOW_H
