#include "cxchannel_serial.h"

using namespace std;

//CxChannelSerialFactory* CxChannelSerialFactory::s_instance =
//        CxChannelFactoryManager::createAndRegister<CxChannelSerialFactory>();


#if defined(GM_OS_WIN)
const string CSDefaultSerialPortNames[9] = {"COM1", "COM2", "COM3", "COM4", "COM5"
                                            , "COM6", "COM7", "COM8", "COM9" };
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

const int    CIDefaultSerialBaudRate     = CxChannelSerial::BaudRate_9600;
const int    CIDefaultSerialFlowControl  = CxChannelSerial::flowNone;
const int    CIDefaultSerialParity       = CxChannelSerial::parityNone;
const int    CIDefaultSerialStopBits     = CxChannelSerial::stopBitsOne;
const int    CIDefaultSerialCharacterSize= CxChannelSerial::CharacterSizeEight;












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

    DCB * attr = (DCB *)current;
    DCB * orig = (DCB *)original;

    attr->DCBlength = sizeof(DCB);
    orig->DCBlength = sizeof(DCB);

    GetCommState(dev, orig);
    GetCommState(dev, attr);

    attr->DCBlength = sizeof(DCB);
    attr->BaudRate = 9600;
    attr->ByteSize = 8;
    attr->XoffLim = 512;
    attr->XonLim = 2048;
    attr->fBinary = 1;
    attr->fTXContinueOnXoff = 1;

    SetCommState(dev, attr);

    COMMTIMEOUTS CommTimeouts;
    CommTimeouts.ReadIntervalTimeout = MAXDWORD;
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
    SetCommState(dev, (DCB *)current);
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
    if(dev == INVALID_HANDLE_VALUE && original)
        SetCommState(dev, (DCB *)original);

    if(current)
        delete (DCB *)current;

    if(original)
        delete (DCB *)original;
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

    if(ser.dev < 0)
        return *this;

#ifdef  GM_OS_WIN
    HANDLE process = GetCurrentProcess();

    int result = DuplicateHandle(process, ser.dev, process, &dev, DUPLICATE_SAME_ACCESS, 0, 0);

    if (0 == result) {
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
    dev = CreateFile(_portName.c_str(),
                     GENERIC_READ | GENERIC_WRITE,
                     0,                    // exclusive access
                     NULL,                 // no security attrs
                     OPEN_EXISTING,
                     FILE_FLAG_OVERLAPPED,
                     NULL);
    if(dev != INVALID_HANDLE_VALUE)
        initConfig();
#else
    int cflags = O_RDWR | O_NDELAY;
    dev = ::open(_portName.c_str(), cflags);
    if(dev > -1)
        initConfig();
#endif
}

#ifdef  GM_OS_WIN
int CxChannelSerial::doRead(char * Data, const int Length)
{

    unsigned long   dwLength = 0, dwError, dwReadLength;
    COMSTAT cs;
    OVERLAPPED ol;

    // Return zero if handle is invalid
    if(dev == INVALID_HANDLE_VALUE)
        return 0;

    // Read max length or only what is available
    ClearCommError(dev, &dwError, &cs);

    // If not requiring an exact byte count, get whatever is available
    if(Length > (int)cs.cbInQue)
        dwReadLength = cs.cbInQue;
    else
        dwReadLength = Length;

    memset(&ol, 0, sizeof(OVERLAPPED));
    ol.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    if(dwReadLength > 0) {
        if(ReadFile(dev, Data, dwReadLength, &dwLength, &ol) == FALSE) {
            if(GetLastError() == ERROR_IO_PENDING) {
                WaitForSingleObject(ol.hEvent, INFINITE);
                GetOverlappedResult(dev, &ol, &dwLength, TRUE);
            }
            else
                ClearCommError(dev, &dwError, &cs);
        }
    }

    if(ol.hEvent != INVALID_HANDLE_VALUE)
        CloseHandle(ol.hEvent);

    return dwLength;
}

int CxChannelSerial::doWrite(const char * Data, const int Length)
{
    COMSTAT cs;
    unsigned long dwError = 0;
    OVERLAPPED ol;

    // Clear the com port of any error condition prior to read
    ClearCommError(dev, &dwError, &cs);

    unsigned long retSize = 0;

    memset(&ol, 0, sizeof(OVERLAPPED));
    ol.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    if(WriteFile(dev, Data, Length, &retSize, &ol) == FALSE) {
        if(GetLastError() == ERROR_IO_PENDING) {
            WaitForSingleObject(ol.hEvent, INFINITE);
            GetOverlappedResult(dev, &ol, &retSize, TRUE);
        }
        else
            ClearCommError(dev, &dwError, &cs);
    }

    if(ol.hEvent != INVALID_HANDLE_VALUE)
        CloseHandle(ol.hEvent);

    return retSize;
}
#else
int CxChannelSerial::doRead(char *Data, const int Length)
{
    return ::read(dev, Data, Length);
}

int CxChannelSerial::doWrite(const char *Data, const int Length)
{
    return ::write(dev, Data, Length);
}
#endif

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

bool CxChannelSerial::doSetSpeed(unsigned long speed)
{
    unsigned long rate;

    switch(speed) {
    case 115200:
    case 57600:
    case 38400:
    case 19200:
    case 9600:
    case 4800:
    case 2400:
    case 1200:
    case 600:
    case 300:
    case 110:
    case 0:
        rate = speed;
        break;
    default:
        return false;
    }

#ifdef  GM_OS_WIN

    DCB     * dcb = (DCB *)current;
    dcb->DCBlength = sizeof(DCB);
    GetCommState(dev, dcb);

    dcb->BaudRate = rate;
    SetCommState(dev, dcb) ;

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

    DCB * attr = (DCB *)current;
    attr->XonChar = ASCII_XON;
    attr->XoffChar = ASCII_XOFF;
    attr->XonLim = 100;
    attr->XoffLim = 100;

    switch(flow) {
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

bool CxChannelSerial::doSetStopBits(int bits)
{
#ifdef  GM_OS_WIN

    DCB * attr = (DCB *)current;
    switch(bits) {
    case 1:
        attr->StopBits = ONESTOPBIT;
        break;
    case 2:
        attr->StopBits = TWOSTOPBITS;
        break;
    default:
        return false;
    }

    SetCommState(dev, attr);
#else
    struct termios *attr = (struct termios *)current;
    attr->c_cflag &= ~CSTOPB;

    switch(bits) {
    case 1:
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

bool CxChannelSerial::doSetCharBits(int bits)
{
#ifdef  GM_OS_WIN

    DCB * attr = (DCB *)current;
    switch(bits) {
    case 5:
    case 6:
    case 7:
    case 8:
        attr->ByteSize = bits;
        break;
    default:
        return false;
    }
    SetCommState(dev, attr);
#else
    struct termios *attr = (struct termios *)current;
    attr->c_cflag &= ~CSIZE;

    switch(bits) {
    case 5:
        attr->c_cflag |= CS5;
        break;
    case 6:
        attr->c_cflag |= CS6;
        break;
    case 7:
        attr->c_cflag |= CS7;
        break;
    case 8:
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

    DCB * attr = (DCB *)current;
    switch(parity) {
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
    if(millisec) {
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
    unsigned long   dwError;
    COMSTAT cs;

    ClearCommError(dev, &dwError, &cs);

    if(timeout == 0 || ((pending == pendingInput) && (0 != cs.cbInQue)) ||
            ((pending == pendingOutput) && (0 != cs.cbOutQue)) || (pending == pendingError))
    {
        switch(pending) {
        case pendingInput:
            return (0 != cs.cbInQue);
        case pendingOutput:
            return (0 != cs.cbOutQue);
        case pendingError:
            return false;
        }
    }
    else {
        OVERLAPPED ol;
        DWORD dwMask;
        DWORD dwEvents = 0;
        BOOL suc;

        memset(&ol, 0, sizeof(OVERLAPPED));
        ol.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

        if(pending == pendingInput)
            dwMask = EV_RXCHAR;
        else if(pending == pendingOutput)
            dwMask = EV_TXEMPTY;
        else   // on error
            dwMask = EV_ERR;

        SetCommMask(dev, dwMask);
        // let's wait for event or timeout
        if((suc = WaitCommEvent(dev, &dwEvents, &ol)) == FALSE) {
            if(GetLastError() == ERROR_IO_PENDING) {
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

        if(ol.hEvent != INVALID_HANDLE_VALUE)
            CloseHandle(ol.hEvent);

        if(suc == FALSE)
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

    _portName       =       CSDefaultSerialPortName         ;
    _baudRateEnum       =       CIDefaultSerialBaudRate         ;
    _flowControl       =    CIDefaultSerialFlowControl      ;
    _parity       =         CIDefaultSerialParity           ;
    _stopBits       =       CIDefaultSerialStopBits         ;
    _characterSize       =  CIDefaultSerialCharacterSize    ;

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

void CxChannelSerial::toContext(CxISetSkv& context) const
{
    CxChannelBase::toContext(context);
    context.setValue(CS_EntryPortName     , _portName        );
    context.setValue(CS_EntryBaudRate     , _baudRateEnum        );
    context.setValue(CS_EntryCharacterSize, _characterSize   );
    context.setValue(CS_EntryStopBits     , _stopBits        );
    context.setValue(CS_EntryParity       , _parity          );
    context.setValue(CS_EntryFlowControl  , _flowControl     );
}

void CxChannelSerial::fromContext(const CxIGetSkv& context)
{
    CxChannelBase::fromContext(context);
    setPortName     (context.getValue(CS_EntryPortName     , CSDefaultSerialPortName ));
    setBaudRate     (context.getValue(CS_EntryBaudRate     , CIDefaultSerialBaudRate ));
    setCharacterSize(CharacterSize (context.getValue(CS_EntryCharacterSize, CIDefaultSerialCharacterSize )));
    setStopBits     (StopBits (context.getValue(CS_EntryStopBits     , CIDefaultSerialStopBits )));
    setParity       (Parity (context.getValue(CS_EntryParity       , CIDefaultSerialParity )));
    setFlowControl  (Flow (context.getValue(CS_EntryFlowControl  , CIDefaultSerialFlowControl)));
}

void CxChannelSerial::toReport(std::vector<std::string> &sReports) const
{
    sReports.push_back(CS_EntryPortName      + "=" + _portName);
    sReports.push_back(CS_EntryBaudRate      + "=" + CxString::toString(_baudRateEnum     ));
    sReports.push_back(CS_EntryCharacterSize + "=" + CxString::toString(_characterSize));
    sReports.push_back(CS_EntryStopBits      + "=" + CxString::toString(_stopBits     ));
    sReports.push_back(CS_EntryParity        + "=" + CxString::toString(_parity       ));
    sReports.push_back(CS_EntryFlowControl   + "=" + CxString::toString(_flowControl  ));
}

void CxChannelSerial::toDescribe(CxDescribeUnit & describeUnit) const
{
    CxDescribeString*  oEntryPortNameDescribe    =describeUnit.createAndRegister<CxDescribeString>(CS_EntryPortName    );  oEntryPortNameDescribe->initValue( _portName       );
    CxDescribeEnum*    oEntryBaudRateDescribe    =describeUnit.createAndRegister<CxDescribeEnum>(CS_EntryBaudRate    );    oEntryBaudRateDescribe->initValue( _baudRateEnum   );  oEntryBaudRateDescribe->setEnumCopeFromZero(10);
    CxDescribeEnum*    oEntryDataBitsDescribe    =describeUnit.createAndRegister<CxDescribeEnum>(CS_EntryCharacterSize    );  oEntryDataBitsDescribe->initValue(_characterSize);  oEntryDataBitsDescribe->setEnumCopeFromZero(3);
    CxDescribeEnum*    oEntryStopBitsDescribe    =describeUnit.createAndRegister<CxDescribeEnum>(CS_EntryStopBits    );    oEntryStopBitsDescribe->initValue(   _stopBits     );  oEntryStopBitsDescribe->setEnumCopeFromZero(1);
    CxDescribeEnum*    oEntryParityDescribe      =describeUnit.createAndRegister<CxDescribeEnum>(CS_EntryParity      );    oEntryParityDescribe->initValue(      _parity      );  oEntryParityDescribe->setEnumCopeFromZero(2);
    CxDescribeEnum*    oEntryFlowDescribe        =describeUnit.createAndRegister<CxDescribeEnum>(CS_EntryFlowControl        );    oEntryFlowDescribe->initValue( _flowControl );  oEntryFlowDescribe->setEnumCopeFromZero(3);
}

bool CxChannelSerial::isSameChannelImpl(const std::map<std::string, std::string> &params) const
{
    std::string sDefault = "-1";
    std::string sPortName = CxContainer::value( params, CS_EntryPortName );
    int iBaudRate = CxString::fromString(CxContainer::value( params, CS_EntryBaudRate, sDefault), ci_int_minus_one);
    int iDataBits = CxString::fromString(CxContainer::value( params, CS_EntryCharacterSize, sDefault), ci_int_minus_one);
    int iStopBits = CxString::fromString(CxContainer::value( params, CS_EntryStopBits, sDefault), ci_int_minus_one);
    int iParity   = CxString::fromString(CxContainer::value( params, CS_EntryParity  , sDefault), ci_int_minus_one);
    int iFlow     = CxString::fromString(CxContainer::value( params, CS_EntryFlowControl    , sDefault), ci_int_minus_one);

    return sPortName == _portName
            && iBaudRate == _baudRateEnum
            && iDataBits == _characterSize
            && iStopBits == _stopBits
            && iParity   == _parity
            && iFlow     == _flowControl;
}

int CxChannelSerial::writeDataImpl(const char *pData, int iLength, void * oTarget)
{
#ifdef GM_OS_WIN
    unsigned long retSize = 0;
    try
    {
        if(::WriteFile(dev, pData, iLength, &retSize, &_overLapped) == FALSE)
        {
            if(GetLastError() == ERROR_IO_PENDING) {
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

void CxChannelSerial::stopAndDeleteRecieverThread()
{
    if (! _receiver) return;
    _receiverStatus = ThreadStatus_Stop;
    _receiver->waitExit();
    delete _receiver;
    _receiver = NULL;
}

void CxChannelSerial::openChannelImpl()
{
    if (!getConnectedImpl())
    {
        doOpen();
        if (getConnectedImpl())
        {
            if ( doSetSpeed(getBaudRateInter())
                 && doSetFlowControl((Flow)_flowControl)
                 && doSetParity((Parity)_parity)
                 && doSetStopBits(_stopBits + 1)
                 && doSetCharBits(getCharBitsInter()) )
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
    if(_overLapped.hEvent != INVALID_HANDLE_VALUE && _overLapped.hEvent != 0)
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
    if(hFd == INVALID_HANDLE_VALUE)
        return;

    unsigned long   dwLength = 0, dwError;
    COMSTAT cs;
    OVERLAPPED ol;

    // Read max length or only what is available
    ClearCommError(hFd, &dwError, &cs);

    memset(&ol, 0, sizeof(OVERLAPPED));
    ol.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

    * _status = ThreadStatus_Running;
    while (* _status == ThreadStatus_Running)
    {
        if(::ReadFile(hFd, _buffer, sizeof(_buffer), &dwLength, &ol) == FALSE)
        {
            if(GetLastError() == ERROR_IO_PENDING)
            {
                WaitForSingleObject(ol.hEvent, INFINITE);
                GetOverlappedResult(hFd, &ol, &dwLength, TRUE);
            }
            else
            {
                ClearCommError(hFd, &dwError, &cs);
                threadEventNotify(_channel, ChannelEvent_Receive_Error);
                break;
            }
        }
        else
        {
            if (dwLength > 0)
                threadEventNotify(_channel, ChannelEvent_Received_Data, 0, _buffer, dwLength);
            CxThread::sleep(1);
        }
    }

    if(ol.hEvent != INVALID_HANDLE_VALUE)
        CloseHandle(ol.hEvent);
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
    * _status = ThreadStatus_Exit;
    CxJoinableThread::exit();
}

