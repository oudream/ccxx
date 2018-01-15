#include "serial_config_win.h"
#include "ui_serial_config_win.h"

serial_config_win::serial_config_win(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::serial_config_win)
{
    ui->setupUi(this);

    ui->DataBitsEd->setCurrentIndex(3);
    ui->BaudRateEd->setCurrentIndex(6);
}

serial_config_win::~serial_config_win()
{
    delete ui;
}

bool serial_config_win::inputConfig(std::string &sPortName, int & iBaudRateEnum, int & iFlowControl, int & iParity, int & iStopBits, int & iCharacterSize)
{
    serial_config_win w;
    if ( w.exec() == QDialog::Accepted)
    {
        sPortName = w.ui->PortNameEd->text().toStdString();
        iBaudRateEnum = w.ui->BaudRateEd->currentIndex();
        iFlowControl = w.ui->FlowEd->currentIndex();
        iParity = w.ui->ParityEd->currentIndex();
        iStopBits = w.ui->StopBitsEd->currentIndex();
        iCharacterSize = w.ui->DataBitsEd->currentIndex();
        return true;
    }
    return false;
}

void serial_config_win::on_pushButton_clicked()
{
    this->accept();
}

void serial_config_win::on_pushButton_2_clicked()
{
    this->reject();
}
