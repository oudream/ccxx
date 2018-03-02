#ifndef CXINTERINFO_H
#define CXINTERINFO_H


#include "cxglobal.h"


class GM_CCXX_CORE_API CxInterinfo
{
public:
    enum PlatformEnum {
        Platform_None = 0,
        Platform_Desktop,
        Platform_Cmd,
        Platform_Log,
        Platform_Net,
        Platform_Tty,
        Platform_12,
        Platform_13,
        Platform_14,
        Platform_15,
        Platform_16,
        Platform_17,
        Platform_18,
        Platform_19
    };

    enum TypeEnum {
        Type_None = 0,
        Type_Platform = 0x01000001,
        Type_Program = 0x01000002,
        Type_Safe = 0x01000003,
        Type_IO_Ram = 0x02000004,
        Type_IO_Rom = 0x02000005,
        Type_IO_File = 0x02000006,
        Type_IO_Channel =   0x04000100,
        Type_IO_Tcpclient = 0x04000102,
        Type_IO_Tcpserver = 0x04000103,
        Type_IO_Udp =       0x04000104,
        Type_IO_Icmp =      0x04000105,
        Type_IO_Serial =    0x04000106,
        Type_IO_None =      0x04000107,
        Type_Protocol =     0x04000201,
        Type_Terminal =     0x04000301,
        Type_Business =     0x04000401,
        Type_Task = 0x05000001,
        Type_Script = 0x06000001,
        Type_18,
        Type_19
    };

    enum InDealEnum {
        InDeal_None = 0,
        InDeal_Ed,
        InDeal_Lock
    };

    enum LevelEnum {
        LevelNone = 0x00000000,
        LevelFatal = 0x00010000,
        LevelError = 0x00020000,
        LevelWarn = 0x00040000,
        LevelInfo = 0x00080000,
        LevelDebug = 0x00100000
    };

    //itc : inter thread communication
    static void startInterInfo(fn_void_queue_msg_tlv_t fnITCPostDefault, fn_void_t fnITCSignalDefault, bool bRunInputThread=true);

    static void stopInterInfo();

    static PlatformEnum platformFrom(const std::string & sPlatform);

    static std::string platformToString(PlatformEnum e);

    static TypeEnum typeFrom(const std::string & sType);

    static std::string typeToString(TypeEnum e);

    static LevelEnum levelFrom(const std::string & sLevel);

    static std::string levelToString(LevelEnum e);

    static std::string levelUnmberToString(int eNum);

};


///
/// \brief The CxInterinfoFilter class
/// outinfo control only : type source reason
class GM_CCXX_CORE_API CxInterinfoFilter
{
public:
    CxInterinfoFilter() : m_isReverse(false) {}

    ~CxInterinfoFilter(){}

    int isEnable(int type , int source , int reason);

    void enableAll();

    void disableAll();

    void enableType(int type);

    void disableType(int type);

    void enableType(int type , int source);

    void disableType(int type , int source);

    void enableType(int type , int source, int reason);

    void disableType(int type , int source, int reason);

    inline bool getIsReverse() { return m_isReverse; }
    inline void setReverse(bool value) { m_isReverse = value; }

private:
    bool m_isReverse;
    std::map<int, std::map<int, std::vector<int> > > m_types; //type source reason

};


class CxInterinfoOut_I
{
public:
    inline CxInterinfoFilter * filter() {
        return & m_filter;
    }

protected:
    virtual void interinfo_out( const std::string& sInfo, const std::string& sTitle, int type = 0, int reason = 0, int source = 0, int target = 0, int iTag = 0 ) = 0;

    virtual CxInterinfo::PlatformEnum platformValue() = 0;

    friend class CxInterinfoOut;

private:
    CxInterinfoFilter m_filter;

};


typedef int (*fn_interinfo_in_line_t)( const std::string & sInfo, const msepoch_t & dtIn, int iSource, int eInType, int iTag);

typedef int (*fn_interinfo_in_cmd_t)( const std::string & sCommand, const std::map<std::string, std::string> & sParams, const msepoch_t & dtIn, int iSource, int eInType, int iTag);

class CxInterinfoIn_I
{
protected:
    virtual int interinfo_in_line( const std::string& sInfo, const msepoch_t & dtIn, int iSource = 0, int eInType = 0, int iTag = 0 ) = 0;

    virtual int interinfo_in_cmd( const std::string & sCommand, const std::map<std::string, std::string> & sParams, const msepoch_t & dtIn, int iSource = 0, int eInType = 0, int iTag = 0 ) = 0;

    friend class CxConsoleInterinfo;

};


class GM_CCXX_CORE_API CxInterinfoOut
{
public:
    static void addObserver(CxInterinfoOut_I* oSubject);

    static void removeObserver(CxInterinfoOut_I* oSubject);

    /**
     * summary : out into to all io, inclue : desktop, tty, log, cmd
     * @param sInfo
     * @param sTitle
     * @param type
     * @param reason
     * @param source
     * @param target
     * @param iTag
     */
    static void outInfo(const std::string& sInfo, const std::string& sTitle, int type = 0, int reason = 0, int source = 0, int target = 0, int iTag = 0);

    static void outLog(const std::string& sInfo, const std::string& sTitle, int type = 0, int reason = 0, int source = 0, int target = 0, int iTag = 0);

    static int isEnable(CxInterinfo::PlatformEnum ePlatform, int type , int source , int reason);

    static void enableAll(CxInterinfo::PlatformEnum ePlatform = CxInterinfo::Platform_None);

    static void diableAll(CxInterinfo::PlatformEnum ePlatform = CxInterinfo::Platform_None);

    static void enableType(int type, CxInterinfo::PlatformEnum ePlatform = CxInterinfo::Platform_None, bool bOnly = false);

    static void disableType(int type, CxInterinfo::PlatformEnum ePlatform = CxInterinfo::Platform_None);

    static void enableType(int type , int source, CxInterinfo::PlatformEnum ePlatform = CxInterinfo::Platform_None, bool bOnly = false);

    static void disableType(int type , int source, CxInterinfo::PlatformEnum ePlatform = CxInterinfo::Platform_None);

    static void enableType(int type , int source, int reason, CxInterinfo::PlatformEnum ePlatform = CxInterinfo::Platform_None, bool bOnly = false);

    static void disableType(int type , int source, int reason, CxInterinfo::PlatformEnum ePlatform = CxInterinfo::Platform_None);

    static void update(const std::string & sContent);

    static void update(const std::vector<std::map<std::string, std::string> > & rows);

    static void update(const std::map<std::string, std::string> & row);

    static void update( bool bEnable, const std::string & sPlatform, const std::string & sType, const std::string & sSource, const std::string & sReason );

private:
    static CxInterinfoOut_I * findInterinfoOut(CxInterinfo::PlatformEnum);

};


class GM_CCXX_CORE_API CxInterinfoIn
{
public:
    static void addObserver(CxInterinfoIn_I * oSubject);

    static void removeObserver(CxInterinfoIn_I * oSubject);

    static void addObserver(fn_interinfo_in_line_t fn);

    static void removeObserver(fn_interinfo_in_line_t fn);

    static void addObserver(fn_interinfo_in_cmd_t fn);

    static void removeObserver(fn_interinfo_in_cmd_t fn);

};


class GM_CCXX_CORE_API CxConsoleInterinfo : public CxInterinfoOut_I
{
//*input
public:
    static std::string waitInputLine(fn_void_t fnProcessEvents, int iTimeOut = 0 , msepoch_t * pDtIn = NULL, int  * pSource = NULL, int * pInType = NULL, int * pTag = NULL);

    static std::string waitInputCmd(fn_void_t fnProcessEvents, int iTimeOut = 0 , std::map<std::string, std::string> * oParams = NULL, msepoch_t * pDtIn = NULL, int  * pSource = NULL, int * pInType = NULL, int * pTag = NULL);

//*output interface
protected:
    void interinfo_out( const std::string& sInfo, const std::string& sTitle, int type = 0, int reason = 0, int source = 0, int target = 0, int tag = 0 );

    inline CxInterinfo::PlatformEnum platformValue() { return CxInterinfo::Platform_Cmd; }

//*input
private:
    /**
     * input to public
     * @param sLine
     */
    static void updateConsoleInputString(const std::string & sLine);

    static void processInputString(int, int, const void *, int, void *, void *);

    static int processInputLine( const std::string& sInfo, const msepoch_t & dtIn, int iSource = 0, int eInType = 0, int iTag = 0 );

    static int processInputCmd( const std::string & sCommand, const std::map<std::string, std::string> & sParams, const msepoch_t & dtIn, int iSource = 0, int eInType = 0, int iTag = 0 );

    friend class CxConsoleInputThread;

};


class CxEndFlag {};

class CxLineFlag {};

class CxLineEndFlag {};


class CxOutStreamBase
{
protected:
    struct Stream
    {
        Stream() : ts(), title(), ref(1), enable(true), end(true), space(true), lf(false), type(0), reason(0), source(0), target(0), tag(0) {}
        std::stringstream ts;
        std::string title;
        volatile int ref;
        bool enable;
        bool end;
        bool space;
        bool lf;
        int type;
        int reason;
        int source;
        int target;
        int tag;
    } * stream;
public:
    inline CxOutStreamBase() : stream(new Stream()) {}
    inline CxOutStreamBase(const CxOutStreamBase &o) : stream(o.stream) { ++stream->ref; }

    inline CxOutStreamBase &space() { stream->space = true; return *this; }
    inline CxOutStreamBase &nospace() { stream->space = false; return *this; }
    inline CxOutStreamBase &lf() { stream->lf = true; return *this; }
    inline CxOutStreamBase &nolf() { stream->lf = false; return *this; }
    inline CxOutStreamBase &enable() { stream->enable = true; return *this; }
    inline CxOutStreamBase &disable() { stream->enable = false; return *this; }
    inline CxOutStreamBase &setEnable(bool value) { stream->enable = value; return *this; }
    inline CxOutStreamBase &end() { stream->end = true; return *this; }
    inline CxOutStreamBase &noend() { stream->end = false; return *this; }
    inline CxOutStreamBase &type(int value) { stream->type = value; return *this; }
    inline CxOutStreamBase &source(int value) { stream->source = value; return *this; }
    inline CxOutStreamBase &target(int value) { stream->target = value; return *this; }
    inline CxOutStreamBase &reason(int value) { stream->reason = value; return *this; }
    inline CxOutStreamBase &tag(int value) { stream->tag = value; return *this; }
    inline CxOutStreamBase &title(const std::string& value) { stream->title = value; return *this; }
    inline CxOutStreamBase &level(int value) { stream->reason = stream->reason | value ; return *this; }

    inline CxOutStreamBase &operator<<(const CxEndFlag&) { stream->end = true; return output(); }

    inline CxOutStreamBase &operator<<(const CxLineFlag&) { stream->ts << '\n'; return output(); }

    inline CxOutStreamBase &operator<<(const CxLineEndFlag&) { stream->ts << '\n'; stream->end = true; return output(); }

    template<typename T>
    inline CxOutStreamBase &operator<<(const T & t) { stream->ts << t; return output(); }

	inline CxOutStreamBase &operator<<(const bool& t) { stream->ts << (t ? "true" : "false"); return output(); }

    inline CxOutStreamBase &operator<<(const std::string& t) { stream->ts << t; return output(); }

    inline CxOutStreamBase &operator<<(char * t) { stream->ts << std::string(t); return output(); }

    inline CxOutStreamBase &operator<<(const char * t) { stream->ts << std::string(t); return output(); }

    template<typename T>
    inline CxOutStreamBase &operator<<(const std::vector<T>& t)
    {
        for (typename std::vector<T>::const_iterator it = t.begin(); it != t.end(); ++it)
        {
            stream->ts << (* it);
            if (stream->lf)
                stream->ts << "\n";
            else
                stream->ts << " ";
        }
        return output();
    }

    template<typename TKey, typename TValue>
    inline CxOutStreamBase &operator<<(const std::map<TKey, TValue> & m)
    {
        for (typename std::map<TKey, TValue>::const_iterator it = m.begin(); it != m.end(); ++it)
        {
            stream->ts << it->first << ":" << it->second;
            if (stream->lf)
                stream->ts << "\n";
            else
                stream->ts << ",";
        }
        return output();
    }

protected:
    virtual CxOutStreamBase &output() = 0;

};

class CxNoOutStreamBase
{
public:
    inline CxNoOutStreamBase(){}
    inline CxNoOutStreamBase(const CxOutStreamBase &){}
    inline ~CxNoOutStreamBase(){}

    inline CxNoOutStreamBase &space() { return *this; }
    inline CxNoOutStreamBase &nospace() { return *this; }
    inline CxNoOutStreamBase &lf() { return *this; }
    inline CxNoOutStreamBase &nolf() { return *this; }
    inline CxNoOutStreamBase &enable() { return *this; }
    inline CxNoOutStreamBase &disable() { return *this; }
    inline CxNoOutStreamBase &setEnable(bool) { return *this; }
    inline CxNoOutStreamBase &end() { return *this; }
    inline CxNoOutStreamBase &noend() { return *this; }
    inline CxNoOutStreamBase &type(int) { return *this; }
    inline CxNoOutStreamBase &source(int) { return *this; }
    inline CxNoOutStreamBase &target(int) { return *this; }
    inline CxNoOutStreamBase &reason(int) { return *this; }
    inline CxNoOutStreamBase &tag(int) { return *this; }
    inline CxNoOutStreamBase &title(const std::string&) { return *this; }
    inline CxNoOutStreamBase &level(int) { return *this; }

    template<typename T>
    inline CxNoOutStreamBase &operator<<(const T &) { return *this; }

};


class CxInterinfoOutStream : public CxOutStreamBase {
public:
    inline ~CxInterinfoOutStream()
    {
        if (!--stream->ref)
        {
            if ((! stream->end) && (stream->enable))
            {
                stream->ts << '\n';
                CxInterinfoOut::outInfo(stream->ts.str(), stream->title, stream->type, stream->reason, stream->source, stream->target, stream->tag);
            }
            delete stream;
        }
    }

    inline CxInterinfoOutStream &operator=(const CxInterinfoOutStream &other)
    {
        if (this != &other)
        {
            CxInterinfoOutStream outStream(other);
            std::swap(stream, outStream.stream);
        }
        return *this;
    }

protected:
    virtual inline CxOutStreamBase &output()
    {
        if (stream->end)
        {
            if (stream->enable)
            {
                CxInterinfoOut::outInfo(stream->ts.str(), stream->title, stream->type, stream->reason, stream->source, stream->target, stream->tag);
            }
        }
        else
        {
            if (stream->space) stream->ts << ' ';
            if (stream->lf) stream->ts << '\n';
        }
        return *this;
    }

};

class CxInterinfoNoOutStream : public CxNoOutStreamBase {

};


#if defined(GM_DEBUG)
inline CxInterinfoOutStream cxDebug() { CxInterinfoOutStream r; r.noend(); r << CxGlobal::debugString; return r; }
#else
#undef cxDebug
inline CxInterinfoNoOutStream cxDebug() { return CxInterinfoNoOutStream(); }
#define CX_NO_CXDEBUG_MACRO while (false) cxDebug
#define cxDebug CX_NO_CXDEBUG_MACRO
#endif

#if defined(GM_TRACK_VALUE)
#ifndef GM_TRACK_BEGIN
#define GM_TRACK_BEGIN GM_TRACK_VALUE
#endif
#ifndef GM_TRACK_END
#define GM_TRACK_END (GM_TRACK_VALUE+1)
#endif
#endif


#if defined(GM_DEBUG) && defined(GM_TRACK_BEGIN) && defined(GM_TRACK_END)
inline CxInterinfoOutStream cxTrack(int iTrack) { CxInterinfoOutStream r;  r.noend(); r.setEnable((iTrack>=GM_TRACK_BEGIN) && (iTrack<GM_TRACK_END)); r << CxGlobal::trackString << iTrack << " : "; return r; }
#else
#undef cxTrack
inline CxInterinfoNoOutStream cxTrack(int iProcess) { return CxInterinfoNoOutStream(); }
#define CX_NO_CXTRACK_MACRO while (false) cxTrack
#define cxTrack CX_NO_CXTRACK_MACRO
#endif

inline CxInterinfoOutStream cxPrompt() { CxInterinfoOutStream r; r.noend(); r << CxGlobal::promptString; return r; }

inline CxInterinfoOutStream cxWarn() { CxInterinfoOutStream r; r.noend(); r << CxGlobal::warningString; return r; }

inline CxInterinfoOutStream cxWarn_(std::string sFileName, int iLineIndex) { CxInterinfoOutStream r; r.noend(); r << CxGlobal::warningString << sFileName << " at line " << iLineIndex << "\n" << CxGlobal::warningString; return r; }

#ifndef cxWarning
#define cxWarning() cxWarn_(std::string(__FILE__),int(__LINE__))
#endif

extern GM_CCXX_CORE_API const CxEndFlag cxEnd;
extern GM_CCXX_CORE_API const CxLineFlag cxLine;
extern GM_CCXX_CORE_API const CxLineEndFlag cxEndLine;

#define cxPromptOut(p1)  ( cxPrompt() << p1 << cxEndLine )
#define cxPromptOut1(p1) ( cxPrompt() << p1 << cxEndLine )
#define cxPromptOut2(p1,p2) ( cxPrompt().space() << p1  << p2 << cxEndLine )
#define cxPromptOut3(p1,p2,p3) ( cxPrompt().space() << p1 << p2 << p3 << cxEndLine )
#define cxPromptOut4(p1,p2,p3,p4) ( cxPrompt().space() << p1 << p2 << p3 << p4 << cxEndLine )
#define cxPromptOut5(p1,p2,p3,p4,p5) ( cxPrompt().space() << p1 << p2 << p3 << p4 << p5 << cxEndLine )
#define cxPromptOut6(p1,p2,p3,p4,p5,p6) ( cxPrompt().space() << p1 << p2 << p3 << p4 << p5 << p6 << cxEndLine )

#define cxDebugOut(p1)  ( cxDebug() << p1 << cxEndLine )
#define cxDebugOut1(p1) ( cxDebug() << p1 << cxEndLine )
#define cxDebugOut2(p1,p2) ( cxDebug().space() << p1  << p2 << cxEndLine )
#define cxDebugOut3(p1,p2,p3) ( cxDebug().space() << p1 << p2 << p3 << cxEndLine )
#define cxDebugOut4(p1,p2,p3,p4) ( cxDebug().space() << p1 << p2 <<  p3 << p4 << cxEndLine )
#define cxDebugOut5(p1,p2,p3,p4,p5) ( cxDebug().space() << p1 << p2 << p3 << p4 << p5 << cxEndLine )
#define cxDebugOut6(p1,p2,p3,p4,p5,p6) ( cxDebug().space() << p1 << p2 << p3 << p4 << p5 << p6 << cxEndLine )

#define cxWarningtOut(p1)  ( cxWarning() << p1 << cxEndLine )
#define cxWarningtOut1(p1) ( cxWarning() << p1 << cxEndLine )
#define cxWarningtOut2(p1,p2) ( cxWarning().space() << p1  << p2 << cxEndLine )
#define cxWarningtOut3(p1,p2,p3) ( cxWarning().space() << p1 << p2 << p3 << cxEndLine )
#define cxWarningtOut4(p1,p2,p3,p4) ( cxWarning().space() << p1 << p2 <<  p3 << p4 << cxEndLine )
#define cxWarningtOut5(p1,p2,p3,p4,p5) ( cxWarning().space() << p1 << p2 << p3 << p4 << p5 << cxEndLine )
#define cxWarningtOut6(p1,p2,p3,p4,p5,p6) ( cxWarning().space() << p1 << p2 << p3 << p4 << p5 << p6 << cxEndLine )

#define cxPromptCheck(b,r)  if(! (b) ) { cxPrompt().space() << "fail. __FILE__=" << std::string(__FILE__) << ", __LINE__=" << __LINE__ << std::string(#b) << cxEndLine; r; }
#define cxPromptCheck1(b,r,p1)  if(! (b) ) { cxPrompt().space() << "fail. __FILE__=" << std::string(__FILE__) << ", __LINE__=" << __LINE__ << std::string(#b) << p1 << cxEndLine; r; }
#define cxPromptCheck2(b,r,p1,p2)  if(! (b) ) { cxPrompt().space() << "fail. __FILE__=" << std::string(__FILE__) << ", __LINE__=" << __LINE__ << std::string(#b) << p1 << p2 << cxEndLine; r; }
#define cxPromptCheck3(b,r,p1,p2,p3)  if(! (b) ) { cxPrompt().space() << "fail. __FILE__=" << std::string(__FILE__) << ", __LINE__=" << __LINE__ << std::string(#b) << p1 << p2 << p3 << cxEndLine; r; }
#define cxPromptCheck4(b,r,p1,p2,p3,p4) if(! (b) ) { cxPrompt().space() << "fail. __FILE__=" << std::string(__FILE__) << ", __LINE__=" << __LINE__ << std::string(#b) << p1 << p2 << p3 << p4 << cxEndLine; r; }
#define cxPromptCheck5(b,r,p1,p2,p3,p4,p5) if(! (b) ) { cxPrompt().space() << "fail. __FILE__=" << std::string(__FILE__) << ", __LINE__=" << __LINE__ << std::string(#b) << p1 << p2 << p3 << p4 << p5 << cxEndLine; r; }
#define cxPromptCheck6(b,r,p1,p2,p3,p4,p5,p6) if(! (b) ) { cxPrompt().space() << "fail. __FILE__=" << std::string(__FILE__) << ", __LINE__=" << __LINE__ << std::string(#b) << p1 << p2 << p3 << p4 << p5 << p6 << cxEndLine; r; }

#if defined(GM_DEBUG)
#define cxDebugCheck(b,r)  if(! (b) ) { cxDebug().space() << "fail. __FILE__=" << std::string(__FILE__) << ", __LINE__=" << __LINE__ << std::string(#b) << cxEndLine; r; }
#define cxDebugCheck1(b,r,p1)  if(! (b) ) { cxDebug().space() << "fail. __FILE__=" << std::string(__FILE__) << ", __LINE__=" << __LINE__ << std::string(#b) << p1 << cxEndLine; r; }
#define cxDebugCheck2(b,r,p1,p2)  if(! (b) ) { cxDebug().space() << "fail. __FILE__=" << std::string(__FILE__) << ", __LINE__=" << __LINE__ << std::string(#b) << p1 << p2 << cxEndLine; r; }
#define cxDebugCheck3(b,r,p1,p2,p3)  if(! (b) ) { cxDebug().space() << "fail. __FILE__=" << std::string(__FILE__) << ", __LINE__=" << __LINE__ << std::string(#b) << p1 << p2 << p3 << cxEndLine; r; }
#define cxDebugCheck4(b,r,p1,p2,p3,p4) if(! (b) ) { cxDebug().space() << "fail. __FILE__=" << std::string(__FILE__) << ", __LINE__=" << __LINE__ << std::string(#b) << p1 << p2 << p3 << p4 << cxEndLine; r; }
#define cxDebugCheck5(b,r,p1,p2,p3,p4,p5) if(! (b) ) { cxDebug().space() << "fail. __FILE__=" << std::string(__FILE__) << ", __LINE__=" << __LINE__ << std::string(#b) << p1 << p2 << p3 << p4 << p5 << cxEndLine; r; }
#define cxDebugCheck6(b,r,p1,p2,p3,p4,p5,p6) if(! (b) ) { cxDebug().space() << "fail. __FILE__=" << std::string(__FILE__) << ", __LINE__=" << __LINE__ << std::string(#b) << p1 << p2 << p3 << p4 << p5 << p6 << cxEndLine; r; }
#else
#define cxDebugCheck(b,r)
#define cxDebugCheck1(b,r,p1)
#define cxDebugCheck2(b,r,p1,p2)
#define cxDebugCheck3(b,r,p1,p2,p3)
#define cxDebugCheck4(b,r,p1,p2,p3,p4)
#define cxDebugCheck5(b,r,p1,p2,p3,p4,p5)
#define cxDebugCheck6(b,r,p1,p2,p3,p4,p5,p6)
#endif


//__FILE__, __LINE__,


#endif // CXINTERINFO_H
