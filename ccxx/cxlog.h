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

class CxLogManager
{
public:
    static void setLogFileTactics(const CxLogFileTactics_I * oLogFileTactics);

    static std::string fileName(int type, int reason, int source, int target, int tag);

    static FILE * fileByFileName(const std::string& sFileName);

};

class CxLogRealtime : public CxInterinfoOut_I
{
public:
    static void startLog();

    static void stopLog();

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

};

class CxLogThread : public CxJoinableThread , public CxInterinfoOut_I
{
public:
    static void startLog();

    static void stopLog();

    CxLogThread();

    ~CxLogThread();

protected:
    void run();

    inline void waitExit() { join(); }

    void interinfo_out( const std::string& sInfo, const std::string& sTitle, int type = 0, int reason = 0, int source = 0, int target = 0, int tag = 0 );

    CxInterinfo::PlatformEnum platformValue() { return CxInterinfo::Platform_Log; }

private:
    volatile bool _stop;
    CxMutex _lockTh;
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

};

#endif // CXLOG_H
