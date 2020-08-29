#include "cxqtutil.h"


#include <QApplication>
#include <QTextCodec>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QComboBox>

#include "ccxx.h"

using namespace std;

const QChar HexQChar16[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

QTextCodec * f_gbk = QTextCodec::codecForName("GB18030");
QTextCodec * f_utf8 = QTextCodec::codecForName("UTF-8");

///************ ToString begin
QString CxQString::gbkToQString(const std::string &s)
{
    return QString::fromLocal8Bit(s.data(), s.size());
}

QStringList CxQString::gbkToQStrings(const std::vector<std::string> &ss)
{
    QStringList r;
    for (size_t i = 0; i < ss.size(); ++i)
    {
        const std::string & s = ss.at(i);
        r.append(QString::fromLocal8Bit(s.data(), s.size()));
    }
    return r;
}

QStringList CxQString::gbkToQStrings(const std::string *oSs, int iCount)
{
    QStringList r;
    for (size_t i = 0; i < iCount; ++i)
    {
        const std::string & s = oSs[i];
        r.append(QString::fromLocal8Bit(s.data(), s.size()));
    }
    return r;
}

std::vector<std::string> CxQString::gbkToStdStrings(const QStringList &ss)
{
    std::vector<std::string> r;
    for (size_t i = 0; i < ss.count(); ++i)
    {
        const QString & s = ss[i];
        r.push_back(gbkToStdString(s));
    }
    return r;
}

std::string CxQString::gbkToStdString(const QString &s)
{
    QByteArray data = s.toLocal8Bit();
    return std::string( data.data(), data.size() );
}

QString CxQString::ToString(const int& i)
{
    return QString::number(i);
}

QString CxQString::ToString(const unsigned int &i)
{
    return QString::number(i);
}

QString CxQString::ToString(const int &i, int iZeroCount)
{
    QString s = QString::number(i);
    QString v;
    for (int i = 0; i < iZeroCount-s.length(); ++i)
    {
        v += "0";
    }
    return v+s;
}


QString CxQString::ToString(const bool& b)
{
    return QString::number(b);
}


QString CxQString::ToString(const double& d)
{
    return QString::number(d);
}


QString CxQString::ToString(const QString& s)
{
    return s;
}

QString CxQString::ToString(const std::string &s)
{
    return QString::fromLocal8Bit(s.data(), s.size());
}

QString CxQString::ToString(const qlonglong& dt)
{
    return DateTimeToString(dt);
}

QString CxQString::ToString(const QHash<QString, QString>& keyValues)
{
    QStringList sResults;
    QHash<QString, QString>::const_iterator it = keyValues.constBegin();
    while (it != keyValues.constEnd()) {
        sResults.append(QString("%1:%2").arg(it.key()).arg(it.value()));
        ++it;
    }
    return sResults.join("|");
}

QString CxQString::ToString(const QStringList& keyValues){
    return keyValues.join("|");
}

QString CxQString::ToString(const char *pData, int iLength)
{
#if (QT_VERSION >= QT_VERSION_CHECK(4, 8, 6))
        QString s = QString::fromLocal8Bit(pData, iLength);
#else
        QString s = QString::fromLocal8Bit(pData, iLength);
#endif
        return s;
}

QString CxQString::ToString(const char *pData)
{
#if (QT_VERSION >= QT_VERSION_CHECK(4, 8, 6))
        QString s = QString::fromLocal8Bit(pData, strlen(pData));
#else
        QString s = QString::fromLocal8Bit(pData, strlen(pData));
#endif
        return s;
}

QString CxQString::ToString(const uchar *pData, int iLength)
{
#if (QT_VERSION >= QT_VERSION_CHECK(4, 8, 6))
        QString s = QString::fromLocal8Bit((char*)pData, iLength);
#else
        QString s = QString::fromLocal8Bit((char*)pData, iLength);
#endif
    return s;
}

///************ ToString end


///************ FromString begin
int CxQString::FromString(const QString& s, const int& iDefault, bool *ok){
    if (s.length()<=0)
        return iDefault;
    bool b;
    int v = s.toInt(&b, 10);
    if ( ok ) *ok = b;
    if (b)
        return v;
    else
        return iDefault;
}


bool CxQString::FromString(const QString& s, const bool& bDefault, bool *ok){
    if (s.length()<=0)
        return bDefault;
    bool b;
    int v = s.toInt(&b, 10);
    if ( ok ) *ok = b;
    if (b)
        return (bool)v;
    else
        return bDefault;
}


float CxQString::FromString(const QString& s, const float& fDefault, bool *ok){
    if (s.length()<=0)
        return fDefault;
    bool b;
    float v = s.toFloat(&b);
    if ( ok ) *ok = b;
    if (b)
        return v;
    else
        return fDefault;
}


double CxQString::FromString(const QString& s, const double& dDefault, bool *ok){
    if (s.length()<=0)
        return dDefault;
    bool b;
    double v = s.toDouble(&b);
    if ( ok ) *ok = b;
    if (b)
        return v;
    else
        return dDefault;
}


QString CxQString::FromString(const QString& s, const QString& sDefault, bool *ok){
    if (ok) *ok=true;
    return s;
}

qlonglong CxQString::FromString(const QString& s, const qlonglong& dtDefault, bool *ok)
{
    if (s.length()<=0)
    {
        return dtDefault;
    }
    qlonglong dt = DateTimeFromString(s);
    if ( ok )
    {
        *ok = dt == 0;
    }
    return dt;
}

QHash<QString, QString> CxQString::FromString(const QString& s, const QHash<QString, QString>& hashDefault, bool *ok)
{
    if (s.length()<=0)
        return hashDefault;
    QHash<QString, QString> hashResult;
    QStringList ss = s.split("|");
    foreach (QString s2, ss) {
        QString sKey = FirstToken(s2, ':');
        hashResult[sKey] = s2;
    }
    if (hashResult.count()>0)
        return hashResult;
    else
        return hashDefault;
}


QStringList CxQString::FromString(const QString& s, const QStringList& hashDefault, bool *ok)
{
    if (s.length()<=0)
        return hashDefault;
    return s.split("|");
}

QString CxQString::DateTimeToString(const qlonglong &ms)
{
    QDateTime _g_datetime;

    _g_datetime.setMSecsSinceEpoch(ms);

    QDate date = _g_datetime.date();

    QTime t = _g_datetime.time();

    int y, m, d;
    date.getDate(&y, &m, &d);

    QString s = DateTimeToString(y, m, d, t.hour(), t.minute(), t.second());

    return s;
}

QString CxQString::DateTimeToString(int y, int m, int d, int h, int mi, int ms)
{
    const QChar HexQChar16[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

    QString dts("2012/12/12 12:12:12");

    if (y > -1 && y < 10000 && m > 0 && m < 13 && d > 0 && d < 32 && h > -1 && h < 24 && mi > -1 && mi < 60 && ms > -1 && ms < 100) {
        QChar * data = dts.data();

        int hc, lc;

        int yy = y % 100;

        hc = yy / 10;
        lc = yy % 10;
        data[2] = HexQChar16[ hc ];
        data[3] = HexQChar16[ lc ];

        hc = m / 10;
        lc = m % 10;
        data[5] = HexQChar16[ hc ];
        data[6] = HexQChar16[ lc ];

        hc = m / 10;
        lc = m % 10;
        data[5] = HexQChar16[ hc ];
        data[6] = HexQChar16[ lc ];

        hc = d / 10;
        lc = d % 10;
        data[8] = HexQChar16[ hc ];
        data[9] = HexQChar16[ lc ];

        hc = h / 10;
        lc = h % 10;
        data[11] = HexQChar16[ hc ];
        data[12] = HexQChar16[ lc ];

        hc = mi / 10;
        lc = mi % 10;
        data[14] = HexQChar16[ hc ];
        data[15] = HexQChar16[ lc ];

        hc = ms / 10;
        lc = ms % 10;
        data[17] = HexQChar16[ hc ];
        data[18] = HexQChar16[ lc ];
    } else {
//        dts = QString("err:%1-%2-%3 %4:%5:%6").arg(y).arg(m).arg(d).arg(h).arg(mi).arg(ms);
    }

    return dts;
}

qlonglong CxQString::DateTimeFromString(const QString &str)
{
//    QString dts("2012/12/12 12:12:12");
#if (QT_VERSION >= QT_VERSION_CHECK(4, 8, 6))
    QByteArray data = str.toLocal8Bit();
#else
    QByteArray data = str.toAscii();
#endif
    if (data.size() == 19)
    {

        int y, m, d, h, mi, s;

        const char zero = '0';

        y = (data[2] - zero) * 10 + (data[3] - zero);
        m = (data[5] - zero) * 10 + (data[6] - zero);
        d = (data[8] - zero) * 10 + (data[9] - zero);
        h = (data[11] - zero) * 10 + (data[12] - zero);
        mi = (data[14] - zero) * 10 + (data[15] - zero);
        s = (data[17] - zero) * 10 + (data[18] - zero);

        QDate date(y, m, d);
        QTime time(h, mi, s);
        return QDateTime(date, time).toMSecsSinceEpoch();
    }
    else
    {
        return 0;
    }
}

QString CxQString::FirstToken(QString &s, const QChar &sep)
{
    QString sResult;
    int iIndex = s.indexOf(sep);
    if ( iIndex > -1 ){
        sResult = s.left(iIndex);
        s = s.right(s.size()-iIndex-1);
    }
    return sResult;
}

QString CxQString::RightToken(const QString &s, const QChar &sep)
{
    QString sResult;
    int iIndex = s.indexOf(sep);
    if ( iIndex > -1 ){
        sResult = s.right(s.size()-iIndex-1);
    }
    return sResult;
}

QString CxQString::ToHexstring(const char *pData, int iLength)
{
    QByteArray hex(iLength * 3, Qt::Uninitialized);
    char *hexData = hex.data();
    const uchar *data = (const uchar *)pData;
    for (int i = 0; i < iLength; ++i) {
        int j = (data[i] >> 4) & 0xf;
        if (j <= 9)
            hexData[i*3] = (j + '0');
        else
            hexData[i*3] = (j + 'a' - 10);
        j = data[i] & 0xf;
        if (j <= 9)
            hexData[i*3+1] = (j + '0');
        else
            hexData[i*3+1] = (j + 'a' - 10);
        hexData[i*3+2] = 0x20;
    }
    return hex;
}

QString CxQString::ToHexstring(const uchar *pData, int iLength)
{
    QByteArray hex(iLength * 3, Qt::Uninitialized);
    char *hexData = hex.data();
    const uchar *data = (const uchar *)pData;
    for (int i = 0; i < iLength; ++i) {
        int j = (data[i] >> 4) & 0xf;
        if (j <= 9)
            hexData[i*3] = (j + '0');
        else
            hexData[i*3] = (j + 'a' - 10);
        j = data[i] & 0xf;
        if (j <= 9)
            hexData[i*3+1] = (j + '0');
        else
            hexData[i*3+1] = (j + 'a' - 10);
        hexData[i*3+2] = 0x20;
    }
    return hex;
}

QString CxQString::ToHexstring(const QByteArray& data)
{
    return ToHexstring(data.data(), data.size());
}

QString CxQString::ToHexstring(const char& v)
{
    QString s = "0x00";
    QChar* data = s.data();
    int j = (v >> 4) & 0xf;
    data[2] = HexQChar16[j];
    j = v & 0xf;
    data[3] = HexQChar16[j];
    return s;
}

QString CxQString::ToHexstring(const uchar& v)
{
    QString s = "0x00";
    QChar* data = s.data();
    int j = (v >> 4) & 0xf;
    data[2] = HexQChar16[j];
    j = v & 0xf;
    data[3] = HexQChar16[j];
    return s;
}

QString CxQString::ToHexstring(const short& v)
{
    QString s = "0x0000";
    QChar* data = s.data();
    int j = (v >> 12) & 0xf;
    data[2] = HexQChar16[j];
    j = (v >> 8) & 0xf;
    data[3] = HexQChar16[j];
    j = (v >> 4) & 0xf;
    data[4] = HexQChar16[j];
    j = v & 0xf;
    data[5] = HexQChar16[j];
    return s;
}

QString CxQString::ToHexstring(const ushort& v)
{
    QString s = "0x0000";
    QChar* data = s.data();
    int j = (v >> 12) & 0xf;
    data[2] = HexQChar16[j];
    j = (v >> 8) & 0xf;
    data[3] = HexQChar16[j];
    j = (v >> 4) & 0xf;
    data[4] = HexQChar16[j];
    j = v & 0xf;
    data[5] = HexQChar16[j];
    return s;
}

QString CxQString::ToHexstring(const int& v)
{
    QString s = "0x00000000";
    QChar* data = s.data();
    int j = (v >> 28) & 0xf;
    data[2] = HexQChar16[j];
    j = (v >> 24) & 0xf;
    data[3] = HexQChar16[j];
    j = (v >> 20) & 0xf;
    data[4] = HexQChar16[j];
    j = (v >> 16) & 0xf;
    data[5] = HexQChar16[j];
    j = (v >> 12) & 0xf;
    data[6] = HexQChar16[j];
    j = (v >> 8) & 0xf;
    data[7] = HexQChar16[j];
    j = (v >> 4) & 0xf;
    data[8] = HexQChar16[j];
    j = v & 0xf;
    data[9] = HexQChar16[j];
    return s;
}

QString CxQString::ToHexstring(const uint& v)
{
    QString s = "0x00000000";
    QChar* data = s.data();
    int j = (v >> 28) & 0xf;
    data[2] = HexQChar16[j];
    j = (v >> 24) & 0xf;
    data[3] = HexQChar16[j];
    j = (v >> 20) & 0xf;
    data[4] = HexQChar16[j];
    j = (v >> 16) & 0xf;
    data[5] = HexQChar16[j];
    j = (v >> 12) & 0xf;
    data[6] = HexQChar16[j];
    j = (v >> 8) & 0xf;
    data[7] = HexQChar16[j];
    j = (v >> 4) & 0xf;
    data[8] = HexQChar16[j];
    j = v & 0xf;
    data[9] = HexQChar16[j];
    return s;
}

QString CxQString::ToHexstring(const float& v)
{
    char fData[4];
    memcpy(fData, &v, 4);
    return ToHexstring(fData, 4);
}

QString CxQString::ToHexstring(const double& v)
{
    char fData[8];
    memcpy(fData, &v, 8);
    return ToHexstring(fData, 8);
}

QByteArray CxQString::FromHexstring(const QString &s)
{
    QString s2 = s;
    s2.remove(' ');
#if (QT_VERSION >= QT_VERSION_CHECK(4, 8, 6))
    QByteArray hexEncoded = s2.toLocal8Bit();
#else
    QByteArray hexEncoded = s2.toAscii();
#endif
    QByteArray res((hexEncoded.size() + 1)/ 2, Qt::Uninitialized);
    uchar *result = (uchar *)res.data() + res.size();

    bool odd_digit = true;
    for (int i = hexEncoded.size() - 1; i >= 0; --i) {
        int ch = hexEncoded.at(i);
        int tmp;
        if (ch >= '0' && ch <= '9')
            tmp = ch - '0';
        else if (ch >= 'a' && ch <= 'f')
            tmp = ch - 'a' + 10;
        else if (ch >= 'A' && ch <= 'F')
            tmp = ch - 'A' + 10;
        else
            continue;
        if (odd_digit) {
            --result;
            *result = tmp;
            odd_digit = false;
        } else {
            *result |= tmp << 4;
            odd_digit = true;
        }
    }

    res.remove(0, result - (const uchar *)res.constData());
    return res;
}

bool CxQString::CompareName(const QString &name1, const QString &name2)
{
    QString s1 = name1.trimmed().toLower();
    QString s2 = name2.trimmed().toLower();
    return s1 == s2;
}


















































































void CxQDialog::ShowPrompt(const QString& sText, const QString& sTitle)
{
    QMessageBox::information(0, sTitle, sText, GM_QString_TR("OK"));
}


void CxQDialog::ShowWarning(const QString& sText){
    QMessageBox::warning(0, "warning", sText, "OK");
}


void CxQDialog::ShowError(const QString& sText){
    QMessageBox::warning(0, "error", sText, "OK");
}


bool CxQDialog::ShowQuery(const QString& sQuery){
    int ret = QMessageBox::warning(0, GM_QString_TR("Please Option"),
                                   sQuery,
                                   GM_QString_TR("Yes"),
                                   GM_QString_TR("No"));
    return ret == 0;
}


int CxQDialog::ShowQuery3(const QString& sQuery){
    int ret = QMessageBox::warning(0, GM_QString_TR("Please Option"),
                                   sQuery,
                                   GM_QString_TR("Yes"),
                                   GM_QString_TR("No"),
                                   GM_QString_TR("Cancel"));
    switch (ret) {
    case 0:
        return 1;
    case 1:
        return 0;
    default:
        return -1;
    }
}


void CxQDialog::ShowTerminate(const QString& sText){
    QMessageBox::information(0, "terminate", sText, "My God");
}

//sameple : sExtension = *.png *.jpg *.bmp
bool CxQDialog::DialogOpen(QString& sFileName, const QString& sExtension) {
    QString sExtension2 = sExtension;
    if (sExtension2.length() <= 0) {
        sExtension2 = "*.*";
    }
    sFileName = QFileDialog::getOpenFileName(0,
        "Open File", "", QString("Files (%1)").arg(sExtension2));
    return sFileName.length();
}

//sameple : sExtension = *.png *.jpg *.bmp
bool CxQDialog::DialogSave(QString& sFileName, const QString& sExtension){
    QString sExtension2 = sExtension;
    if (sExtension2.length() <= 0) {
        sExtension2 = "*.*";
    }
    sFileName = QFileDialog::getOpenFileName(0,
        "Save File", "", QString("Files (%1)").arg(sExtension2));
    return sFileName.length();
}

QString CxQDialog::DialogDir(const QString &sRootDir, const QString &sTitle)
{
    QString sCaption = sTitle.size()>0 ? sTitle : "Select Dir";
    return QFileDialog::getExistingDirectory(0,
        sCaption, sRootDir);
}

bool CxQDialog::DialogInput(const QString& sPrompt, QString& sVaule){
    bool ok;
    QString text = QInputDialog::getText(0, GM_QString_TR("Input Box"),
                                         sPrompt, QLineEdit::Normal,
                                         sVaule, &ok);
    if (ok){
        sVaule = text;
        return true;
    } else
        return false;
}

bool CxQDialog::DialogInput2(const QString &sTitle, const QString &sValue1Title, const QString &sValue2Title, QString &sValue1, QString &sValue2, int iValue1EchoMode, int iValue2EchoMode)
{
    QDialog dialog;

    QLabel* value1Label = new QLabel( sValue1Title );
    QLabel* value2Label = new QLabel( sValue2Title );
    QLineEdit* value1Ed = new QLineEdit;
    value1Ed->setEchoMode( (QLineEdit::EchoMode)iValue1EchoMode );
    QLineEdit* value2Ed = new QLineEdit;
    value2Ed->setEchoMode( (QLineEdit::EchoMode)iValue2EchoMode );

    value1Ed->setText(sValue1);
    value2Ed->setText(sValue1);

    QGridLayout* gridLayout = new QGridLayout;
    gridLayout->addWidget( value1Label, 0, 0, 1, 1);
    gridLayout->addWidget( value1Ed, 0, 1, 1, 3 );
    gridLayout->addWidget( value2Label, 1, 0, 1, 1 );
    gridLayout->addWidget( value2Ed, 1, 1, 1, 3 );

    QPushButton* okBtn = new QPushButton( QString::fromUtf8("OK") );
    QPushButton* cancelBtn =new QPushButton  ( QString::fromUtf8("Cancel") );
    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->setSpacing( 60 );
    btnLayout->addWidget( okBtn );
    btnLayout->addWidget( cancelBtn );

    QVBoxLayout* dlgLayout = new QVBoxLayout;
    dlgLayout->setMargin( 40 );
    dlgLayout->addLayout( gridLayout );
    dlgLayout->addStretch( 40 );
    dlgLayout->addLayout( btnLayout );
    dialog.setLayout( dlgLayout );

    dialog.connect( okBtn, SIGNAL(clicked()), &dialog, SLOT(accept()) );
    dialog.connect( cancelBtn, SIGNAL(clicked()), &dialog, SLOT(reject()) );

    dialog.setWindowTitle( sTitle );
    dialog.resize( 380, 220 );
    if ( dialog.exec() == QDialog::Accepted )
    {
        sValue1 = value1Ed->text();
        sValue2 = value2Ed->text();
        return true;
    }
    else
    {
        return false;
    }
}

bool CxQDialog::DialogInput3(const QString &sTitle, const QString &sValue1Title, const QString &sValue2Title, const QString &sValue3Title, QString &sValue1, QString &sValue2, QString &sValue3, int iValue1EchoMode, int iValue2EchoMode, int iValue3EchoMode)
{
    QDialog dialog;

    QLabel* value1Label = new QLabel( sValue1Title );
    QLabel* value2Label = new QLabel( sValue2Title );
    QLabel* value3Label = new QLabel( sValue3Title );
    QLineEdit* value1Ed = new QLineEdit;
    value1Ed->setMaxLength(13);
    value1Ed->setEchoMode( (QLineEdit::EchoMode)iValue1EchoMode );
    QLineEdit* value2Ed = new QLineEdit;
    value2Ed->setMaxLength(13);
    value2Ed->setEchoMode( (QLineEdit::EchoMode)iValue2EchoMode );
    QLineEdit* value3Ed = new QLineEdit;
    value3Ed->setMaxLength(13);
    value3Ed->setEchoMode( (QLineEdit::EchoMode)iValue3EchoMode );

    value1Ed->setText(sValue1);
    value2Ed->setText(sValue2);
    value3Ed->setText(sValue3);

    QGridLayout* gridLayout = new QGridLayout;
    gridLayout->addWidget( value1Label, 0, 0, 1, 1);
    gridLayout->addWidget( value1Ed, 0, 1, 1, 3 );
    gridLayout->addWidget( value2Label, 1, 0, 1, 1 );
    gridLayout->addWidget( value2Ed, 1, 1, 1, 3 );
    gridLayout->addWidget( value3Label, 2, 0, 1, 1 );
    gridLayout->addWidget( value3Ed, 2, 1, 1, 3 );

    QPushButton* okBtn = new QPushButton( QPushButton::tr("OK") );
    QPushButton* cancelBtn =new QPushButton  ( QPushButton::tr("Cancel") );
    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->setSpacing( 60 );
    btnLayout->addWidget( okBtn );
    btnLayout->addWidget( cancelBtn );

    QVBoxLayout* dlgLayout = new QVBoxLayout;
    dlgLayout->setMargin( 40 );
    dlgLayout->addLayout( gridLayout );
    dlgLayout->addStretch( 40 );
    dlgLayout->addLayout( btnLayout );
    dialog.setLayout( dlgLayout );

    dialog.connect( okBtn, SIGNAL(clicked()), &dialog, SLOT(accept()) );
    dialog.connect( cancelBtn, SIGNAL(clicked()), &dialog, SLOT(reject()) );

    dialog.setWindowTitle( sTitle );
    dialog.resize( 380, 220 );
    if ( dialog.exec() == QDialog::Accepted )
    {
        sValue1 = value1Ed->text();
        sValue2 = value2Ed->text();
        sValue3 = value3Ed->text();
        return true;
    }
    else
    {
        return false;
    }
}

bool CxQDialog::DialogInput4(const QString &sTitle, const QString &sPrompt, QString &sVaule)
{
    bool ok;
    QString text = QInputDialog::getText(0, sTitle,
                                         sPrompt, QLineEdit::Normal,
                                         sVaule, &ok);
    if (ok){
        sVaule = text;
        return true;
    } else
        return false;
}

bool CxQDialog::DialogCombox(const QString &sTitle, const QString &sPrompt, const QStringList &sValues, QString& sValue )
{
    QDialog dialog;

    QLabel* valueLabel = new QLabel( sPrompt );
    QComboBox * valueCombo = new QComboBox();

    valueCombo->addItems( sValues );

    QGridLayout* gridLayout = new QGridLayout;
    gridLayout->addWidget( valueLabel, 0, 0, 1, 1);
    gridLayout->addWidget( valueCombo, 0, 1, 1, 3 );

    QPushButton* okBtn = new QPushButton( QObject::tr("OK") );
    QPushButton* cancelBtn =new QPushButton  ( QObject::tr("Cancel") );
    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->setSpacing( 60 );
    btnLayout->addWidget( okBtn );
    btnLayout->addWidget( cancelBtn );

    QVBoxLayout* dlgLayout = new QVBoxLayout;
    dlgLayout->setMargin( 40 );
    dlgLayout->addLayout( gridLayout );
    dlgLayout->addStretch( 40 );
    dlgLayout->addLayout( btnLayout );
    dialog.setLayout( dlgLayout );

    dialog.connect( okBtn, SIGNAL(clicked()), &dialog, SLOT(accept()) );
    dialog.connect( cancelBtn, SIGNAL(clicked()), &dialog, SLOT(reject()) );

    valueCombo->setCurrentIndex(sValues.indexOf(sValue));

    dialog.setWindowTitle( sTitle );
    dialog.resize( 240, 140 );
    if ( dialog.exec() == QDialog::Accepted )
    {
        sValue = valueCombo->currentText();
        return true;
    }
    else
    {
        return false;
    }
}












int CxQWidget::setItemValue16(QTableWidgetItem* oItem, const int & value)
{
    if (! oItem) return FALSE;
    QVariant data = oItem->data(Qt::UserRole);
    if (data.type() == QVariant::Invalid || data.toInt() != value)
    {
        oItem->setData(Qt::UserRole, value);
        oItem->setText(QString::number(value, 16));
        return TRUE;
    }
    return FALSE;
}

int CxQWidget::setItemValue(QTableWidgetItem* oItem, const int & value)
{
    if (! oItem) return FALSE;
    QVariant data = oItem->data(Qt::UserRole);
    if (data.type() == QVariant::Invalid || data.toInt() != value)
    {
        oItem->setData(Qt::UserRole, value);
        oItem->setText(QString::number(value));
        return TRUE;
    }
    return FALSE;
}

int CxQWidget::setItemValue(QTableWidgetItem* oItem, const double & value)
{
    if (! oItem) return FALSE;
    QVariant data = oItem->data(Qt::UserRole);
    if (data.type() == QVariant::Invalid || data.toDouble() != value)
    {
        oItem->setData(Qt::UserRole, value);
        oItem->setText(QString::number(value, 'g', 3));
        return TRUE;
    }
    return FALSE;
}

int CxQWidget::setItemValue(QTableWidgetItem* oItem, const long long & value)
{
    if (! oItem) return FALSE;
    QVariant data = oItem->data(Qt::UserRole);
    if (data.type() == QVariant::Invalid || data.toLongLong() != value)
    {
        oItem->setData(Qt::UserRole, value);
        oItem->setText(QString::number(value));
        return TRUE;
    }
    return FALSE;
}

int CxQWidget::setItemValue(QTableWidgetItem* oItem, const char * value)
{
    if (! oItem) return FALSE;
    QVariant data = oItem->data(Qt::UserRole);
    if (data.type() == QVariant::Invalid || data.toByteArray() != value)
    {
        QByteArray pData(value);
        oItem->setData(Qt::UserRole, pData);
        oItem->setText(CxQString::ToString(value));
        return TRUE;
    }
    return FALSE;
}

int CxQWidget::setItemValue(QTableWidgetItem *oItem, const std::string &value)
{
    return setItemValue(oItem, value.c_str());
}

int CxQWidget::setItemValue(QTableWidgetItem *oItem, const QString &value)
{
    if (! oItem) return FALSE;
    QVariant data = oItem->data(Qt::UserRole);
    if (data.type() == QVariant::Invalid || data.toString() != value)
    {
        oItem->setData(Qt::UserRole, value);
        oItem->setText(value);
        return TRUE;
    }
    return FALSE;
}

void CxQWidget::setItemValueDateTime(QTableWidgetItem *oItem, const long long &value)
{
    if (! oItem) return;
    QVariant data = oItem->data(Qt::UserRole);
    if (data.type() == QVariant::Invalid || data.toLongLong() != value)
    {
        oItem->setData(Qt::UserRole, value);
        oItem->setText(CxQString::ToString(value));
    }
}






int CxQWidget::getItemValue16(QTableWidgetItem* oItem, const int & value)
{
    if (oItem)
    {
        QVariant data = oItem->data(Qt::UserRole);
        if (data.type() != QVariant::Invalid)
        {
            return data.toInt();
        }
    }
    return value;
}

int CxQWidget::getItemValue(QTableWidgetItem* oItem, const int & value)
{
    if (oItem)
    {
        QVariant data = oItem->data(Qt::UserRole);
        if (data.type() != QVariant::Invalid)
        {
            return data.toInt();
        }
    }
    return value;
}

double CxQWidget::getItemValue(QTableWidgetItem* oItem, const double & value)
{
    if (oItem)
    {
        QVariant data = oItem->data(Qt::UserRole);
        if (data.type() != QVariant::Invalid)
        {
            return  data.toDouble();
        }
    }
    return value;
}

long long CxQWidget::getItemValue(QTableWidgetItem* oItem, const long long & value)
{
    if (oItem)
    {
        QVariant data = oItem->data(Qt::UserRole);
        if (data.type() != QVariant::Invalid )
        {
            return data.toLongLong();
        }
    }
    return value;
}

std::string CxQWidget::getItemValue(QTableWidgetItem *oItem, const std::string &value)
{
    if (oItem)
    {
        QVariant data = oItem->data(Qt::UserRole);
        if (data.type() != QVariant::Invalid )
        {
            return data.toString().toStdString();
        }
    }
    return value;
}

QString CxQWidget::getItemValue(QTableWidgetItem *oItem, const QString &value)
{
    if (oItem)
    {
        QVariant data = oItem->data(Qt::UserRole);
        if (data.type() != QVariant::Invalid )
        {
            return data.toString();
        }
    }
    return value;
}

long long CxQWidget::getItemValueDateTime(QTableWidgetItem *oItem, const long long &value)
{
    if (oItem)
    {
        QVariant data = oItem->data(Qt::UserRole);
        if (data.type() != QVariant::Invalid)
        {
            return data.toLongLong();
        }
    }
    return value;
}







int CxQWidget::setItemValue16(QListWidgetItem* oItem, const int & value)
{
    if (! oItem) return FALSE;
    QVariant data = oItem->data(Qt::UserRole);
    if (data.type() == QVariant::Invalid || data.toInt() != value)
    {
        oItem->setData(Qt::UserRole, value);
        oItem->setText(QString::number(value, 16));
        return TRUE;
    }
    return FALSE;
}

int CxQWidget::setItemValue(QListWidgetItem* oItem, const int & value)
{
    if (! oItem) return FALSE;
    QVariant data = oItem->data(Qt::UserRole);
    if (data.type() == QVariant::Invalid || data.toInt() != value)
    {
        oItem->setData(Qt::UserRole, value);
        oItem->setText(QString::number(value));
        return TRUE;
    }
    return FALSE;
}

int CxQWidget::setItemValue(QListWidgetItem* oItem, const double & value)
{
    if (! oItem) return FALSE;
    QVariant data = oItem->data(Qt::UserRole);
    if (data.type() == QVariant::Invalid || data.toDouble() != value)
    {
        oItem->setData(Qt::UserRole, value);
        oItem->setText(QString::number(value, 'g', 3));
        return TRUE;
    }
    return FALSE;
}

int CxQWidget::setItemValue(QListWidgetItem* oItem, const long long & value)
{
    if (! oItem) return FALSE;
    QVariant data = oItem->data(Qt::UserRole);
    if (data.type() == QVariant::Invalid || data.toLongLong() != value)
    {
        oItem->setData(Qt::UserRole, value);
        oItem->setText(QString::number(value));
        return TRUE;
    }
    return FALSE;
}

int CxQWidget::setItemValue(QListWidgetItem* oItem, const char * value)
{
    if (! oItem) return FALSE;
    QVariant data = oItem->data(Qt::UserRole);
    if (data.type() == QVariant::Invalid || data.toByteArray() != value)
    {
        QByteArray pData(value);
        oItem->setData(Qt::UserRole, pData);
        oItem->setText(CxQString::ToString(value));
        return TRUE;
    }
    return FALSE;
}

int CxQWidget::setItemValue(QListWidgetItem *oItem, const std::string &value)
{
    return setItemValue(oItem, CxQString::gbkToQString(value));
}

int CxQWidget::setItemValue(QListWidgetItem *oItem, const QString &value)
{
    if (! oItem) return FALSE;
    QVariant data = oItem->data(Qt::UserRole);
    if (data.type() == QVariant::Invalid || data.toString() != value)
    {
        oItem->setData(Qt::UserRole, value);
        oItem->setText(value);
        return TRUE;
    }
    return FALSE;
}

void CxQWidget::setItemValueDateTime(QListWidgetItem *oItem, const long long &value)
{
    if (! oItem) return;
    QVariant data = oItem->data(Qt::UserRole);
    if (data.type() == QVariant::Invalid || data.toLongLong() != value)
    {
        oItem->setData(Qt::UserRole, value);
        oItem->setText(CxQString::ToString(value));
    }
}












int CxQWidget::setItemValue16(QStandardItem* oItem, const int & value)
{
    if (! oItem) return FALSE;
    QVariant data = oItem->data(Qt::UserRole);
    if (data.type() == QVariant::Invalid || data.toInt() != value)
    {
        oItem->setData(value, Qt::UserRole);
        oItem->setText(QString::number(value, 16));
        return TRUE;
    }
    return FALSE;
}

int CxQWidget::setItemValue(QStandardItem* oItem, const int & value)
{
    if (! oItem) return FALSE;
    QVariant data = oItem->data(Qt::UserRole);
    if (data.type() == QVariant::Invalid || data.toInt() != value)
    {
        oItem->setData(value, Qt::UserRole);
        oItem->setText(QString::number(value));
        return TRUE;
    }
    return FALSE;
}

int CxQWidget::setItemValue(QStandardItem* oItem, const double & value)
{
    if (! oItem) return FALSE;
    QVariant data = oItem->data(Qt::UserRole);
    if (data.type() == QVariant::Invalid || data.toDouble() != value)
    {
        oItem->setData(value, Qt::UserRole);
        oItem->setText(QString::number(value, 'g', 3));
        return TRUE;
    }
    return FALSE;
}

int CxQWidget::setItemValue(QStandardItem* oItem, const long long & value)
{
    if (! oItem) return FALSE;
    QVariant data = oItem->data(Qt::UserRole);
    if (data.type() == QVariant::Invalid || data.toLongLong() != value)
    {
        oItem->setData(Qt::UserRole, value);
        oItem->setText(QString::number(value));
        return TRUE;
    }
    return FALSE;
}

int CxQWidget::setItemValue(QStandardItem* oItem, const char * value)
{
    if (! oItem) return FALSE;
    QVariant data = oItem->data(Qt::UserRole);
    if (data.type() == QVariant::Invalid || data.toByteArray() != value)
    {
        QByteArray pData(value);
        oItem->setData(pData, Qt::UserRole);
        oItem->setText(CxQString::ToString(value));
        return TRUE;
    }
    return FALSE;
}

int CxQWidget::setItemValue(QStandardItem *oItem, const std::string &value)
{
    return setItemValue(oItem, CxQString::gbkToQString(value));
}

int CxQWidget::setItemValue(QStandardItem *oItem, const QString &value)
{
    QVariant data = oItem->data(Qt::UserRole);
    if (data.type() == QVariant::Invalid || data.toString() != value)
    {
        oItem->setData(value, Qt::UserRole);
        oItem->setText(value);
        return TRUE;
    }
    return FALSE;
}

int CxQWidget::setItemValueEditRole(QStandardItem *oItem, const QString &value)
{
    if (! oItem) return FALSE;
    QVariant data = oItem->data(Qt::EditRole);
    if (data.type() == QVariant::Invalid || data.toString() != value)
    {
        oItem->setData(value, Qt::EditRole);
//        oItem->setText(value);
        return TRUE;
    }
    return FALSE;
}

void CxQWidget::setItemValueDateTime(QStandardItem *oItem, const long long &value)
{
    if (! oItem) return;
    QVariant data = oItem->data(Qt::UserRole);
    if (data.type() == QVariant::Invalid || data.toLongLong() != value)
    {
        oItem->setData(Qt::UserRole, value);
        oItem->setText(CxQString::ToString(value));
        return;
    }
    return;
}

std::vector<int> CxQWidget::getSelectRows(QTableWidget *oGrid, QString * pRows)
{
    std::vector<int> r;
    if (! oGrid) return r;
    QItemSelectionModel *selections = oGrid->selectionModel();
    QModelIndexList selected = selections->selectedIndexes();

    int iRow;
    foreach (QModelIndex index, selected)
    {
        iRow = index.row();
        if (! CxContainer::contain(r, iRow))
        {
            if (pRows)
            {
                * pRows += QString::fromUtf8(" %1 ,").arg(iRow);
            }
            r.push_back(iRow);
        }
    }

    CxContainer::sort(r, false);

    return r;
}

std::vector<int> CxQWidget::deleteSelectRows(QTableWidget *oGrid)
{
    QString sRows;
    vector<int> iRows = getSelectRows(oGrid, &sRows);

    if (iRows.size()>0)
    {
        QString sMsg = QString::fromUtf8("? Do you sure delete row: \n  %1").arg(sRows);
        if (CxQDialog::ShowQuery(sMsg))
        {
            for (size_t i = 0; i < iRows.size(); ++i)
            {
                int iRow = iRows.at(i);
                oGrid->removeRow(iRow);
            }
        }
        else
        {
            iRows.clear();
        }
    }
    return iRows;
}

void CxQWidget::setQSS(QWidget *widget, const QString &fp)
{
    QFile file(fp);
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    widget->setStyleSheet(styleSheet);
}

void CxQWidget::setQSS(QWidget *widget, const string &fp)
{
    QFile file(CxQString::gbkToQString(fp));
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    widget->setStyleSheet(styleSheet);
}







