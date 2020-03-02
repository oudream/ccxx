#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    quitAction = new QAction(tr("退出(&Quit)"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    // QIcon icon(":/images/logo_ics.ico");
    string sIconFilePath = "./images/logo.ico";
    QIcon icon(QString::fromStdString(sIconFilePath));

    trayIcon->setIcon(icon);
    qApp->setWindowIcon(icon);

    trayIcon->show();

    _timer1.init(this, 1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timer_timeOut(const CxTimer *oTimer)
{
    if (oTimer == &_timer1)
    {
        outInfo(CxString::toString(CxTime::currentSystemTime()));
    }
}

void MainWindow::outInfo(const std::string &sInfo)
{
    ui->plainTextEdit->appendPlainText(CxQString::gbkToQString(sInfo));
    ui->plainTextEdit->appendPlainText("");
}

void MainWindow::outInfo(const QString &sInfo)
{
    ui->plainTextEdit->appendPlainText((sInfo));
    ui->plainTextEdit->appendPlainText("");
}

void MainWindow::on_pushButton_1_clicked()
{
    outInfo(CxTime::currentSystemTimeString());
}

void MainWindow::on_pushButton_2_clicked()
{
    outInfo(CxApplication::applicationDeployPath());
}

void MainWindow::on_checkBox_1_clicked()
{
    if (ui->checkBox_1->isChecked())
        CxTimerManager::startTimer(&_timer1);
    else
        CxTimerManager::stopTimer(&_timer1);
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->plainTextEdit->clear();
}
