
#include "global.h"

class CxEndFlag {};

class CxLineFlag {};

class CxLineEndFlag {};

const CxEndFlag cxEnd = CxEndFlag();
const CxLineFlag cxLine = CxLineFlag();
const CxLineEndFlag cxEndLine = CxLineEndFlag();

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
    virtual ~CxOutStreamBase(){}

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
	virtual CxOutStreamBase &output() { return *this; }

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
                cout << " ~CxInterinfoOutStream" << endl;
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
                cout << " ~CxInterinfoOutStream::output" << endl;
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


class CxLogOutStream : public CxOutStreamBase {
public:
    inline ~CxLogOutStream()
    {
        if (!--stream->ref)
        {
            if ((! stream->end) && (stream->enable))
            {
                stream->ts << '\n';
                cout << " ~CxLogOutStream" << endl;
            }
            delete stream;
        }
    }

    inline CxLogOutStream &operator=(const CxLogOutStream &other)
    {
        if (this != &other)
        {
            CxLogOutStream outStream(other);
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
                cout << " ~CxLogOutStream::output" << endl;
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

typedef CxOutStreamBase(*fn_outStream_t)(void);

inline CxLogOutStream cxLogInfo() { CxLogOutStream r; r.noend(); r.level(1); return r; }

inline CxInterinfoOutStream cxPrompt() { CxInterinfoOutStream r; r.noend(); r << '+'; return r; }


void testClass11()
{
    CxLogOutStream logOutStream; logOutStream.disable();
    CxInterinfoOutStream interinfoOutStream; interinfoOutStream.disable();
    CxOutStreamBase & outStreamBase = logOutStream;
    outStreamBase.enable();
//
////    fn1() << "fn1";
    outStreamBase << "cxLogInfo";
////    fn2() << "fn2";
    outStreamBase << "cxPrompt";
//    outStreamBase.end();
}