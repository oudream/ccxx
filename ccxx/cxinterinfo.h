#ifndef CXINTERINFO_H
#define CXINTERINFO_H


#include "cxglobal.h"


class CxInterinfo
{
public:
    enum PlatformEnum {
        Platform_None = 0,
        Platform_Desktop,
        Platform_Cmd,
        Platform_Log,
        Platform_Net,
        Platform_11,
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

    static PlatformEnum platformFrom(const std::string & sPlatform);

    static std::string platformToString(PlatformEnum e);

    static TypeEnum typeFrom(const std::string & sType);

    static std::string typeToString(TypeEnum e);

};


///
/// \brief The CxInterinfoFilter class
/// outinfo control only : type source reason
class CxInterinfoFilter
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


class CxInterinfoOut
{
public:
    static void addObserver(CxInterinfoOut_I* oSubject);

    static void removeObserver(CxInterinfoOut_I* oSubject);

    //信息输出到所有IO端（包括命令行、日志、网络信息口等等）
    static void outInfo(const std::string& sInfo, const std::string& sTitle, int type = 0, int reason = 0, int source = 0, int target = 0, int iTag = 0);

    static int isEnable(CxInterinfo::PlatformEnum ePlatform, int type , int source , int reason);

    static void enableAll(CxInterinfo::PlatformEnum ePlatform = CxInterinfo::Platform_None);

    static void diableAll(CxInterinfo::PlatformEnum ePlatform = CxInterinfo::Platform_None);

    static void enableType(int type, CxInterinfo::PlatformEnum ePlatform = CxInterinfo::Platform_None, bool bOnly = false);

    static void disableType(int type, CxInterinfo::PlatformEnum ePlatform = CxInterinfo::Platform_None);

    static void enableType(int type , int source, CxInterinfo::PlatformEnum ePlatform = CxInterinfo::Platform_None, bool bOnly = false);

    static void disableType(int type , int source, CxInterinfo::PlatformEnum ePlatform = CxInterinfo::Platform_None);

    static void enableType(int type , int source, int reason, CxInterinfo::PlatformEnum ePlatform = CxInterinfo::Platform_None, bool bOnly = false);

    static void disableType(int type , int source, int reason, CxInterinfo::PlatformEnum ePlatform = CxInterinfo::Platform_None);

    //sFilePath is xml style :
    /*
<?xml version="1.0" encoding="utf-8"?>
<xxx>
    <interinfo>
        <enable>
            <a>
                <prop platform="log" type="" source="" reason=""/>
            </a>
        </enable>
    </interinfo>
</xxx>
*/
    static void updateByXml(const std::string & sFilePath);

    //pData is xml style
    static void updateByXml(const char * pData, int iLength);

    static void update(const std::string & sContent);

    static void update(const std::vector<std::map<std::string, std::string> > & rows);

    static void update(const std::map<std::string, std::string> & row);

    static void update( bool bEnable, const std::string & sPlatform, const std::string & sType, const std::string & sSource, const std::string & sReason );

private:
    static CxInterinfoOut_I * findInterinfoOut(CxInterinfo::PlatformEnum);

};



class CxConsoleInterinfo : public CxInterinfoOut_I
{
public:
    static void addObserver(CxInterinfoIn_I * oSubject);

    static void removeObserver(CxInterinfoIn_I * oSubject);

    static void addObserver(fn_interinfo_in_line_t fn);

    static void removeObserver(fn_interinfo_in_line_t fn);

    static void addObserver(fn_interinfo_in_cmd_t fn);

    static void removeObserver(fn_interinfo_in_cmd_t fn);

    static void setFilterChannelInfo(bool value);

    static void start();

    static void stop();

protected:
    void interinfo_out( const std::string& sInfo, const std::string& sTitle, int type = 0, int reason = 0, int source = 0, int target = 0, int tag = 0 );

    inline CxInterinfo::PlatformEnum platformValue() { return CxInterinfo::Platform_Cmd; }

private:
    static void updateInputString(const std::string & sLine);

    static void processInputString(int, int, const void *, int, void *, void *);

    friend class ThreadInputPrivate;

};


class CxEndFlag {};

class CxLineFlag {};

class CxLineEndFlag {};


class CxOutStream
{
private:
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
    inline CxOutStream() : stream(new Stream()) {}
    inline CxOutStream(const CxOutStream &o) : stream(o.stream) { ++stream->ref; }
    inline CxOutStream &operator=(const CxOutStream &other)
    {
        if (this != &other)
        {
            CxOutStream outStream(other);
            std::swap(stream, outStream.stream);
        }
        return *this;
    }

    inline ~CxOutStream()
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

    inline CxOutStream &space() { stream->space = true; return *this; }
    inline CxOutStream &nospace() { stream->space = false; return *this; }
    inline CxOutStream &lf() { stream->lf = true; return *this; }
    inline CxOutStream &nolf() { stream->lf = false; return *this; }
    inline CxOutStream &enable() { stream->enable = true; return *this; }
    inline CxOutStream &disable() { stream->enable = false; return *this; }
    inline CxOutStream &setEnable(bool value) { stream->enable = value; return *this; }
    inline CxOutStream &end() { stream->end = true; return *this; }
    inline CxOutStream &noend() { stream->end = false; return *this; }
    inline CxOutStream &type(int value) { stream->type = value; return *this; }
    inline CxOutStream &source(int value) { stream->source = value; return *this; }
    inline CxOutStream &target(int value) { stream->target = value; return *this; }
    inline CxOutStream &reason(int value) { stream->reason = value; return *this; }
    inline CxOutStream &tag(int value) { stream->tag = value; return *this; }
    inline CxOutStream &title(const std::string& value) { stream->title = value; return *this; }

    inline CxOutStream &operator<<(const CxEndFlag&) { stream->end = true; return output(); }

    inline CxOutStream &operator<<(const CxLineFlag&) { stream->ts << '\n'; return output(); }

    inline CxOutStream &operator<<(const CxLineEndFlag&) { stream->ts << '\n'; stream->end = true; return output(); }

    template<typename T>
    inline CxOutStream &operator<<(const T & t) { stream->ts << t; return output(); }

    inline CxOutStream &operator<<(const bool& t) { stream->ts << (t ? "true" : "false"); return output(); }

    template<typename T>
    inline CxOutStream &operator<<(const std::vector<T>& t)
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
    inline CxOutStream &operator<<(const std::map<TKey, TValue> & m)
    {
        for (typename std::map<TKey, TValue>::const_iterator it = m.begin(); it != m.end(); ++it)
        {
            std::cout << it->first << "=" << it->second;
            if (stream->lf)
                stream->ts << "\n";
            else
                stream->ts << " ";
        }
        return output();
    }

private:
    inline CxOutStream &output()
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


class CxNoOutStream
{
public:
    inline CxNoOutStream(){}
    inline CxNoOutStream(const CxOutStream &){}
    inline ~CxNoOutStream(){}

    inline CxNoOutStream &space() { return *this; }
    inline CxNoOutStream &nospace() { return *this; }
    inline CxNoOutStream &lf() { return *this; }
    inline CxNoOutStream &nolf() { return *this; }
    inline CxNoOutStream &enable() { return *this; }
    inline CxNoOutStream &disable() { return *this; }
    inline CxNoOutStream &setEnable(bool) { return *this; }
    inline CxNoOutStream &end() { return *this; }
    inline CxNoOutStream &noend() { return *this; }
    inline CxNoOutStream &type(int) { return *this; }
    inline CxNoOutStream &source(int) { return *this; }
    inline CxNoOutStream &target(int) { return *this; }
    inline CxNoOutStream &reason(int) { return *this; }
    inline CxNoOutStream &tag(int) { return *this; }
    inline CxNoOutStream &title(const std::string&) { return *this; }

    template<typename T>
    inline CxNoOutStream &operator<<(const T &) { return *this; }

};


#if defined(GM_DEBUG)
inline CxOutStream cxDebug() { CxOutStream r; r.noend(); r << CxGlobal::debugString; return r; }
#else
#undef cxDebug
inline CxNoOutStream cxDebug() { return CxNoOutStream(); }
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
inline CxOutStream cxTrack(int iTrack) { CxOutStream r;  r.noend(); r.setEnable((iTrack>=GM_TRACK_BEGIN) && (iTrack<GM_TRACK_END)); r << CxGlobal::trackString << iTrack << " : "; return r; }
#else
#undef cxTrack
inline CxNoOutStream cxTrack(int iProcess) { return CxNoOutStream(); }
#define CX_NO_CXTRACK_MACRO while (false) cxTrack
#define cxTrack CX_NO_CXTRACK_MACRO
#endif

inline CxOutStream cxPrompt() { CxOutStream r; r.noend(); r << CxGlobal::promptString; return r; }

inline CxOutStream cxWarn(const std::string & sFileName, int iLineIndex) { CxOutStream r; r.noend(); r << CxGlobal::warningString << sFileName << " at line " << iLineIndex << "\n" << CxGlobal::warningString; return r; }

#ifndef cxWarning
#define cxWarning() cxWarn(__FILE__,__LINE__)
#endif

extern const CxEndFlag cxEnd;
extern const CxLineFlag cxLine;
extern const CxLineEndFlag cxEndLine;

#define cxPromptOut(p1)  ( cxPrompt() << p1 << cxEndLine )
#define cxPromptOut1(p1) ( cxPrompt() << p1 << cxEndLine )
#define cxPromptOut2(p1,p2) ( cxPrompt().space() << p1  << p2 << cxEndLine )
#define cxPromptOut3(p1,p2,p3) ( cxPrompt().space() << p1 << p2 << p3 << cxEndLine )
#define cxPromptOut4(p1,p2,p3,p4) ( cxPrompt().space() << p1 << p2 << << p3 << p4 << cxEndLine )
#define cxPromptOut5(p1,p2,p3,p4,p5) ( cxPrompt().space() << p1 << p2 << p3 << p4 << p5 << cxEndLine )
#define cxPromptOut6(p1,p2,p3,p4,p5,p6) ( cxPrompt().space() << p1 << p2 << p3 << p4 << p5 << p6 << cxEndLine )

#define cxWarningtOut(p1)  ( cxWarning() << p1 << cxEndLine )
#define cxWarningtOut1(p1) ( cxWarning() << p1 << cxEndLine )
#define cxWarningtOut2(p1,p2) ( cxWarning().space() << p1  << p2 << cxEndLine )
#define cxWarningtOut3(p1,p2,p3) ( cxWarning().space() << p1 << p2 << p3 << cxEndLine )
#define cxWarningtOut4(p1,p2,p3,p4) ( cxWarning().space() << p1 << p2 << << p3 << p4 << cxEndLine )
#define cxWarningtOut5(p1,p2,p3,p4,p5) ( cxWarning().space() << p1 << p2 << p3 << p4 << p5 << cxEndLine )
#define cxWarningtOut6(p1,p2,p3,p4,p5,p6) ( cxWarning().space() << p1 << p2 << p3 << p4 << p5 << p6 << cxEndLine )

#define cxPromptCheck(b)  if(! (b) ) { cxPrompt().space() << "fail : " << std::string(__FILE__) << __LINE__ << std::string(#b) << cxEndLine; }
#define cxPromptCheck1(b,p1) if(! (b) ) { cxPrompt().space() << "fail : " << std::string(__FILE__) << __LINE__ << std::string(#b) << p1 << cxEndLine; }
#define cxPromptCheck2(b,p1,p2) if(! (b) ) { cxPrompt().space() << "fail : " << std::string(__FILE__) << __LINE__ << std::string(#b) << p1 << p2 << cxEndLine; }
#define cxPromptCheck3(b,p1,p2,p3) if(! (b) ) { cxPrompt().space() << "fail : " << std::string(__FILE__) << __LINE__ << std::string(#b) << p1 << p2 << p3 << cxEndLine; }
#define cxPromptCheck4(b,p1,p2,p3,p4) if(! (b) ) { cxPrompt().space() << "fail : " << std::string(__FILE__) << __LINE__ << std::string(#b) << p1 << p2 << p3 << p4 << cxEndLine; }
#define cxPromptCheck5(b,p1,p2,p3,p4,p5) if(! (b) ) { cxPrompt().space() << "fail : " << std::string(__FILE__) << __LINE__ << std::string(#b) << p1 << p2 << p3 << p4 << p5 << cxEndLine; }
#define cxPromptCheck6(b,p1,p2,p3,p4,p5,p6) if(! (b) ) { cxPrompt().space() << "fail : " << std::string(__FILE__) << __LINE__ << std::string(#b) << p1 << p2 << p3 << p4 << p5 << p6 << cxEndLine; }

#define cxPromptReturn(b)  if(! (b) ) { cxPrompt().space() << "fail : " << std::string(__FILE__) << __LINE__ << std::string(#b) << cxEndLine; return; }
#define cxPromptReturn_(b, r)  if(! (b) ) { cxPrompt().space() << "fail : " << std::string(__FILE__) << __LINE__ << std::string(#b) << cxEndLine; return r; }

#define cxDebugReturn(b)  if(! (b) ) { cxDebug().space() << "fail : " << std::string(__FILE__) << __LINE__ << std::string(#b) << cxEndLine; return; }
#define cxDebugReturn_(b, r)  if(! (b) ) { cxDebug().space() << "fail : " << std::string(__FILE__) << __LINE__ << std::string(#b) << cxEndLine; return r; }

//__FILE__, __LINE__,


#endif // CXINTERINFO_H
