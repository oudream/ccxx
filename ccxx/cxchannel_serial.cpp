#include "cxchannel_serial.h"

using namespace std;

//CxChannelSerialFactory* CxChannelSerialFactory::s_instance =
//        CxChannelFactoryManager::createAndRegister<CxChannelSerialFactory>();


#if defined(GM_OS_WIN)
const string CSDefaultSerialPortNames[9] = {"COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "COM8", "COM9"};
#elif defined(GM_OS_IRIX)
const string CSDefaultSerialPortNames[9] = {"/dev/ttyf1", "/dev/ttyf2", "/dev/ttyf3", "/dev/ttyf4", "/dev/ttyf5"
                                            , "/dev/ttyf6", "/dev/ttyf7", "/dev/ttyf8", "/dev/ttyf9" };
#elif defined(GM_OS_HPUX)
const string CSDefaultSerialPortNames[9] = {"/dev/tty1p0", "/dev/tty2p0", "/dev/tty3p0", "/dev/tty4p0", "/dev/tty5p0"
                                            , "/dev/tty6p0", "/dev/tty7p0", "/dev/tty8p0", "/dev/tty9p0" };
#elif defined(GM_OS_SOLARIS)
const string CSDefaultSerialPortNames[9] = {"/dev/ttya", "/dev/ttyb", "/dev/ttyc", "/dev/ttyd", "/dev/ttye"
                                            , "/dev/ttyf", "/dev/ttyg", "/dev/ttyh", "/dev/ttyi" };
#elif defined(GM_OS_DIGITAL)
const string CSDefaultSerialPortNames[9] = {"/dev/tty01", "/dev/tty02", "/dev/tty03", "/dev/tty04", "/dev/tty05"
                                            , "/dev/tty06", "/dev/tty07", "/dev/tty08", "/dev/tty09" };
#elif defined(GM_OS_FREEBSD)
const string CSDefaultSerialPortNames[9] = {"/dev/ttyd1", "/dev/ttyd2", "/dev/ttyd3", "/dev/ttyd4", "/dev/ttyd5"
                                            , "/dev/ttyd6", "/dev/ttyd7", "/dev/ttyd8", "/dev/ttyd9" };
#else
const string CSDefaultSerialPortNames[9] = {"/dev/ttyS1", "/dev/ttyS2", "/dev/ttyS3", "/dev/ttyS4", "/dev/ttyS5"
                                            , "/dev/ttyS6", "/dev/ttyS7", "/dev/ttyS8", "/dev/ttyS9" };
#endif

const string CSDefaultSerialPortName = CSDefaultSerialPortNames[0];

const int CIDefaultSerialBaudRate = CxChannelSerial::BaudRate_9600;
const int CIDefaultSerialFlowControl = CxChannelSerial::flowNone;
const int CIDefaultSerialParity = CxChannelSerial::parityNone;
const int CIDefaultSerialStopBits = CxChannelSerial::stopBitsOne;
const int CIDefaultSerialCharacterSize = CxChannelSerial::CharacterSizeEight;

//PortName      = \\.\COM1
//BaudRate      = 6
//CharacterSize = 4
//StopBits      = 1
//Parity        = 0
//FlowControl   = 0










#include "cxthread.h"

#ifdef  GM_OS_WIN
#include <io.h>
#else
#include <sys/ioctl.h>
#include <termios.h>
#endif

#include <fcntl.h>
#include <cerrno>
#include <iostream>

using std::streambuf;
using std::iostream;
using std::ios;

#ifndef MAX_INPUT
#define MAX_INPUT 255
#endif

#ifndef MAX_CANON
#define MAX_CANON MAX_INPUT
#endif

#ifdef  __FreeBSD__
#undef  _PC_MAX_INPUT
#undef  _PC_MAX_CANON
#endif

#if defined(__QNX__)
#define CRTSCTS (IHFLOW | OHFLOW)
#endif

#if defined(_THR_UNIXWARE) || defined(__hpux) || defined(_AIX)
#include <sys/termiox.h>
#define CRTSCTS (CTSXON | RTSXOFF)
#endif

// IRIX

#ifndef CRTSCTS
#ifdef  CNEW_RTSCTS
#define CRTSCTS (CNEW_RTSCTS)
#endif
#endif

#if defined(CTSXON) && defined(RTSXOFF) && !defined(CRTSCTS)
#define CRTSCTS (CTSXON | RTSXOFF)
#endif

#ifndef CRTSCTS
#define CRTSCTS 0
#endif

void CxChannelSerial::initConfig(void)
{
#ifdef  GM_OS_WIN

#define ASCII_XON       0x11
#define ASCII_XOFF      0x13

    DCB *attr = (DCB *) current;
    DCB *orig = (DCB *) original;

    GetCommState(dev, orig);
    GetCommState(dev, attr);
    attr->DCBlength = sizeof(DCB);
    orig->DCBlength = sizeof(DCB);
//    attr->BaudRate = 9600;
//    attr->ByteSize = 8;
//    attr->XoffLim = 512;
//    attr->XonLim = 2048;
//    attr->fBinary = 1;
//    attr->fTXContinueOnXoff = 1;
//    SetCommState(dev, attr);

    COMMTIMEOUTS CommTimeouts;
    CommTimeouts.ReadIntervalTimeout = 100;
    CommTimeouts.ReadTotalTimeoutConstant = 0;
    CommTimeouts.ReadTotalTimeoutMultiplier = 0;
    CommTimeouts.WriteTotalTimeoutConstant = 0;
    CommTimeouts.WriteTotalTimeoutMultiplier = 0;
    ::SetCommTimeouts(dev, &CommTimeouts);

#else
    struct termios *attr = (struct termios *)current;
    struct termios *orig = (struct termios *)original;
    long ioflags = fcntl(dev, F_GETFL);

    tcgetattr(dev, (struct termios *)original);
    tcgetattr(dev, (struct termios *)current);

    attr->c_oflag = attr->c_lflag = 0;
    attr->c_cflag = CLOCAL | CREAD | HUPCL;
    attr->c_iflag = IGNBRK;

    memset(attr->c_cc, 0, sizeof(attr->c_cc));
    attr->c_cc[VMIN] = 1;

    // inherit original settings, maybe we should keep more??
    cfsetispeed(attr, cfgetispeed(orig));
    cfsetospeed(attr, cfgetospeed(orig));
    attr->c_cflag |= orig->c_cflag & (CRTSCTS | CSIZE | PARENB | PARODD | CSTOPB);
    attr->c_iflag |= orig->c_iflag & (IXON | IXANY | IXOFF);

    tcsetattr(dev, TCSANOW, attr);
    fcntl(dev, F_SETFL, ioflags & ~O_NDELAY);

#if defined(TIOCM_RTS) && defined(TIOCMODG)
    int mcs = 0;
    ioctl(dev, TIOCMODG, &mcs);
    mcs |= TIOCM_RTS;
    ioctl(dev, TIOCMODS, &mcs);
#endif

#ifdef  _COHERENT
    ioctl(dev, TIOCSRTS, 0);
#endif

#endif  // WIN32
}

void CxChannelSerial::restore(void)
{
#ifdef  GM_OS_WIN
    memcpy(current, original, sizeof(DCB));
    SetCommState(dev, (DCB *) current);
#else
    memcpy(current, original, sizeof(struct termios));
    tcsetattr(dev, TCSANOW, (struct termios *)current);
#endif
}

void CxChannelSerial::initSerial(void)
{
    dev = INVALID_HANDLE_VALUE;
#ifdef  GM_OS_WIN
    current = new DCB;
    original = new DCB;
#else
    current = new struct termios;
    original = new struct termios;
#endif
}

void CxChannelSerial::endSerial(void)
{
#ifdef  GM_OS_WIN
    if (dev == INVALID_HANDLE_VALUE && original)
        SetCommState(dev, (DCB *) original);

    if (current)
        delete (DCB *) current;

    if (original)
        delete (DCB *) original;
#else
    if(dev < 0 && original)
        tcsetattr(dev, TCSANOW, (struct termios *)original);

    if(current)
        delete (struct termios *)current;

        if(original)
            delete (struct termios *)original;
#endif
    CxChannelSerial::doClose();

    current = NULL;
    original = NULL;
}

int CxChannelSerial::setPacketInput(int size, unsigned char btimer)
{
#ifdef  GM_OS_WIN
    //  Still to be done......
    return 0;
#else

#ifdef  _PC_MAX_INPUT
    int max = fpathconf(dev, _PC_MAX_INPUT);
#else
    int max = MAX_INPUT;
#endif
    struct termios *attr = (struct termios *)current;

    if(size > max)
        size = max;

    attr->c_cc[VEOL] = attr->c_cc[VEOL2] = 0;
    attr->c_cc[VMIN] = (unsigned char)size;
    attr->c_cc[VTIME] = btimer;
    attr->c_lflag &= ~ICANON;
    tcsetattr(dev, TCSANOW, attr);
    return size;
#endif
}

int CxChannelSerial::setLineInput(char newline, char nl1)
{
#ifdef  GM_OS_WIN
    //  Still to be done......
    return 0;
#else

    struct termios *attr = (struct termios *)current;
    attr->c_cc[VMIN] = attr->c_cc[VTIME] = 0;
    attr->c_cc[VEOL] = newline;
    attr->c_cc[VEOL2] = nl1;
    attr->c_lflag |= ICANON;
    tcsetattr(dev, TCSANOW, attr);
    int bufsize;
#ifdef _PC_MAX_CANON
    bufsize = fpathconf(dev, _PC_MAX_CANON);
#else
    bufsize = MAX_CANON;
#endif
    return bufsize;
#endif
}

void CxChannelSerial::flushInput(void)
{
#ifdef  GM_OS_WIN
    PurgeComm(dev, PURGE_RXABORT | PURGE_RXCLEAR);
#else
    tcflush(dev, TCIFLUSH);
#endif
}

void CxChannelSerial::flushOutput(void)
{
#ifdef  GM_OS_WIN
    PurgeComm(dev, PURGE_TXABORT | PURGE_TXCLEAR);
#else
    tcflush(dev, TCOFLUSH);
#endif
}

void CxChannelSerial::waitOutput(void)
{
#ifdef  GM_OS_WIN

#else
    tcdrain(dev);
#endif
}

CxChannelSerial &CxChannelSerial::operator=(const CxChannelSerial &ser)
{
    CxChannelSerial::doClose();

    if (ser.dev < 0)
        return *this;

#ifdef  GM_OS_WIN
    HANDLE process = GetCurrentProcess();

    int result = DuplicateHandle(process, ser.dev, process, &dev, DUPLICATE_SAME_ACCESS, 0, 0);

    if (0 == result)
    {
        memcpy(current, ser.current, sizeof(DCB));
        memcpy(original, ser.original, sizeof(DCB));
    }
#else
    dev = dup(ser.dev);

    memcpy(current, ser.current, sizeof(struct termios));
    memcpy(original, ser.original, sizeof(struct termios));
#endif

    //needtodo
    return *this;
}

void CxChannelSerial::doOpen()
{
#ifdef GM_OS_WIN
    // open COMM device
// https://stackoverflow.com/questions/27909666/createfile-serial-communication-issue
// CreateFile() is successful when you use "COM1" through "COM9" for the name of the file;
// however, the message INVALID_HANDLE_VALUE is returned if you use "COM10" or greater.
//    string sDev = "\\\\.\\" + _portName;
//    dev = CreateFile(sDev.c_str(),
    string sDev = (_portName.find("\\\\.\\") == string::npos) ? "\\\\.\\" + _portName : _portName;
    dev = CreateFile(sDev.c_str(),
                     GENERIC_READ | GENERIC_WRITE,
                     0,                    // exclusive access
                     NULL,                 // no security attrs
                     OPEN_EXISTING,
                     FILE_FLAG_OVERLAPPED,
                     NULL);

    if (dev == INVALID_HANDLE_VALUE)
    {
        cxPrompt() << "Serial Fail: CreateFile, dev= " << sDev;
        return;
    }

    if (GetFileType(dev) != FILE_TYPE_CHAR)
    {
        CloseHandle(dev);
        cxPrompt() << "Serial Fail: File handle is not a comm handle.";
        return;
    }

    if (!SetupComm(dev, 4096, 4096))
    {
        CloseHandle(dev);
        cxPrompt() << "Serial Fail: SetupComm( dev, 4096, 4096 ).";
        return;
    }

    // purge any information in the buffer
    PurgeComm(dev, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

    initConfig();
#else
    int cflags = O_RDWR | O_NDELAY;
    dev = ::open(_portName.c_str(), cflags);
    if(dev <= -1)
    {
        cxPrompt() << 'Serial Fail: open, dev= ' << _portName;
        return;
    }
    initConfig();
#endif
}

void CxChannelSerial::doClose()
{
    if (dev != INVALID_HANDLE_VALUE)
    {
#ifdef  GM_OS_WIN
        CloseHandle(dev);
#else
        ::close(dev);
#endif
        dev = INVALID_HANDLE_VALUE;
    }
}

bool CxChannelSerial::doSetSpeed(BaudRateEnum baudRate)
{
    unsigned long rate;
    switch (baudRate)
    {
        case BaudRate_256000:
            rate = 256000;
            break;
        case BaudRate_128000:
            rate = 128000;
            break;
        case BaudRate_115200:
            rate = 115200;
            break;
        case BaudRate_57600:
            rate = 57600;
            break;
        case BaudRate_56000:
            rate = 56000;
            break;
        case BaudRate_38400:
            rate = 38400;
            break;
        case BaudRate_19200:
            rate = 19200;
            break;
        case BaudRate_14400:
            rate = 14400;
            break;
        case BaudRate_9600:
            rate = 9600;
            break;
        case BaudRate_4800:
            rate = 4800;
            break;
        case BaudRate_2400:
            rate = 2400;
            break;
        case BaudRate_1200:
            rate = 1200;
            break;
        case BaudRate_600:
            rate = 600;
            break;
        case BaudRate_300:
            rate = 300;
            break;
        case BaudRate_110:
            rate = 110;
            break;
        default:
            return false;
    }
#ifdef  GM_OS_WIN
    DCB *dcb = (DCB *) current;
    dcb->DCBlength = sizeof(DCB);
    dcb->BaudRate = rate;
    SetCommState(dev, dcb);
#else
    struct termios *attr = (struct termios *)current;
    cfsetispeed(attr, rate);
    cfsetospeed(attr, rate);
    tcsetattr(dev, TCSANOW, attr);
#endif
    return true;
}

bool CxChannelSerial::doSetFlowControl(Flow flow)
{
#ifdef  GM_OS_WIN
    DCB *attr = (DCB *) current;
    switch (flow)
    {
        case flowSoft:
            attr->fInX = attr->fOutX = 1;
            break;
        case flowBoth:
            attr->fInX = attr->fOutX = 1;
        case flowHard:
            attr->fOutxCtsFlow = 1;
            attr->fRtsControl = RTS_CONTROL_HANDSHAKE;
            break;
        case flowNone:
            break;
        default:
            return false;
    }
    attr->XonChar = ASCII_XON;
    attr->XoffChar = ASCII_XOFF;
    attr->XonLim = 100;
    attr->XoffLim = 100;
    SetCommState(dev, attr);
#else
    struct termios *attr = (struct termios *)current;

    attr->c_cflag &= ~CRTSCTS;
    attr->c_iflag &= ~(IXON | IXANY | IXOFF);

    switch(flow) {
    case flowSoft:
        attr->c_iflag |= (IXON | IXANY | IXOFF);
        break;
    case flowBoth:
        attr->c_iflag |= (IXON | IXANY | IXOFF);
    case flowHard:
        attr->c_cflag |= CRTSCTS;
        break;
    case flowNone:
        break;
    default:
        return false;
    }

    tcsetattr(dev, TCSANOW, attr);
#endif
    return true;
}

bool CxChannelSerial::doSetStopBits(StopBits bits)
{
#ifdef  GM_OS_WIN
    DCB *attr = (DCB *) current;
    switch (bits)
    {
        case stopBitsOne:
            attr->StopBits = ONESTOPBIT;
            break;
        case stopBuitOne5:
            attr->StopBits = ONE5STOPBITS;
            break;
        case StopBitsTwo:
            attr->StopBits = TWOSTOPBITS;
            break;
        default:
            return false;
    }
    SetCommState(dev, attr);
#else
    struct termios *attr = (struct termios *) current;

    switch (bits)
    {
        case stopBitsOne:
        case stopBuitOne5:
            attr->c_cflag &= ~CSTOPB;
            break;
        case 2:
            attr->c_cflag |= CSTOPB;
            break;
        default:
            return false;
    }
    tcsetattr(dev, TCSANOW, attr);
#endif
    return true;
}

bool CxChannelSerial::doSetCharBits(CharacterSize bits)
{
#ifdef  GM_OS_WIN
    DCB *attr = (DCB *) current;
    switch (bits)
    {
        case CharacterSizeFive:
            attr->ByteSize = 5;
            break;
        case CharacterSizeSix:
            attr->ByteSize = 6;
            break;
        case CharacterSizeSeven:
            attr->ByteSize = 7;
            break;
        case CharacterSizeEight:
            attr->ByteSize = 8;
            break;
        default:
            return false;
    }
    SetCommState(dev, attr);
#else
    struct termios *attr = (struct termios *)current;
    attr->c_cflag &= ~CSIZE;

    switch(bits) {
    case CharacterSizeFive:
        attr->c_cflag |= CS5;
        break;
    case CharacterSizeSix:
        attr->c_cflag |= CS6;
        break;
    case CharacterSizeSeven:
        attr->c_cflag |= CS7;
        break;
    case CharacterSizeEight:
        attr->c_cflag |= CS8;
        break;
    default:
        return false;
    }
    tcsetattr(dev, TCSANOW, attr);
#endif
    return true;
}

bool CxChannelSerial::doSetParity(Parity parity)
{
#ifdef  GM_OS_WIN
    DCB *attr = (DCB *) current;
    switch (parity)
    {
        case parityEven:
            attr->Parity = EVENPARITY;
            break;
        case parityOdd:
            attr->Parity = ODDPARITY;
            break;
        case parityNone:
            attr->Parity = NOPARITY;
            break;
        default:
            return false;
    }
    SetCommState(dev, attr);
#else
    struct termios *attr = (struct termios *)current;
    attr->c_cflag &= ~(PARENB | PARODD);

    switch(parity) {
    case parityEven:
        attr->c_cflag |= PARENB;
        break;
    case parityOdd:
        attr->c_cflag |= (PARENB | PARODD);
        break;
    case parityNone:
        break;
    default:
        return false;
    }
    tcsetattr(dev, TCSANOW, attr);
#endif
    return true;
}

void CxChannelSerial::sendBreak(void)
{
#ifdef  GM_OS_WIN
    SetCommBreak(dev);
    CxThread::sleep(100L);
    ClearCommBreak(dev);
#else
    tcsendbreak(dev, 0);
#endif
}

void CxChannelSerial::toggleDTR(timems_t millisec)
{
#ifdef  GM_OS_WIN
    EscapeCommFunction(dev, CLRDTR);
    if (millisec)
    {
        CxThread::sleep(millisec);
        EscapeCommFunction(dev, SETDTR);
    }

#else
    struct termios tty, old;
    tcgetattr(dev, &tty);
    tcgetattr(dev, &old);
    cfsetospeed(&tty, B0);
    cfsetispeed(&tty, B0);
    tcsetattr(dev, TCSANOW, &tty);

    if(millisec) {
        CxThread::sleep(millisec);
        tcsetattr(dev, TCSANOW, &old);
    }
#endif
}

bool CxChannelSerial::isPending(Pending pending, timems_t timeout)
{
#ifdef  GM_OS_WIN
    unsigned long dwError;
    COMSTAT cs;

    ClearCommError(dev, &dwError, &cs);

    if (timeout == 0 || ((pending == pendingInput) && (0 != cs.cbInQue)) ||
        ((pending == pendingOutput) && (0 != cs.cbOutQue)) || (pending == pendingError))
    {
        switch (pending)
        {
            case pendingInput:
                return (0 != cs.cbInQue);
            case pendingOutput:
                return (0 != cs.cbOutQue);
            case pendingError:
                return false;
        }
    }
    else
    {
        OVERLAPPED ol;
        DWORD dwMask;
        DWORD dwEvents = 0;
        BOOL suc;

        memset(&ol, 0, sizeof(OVERLAPPED));
        ol.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

        if (pending == pendingInput)
            dwMask = EV_RXCHAR;
        else if (pending == pendingOutput)
            dwMask = EV_TXEMPTY;
        else   // on error
            dwMask = EV_ERR;

        SetCommMask(dev, dwMask);
        // let's wait for event or timeout
        if ((suc = WaitCommEvent(dev, &dwEvents, &ol)) == FALSE)
        {
            if (GetLastError() == ERROR_IO_PENDING)
            {
                DWORD transferred;

                dwError = WaitForSingleObject(ol.hEvent, timeout);
                if (dwError != WAIT_OBJECT_0)
                    SetCommMask(dev, 0);

                suc = GetOverlappedResult(dev, &ol, &transferred, TRUE);
                if (suc)
                    suc = (dwEvents & dwMask) ? TRUE : FALSE;
            }
            else
                ClearCommError(dev, &dwError, &cs);
        }

        if (ol.hEvent != INVALID_HANDLE_VALUE)
            CloseHandle(ol.hEvent);

        if (suc == FALSE)
            return false;
        return true;
    }

#else
    int status = 0;
#ifdef HAVE_POLL
    struct pollfd pfd;

    pfd.fd = dev;
    pfd.revents = 0;
    switch(pending) {
    case pendingInput:
        pfd.events = POLLIN;
        break;
    case pendingOutput:
        pfd.events = POLLOUT;
        break;
    case pendingError:
        pfd.events = POLLERR | POLLHUP;
        break;
    }

    status = 0;
    while(status < 1) {
        if(timeout == TIMEOUT_INF)
            status = poll(&pfd, 1, -1);
        else
            status = poll(&pfd, 1, timeout);

        if(status < 1) {
            if(status == -1 && errno == EINTR)
                continue;
            return false;
        }
    }

    if(pfd.revents & pfd.events)
        return true;

#else
    struct timeval tv;
    fd_set grp;
    struct timeval *tvp = &tv;

    if(timeout == CxGlobal::inf)
        tvp = NULL;
    else {
        tv.tv_usec = (timeout % 1000) * 1000;
        tv.tv_sec = timeout / 1000;
    }

    FD_ZERO(&grp);
    FD_SET(dev, &grp);
    switch(pending) {
    case pendingInput:
        status = select(dev + 1, &grp, NULL, NULL, tvp);
        break;
    case pendingOutput:
        status = select(dev + 1, NULL, &grp, NULL, tvp);
        break;
    case pendingError:
        status = select(dev + 1, NULL, NULL, &grp, tvp);
        break;
    }
    if(status < 1)
        return false;

    if(FD_ISSET(dev, &grp))
        return true;

#endif

#endif  //  WIN32

    return false;
}

CxChannelSerial::CxChannelSerial()
{
    initSerial();

    _portName = CSDefaultSerialPortName;
    _baudRateEnum = CIDefaultSerialBaudRate;
    _flowControlEnum = CIDefaultSerialFlowControl;
    _parityEnum = CIDefaultSerialParity;
    _stopBitsEnum = CIDefaultSerialStopBits;
    _characterSizeEnum = CIDefaultSerialCharacterSize;

    _receiver = NULL;
    _receiverStatus = ThreadStatus_Creating;

#ifdef GM_OS_WIN
    memset(&_overLapped, 0, sizeof(OVERLAPPED));
#endif
}

CxChannelSerial::~CxChannelSerial()
{
    close();
    endSerial();
}

void CxChannelSerial::toContext(CxISetSkv &context) const
{
    CxChannelBase::toContext(context);
    context.setValue(CS_EntryPortName, _portName);
    context.setValue(CS_EntryBaudRate, _baudRateEnum);
    context.setValue(CS_EntryCharacterSize, _characterSizeEnum);
    context.setValue(CS_EntryStopBits, _stopBitsEnum);
    context.setValue(CS_EntryParity, _parityEnum);
    context.setValue(CS_EntryFlowControl, _flowControlEnum);
}

void CxChannelSerial::fromContext(const CxIGetSkv &context)
{
    CxChannelBase::fromContext(context);
    setPortName(context.getValue(CS_EntryPortName, CSDefaultSerialPortName));
    setBaudRate(BaudRateEnum(context.getValue(CS_EntryBaudRate, CIDefaultSerialBaudRate)));
    setCharacterSize(CharacterSize(context.getValue(CS_EntryCharacterSize, CIDefaultSerialCharacterSize)));
    setStopBits(StopBits(context.getValue(CS_EntryStopBits, CIDefaultSerialStopBits)));
    setParity(Parity(context.getValue(CS_EntryParity, CIDefaultSerialParity)));
    setFlowControl(Flow(context.getValue(CS_EntryFlowControl, CIDefaultSerialFlowControl)));
}

void CxChannelSerial::toReport(std::vector<std::string> &sReports) const
{
    sReports.push_back(CS_EntryPortName + "=" + _portName);
    sReports.push_back(CS_EntryBaudRate + "=" + CxString::toString(_baudRateEnum));
    sReports.push_back(CS_EntryCharacterSize + "=" + CxString::toString(_characterSizeEnum));
    sReports.push_back(CS_EntryStopBits + "=" + CxString::toString(_stopBitsEnum));
    sReports.push_back(CS_EntryParity + "=" + CxString::toString(_parityEnum));
    sReports.push_back(CS_EntryFlowControl + "=" + CxString::toString(_flowControlEnum));
}

void CxChannelSerial::toDescribe(CxDescribeUnit &describeUnit) const
{
    CxDescribeString *oEntryPortNameDescribe = describeUnit.createAndRegister<CxDescribeString>(CS_EntryPortName);
    oEntryPortNameDescribe->initValue(_portName);
    CxDescribeEnum *oEntryBaudRateDescribe = describeUnit.createAndRegister<CxDescribeEnum>(CS_EntryBaudRate);
    oEntryBaudRateDescribe->initValue(_baudRateEnum);
    oEntryBaudRateDescribe->setEnumCopeFromZero(10);
    CxDescribeEnum *oEntryDataBitsDescribe = describeUnit.createAndRegister<CxDescribeEnum>(CS_EntryCharacterSize);
    oEntryDataBitsDescribe->initValue(_characterSizeEnum);
    oEntryDataBitsDescribe->setEnumCopeFromZero(3);
    CxDescribeEnum *oEntryStopBitsDescribe = describeUnit.createAndRegister<CxDescribeEnum>(CS_EntryStopBits);
    oEntryStopBitsDescribe->initValue(_stopBitsEnum);
    oEntryStopBitsDescribe->setEnumCopeFromZero(1);
    CxDescribeEnum *oEntryParityDescribe = describeUnit.createAndRegister<CxDescribeEnum>(CS_EntryParity);
    oEntryParityDescribe->initValue(_parityEnum);
    oEntryParityDescribe->setEnumCopeFromZero(2);
    CxDescribeEnum *oEntryFlowDescribe = describeUnit.createAndRegister<CxDescribeEnum>(CS_EntryFlowControl);
    oEntryFlowDescribe->initValue(_flowControlEnum);
    oEntryFlowDescribe->setEnumCopeFromZero(3);
}

bool CxChannelSerial::isSameChannelImpl(const std::map<std::string, std::string> &params) const
{
    std::string sDefault = "-1";
    std::string sPortName = CxContainer::value(params, CS_EntryPortName);
    int iBaudRate = CxString::fromString(CxContainer::value(params, CS_EntryBaudRate, sDefault), ci_int_minus_one);
    int iDataBits = CxString::fromString(CxContainer::value(params, CS_EntryCharacterSize, sDefault), ci_int_minus_one);
    int iStopBits = CxString::fromString(CxContainer::value(params, CS_EntryStopBits, sDefault), ci_int_minus_one);
    int iParity = CxString::fromString(CxContainer::value(params, CS_EntryParity, sDefault), ci_int_minus_one);
    int iFlow = CxString::fromString(CxContainer::value(params, CS_EntryFlowControl, sDefault), ci_int_minus_one);

    return sPortName == _portName
           && iBaudRate == _baudRateEnum
           && iDataBits == _characterSizeEnum
           && iStopBits == _stopBitsEnum
           && iParity == _parityEnum
           && iFlow == _flowControlEnum;
}

void CxChannelSerial::stopAndDeleteRecieverThread()
{
    if (!_receiver) return;
    _receiverStatus = ThreadStatus_Stop;
    _receiver->waitExit();
    delete _receiver;
    _receiver = NULL;
}

int CxChannelSerial::writeDataImpl(const char *pData, int iLength, void *oTarget)
{
#ifdef GM_OS_WIN
    unsigned long retSize = 0;
    try
    {
        if (::WriteFile(dev, pData, iLength, &retSize, &_overLapped) == FALSE)
        {
            if (GetLastError() == ERROR_IO_PENDING)
            {
                WaitForSingleObject(_overLapped.hEvent, INFINITE);
                GetOverlappedResult(dev, &_overLapped, &retSize, TRUE);
            }
            else
            {
                COMSTAT comStat;
                unsigned long dwError = 0;
                // Clear the com port of any error condition prior to read
                ::ClearCommError(dev, &dwError, &comStat);
                threadEventNotify(this, ChannelEvent_Send_Error);
            }
        }
    }
    catch (...)
    {

    }
    return retSize;
#else
    ssize_t r = ::write(dev, pData, iLength);
    if (r < 1 || r > iLength)
    {
        if (r != EAGAIN)
        {
            threadEventNotify(this, ChannelEvent_Send_Error);
        }
    }
    return r;
#endif
}

void CxChannelSerial::openChannelImpl()
{
    if (!getConnectedImpl())
    {
        doOpen();
        if (getConnectedImpl())
        {
            if (doSetSpeed((BaudRateEnum) _baudRateEnum)
                && doSetFlowControl((Flow) _flowControlEnum)
                && doSetParity((Parity) _parityEnum)
                && doSetStopBits((StopBits) _stopBitsEnum)
                && doSetCharBits((CharacterSize) _characterSizeEnum))
            {
                //createAndStartReceiverThread
                stopAndDeleteRecieverThread();
                _receiver = new ReceiverThread();
                _receiver->setStatus(&_receiverStatus);
                _receiver->setChannel(this);
                _receiverStatus = ThreadStatus_Start;
                _receiver->start();

                //createAndStartSenderThread
#ifdef GM_OS_WIN
                COMSTAT comStat;
                unsigned long dwError = 0;
                // Clear the com port of any error condition prior to read
                ::ClearCommError(dev, &dwError, &comStat);
                memset(&_overLapped, 0, sizeof(OVERLAPPED));
                _overLapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
#endif
            }
            else
            {
                doClose();
            }
        }
    }
}

void CxChannelSerial::closeChannelImpl()
{
    doClose();
    stopAndDeleteRecieverThread();
#ifdef GM_OS_WIN
    if (_overLapped.hEvent != INVALID_HANDLE_VALUE && _overLapped.hEvent != 0)
    {
        ::CloseHandle(_overLapped.hEvent);
        memset(&_overLapped, 0, sizeof(OVERLAPPED));
    }
#endif
}

bool CxChannelSerial::getConnectedImpl() const
{
#ifdef  GM_OS_WIN
    return dev != INVALID_HANDLE_VALUE;
#else
    return dev > 0 && isatty(dev);
#endif
}

#ifdef  GM_OS_WIN
void CxChannelSerial::ReceiverThread::run()
{
    fd_t hFd = _channel->dev;
    // Return zero if handle is invalid
    if (hFd == INVALID_HANDLE_VALUE)
        return;

    unsigned long dwError;
    COMSTAT cs;
    // Read max length or only what is available
    ClearCommError(hFd, &dwError, &cs);

    DWORD dwRead;
    BOOL fWaitingOnRead = FALSE;
    OVERLAPPED osReader = {0};

// Create the overlapped event. Must be closed before exiting
// to avoid a handle leak.
    osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (osReader.hEvent == NULL)
    {
        // Error creating overlapped event; abort.
        cxPrompt() << "Serial Fail: Error creating overlapped event; abort.";
        threadEventNotify(_channel, ChannelEvent_Receive_Error);
        return;
    }

    *_status = ThreadStatus_Running;
    while (*_status == ThreadStatus_Running)
    {
        if (!ReadFile(hFd, _buffer, sizeof(_buffer), &dwRead, &osReader))
        {
            if (GetLastError() != ERROR_IO_PENDING) // read not delayed?
            {
                ClearCommError(hFd, &dwError, &cs);
                cxPrompt() << "Serial Fail: ReadFile Error 1. " << dwError;
                threadEventNotify(_channel, ChannelEvent_Receive_Error);
                break;
            } // Error in communications; report it.
            else
            {
                fWaitingOnRead = TRUE;
            }
        }
        else
        {
            if (dwRead>0) // read completed immediately
            {
                threadEventNotify(_channel, ChannelEvent_Received_Data, 0, _buffer, dwRead);
            }
        }

        DWORD dwRes;

        if (fWaitingOnRead)
        {
            dwRes = WaitForSingleObject(osReader.hEvent, 500); //READ_TIMEOUT : 500
            switch(dwRes)
            {
                // Read completed.
                case WAIT_OBJECT_0:
                    if (!GetOverlappedResult(hFd, &osReader, &dwRead, FALSE))  // Error in communications; report it.
                    {
                        ClearCommError(hFd, &dwError, &cs);
                        cxPrompt() << "Serial Fail: ReadFile Error 2. " << dwError;
                        threadEventNotify(_channel, ChannelEvent_Receive_Error);
                    }
                    else  // Read completed successfully.
                    if (dwRead>0)
                    {
                        threadEventNotify(_channel, ChannelEvent_Received_Data, 0, _buffer, dwRead);
                    }

                    //  Reset flag so that another opertion can be issued.
                    fWaitingOnRead = FALSE;
                    break;

                case WAIT_TIMEOUT:
                    // Operation isn't complete yet. fWaitingOnRead flag isn't
                    // changed since I'll loop back around, and I don't want
                    // to issue another read until the first one finishes.
                    //
                    // This is a good time to do some background work.
                    break;

                default:
                    // Error in the WaitForSingleObject; abort.
                    // This indicates a problem with the OVERLAPPED structure's
                    // event handle.
                    break;
            }
        }
    }

    if (osReader.hEvent != INVALID_HANDLE_VALUE)
        CloseHandle(osReader.hEvent);
}

#else

void CxChannelSerial::ReceiverThread::run()
{
    fd_t hFd = _channel->dev;
    // Return zero if handle is invalid
    if(hFd == INVALID_HANDLE_VALUE)
        return;

    * _status = ThreadStatus_Running;
    while (* _status == ThreadStatus_Running)
    {
        ssize_t iSize = ::read(hFd, _buffer, sizeof(_buffer));
        if (iSize > 0)
        {
            threadEventNotify(_channel, ChannelEvent_Received_Data, 0, _buffer, iSize);
        }
        else if (0 == iSize)
        {
        }
        else
        {
            int iErrorCode = errno;
            if (iErrorCode != EAGAIN)
            {
                threadEventNotify(_channel, ChannelEvent_Receive_Error);
            }
        }
    }
}

#endif

void CxChannelSerial::ReceiverThread::exit()
{
    *_status = ThreadStatus_Exit;
    CxJoinableThread::exit();
}

