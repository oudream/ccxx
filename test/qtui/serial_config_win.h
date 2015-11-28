#ifndef SERIAL_CONFIG_WIN_H
#define SERIAL_CONFIG_WIN_H

#include <QDialog>

namespace Ui {
class serial_config_win;
}

class serial_config_win : public QDialog
{
    Q_OBJECT

public:
    explicit serial_config_win(QWidget *parent = 0);
    ~serial_config_win();

    static bool inputConfig(std::string & sPortName, int & iBaudRateEnum, int & iFlowControl, int & iParity, int & iStopBits, int & iCharacterSize);

private slots:
    void on_pushButton_clicked();

private:
    Ui::serial_config_win *ui;
};

#endif // SERIAL_CONFIG_WIN_H
