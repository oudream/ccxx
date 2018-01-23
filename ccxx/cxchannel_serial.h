#ifndef CXCHANNEL_SERIAL_H
#define CXCHANNEL_SERIAL_H

#include "cxchannel.h"


class SerialPrivate;

class GM_CCXX_CORE_API CxChannelSerial : public CxChannelBase
{
public:
    enum BaudRateEnum {
        BaudRate_110,
        BaudRate_300,
        BaudRate_600,
        BaudRate_1200,
        BaudRate_2400,
        BaudRate_4800,
        BaudRate_9600,
        BaudRate_14400,
        BaudRate_19200,
        BaudRate_38400,
        BaudRate_56000,
        BaudRate_57600,
        BaudRate_115200,
        BaudRate_128000,
        BaudRate_256000
    };
    typedef enum BaudRateEnum BaudRateEnum;

    enum Flow {
        flowNone,
        flowSoft,
        flowHard,
        flowBoth
    };
    typedef enum Flow Flow;

    enum Parity {
        parityNone,
        parityOdd,
        parityEven
    };
    typedef enum Parity Parity;

    enum StopBits {
        stopBitsOne,
        StopBitsTwo
    };
    typedef enum StopBits StopBits;

    enum CharacterSize {
        CharacterSizeFive,
        CharacterSizeSix,
        CharacterSizeSeven,
        CharacterSizeEight
    };
    typedef enum CharacterSize CharacterSize;

    enum Pending {
        pendingInput,
        pendingOutput,
        pendingError
    };
    typedef enum Pending Pending;

protected:
    void    *   original;
    void    *   current;
    fd_t    dev;

    /**
     * Used to properly initialize serial object.
     */
    void initSerial(void);

    /**
     * unix only
     * Set packet read mode and "size" of packet read buffer.
     * This sets VMIN to x.  VTIM is normally set to "0" so that
     * "isPending()" can wait for an entire packet rather than just
     * the first byte.
     *
     * @return actual buffer size set.
     * @param size of packet read request.
     * @param btimer optional inter-byte data packet timeout.
     */
    int setPacketInput(int size, unsigned char btimer = 0);

    /**
     * unix only
     * Set "line buffering" read mode and specifies the newline
     * character to be used in seperating line records.  isPending
     * can then be used to wait for an entire line of input.
     *
     * @param newline newline character.
     * @param nl1 EOL2 control character.
     * @return size of conical input buffer.
     */
    int setLineInput(char newline = 13, char nl1 = 0);

    /**
     * Restore serial device to the original settings at time of open.
     */
    void restore(void);

    /**
     * Used to flush the input waiting queue.
     */
    void flushInput(void);

    /**
     * Used to flush any pending output data.
     */
    void flushOutput(void);

    /**
     * unix only
     * Used to wait until all output has been sent.
     */
    void waitOutput(void);

    /**
     * Used as the default destructor for ending serial I/O
     * services.  It will restore the port to it's original state.
     */
    void endSerial(void);

    /**
     * Used to initialize a newly opened serial file handle.  You
     * should set serial properties and DTR manually before first
     * use.
     */
    void initConfig(void);

    /**
     * CxChannelSerial ports may also be duplecated by the assignment
     * operator.
     */
    CxChannelSerial &operator=(const CxChannelSerial &from);

    /**
     * Set serial port speed for both input and output.
     *
     * @return 0 on success.
     * @param speed to select. 0 signifies modem "hang up".
     */
    bool doSetSpeed(unsigned long speed);

    /**
     * Set character size.
     *
     * @return 0 on success.
     * @param bits character size to use (usually 7 or 8).
     */
    bool doSetCharBits(int bits);

    /**
     * Set parity mode.
     *
     * @return 0 on success.
     * @param parity mode.
     */
    bool doSetParity(Parity parity);

    /**
     * Set number of stop bits.
     *
     * @return 0 on success.
     * @param bits stop bits.
     */
    bool doSetStopBits(int bits);

    /**
     * Set flow control.
     *
     * @return 0 on success.
     * @param flow control mode.
     */
    bool doSetFlowControl(Flow flow);

    /**
     * Set the DTR mode off momentarily.
     *
     * @param millisec number of milliseconds.
     */
    void toggleDTR(timems_t millisec);

    /**
     * Send the "break" signal.
     */
    void sendBreak(void);

    /**
     * Opens the serial device.
     *
     * @param fname Pathname of device to open
     */
    void doOpen();

    /**
     * Closes the serial device.
     *
     */
    void doClose(void);

    /**
     * Reads from serial device.
     *
     * @param Data  Point to character buffer to receive data.  Buffers MUST
     *              be at least Length + 1 bytes in size.
     * @param Length Number of bytes to read.
     */
    virtual int doRead(char * Data, const int Length);

    /**
     * Writes to serial device.
     *
     * @param Data  Point to character buffer containing data to write.  Buffers MUST
     * @param Length Number of bytes to write.
     */
    virtual int doWrite(const char * Data, const int Length);

    /**
     * Get the status of pending operations.  This can be used to
     * examine if input or output is waiting, or if an error has
     * occured on the serial device.
     *
     * @return true if ready, false if timeout.
     * @param pend ready check to perform.
     * @param timeout in milliseconds.
     */
    virtual bool isPending(Pending pend, timems_t timeout = CxGlobal::inf);

public:

    CxChannelSerial();

    ~CxChannelSerial();

    inline const std::string& typeSimpleName() const { return CS_TypeSimpleName_Serial; }

    inline int typeSimpleId() const { return CxInterinfo::Type_IO_Serial; }

    void toContext(CxISetSkv& context) const ;

    void fromContext(const CxIGetSkv& context);

    inline std::string portName() const { return _portName; }
    inline void setPortName(const std::string &value) {
        if (! connected())
        {
            _portName = value;
            _localName = CS_TypeSimpleName_Serial + "." + _portName;
        }
    }

    inline ushort baudRate(int mode=1) const {
        if(mode>0)return _baudRateEnum;
        else   return _baudRate;
    }
    inline void setBaudRate(ushort value,int mode=1) {
        if (! connected())
        {
            if(mode>0)_baudRateEnum = value;
            else _baudRate = value;
        }
    }

    inline Flow flowControl() const { return (Flow)_flowControl; }
    inline void setFlowControl(Flow value) {
        if (! connected())
        {
            _flowControl = value;
        }
    }

    inline Parity parity() const { return (Parity)_parity; }
    inline void setParity(Parity value) {
        if (! connected())
        {
            _parity = value;
        }
    }

    inline StopBits stopBits() const { return (StopBits)_stopBits; }
    inline void setStopBits(StopBits value) {
        if (! connected())
        {
            _stopBits = value;
        }
    }

    inline CharacterSize characterSize() const { return (CharacterSize)_characterSize; }
    inline void setCharacterSize(CharacterSize value) {
        if (! connected())
        {
            _characterSize = value;
        }
    }

protected:
    ///----------Abstruct Realization --- begin ---
    bool getConnectedImpl() const;

    void openChannelImpl();

    void closeChannelImpl();

    int writeDataImpl(const char* pData, int iLength, void * oTarget);

    bool isSameChannelImpl(const std::map<std::string, std::string> &params) const;

    void toReport(std::vector<std::string>& sReports) const;

    void toDescribe( CxDescribeUnit & describeUnit) const;
    ///----------Abstruct Realization --- end ---

    friend class ReceiverThread;
    //read thread
    class ReceiverThread : public CxJoinableThread
    {
    public:
        inline void setChannel(CxChannelSerial * v) { _channel = v; }

        inline void setStatus(int * volatile v) { _status = v; }

        void waitExit() { join(); }

    protected:
        void run();

        void exit();

    private:
        CxChannelSerial * _channel;
        int * volatile _status;
        char _buffer[1024 * 4];

    };

private:
    std::string _portName;
    int _baudRateEnum;
    int _flowControl;
    int _parity;
    int _stopBits;
    int _characterSize;
    int _baudRate;
    int _baudMode;

    CxChannelSerial * _serial;

    ReceiverThread * _receiver;
    int _receiverStatus;

#ifdef GM_OS_WIN
    OVERLAPPED _overLapped;
#endif


private:
    void stopAndDeleteRecieverThread();

    int getBaudRateInter(int mode=1) {
        if(mode>0)
        {
            switch (_baudRateEnum)
            {
                case BaudRate_256000:
                    return 256000;
                    break;
                case BaudRate_128000:
                    return 128000;
                    break;
                case BaudRate_115200:
                    return 115200;
                    break;
                case BaudRate_57600:
                    return 57600;
                    break;
                case BaudRate_56000:
                    return 56000;
                    break;
                case BaudRate_38400:
                    return 38400;
                    break;
                case BaudRate_19200:
                    return 19200;
                    break;
                case BaudRate_14400:
                    return 14400;
                    break;
                case BaudRate_9600:
                    return 9600;
                    break;
                case BaudRate_4800:
                    return 4800;
                    break;
                case BaudRate_2400:
                    return 2400;
                    break;
                case BaudRate_1200:
                    return 1200;
                    break;
                case BaudRate_600:
                    return 600;
                    break;
                case BaudRate_300:
                    return 300;
                    break;
                case BaudRate_110:
                    return 110;
                    break;
                default:
                    return _baudRateEnum;
                    break;
            }
        }
        else return _baudRate;
    }

    int getCharBitsInter() {
        switch (_characterSize)
        {
        case CharacterSizeFive:
            return 5;
            break;
        case CharacterSizeSix:
            return 6;
            break;
        case CharacterSizeSeven:
            return 7;
            break;
        case CharacterSizeEight:
            return 8;
            break;
        default:
            return 8;
            break;
        }
    }

};

class GM_CCXX_CORE_API CxChannelSerialFactory : public CxChannelFactoryBase
{

//    GM_SINGLETON_DECLARATION( CxChannelSerialFactory )

public:
    std::string factoryName(){
        return CS_TypeSimpleName_Serial;
    }

    CxChannelBase* createObject(){
        return new CxChannelSerial;
    }

};

#endif // CXCHANNEL_SERIAL_H
