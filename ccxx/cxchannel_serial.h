#ifndef CXCHANNEL_SERIAL_H
#define CXCHANNEL_SERIAL_H

#include "cxchannel.h"


class SerialPrivate;

class GM_CCXX_CORE_API CxChannelSerial : public CxChannelBase
{
public:
    enum BaudRateEnum {
        BaudRate_110 = 0,
        BaudRate_300 = 1,
        BaudRate_600 = 2,
        BaudRate_1200 = 3,
        BaudRate_2400 = 4,
        BaudRate_4800 = 5,
        BaudRate_9600 = 6,
        BaudRate_14400 = 7,
        BaudRate_19200 = 8,
        BaudRate_38400 = 9,
        BaudRate_56000 = 10,
        BaudRate_57600 = 11,
        BaudRate_115200 = 12,
        BaudRate_128000 = 13,
        BaudRate_256000 = 14
    };
    typedef enum BaudRateEnum BaudRateEnum;

    enum Flow {
        flowNone = 0,
        flowSoft = 1,
        flowHard = 2,
        flowBoth = 3
    };
    typedef enum Flow Flow;

    enum Parity {
        parityNone = 0,
        parityOdd = 1,
        parityEven = 3
    };
    typedef enum Parity Parity;

    enum StopBits {
        stopBitsOne = 0,
        stopBuitOne5 = 1,
        StopBitsTwo = 2
    };
    typedef enum StopBits StopBits;

    enum CharacterSize {
        CharacterSizeFive = 0,
        CharacterSizeSix = 1,
        CharacterSizeSeven = 2,
        CharacterSizeEight = 3
    };
    typedef enum CharacterSize CharacterSize;

    enum Pending {
        pendingInput = 0,
        pendingOutput = 1,
        pendingError = 2
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
    bool doSetSpeed(BaudRateEnum baudRate);

    /**
     * Set character size.
     *
     * @return 0 on success.
     * @param bits character size to use (usually 7 or 8).
     */
    bool doSetCharBits(CharacterSize bits);

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
    bool doSetStopBits(StopBits bits);

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

    inline BaudRateEnum baudRate(int mode=1) const { return (BaudRateEnum)_baudRateEnum; }
    inline void setBaudRate(BaudRateEnum value) {
        if (! connected())
        {
            _baudRateEnum = value;
        }
    }

    inline Flow flowControl() const { return (Flow)_flowControlEnum; }
    inline void setFlowControl(Flow value) {
        if (! connected())
        {
            _flowControlEnum = value;
        }
    }

    inline Parity parity() const { return (Parity)_parityEnum; }
    inline void setParity(Parity value) {
        if (! connected())
        {
            _parityEnum = value;
        }
    }

    inline StopBits stopBits() const { return (StopBits)_stopBitsEnum; }
    inline void setStopBits(StopBits value) {
        if (! connected())
        {
            _stopBitsEnum = value;
        }
    }

    inline CharacterSize characterSize() const { return (CharacterSize)_characterSizeEnum; }
    inline void setCharacterSize(CharacterSize value) {
        if (! connected())
        {
            _characterSizeEnum = value;
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
    int _flowControlEnum;
    int _parityEnum;
    int _stopBitsEnum;
    int _characterSizeEnum;

    ReceiverThread * _receiver;
    int _receiverStatus;

#ifdef GM_OS_WIN
    OVERLAPPED _overLapped;
#endif


private:
    void stopAndDeleteRecieverThread();
    
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
