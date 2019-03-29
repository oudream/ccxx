#ifndef CXLOG_H
#define CXLOG_H

#include "cxglobal.h"

#include "cxinterinfo.h"
#include "cxthread.h"

class CxLogFileTactics_I
{
public:
    virtual std::string log_directory() const = 0;

    virtual std::string log_fileName( int type, int reason, int source, int target, int tag ) const = 0;

    virtual bool log_hasFileName( int type, int reason, int source, int target, int tag ) const = 0;

};

class GM_CCXX_CORE_API CxLogManager
{
public:
    static void startLog();

    static void stopLog();

    static void loadLogConfig();

    static void setLogFileTactics(const CxLogFileTactics_I * oLogFileTactics);

    static std::string fileName(int type, int reason, int source, int target, int tag);

    static FILE * fileByFileName(const std::string& sFileName);

    static void outLog( const std::string& sInfo, const std::string& sTitle, int type = 0, int reason = 0, int source = 0, int target = 0, int tag = 0 );

};

class GM_CCXX_CORE_API CxLogRealtime : public CxInterinfoOut_I
{
public:
    CxLogRealtime();

    ~CxLogRealtime();

protected:
    void interinfo_out( const std::string& sInfo, const std::string& sTitle, int type = 0, int reason = 0, int source = 0, int target = 0, int tag = 0 );

    CxInterinfo::PlatformEnum platformValue() { return CxInterinfo::Platform_Log; }

private:
    //*file path
    std::string _filePathRt;
    FILE * _fileRt;
    msepoch_t _fileOpenTimeRt;
    int64 _fileCountRt;
    int64 _fileSizeRt;

    friend class CxLogManager;

};

class GM_CCXX_CORE_API CxLogThread : public CxJoinableThread , public CxInterinfoOut_I
{
public:
    CxLogThread();

    ~CxLogThread();

    inline void stop() { _isStarted = false; join(); }

protected:
    void run();

    void interinfo_out( const std::string& sInfo, const std::string& sTitle, int type = 0, int reason = 0, int source = 0, int target = 0, int tag = 0 );

    CxInterinfo::PlatformEnum platformValue() { return CxInterinfo::Platform_Log; }

private:
    void doSaveLog();

private:
    volatile bool _isStarted;
    std::vector<std::string> * volatile _logStringsPush;
    std::vector<std::string> * volatile _logStringsPop;
    std::vector<std::string> _logStrings1;
    std::vector<std::string> _logStrings2;
    std::vector<FILE*> * volatile _logFilesPush;
    std::vector<FILE*> * volatile _logFilesPop;
    std::vector<FILE*> _logFiles1;
    std::vector<FILE*> _logFiles2;
    volatile size_t _logStringsSize;
    std::string _filePathTh;
    FILE * _fileTh;
    msepoch_t _fileOpenTimeTh;
    int64 _fileCountTh;
    int64 _fileSizeTh;

    friend class CxLogManager;

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
                CxLogManager::outLog(stream->ts.str(), stream->title, stream->type, stream->reason, stream->source, stream->target, stream->tag);
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
                CxLogManager::outLog(stream->ts.str(), stream->title, stream->type, stream->reason, stream->source, stream->target, stream->tag);
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

class CxLogNoOutStream : public CxNoOutStreamBase {

};


inline CxLogOutStream cxLog(CxInterinfo::LevelEnum eLevel=CxInterinfo::LevelDebug) { CxLogOutStream r; r.noend(); r.level(eLevel); return r; }
inline CxLogOutStream cxLogDebug() { CxLogOutStream r; r.noend(); r.level(CxInterinfo::LevelDebug); return r; }
inline CxLogOutStream cxLogInfo() { CxLogOutStream r; r.noend(); r.level(CxInterinfo::LevelInfo); return r; }
inline CxLogOutStream cxLogWarn() { CxLogOutStream r; r.noend(); r.level(CxInterinfo::LevelWarn); return r; }
inline CxLogOutStream cxLogError() { CxLogOutStream r; r.noend(); r.level(CxInterinfo::LevelError); return r; }
inline CxLogOutStream cxLogFatal() { CxLogOutStream r; r.noend(); r.level(CxInterinfo::LevelFatal); return r; }

#endif // CXLOG_H
