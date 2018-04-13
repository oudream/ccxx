#include "cxfile.h"

#ifdef GM_OS_WIN
#include <direct.h>
#include <winioctl.h>
#include <io.h>
// removed from some sdk versions...
struct LOCAL_REPARSE_DATA_BUFFER {
    DWORD ReparseTag;
    WORD ReparseDataLength;
    WORD Reserved;

    // IO_REPARSE_TAG_MOUNT_POINT specifics follow
    WORD SubstituteNameOffset;
    WORD SubstituteNameLength;
    WORD PrintNameOffset;
    WORD PrintNameLength;
    WCHAR PathBuffer[1];
};
#else
#include <fcntl.h>
#include <dirent.h>
#include <dlfcn.h>
#ifndef RTLD_GLOBAL
#define RTLD_GLOBAL 0
#endif
#endif

#if !defined(__S_IFMT)
#ifdef _S_IFMT
#define __S_IFMT _S_IFMT
#else
#define __S_IFMT 0
#endif
#endif

#if !defined(__S_IFREG)
#ifdef _S_IFREG
#define __S_IFREG _S_IFREG
#else
#define __S_IFREG 1
#endif
#endif

#if !defined(__S_IFDIR)
#ifdef _S_IFDIR
#define __S_IFDIR _S_IFDIR
#else
#define __S_IFDIR 2
#endif
#endif

#if !defined(__S_IFLNK)
#ifdef _S_IFLNK
#define __S_IFLNK _S_IFLNK
#else
#define __S_IFLNK 3
#endif
#endif

#if !defined(__S_IFCHR)
#ifdef _S_IFCHR
#define __S_IFCHR _S_IFCHR
#else
#define __S_IFCHR 4
#endif
#endif

#if !defined(__S_IFBLK)
#ifdef _S_IFBLK
#define __S_IFBLK _S_IFBLK
#else
#define __S_IFBLK 5
#endif
#endif

#if !defined(__S_IFFIFO)
#ifdef _S_IFFIFO
#define __S_IFFIFO _S_IFFIFO
#else
#define __S_IFFIFO 6
#endif
#endif

#if !defined(__S_IFSOCK)
#ifdef _S_IFSOCK
#define __S_IFSOCK _S_IFSOCK
#else
#define __S_IFSOCK 7
#endif
#endif

using namespace std;

//from cplusplus.com sample
bool CxFile::load(const string &sFilePath, std::string &sOut, size_t iMaxSize)
{
    FILE *pFile;
    size_t iFileSize;
    size_t iBufferSize;
    size_t iResult;

    pFile = fopen(sFilePath.data(), "rb");
    if (pFile == NULL)
    {
//        fputs ("File error",stderr);
        return false;
    }

    // obtain file size:
    fseek(pFile, 0, SEEK_END);
    iFileSize = ftell(pFile);
    if (iFileSize <= 0 || iFileSize > iMaxSize)
    {
//        fputs ("file is empty",stderr);
        fclose(pFile);
        return false;
    }

    rewind(pFile);

    // allocate memory to contain the whole file:
    iBufferSize = sizeof(char) * iFileSize + 1;
    size_t iOldSize = sOut.size();
    sOut.resize(sOut.size() + iBufferSize);
    if (sOut.size() <= iOldSize)
    {
//        fputs ("Memory error",stderr);
        fclose(pFile);
        return false;
    }

    // copy the file into the buffer:
    iResult = fread(const_cast<char *>(sOut.data() + iOldSize), 1, iFileSize, pFile);
    if (iResult != iFileSize)
    {
//        fputs ("Reading error",stderr);
        fclose(pFile);
        return false;
    }

    /* the whole file is now loaded in the memory buffer. */
    fclose(pFile);
    return true;
}

int compareints(const void *a, const void *b)
{
    return (*(short *) a - *(short *) b);
}

//from cplusplus.com sample
int CxFile::load(const string &sFilePath, std::vector<std::string> &sOut, const string &sSplitString)
{
    FILE *pFile;
    size_t iFileSize;
    size_t iOldSize = sOut.size();

    pFile = fopen(sFilePath.data(), "rb");
    if (pFile == NULL)
    {
        return sOut.size() - iOldSize;
    }

    // obtain file size:
    fseek(pFile, 0, SEEK_END);
    iFileSize = ftell(pFile);
    if (iFileSize <= 0)
    {
//        fputs ("file is empty",stderr);
        fclose(pFile);
        return sOut.size() - iOldSize;
    }
    rewind(pFile);

    const size_t ci_bufferSize = 512;
    vector<char> buffer(ci_bufferSize, 0);
    char *pBuffer = &(buffer.front());
    string sLine;
    size_t iReadSize = 0;
    while (iReadSize < iFileSize)
    {
        size_t iRemain = iFileSize - iReadSize;
        size_t iReadingLen = (iRemain > ci_bufferSize) ? ci_bufferSize : iRemain;

        // copy the file into the buffer:
        if (fread(pBuffer, 1, iReadingLen, pFile) != iReadingLen)
        {
            // read to file end
            fclose(pFile);
            return sOut.size() - iOldSize;
        }
        else
        {
            sLine.append(string(pBuffer, iReadingLen));

            size_t start = 0;
            size_t end;
//            while ((end = CxString::search(sLine.data()+start, sLine.size()-start, sSplitString.data(), sSplitString.size())) != string::npos)
            while ((end = sLine.find(sSplitString, start)) != string::npos)
            {
                if (start != end)
                    sOut.push_back(sLine.substr(start, end - start));
                else
                    sOut.push_back(string());
                start = end + sSplitString.size();
            }
            if (start > 0)
            {
                sLine = sLine.substr(start);
            }
        }
        iReadSize += iReadingLen;
    }
    if (sLine.size() > 0)
    {
        sOut.push_back(sLine);
    }

    /* the whole file is now loaded in the memory buffer. */
    fclose(pFile);
    return sOut.size() - iOldSize;
}

//from cplusplus.com sample
int CxFile::load(const string &sFilePath, std::vector<std::string> &sOut, char cSplit)
{
    FILE *pFile;
    size_t iFileSize;
    size_t iOldSize = sOut.size();

    pFile = fopen(sFilePath.data(), "rb");
    if (pFile == NULL)
    {
        return sOut.size() - iOldSize;
    }

    // obtain file size:
    fseek(pFile, 0, SEEK_END);
    iFileSize = ftell(pFile);
    if (iFileSize <= 0)
    {
        //        fputs ("file is empty",stderr);
        fclose(pFile);
        return sOut.size() - iOldSize;
    }
    rewind(pFile);

    const size_t ci_bufferSize = 512;
    vector<char> buffer(ci_bufferSize, 0);
    char *pBuffer = &(buffer.front());
    string sLine;
    size_t iReadSize = 0;
    while (iReadSize < iFileSize)
    {
        size_t iRemain = iFileSize - iReadSize;
        size_t iReadingLen = (iRemain > ci_bufferSize) ? ci_bufferSize : iRemain;

        // copy the file into the buffer:
        if (fread(pBuffer, 1, iReadingLen, pFile) != iReadingLen)
        {
            // read to file end
            fclose(pFile);
            return sOut.size() - iOldSize;
        }
        else
        {
            sLine.append(string(pBuffer, iReadingLen));

            size_t start = 0;
            size_t end;
            while ((end = sLine.find(cSplit, start)) != string::npos)
            {
                if (start != end)
                    sOut.push_back(sLine.substr(start, end - start));
                start = end + 1;
            }
            if (start > 0)
            {
                sLine = sLine.substr(start);
            }
        }
        iReadSize += iReadingLen;
    }
    if (sLine.size() > 0)
    {
        sOut.push_back(sLine);
    }

    /* the whole file is now loaded in the memory buffer. */
    fclose(pFile);
    return sOut.size() - iOldSize;
}

int CxFile::load(const string &sFilePath, std::vector<string> &sOut, int iSectionLength)
{
    FILE *pFile;
    size_t iFileSize;
    size_t iOldSize = sOut.size();

    pFile = fopen(sFilePath.data(), "rb");
    if (pFile == NULL)
    {
        return sOut.size() - iOldSize;
    }

    // obtain file size:
    fseek(pFile, 0, SEEK_END);
    iFileSize = ftell(pFile);
    if (iFileSize <= 0)
    {
        //        fputs ("file is empty",stderr);
        fclose(pFile);
        return sOut.size() - iOldSize;
    }
    rewind(pFile);

    const size_t ci_bufferSize = iSectionLength;
    vector<char> buffer(ci_bufferSize, 0);
    char *pBuffer = &(buffer.front());
    size_t iReadSize = 0;
    while (iReadSize < iFileSize)
    {
        size_t iRemain = iFileSize - iReadSize;
        size_t iReadingLen = (iRemain > ci_bufferSize) ? ci_bufferSize : iRemain;

        // copy the file into the buffer:
        if (fread(pBuffer, 1, iReadingLen, pFile) != iReadingLen)
        {
            // read to file end
            fclose(pFile);
            return sOut.size() - iOldSize;
        }
        else
        {
            sOut.push_back(string(pBuffer, iReadingLen));
        }
        iReadSize += iReadingLen;
    }

    /* the whole file is now loaded in the memory buffer. */
    fclose(pFile);
    return sOut.size() - iOldSize;
}

int CxFile::load(const string &sFilePath, void *oObject, fn_int_object_tlv_t fn)
{
    FILE *pFile;
    size_t iFileSize;

    pFile = fopen(sFilePath.data(), "rb");
    if (pFile == NULL)
    {
        return 0;
    }

    // obtain file size:
    fseek(pFile, 0, SEEK_END);
    iFileSize = ftell(pFile);
    if (iFileSize <= 0)
    {
//        fputs ("file is empty",stderr);
        fclose(pFile);
        return 0;
    }
    rewind(pFile);

    const size_t ci_bufferSize = 512;
    vector<char> buffer(ci_bufferSize, 0);
    char *pBuffer = &(buffer.front());
    size_t iReadSize = 0;
    while (iReadSize < iFileSize)
    {
        size_t iRemain = iFileSize - iReadSize;
        size_t iReadingLen = (iRemain > ci_bufferSize) ? ci_bufferSize : iRemain;

        // copy the file into the buffer:
        if (fread(pBuffer, 1, iReadingLen, pFile) != iReadingLen)
        {
            // read to file end
            fclose(pFile);
            return iReadSize;
        }
        else
        {
            fn(oObject, 0, iReadingLen, pBuffer);
        }
        iReadSize += iReadingLen;
    }

    /* the whole file is now loaded in the memory buffer. */
    fclose(pFile);
    return iReadSize;
}

int CxFile::loadFirst(const string &sFilePath, size_t iSize, std::vector<std::string> &sOut, const string &sSplitString)
{
    FILE *pFile;
    size_t iFileSize;
    size_t iOldSize = sOut.size();

    pFile = fopen(sFilePath.data(), "rb");
    if (pFile == NULL)
    {
        return sOut.size() - iOldSize;
    }

    // obtain file size:
    fseek(pFile, 0, SEEK_END);
    iFileSize = ftell(pFile);
    if (iFileSize <= 0)
    {
//        fputs ("file is empty",stderr);
        fclose(pFile);
        return sOut.size() - iOldSize;
    }
    rewind(pFile);

    const size_t ci_bufferSize = 512;
    vector<char> buffer(ci_bufferSize, 0);
    char *pBuffer = &(buffer.front());
    string sLine;
    size_t iReadSize = 0;
    size_t iMaxSize = iSize > iFileSize ? iFileSize : iSize;
    while (iReadSize < iMaxSize)
    {
        size_t iRemain = iMaxSize - iReadSize;
        size_t iReadingLen = (iRemain > ci_bufferSize) ? ci_bufferSize : iRemain;

        // copy the file into the buffer:
        if (fread(pBuffer, 1, iReadingLen, pFile) != iReadingLen)
        {
            // read to file end
            fclose(pFile);
            return sOut.size() - iOldSize;
        }
        else
        {
            sLine.append(string(pBuffer, iReadingLen));

            size_t start = 0;
            size_t end;
//            while ((end = CxString::search(sLine.data()+start, sLine.size()-start, sSplitString.data(), sSplitString.size())) != string::npos)
            while ((end = sLine.find(sSplitString, start)) != string::npos)
            {
                if (start != end)
                    sOut.push_back(sLine.substr(start, end - start));
                else
                    sOut.push_back(string());
                start = end + sSplitString.size();
            }
            if (start > 0)
            {
                sLine = sLine.substr(start);
            }
        }
        iReadSize += iReadingLen;
    }
    if (sLine.size() > 0)
    {
        sOut.push_back(sLine);
    }

    /* the whole file is now loaded in the memory buffer. */
    fclose(pFile);
    return sOut.size() - iOldSize;
}

int CxFile::loadLast(const string &sFilePath, size_t iSize, std::vector<std::string> &sOut, const string &sSplitString)
{
    FILE *pFile;
    size_t iFileSize;
    size_t iOldSize = sOut.size();

    pFile = fopen(sFilePath.data(), "rb");
    if (pFile == NULL)
    {
        return sOut.size() - iOldSize;
    }

    // obtain file size:
    fseek(pFile, 0, SEEK_END);
    iFileSize = ftell(pFile);
    if (iFileSize <= 0)
    {
//        fputs ("file is empty",stderr);
        fclose(pFile);
        return sOut.size() - iOldSize;
    }
    rewind(pFile);
    size_t iOffset = iSize > iFileSize ? 0 : iFileSize - iSize;
    fseek(pFile, iOffset, SEEK_SET );

    const size_t ci_bufferSize = 512;
    vector<char> buffer(ci_bufferSize, 0);
    char *pBuffer = &(buffer.front());
    string sLine;
    size_t iReadSize = 0;
    while (iReadSize < iSize)
    {
        size_t iRemain = iSize - iReadSize;
        size_t iReadingLen = (iRemain > ci_bufferSize) ? ci_bufferSize : iRemain;

        // copy the file into the buffer:
        if (fread(pBuffer, 1, iReadingLen, pFile) != iReadingLen)
        {
            // read to file end
            fclose(pFile);
            return sOut.size() - iOldSize;
        }
        else
        {
            sLine.append(string(pBuffer, iReadingLen));

            size_t start = 0;
            size_t end;
//            while ((end = CxString::search(sLine.data()+start, sLine.size()-start, sSplitString.data(), sSplitString.size())) != string::npos)
            while ((end = sLine.find(sSplitString, start)) != string::npos)
            {
                if (start != end)
                    sOut.push_back(sLine.substr(start, end - start));
                else
                    sOut.push_back(string());
                start = end + sSplitString.size();
            }
            if (start > 0)
            {
                sLine = sLine.substr(start);
            }
        }
        iReadSize += iReadingLen;
    }
    if (sLine.size() > 0)
    {
        sOut.push_back(sLine);
    }

    /* the whole file is now loaded in the memory buffer. */
    fclose(pFile);
    return sOut.size() - iOldSize;
}

//from cplusplus.com sample
bool CxFile::save(const string &sFilePath, const string &sBuffer)
{
    FILE *pFile;
    pFile = fopen(sFilePath.data(), "wb");
    if (pFile == NULL) return false;
    rewind(pFile);
    size_t iWrote = fwrite(const_cast<char *>(sBuffer.data()), 1, sBuffer.size(), pFile);
    fclose(pFile);
    return iWrote == sBuffer.size();
}

//from cplusplus.com sample
bool CxFile::save(const string &sFilePath, const vector<string> &sStrings, const string &sSplitString, bool saveReturn)
{
    FILE *pFile;
    pFile = fopen(sFilePath.data(), "wb");
    if (pFile == NULL) return false;
    rewind(pFile);
    size_t iStringsSize = 0;
    size_t iWrote = 0;
    for (size_t i = 0; i < sStrings.size(); ++i)
    {
        string s = sStrings[i] + sSplitString;
        iStringsSize += s.size();
        iWrote += fwrite(const_cast<char *>(s.data()), 1, s.size(), pFile);
        if (iWrote != iStringsSize)
            return false;
        if (saveReturn)
        {
            if (fflush(pFile) != 0)
                return false;
        }
    }
    fclose(pFile);
    return iWrote == iStringsSize;
}

int CxFileSystem::getFileLastError()
{
#ifdef GM_OS_WIN
    DWORD err = GetLastError();

    switch (err)
    {
        case ERROR_FILE_NOT_FOUND:
        case ERROR_PATH_NOT_FOUND:
        case ERROR_INVALID_NAME:
        case ERROR_BAD_PATHNAME:
            return ENOENT;
        case ERROR_TOO_MANY_OPEN_FILES:
            return EMFILE;
        case ERROR_ACCESS_DENIED:
        case ERROR_WRITE_PROTECT:
        case ERROR_SHARING_VIOLATION:
        case ERROR_LOCK_VIOLATION:
            return EACCES;
        case ERROR_INVALID_HANDLE:
            return EBADF;
        case ERROR_NOT_ENOUGH_MEMORY:
        case ERROR_OUTOFMEMORY:
            return ENOMEM;
        case ERROR_INVALID_DRIVE:
        case ERROR_BAD_UNIT:
        case ERROR_BAD_DEVICE:
            return ENODEV;
        case ERROR_NOT_SAME_DEVICE:
            return EXDEV;
        case ERROR_NOT_SUPPORTED:
        case ERROR_CALL_NOT_IMPLEMENTED:
            return ENOSYS;
        case ERROR_END_OF_MEDIA:
        case ERROR_EOM_OVERFLOW:
        case ERROR_HANDLE_DISK_FULL:
        case ERROR_DISK_FULL:
            return ENOSPC;
        case ERROR_BAD_NETPATH:
        case ERROR_BAD_NET_NAME:
            return EACCES;
        case ERROR_FILE_EXISTS:
        case ERROR_ALREADY_EXISTS:
            return EEXIST;
        case ERROR_CANNOT_MAKE:
        case ERROR_NOT_OWNER:
            return EPERM;
        case ERROR_NO_PROC_SLOTS:
            return EAGAIN;
        case ERROR_BROKEN_PIPE:
        case ERROR_NO_DATA:
            return EPIPE;
        case ERROR_OPEN_FAILED:
            return EIO;
        case ERROR_NOACCESS:
            return EFAULT;
        case ERROR_IO_DEVICE:
        case ERROR_CRC:
        case ERROR_NO_SIGNAL_SENT:
            return EIO;
        case ERROR_CHILD_NOT_COMPLETE:
        case ERROR_SIGNAL_PENDING:
        case ERROR_BUSY:
            return EBUSY;
        case ERROR_DIR_NOT_EMPTY:
            return ENOTEMPTY;
        case ERROR_DIRECTORY:
            return ENOTDIR;
        default:
            return EINVAL;
    }
#else
    return errno;
#endif
}

bool CxFileSystem::setCurrentDir(const string &sDir)
{
#ifdef GM_OS_WIN
//attention : it use io.h
//    if (_chdir(sDir.c_str()))
//        return getFileLastError();
//    return true;
    return SetCurrentDirectory(sDir.c_str());
#else
    return ::chdir(sDir.c_str()) == 0;
#endif
}

string CxFileSystem::getCurrentDir()
{
    const size_t nMaxPath = 260;
    char sPath[nMaxPath];
#ifdef GM_OS_WIN
    DWORD dwRet = GetCurrentDirectory(nMaxPath, sPath);
    if (dwRet > 0 && dwRet <= nMaxPath)
        return string(sPath, dwRet);
    else
        return std::string();
#else
    if(::getcwd(sPath, nMaxPath))
        return string(sPath);
    return std::string();
#endif
}

bool CxFileSystem::deleteFile(const string &sPath)
{
    const char *path = sPath.c_str();

//    if (isDevice(path))
//        return ENOSYS;
//    if(::remove(path))
//        return getFileLastError();
//    return 0;

    if (!isDevice(path) && ::remove(path) == 0)
        return true;
    else
        return false;
}

int CxFileSystem::copyFile(const string &sSource, string &sTarget, bool bOvert, int *iSourceSize)
{
    const char *oldpath = sSource.c_str();
    const char *newpath = sTarget.c_str();

    int result = 0;
    char buffer[1024];
    FILE *src;
    FILE *dest;

    src = fopen(oldpath, "rb");
    if (src == NULL)
        return result;

    dest = fopen(newpath, "wb");
    if (dest == NULL)
    {
        fclose(src);
        return result;
    }
    if (bOvert)
        rewind(dest);

    if (iSourceSize)
    {
        fseek(src, 0, SEEK_END);
        *iSourceSize = ftell(src);
    }
    rewind(src);
    ssize_t count = sizeof(buffer);
    while (count > 0)
    {
        count = fread(buffer, sizeof(char), sizeof(buffer), src);
        if (count < 0)
        {
            break;
        }
        if (count > 0)
        {
            count = fwrite(buffer, sizeof(char), count, dest);
        }
        if (count < 0)
        {
            break;
        }
        result += count;
    }

    if (src)
        fclose(src);

    if (dest)
        fclose(dest);

    //todo
    result++;
    return result;
}

bool CxFileSystem::renameFile(const string &sOldPath, const string &sNewPath)
{
    const char *oldpath = sOldPath.c_str();
    const char *newpath = sNewPath.c_str();
//    if(::rename(oldpath, newpath))
//        return getFileLastError();
//    return 0;
    if (::rename(oldpath, newpath) == 0)
        return true;
    return false;
}

bool CxFileSystem::changeMode(const string &sPath, unsigned value)
{
#ifdef GM_OS_WIN
//attention : it use stat.h
//    if(_chmod(path, value))
//        return getFileLastError();
//    return 0;
    DWORD attr = GetFileAttributes(sPath.c_str());
    if (attr == INVALID_FILE_ATTRIBUTES)
        return false;
    if (value & (GM_FILE_MODE_IWRITE | GM_FILE_MODE_IEXEC))
    {
        return SetFileAttributes(sPath.c_str(), attr | FILE_ATTRIBUTE_READONLY);
    }
    return true;
#else
    return ::chmod(sPath.c_str(), value) == 0;
#endif
}

bool CxFileSystem::isExist(const string &sPath)
{
#ifdef GM_OS_WIN
//attention : it use stat.h
//    if(_access(sPath.c_str(), F_OK))
//        return false;
//    return true;
    DWORD attr = GetFileAttributes(sPath.c_str());
    if (attr == INVALID_FILE_ATTRIBUTES)
        return false;
    return true;
#else
    if(::access(sPath.c_str(), F_OK))
        return false;

    return true;
#endif
}

bool CxFileSystem::isExist(const string &sPath, const string &sDir)
{
    if (hasRootPath(sPath))
        return isExist(sPath);
    if (sDir.size() > 0)
        return isExist(mergeFilePath(sDir, sPath));
    return isExist(sPath);
}

bool CxFileSystem::isExist(const string &sPath, const vector<string> &sDirs)
{
    if (hasRootPath(sPath))
        return isExist(sPath);
    if (isExist(sPath))
        return true;
    for (size_t i = 0; i < sDirs.size(); ++i)
    {
        if (isExist(mergeFilePath(sDirs.at(i), sPath)))
            return true;
    }
    return false;
}

bool CxFileSystem::isReadable(const string &sPath)
{
#ifdef GM_OS_WIN
//attention : it use stat.h
//    if(_access(path, R_OK))
//        return false;
//    return true;
    DWORD attr = GetFileAttributes(sPath.c_str());
    if (attr == INVALID_FILE_ATTRIBUTES)
        return false;
    return true;
#else
    if(::access(sPath.c_str(), R_OK))
        return false;
    return true;
#endif
}

bool CxFileSystem::isWritable(const std::string &sPath)
{
#ifdef GM_OS_WIN
//attention : it use stat.h
//    if(_access(path, W_OK))
//        return false;
//    return true;
    DWORD attr = GetFileAttributes(sPath.c_str());
    if (attr == INVALID_FILE_ATTRIBUTES)
        return false;
    if (attr & FILE_ATTRIBUTE_READONLY)
        return false;
    return true;
#else
    if(::access(sPath.c_str(), W_OK))
        return false;
    return true;
#endif
}

bool CxFileSystem::isExecutable(const string &sPath)
{
    const char *path = sPath.c_str();
#ifdef GM_OS_WIN
    path = strrchr(path, '.');
    if (!path)
        return false;
    string sSuffixName = CxString::toLower(path);
    if (sSuffixName == ".exe")
        return true;
    if (sSuffixName == ".bat")
        return true;
    if (sSuffixName == ".com")
        return true;
    if (sSuffixName == ".cmd")
        return true;
    if (sSuffixName == ".ps1")
        return true;
    return false;
#else
    if(isDir(path))
        return false;
    if(::access(path, X_OK))
        return false;
    return true;
#endif
}

#ifdef GM_OS_WIN

inline bool fn_isFile(const DWORD &attr)
{
    if (attr == (DWORD) ~0l)
        return false;
    if (attr & FILE_ATTRIBUTE_DIRECTORY)
        return false;
    return true;
}

inline bool fn_isDir(const DWORD &attr)
{
    if (attr == (DWORD) ~0l)
        return false;
    if (attr & FILE_ATTRIBUTE_DIRECTORY)
        return true;
    return false;
}

inline bool fn_isLink(const DWORD &attr, const string &sPath)
{
    if (attr == 0xffffffff || attr == (DWORD) ~0l) return false;
    if (attr & FILE_ATTRIBUTE_REPARSE_POINT) return true;
    const char *path = sPath.c_str();
    path = strrchr(path, '.');
    if (!path)
        return false;
    string sSuffixName = CxString::toLower(path);
    if (sSuffixName == ".lnk")
        return true;
    else
        return false;
}

bool fn_isHidden(const DWORD &attr)
{
    if (attr == (DWORD) ~0l)
        return false;
    return ((attr & FILE_ATTRIBUTE_HIDDEN) != 0);
}

msepoch_t fn_toMsepoch(const FILETIME &filetime)
{
    const msepoch_t iShift = 116444736000000000LL;
    union {
        FILETIME as_file_time;
        msepoch_t as_integer;
    } caster;
    caster.as_file_time = filetime;
    caster.as_integer -= iShift;
    return caster.as_integer / 10000LL;
}

inline CxFileSystem::PathTypeEnum fn_getPathType(const string &sPath, DWORD attr)
{
    if (attr == INVALID_FILE_ATTRIBUTES)
        return CxFileSystem::PathTypeNone;
    else if (fn_isFile(attr))
        return CxFileSystem::PathTypeFile;
    else if (fn_isDir(attr))
        return CxFileSystem::PathTypeDir;
    else if (fn_isLink(attr, sPath))
        return CxFileSystem::PathTypeSoftlink;
    else if (CxFileSystem::isDevice(sPath))
        return CxFileSystem::PathTypeDevChar;
    else
        return CxFileSystem::PathTypeSystem;
}

#else

inline CxFileSystem::PathTypeEnum fn_getPathType(ushort mode)
{
    ushort nMode = ((mode) & __S_IFMT);
    switch (nMode)
    {
    case __S_IFREG:
        return CxFileSystem::PathTypeFile;
        break;
    case __S_IFDIR:
        return CxFileSystem::PathTypeDir;
        break;
    case __S_IFLNK:
        return CxFileSystem::PathTypeSoftlink;
        break;
    case __S_IFCHR:
        return CxFileSystem::PathTypeDevChar;
        break;
    case __S_IFBLK:
        return CxFileSystem::PathTypeDevBlock;
        break;
    case __S_IFFIFO:
        return CxFileSystem::PathTypeSystem;
        break;
    case __S_IFSOCK:
        return CxFileSystem::PathTypeSystem;
        break;
    default:
        return CxFileSystem::PathTypeNone;
        break;
    }
}

#endif // fn_xxx

bool CxFileSystem::isFile(const string &sPath)
{
    const char *path = sPath.c_str();
#ifdef GM_OS_WIN
    DWORD attr = GetFileAttributes(path);
    return fn_isFile(attr);
#else
    struct stat ino;
    if(::stat(path, &ino))
        return false;
    if(fn_getPathType(ino.st_mode)==PathTypeFile)
        return true;
    return false;
#endif
}

bool CxFileSystem::isDir(const string &sPath)
{
    const char *path = sPath.c_str();
#ifdef GM_OS_WIN
    DWORD attr = GetFileAttributes(path);
    return fn_isDir(attr);
#else
    struct stat ino;
    if(::stat(path, &ino))
        return false;
    if(fn_getPathType(ino.st_mode)==PathTypeDir)
        return true;
    return false;
#endif
}

bool CxFileSystem::isLink(const string &sPath)
{
    const char *path = sPath.c_str();
#ifdef GM_OS_WIN
    DWORD attr = GetFileAttributes(path);
    return fn_isLink(attr, sPath);
#else
    struct stat ino;
    if(::lstat(path, &ino))
        return false;
    if(fn_getPathType(ino.st_mode)==PathTypeSoftlink)
        return true;
    return false;
#endif
}

bool CxFileSystem::isDevice(const string &sPath)
{
    const char *path = sPath.c_str();
    if (sPath.size() < 3)
        return false;
#ifdef GM_OS_WIN
    if (path[1] == ':' && !path[2] && isalpha(*path))
        return true;

    if (!strncmp(path, "com", 3) || !strncmp(path, "lpt", 3))
    {
        path += 3;
        while (isdigit(*path))
            ++path;
        if (!path || *path == ':')
            return true;
        return false;
    }

    if (!strcmp(path, "aux") || !strcmp(path, "prn"))
    {
        if (!path[3] || path[3] == ':')
            return true;
        return false;
    }

    if (!strncmp(path, "\\\\.\\", 4))
        return true;

    if (!cx_strnicmp(path, "\\\\?\\Device\\", 12))
        return true;

    return false;
#else
    if(isDir(path))
        return false;

    if(!strncmp(path, "/dev/", 5))
        return true;

    return false;
#endif
}

bool CxFileSystem::isHidden(const string &sPath)
{
    const char *path = sPath.c_str();
#ifdef  GM_OS_WIN
    DWORD attr = GetFileAttributes(path);
    return fn_isHidden(attr);
#else
    const char *cp = strrchr(path, '/');
    if(cp)
        ++cp;
    else
        cp = path;

    if(*cp == '.')
        return true;

    return false;
#endif
}

bool CxFileSystem::getPathInfo(const string &sFilePath, CxFileSystem::PathInfo *pathInfo)
{
    if (!pathInfo)
        return false;
#ifdef GM_OS_WIN
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind = ::FindFirstFile(sFilePath.c_str(), &FindFileData);
    if (INVALID_HANDLE_VALUE != hFind)
    {
        pathInfo->pathType = fn_getPathType(sFilePath, FindFileData.dwFileAttributes);
        pathInfo->path = extractPath(sFilePath);
        pathInfo->fileName = extractFileName(sFilePath);
        pathInfo->fileSize = FindFileData.nFileSizeHigh * (MAXDWORD + 1);
        pathInfo->fileSize += FindFileData.nFileSizeLow;
        pathInfo->creationTime = fn_toMsepoch(FindFileData.ftCreationTime);
        pathInfo->lastWriteTime = fn_toMsepoch(FindFileData.ftLastWriteTime);
        FindClose(hFind);
        return true;
    }
#else
    struct stat stat1;
    if(::stat(sFilePath.data(), &stat1))
    {
        return false;
    }
    else
    {
        pathInfo->pathType = fn_getPathType(stat1.st_mode);
        pathInfo->path = extractPath(sFilePath);
        pathInfo->fileName = extractFileName(sFilePath);
        pathInfo->fileSize = stat1.st_size;
        pathInfo->creationTime = stat1.st_ctime * 1000;
        pathInfo->lastWriteTime = stat1.st_mtime * 1000;
        return true;
    }
#endif
    return false;
}

CxFileSystem::PathInfo CxFileSystem::getPathInfo(const string &sFilePath)
{
    CxFileSystem::PathInfo pathInfo;
    getPathInfo(sFilePath, &pathInfo);
    return pathInfo;
}

std::string CxFileSystem::getPathTypeString(PathTypeEnum pathType)
{
    switch (pathType)
    {
        case PathTypeFile:
            return "file";
            break;
        case PathTypeDir:
            return "directory";
            break;
        case PathTypeSoftlink:
            return "link";
            break;
        case PathTypeDevChar:
            return "io";
            break;
        case PathTypeDevBlock:
            return "disk";
            break;
        case PathTypeSystem:
            return "system";
            break;
        default:
            return "none";
            break;
    }
}

bool CxFileSystem::createLink(const std::string &sPath, const std::string &sTarget)
{
    const char *path = sPath.c_str();
    const char *target = sTarget.c_str();
#ifdef GM_OS_WIN
    TCHAR dest[512];
    HANDLE h;
    char reparse[MAXIMUM_REPARSE_DATA_BUFFER_SIZE];
    char *part;
    DWORD size;
    WORD len;

    lstrcpy(dest, "\\??\\");
    if (!GetFullPathName(path, sizeof(dest) - (4 * sizeof(TCHAR)), &dest[4], &part)
        || GetFileAttributes(&dest[4]) == INVALID_FILE_ATTRIBUTES)
        return false;

    memset(reparse, 0, sizeof(reparse));
    LOCAL_REPARSE_DATA_BUFFER *rb = (LOCAL_REPARSE_DATA_BUFFER *) &reparse;

    if (!MultiByteToWideChar(CP_THREAD_ACP, MB_PRECOMPOSED, dest,
                             lstrlenA(dest) + 1, rb->PathBuffer, lstrlenA(dest) + 1))
        return false;

    len = lstrlenW(rb->PathBuffer) * 2;
    rb->ReparseTag = IO_REPARSE_TAG_MOUNT_POINT;
    rb->ReparseDataLength = len + 12;
    rb->SubstituteNameLength = len;
    rb->PrintNameOffset = len + 2;
    h = CreateFile(target, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING,
                   FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT, 0);
    if (!h || h == INVALID_HANDLE_VALUE)
        return createHardlink(path, target);
    if (!DeviceIoControl(h, FSCTL_SET_REPARSE_POINT, (LPVOID) rb, rb->ReparseDataLength
                                                                  + FIELD_OFFSET(LOCAL_REPARSE_DATA_BUFFER, SubstituteNameOffset), NULL, 0, &size, 0))
    {
        CloseHandle(h);
        return createHardlink(path, target);
    }
    CloseHandle(h);
    return true;
#elif defined(HAVE_SYMLINK)
    if(::symlink(path, target))
        return false;
    return true;
#else
    if(::link(path, target))
        return false;
    return true;
#endif
}

bool CxFileSystem::unlink(const string &sPath)
{
    const char *path = sPath.c_str();
#ifdef GM_OS_WIN
    HANDLE h = INVALID_HANDLE_VALUE;
    if (isLink(path))
    {
        h = CreateFile(path, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING,
                       FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT, 0);
    }
    if (!h || h != INVALID_HANDLE_VALUE)
    {
        REPARSE_GUID_DATA_BUFFER rb;
        memset(&rb, 0, sizeof(rb));
        DWORD size;
        rb.ReparseTag = IO_REPARSE_TAG_MOUNT_POINT;
        if (!DeviceIoControl(h, FSCTL_DELETE_REPARSE_POINT, &rb,
                             REPARSE_GUID_DATA_BUFFER_HEADER_SIZE, NULL, 0, &size, 0))
        {
            CloseHandle(h);
            return false;
        }
        CloseHandle(h);
    }
#endif
    return ::remove(path) == 0;
}

bool CxFileSystem::createHardlink(const std::string &sPath, const std::string &sTarget)
{
    const char *path = sPath.c_str();
    const char *target = sTarget.c_str();
#ifdef GM_OS_WIN
    if (!CreateHardLink(target, path, NULL))
        return false;
    return true;
#else
    if(::link(path, target))
        return false;
    return true;
#endif
}

int CxFileSystem::getLinkinfo(const string &sPath, char *buffer, size_t size)
{
    const char *path = sPath.c_str();
#ifdef GM_OS_WIN
    HANDLE h;
    char reparse[MAXIMUM_REPARSE_DATA_BUFFER_SIZE];
    DWORD rsize;

    if (!CxFileSystem::isLink(path))
        return EINVAL;

    h = CreateFile(path, GENERIC_READ, 0, 0, OPEN_EXISTING,
                   FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT, 0);

    if (!h || h == INVALID_HANDLE_VALUE)
        return EINVAL;

    memset(reparse, 0, sizeof(reparse));
    LOCAL_REPARSE_DATA_BUFFER *rb = (LOCAL_REPARSE_DATA_BUFFER *) &reparse;

    if (!DeviceIoControl(h, FSCTL_GET_REPARSE_POINT, NULL, 0, (LPVOID *) rb, MAXIMUM_REPARSE_DATA_BUFFER_SIZE, &rsize, 0))
    {
        CloseHandle(h);
        return getFileLastError();
    }

#ifdef  UNICODE
    CxStringC::set(buffer, size, rb.PathBuffer);
#else
    WideCharToMultiByte(CP_THREAD_ACP, 0, rb->PathBuffer,
                        rb->SubstituteNameLength / sizeof(WCHAR) + 1, buffer, size, "", FALSE);
#endif
    CloseHandle(h);
    return 0;
#else
    if(::readlink(path, buffer, size))
        return getFileLastError();
    return 0;
#endif
}

int CxFileSystem::createPipe(fd_t &input, fd_t &output, size_t size)
{
#ifdef GM_OS_WIN
    input = output = NULL;
    SECURITY_ATTRIBUTES sattr;

    sattr.nLength = sizeof(SECURITY_ATTRIBUTES);
    sattr.bInheritHandle = TRUE;
    sattr.lpSecurityDescriptor = NULL;

    if (!CreatePipe(&input, &output, &sattr, size))
        return getFileLastError();

    return 0;
#else
    input = output = -1;
    int pfd[2];
    if(::pipe(pfd))
        return getFileLastError();
    input = pfd[0];
    output = pfd[1];
    return 0;
#endif
}

bool CxFileSystem::dllCanLoad(const std::string &sPath)
{
    CxDll module;
    module.map(sPath.c_str());
    if (module.ptr)
    {
        module.ptr = 0;
        return true;
    }
#ifdef  GM_OS_WIN
    return getFileLastError();
#else
    return module.error;
#endif
}

bool CxFileSystem::createDir(const std::string &sPath, unsigned perms)
{
    const char *path = sPath.c_str();
#ifdef GM_OS_WIN
    if (!CreateDirectory(path, NULL))
        return false;
//   return changeMode(path, perms);
    HANDLE handle = CreateFileA(path,
                         GENERIC_WRITE,
                         0,
                         NULL,
                         OPEN_EXISTING,
                         FILE_FLAG_BACKUP_SEMANTICS |
                         FILE_FLAG_OPEN_REPARSE_POINT,
                         NULL);
    if (handle == INVALID_HANDLE_VALUE) {
        return false;
//        SET_REQ_WIN32_ERROR(req, GetLastError());
//        goto error;
    }
    /* Clean up */
    CloseHandle(handle);
    return true;
#else
    if(perms & 06)
        perms |= 01;
    if(perms & 060)
        perms |= 010;
    if(perms & 0600)
        perms |= 0100;
    //cmx 20160823
    //if(::mkdir(path, perms))
    //    return getFileLastError();
    //return 0;
    return ::mkdir(path, perms) == 0;
#endif
}

int CxFileSystem::createDirMultiLevel(const string &sPath, unsigned model)
{
    int r = 0;
    size_t found = sPath.find_first_of("/\\");
    string sDir;
    while (found != string::npos)
    {
        sDir = sPath.substr(0, found + 1);
        if (!isExist(sDir))
        {
            if (createDir(sDir, model))
                r++;
        }
        found = sPath.find_first_of("/\\", found + 1);
    }
    if (sDir.size() != sPath.size() && !isExist(sPath))
    {
        if (createDir(sPath, model))
            r++;
    }
    return r;
}

//#include <Shellapi.h>
//
//bool DeleteFile1(const char * lpszPath)
//{
//    SHFILEOPSTRUCT FileOp={0};
//    FileOp.fFlags = FOF_ALLOWUNDO |   // can put to Recycle Bin
//                    FOF_NOCONFIRMATION; // do not show prompt
//    FileOp.pFrom = lpszPath;
//    FileOp.pTo = NULL;      // must NULL
//    FileOp.wFunc = FO_DELETE;    // operation : delete
//    return SHFileOperation(&FileOp) == 0;
//}

bool fn_removeImpl(const string & sPath, bool bRemoveDir)
{
    bool r = false;

    if (sPath.empty())
    {
        return r;
    }

#ifdef GM_OS_WIN
    if (bRemoveDir)
    {
//        if (DeleteFile1(sPath.c_str()))
        if (RemoveDirectoryA(sPath.c_str()) != 0)
            r = true;
    }
    else
    {
        if (DeleteFileA(sPath.c_str()) != 0)
            r = true;
    }
#else
    int rc;
    if (bRemoveDir)
        rc = rmdir(sPath.c_str());
    else
        rc = unlink(sPath.c_str());
    if (! rc) r = true;
#endif

    return r;
}

bool CxFileSystem::removeDir(const string &sPath)
{
    if (!isLink(sPath) && isDir(sPath))
    {
        int iFailCount = 0;
        std::vector<PathInfo> pathInfos;
        scanDir(sPath, pathInfos, true, true);
        for (int i = 0; i < pathInfos.size(); ++i)
        {
            const PathInfo &pathInfo = pathInfos.at(i);
            if ( pathInfo.pathType != PathTypeDir )
            {
                if (! fn_removeImpl(pathInfo.filePath(), false))
                {
                    iFailCount++;
                }
            }
        }
        for (int i = 0; i < pathInfos.size(); ++i)
        {
            const PathInfo &pathInfo = pathInfos.at(i);
            if ( pathInfo.pathType == PathTypeDir )
            {
                if (! fn_removeImpl(pathInfo.filePath(), true))
                {
                    iFailCount++;
                }
            }
        }

        // Note: On Windows, removing a directory may not succeed at first
        // try because deleting files is not a synchronous operation. Files
        // are merely marked as deleted, and actually removed at a later time.
        //
        // An alternate strategy would be moving files to a different directory
        // first (on the same drive, but outside the deleted tree), and marking
        // them as hidden, before deleting them, but this could lead to other issues.
        // So we simply retry after some time until we succeed, or give up.

        int retry = 8;
        long sleep = 10;
        while (retry > 0)
        {
            if (! fn_removeImpl(sPath, true))
            {
                if (--retry == 0) return false;
#if defined(GM_OS_WIN)
                ::Sleep(sleep);
#else
                usleep(sleep * 1000);
//    nanosleep(timeout * 1000);
#endif
                sleep *= 2;
            }
            else
            {
                retry = 0;
            }
        }
        return iFailCount == 0;
    }
    else
    {
        return fn_removeImpl(sPath, false);
    }
}

void CxFileSystem::scanDir(const std::string &sPath, vector<PathInfo> &pathInfos, bool bIncludeSubDir, bool bContainDir, const std::vector<std::string> & ignoreDirNames)
{
    const char *path = sPath.c_str();
#ifdef GM_OS_WIN
    char szFind[MAX_PATH];
    char szFile[MAX_PATH];

    WIN32_FIND_DATA FindFileData;

    strcpy(szFind, path);
    strcat(szFind, "//*.*");

    HANDLE hFind = ::FindFirstFile(szFind, &FindFileData);
    if (INVALID_HANDLE_VALUE == hFind) return;
    while (TRUE)
    {

        if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if (FindFileData.cFileName[0] != '.')
            {
                if (bContainDir)
                {
                    PathInfo pathInfo;
                    if (getPathInfo(mergeFilePath(path,FindFileData.cFileName), &pathInfo))
                    {
                        pathInfos.push_back(pathInfo);
                    }
                }
                strcpy(szFile, path);
                strcat(szFile, "//");
                strcat(szFile, FindFileData.cFileName);
                if (bIncludeSubDir)
                {
                    scanDir(szFile, pathInfos, bIncludeSubDir, bContainDir, ignoreDirNames);
                }
            }
        }
        else
        {
            PathInfo pathInfo;
            if (getPathInfo(mergeFilePath(path, FindFileData.cFileName), &pathInfo))
            {
                pathInfos.push_back(pathInfo);
            }
//            cout<<FindFileData.cFileName<<endl;
        }
        if (!FindNextFile(hFind, &FindFileData))
            break;
    }
    FindClose(hFind);
#else
    DIR              *pDir ;
    struct dirent    *ent  ;
    int               i=0  ;
    char              childpath[512];

    pDir=opendir(path);
    if (pDir == NULL) return;
    memset(childpath,0,sizeof(childpath));
    while((ent=readdir(pDir))!=NULL)
    {
        if(ent->d_type & DT_DIR)
        {
            if(ent->d_name[0] != '.')
            {
                sprintf(childpath,"%s/%s",path,ent->d_name);
                string sFilePath(childpath);
//            printf("path:%s/n",childpath);
                if (bContainDir)
                {
                    PathInfo pathInfo;
                    if (getPathInfo(sFilePath, &pathInfo))
                    {
                        pathInfos.push_back(pathInfo);
                    }
                }
                if (bIncludeSubDir)
                {
                    string sPathName(ent->d_name);
                    if( std::find(ignoreDirNames.begin(), ignoreDirNames.end(), sPathName) != ignoreDirNames.end() )
                    {
                        continue;
                    }
                    scanDir(sFilePath, pathInfos, bIncludeSubDir, bContainDir, ignoreDirNames);
                }
            }
        }
        else
        {
            PathInfo pathInfo;
            if (getPathInfo(mergeFilePath(path, ent->d_name), &pathInfo))
            {
                pathInfos.push_back(pathInfo);
            }
//            cout<<ent->d_name<<endl;
        }
    }
#endif
}

void
CxFileSystem::scanDir(const std::string &sPath, CxFileSystem::fn_scan_result_t fn_scan_result, bool bIncludeSubDir, std::string *sParam, int *iParam)
{
    const char *path = sPath.c_str();
#ifdef GM_OS_WIN
    char szFind[MAX_PATH];
    char szFile[MAX_PATH];

    WIN32_FIND_DATA FindFileData;

    strcpy(szFind, path);
    strcat(szFind, "//*.*");

    HANDLE hFind = ::FindFirstFile(szFind, &FindFileData);
    if (INVALID_HANDLE_VALUE == hFind) return;
    while (TRUE)
    {

        if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if (FindFileData.cFileName[0] != '.')
            {
                strcpy(szFile, path);
                strcat(szFile, "//");
                strcat(szFile, FindFileData.cFileName);
                if (bIncludeSubDir)
                {
                    scanDir(szFile, fn_scan_result, bIncludeSubDir);
                }
            }
        }
        else
        {
            PathInfo pathInfo;
            if (getPathInfo(mergeFilePath(path, FindFileData.cFileName), &pathInfo))
            {
                fn_scan_result(pathInfo, sParam, iParam);
            }
//            cout<<FindFileData.cFileName<<endl;
        }
        if (!FindNextFile(hFind, &FindFileData))
            break;
    }
    FindClose(hFind);
#else
    DIR              *pDir ;
    struct dirent    *ent  ;
    int               i=0  ;
    char              childpath[512];

    pDir=opendir(path);
    if (pDir == NULL) return;
    memset(childpath,0,sizeof(childpath));
    while((ent=readdir(pDir))!=NULL)
    {
        if(ent->d_type & DT_DIR)
        {
            if(ent->d_name[0] != '.')
            {
                sprintf(childpath, "%s/%s", path, ent->d_name);
                string sFilePath(childpath);
//            printf("path:%s/n",childpath);
                PathInfo pathInfo;
                if (getPathInfo(sFilePath, &pathInfo))
                {
                    fn_scan_result(pathInfo, sParam, iParam);
                }
                if (bIncludeSubDir)
                {
                    scanDir(sFilePath, fn_scan_result, bIncludeSubDir);
                }
            }
        }
        else
        {
            PathInfo pathInfo;
            if (getPathInfo(mergeFilePath(path, ent->d_name), &pathInfo))
            {
                fn_scan_result(pathInfo, sParam, iParam);
            }
//            cout<<ent->d_name<<endl;
        }
    }
#endif
}

int64 CxFileSystem::sizeOfDir(const string &sPath)
{
    vector<PathInfo> pathInfos;
    scanDir(sPath, pathInfos);
    int64 r = 0;
    for (size_t i = 0; i < pathInfos.size(); ++i)
    {
        const PathInfo &pathInfo = pathInfos.at(i);
        r += pathInfo.fileSize;
    }
    return r;
}

string CxFileSystem::mergeFilePath(const string &sPath, const string &sFileName)
{
    char cPathCharacter = CxGlobal::pathCharacter;
    if (sPath.find('/') != string::npos)
        cPathCharacter = '/';
    string r = CxString::trim(sPath);
    if (!(r.size() > 0 && r[r.size() - 1] == cPathCharacter))
        r.push_back(cPathCharacter);
    r.append(sFileName);
    return r;
}

string CxFileSystem::relativeFilePath(const string &sPath, const string &sFilePath, bool *bOk)
{
    string sPath1;
    string sPath2;
    int iStype = isWin32PathStyle(sFilePath);
    sPath1 = CxString::toLower(CxFileSystem::convertPathStyle(sPath, iStype));
    sPath2 = CxString::toLower(CxFileSystem::convertPathStyle(sFilePath, iStype));
    size_t iPos = sPath2.find(sPath1);
    if (iPos == 0)
    {
        string r = sFilePath.substr(sPath1.size());
        if (r.size() > 0 && (r[0] == '\\' || r[0] == '/'))
        {
            if (bOk) *bOk = true;
            return r.substr(1);
        }
    }
    if (bOk) *bOk = false;
    return string();
}

bool CxFileSystem::hasRootPath(const string &sFilePath)
{
#ifdef GM_OS_WIN
    return sFilePath.size() > 1 && (sFilePath[1] == ':' || (sFilePath[0] == '\\' && sFilePath[1] == '\\'));
#else
    return sFilePath.size() > 0 && sFilePath[0] == '/';
#endif
}

string CxFileSystem::fullFilePath(const string &sPath, const string &sFilePath, bool *bOk)
{
    bool b = hasRootPath(sFilePath);
    if (bOk) *bOk = b;
    if (!b)
    {
        return mergeFilePath(sPath, sFilePath);
    }
    else
    {
        return sFilePath;
    }
}

string CxFileSystem::fullFilePath(const string &sRootPath, const string &sRelativePath, const string &sFilePath)
{
    if (!hasRootPath(sFilePath))
    {
        if (!hasRootPath(sRelativePath))
        {
            return mergeFilePath(mergeFilePath(sRootPath, sRelativePath), sFilePath);
        }
        else
        {
            return mergeFilePath(sRelativePath, sFilePath);
        }
    }
    else
    {
        return sFilePath;
    }
}

string CxFileSystem::extractFileName(const string &sFilePath)
{
    size_t found = sFilePath.find_last_of("/\\");
    if (found == string::npos)
    {
        return sFilePath;
    }
    else
    {
        return sFilePath.substr(found + 1);
    }
}

string CxFileSystem::extractPath(const string &sFilePath)
{
    size_t found = sFilePath.find_last_of("/\\");
    if (found != string::npos)
    {
        return sFilePath.substr(0, found);
    }
    return string();
}

string CxFileSystem::parentPath(const string &sPath)
{
    if (sPath.size() > 1)
    {
        string rPath;
        char cLast = sPath[sPath.size() - 1];
        if (cLast == '\\' || cLast == '/')
            rPath = sPath.substr(0, sPath.size() - 1);
        else
            rPath = sPath;

        size_t found = rPath.find_last_of("/\\");
        if (found != string::npos)
        {
            return rPath.substr(0, found);
        }
    }
    return sPath;
}

string CxFileSystem::parentPath(const string &sPath, int iLevel)
{
    if (sPath.size() > 1)
    {
        string rPath;
        char cLast = sPath[sPath.size() - 1];
        if (cLast == '\\' || cLast == '/')
            rPath = sPath.substr(0, sPath.size() - 1);
        else
            rPath = sPath;

        for (int i = 0; i < iLevel; ++i)
        {
            size_t found = rPath.find_last_of("/\\");
            if (found != string::npos)
            {
                rPath = rPath.substr(0, found);
            }
            else
            {
                break;
            }
        }
        return rPath;
    }
    return sPath;
}

string CxFileSystem::extractFilePrefixName(const string &sFilePath)
{
    size_t foundr = sFilePath.rfind('.');
    size_t foundl = sFilePath.find_last_of("/\\");
    if (foundr == string::npos)
    {
        //* filename
        if (foundl == string::npos)
        {
            return sFilePath;
        }
            //* /tmp/dir/filename
        else
        {
            return sFilePath.substr(foundl);
        }
    }
    else
    {
        //* filename.suffix
        if (foundl == string::npos)
        {
            return sFilePath.substr(0, foundr);
        }
            //* /tmp/dir/filename.suffix
        else
        {
            return sFilePath.substr(foundl + 1, foundr - foundl - 1);
        }
    }
}

string CxFileSystem::extractFileSuffixName(const string &sPath)
{
    const char *sSuffixName = strrchr(sPath.c_str(), '.');
    if (sSuffixName)
        return CxString::toLower(sSuffixName);
    else
        return std::string();
}

bool CxFileSystem::isWin32PathStyle(const string &sPath)
{
    return sPath.find('\\') != string::npos;
}

char CxFileSystem::getPathStyle(const string &sPath)
{
    return ( sPath.find('\\') != string::npos ) ? '\\' : '/';
}

string CxFileSystem::convertPathStyle(const string &sPath, int iStype)
{
    if (0 == iStype)
        return CxString::replace(sPath, '\\', '/');
    else
        return CxString::replace(sPath, '/', '\\');
}

string CxFileSystem::normalizePathStyle(const string &sPath)
{
    if (isWin32PathStyle(sPath))
        return CxString::replace(sPath, '/', '\\');
    else
        return CxString::replace(sPath, '\\', '/');
}

string CxFileSystem::normalizeStringWin32(const string & sPath, bool bAllowAboveRoot)
{
    string res = string();
    int lastSlash = -1;
    int dots = 0;
    char code;
    for (int i = 0; i <= sPath.size(); ++i) {
        if (i < sPath.size())
            code = sPath[i];
        else if (code == 47/*/*/ || code == 92/*\*/)
            break;
        else
            code = 47/*/*/;
        if (code == 47/*/*/ || code == 92/*\*/) {
            if (lastSlash == i - 1 || dots == 1) {
                // NOOP
            } else if (lastSlash != i - 1 && dots == 2) {
                if (res.size() < 2 ||
                    res[res.size() - 1] != 46/*.*/ ||
                    res[res.size() - 2] != 46/*.*/) {
                    if (res.size() > 2) {
                        int start = res.size() - 1;
                        int j = start;
                        for (; j >= 0; --j) {
                            if (res[j] == 92/*\*/)
                                break;
                        }
                        if (j != start) {
                            if (j == -1)
                                res = string();
                            else
                                res = res.substr(0, j);
                            lastSlash = i;
                            dots = 0;
                            continue;
                        }
                    } else if (res.size() == 2 || res.size() == 1) {
                        res = string();
                        lastSlash = i;
                        dots = 0;
                        continue;
                    }
                }
                if (bAllowAboveRoot) {
                    if (res.size() > 0)
                        res += "\\..";
                    else
                        res = "..";
                }
            } else {
                if (res.size() > 0)
                    res += "\\" + sPath.substr(lastSlash + 1, i - lastSlash - 1);
                else
                    res = sPath.substr(lastSlash + 1, i - lastSlash - 1);
            }
            lastSlash = i;
            dots = 0;
        } else if (code == 46/*.*/ && dots != -1) {
            ++dots;
        } else {
            dots = -1;
        }
    }
    return res;
}

string CxFileSystem::normalize(const string & sPath)
{
    int len = sPath.size();
    if (len == 0)
        return ".";
    int rootEnd = 0;
    char code = sPath[0];
    string device;
    bool isAbsolute = false;

    // Try to match a root
    if (len > 1) {
        if (code == 47/*/*/ || code == 92/*\*/) {
            // Possible UNC root

            // If we started with a separator, we know we at least have an absolute
            // path of some kind (UNC or otherwise)
            isAbsolute = true;

            code = sPath[1];
            if (code == 47/*/*/ || code == 92/*\*/) {
                // Matched double path separator at beginning
                int j = 2;
                int last = j;
                // Match 1 or more non-path separators
                for (; j < len; ++j) {
                    code = sPath[j];
                    if (code == 47/*/*/ || code == 92/*\*/)
                        break;
                }
                if (j < len && j != last) {
                    string firstPart = sPath.substr(last, j - last);
                    // Matched!
                    last = j;
                    // Match 1 or more path separators
                    for (; j < len; ++j) {
                        code = sPath[j];
                        if (code != 47/*/*/ && code != 92/*\*/)
                            break;
                    }
                    if (j < len && j != last) {
                        // Matched!
                        last = j;
                        // Match 1 or more non-path separators
                        for (; j < len; ++j) {
                            code = sPath[j];
                            if (code == 47/*/*/ || code == 92/*\*/)
                                break;
                        }
                        if (j == len) {
                            // We matched a UNC root only
                            // Return the normalized version of the UNC root since there
                            // is nothing left to process

                            return "\\\\" + firstPart + "\\" + sPath.substr(last) + "\\";
                        } else if (j != last) {
                            // We matched a UNC root with leftovers

                            device = "\\\\" + firstPart + "\\" + sPath.substr(last, j - last);
                            rootEnd = j;
                        }
                    }
                }
            } else {
                rootEnd = 1;
            }
        } else if ((code >= 65/*A*/ && code <= 90/*Z*/) ||
                   (code >= 97/*a*/ && code <= 122/*z*/)) {
            // Possible device root

            code = sPath[1];
            if (sPath[1] == 58/*:*/) {
                device = sPath.substr(0, 2);
                rootEnd = 2;
                if (len > 2) {
                    code = sPath[2];
                    if (code == 47/*/*/ || code == 92/*\*/) {
                        // Treat separator following drive name as an absolute path
                        // indicator
                        isAbsolute = true;
                        rootEnd = 3;
                    }
                }
            }
        }
    } else if (code == 47/*/*/ || code == 92/*\*/) {
        // `path` contains just a path separator, exit early to avoid unnecessary
        // work
        return "\\";
    }

    code = sPath[len - 1];
    bool trailingSeparator = (code == 47/*/*/ || code == 92/*\*/);
    string tail;
    if (rootEnd < len)
        tail = CxFileSystem::normalizeStringWin32(sPath.substr(rootEnd), !isAbsolute);
    else
        tail = string();
    if (tail.size() == 0 && !isAbsolute)
        tail = ".";
    if (tail.size() > 0 && trailingSeparator)
        tail += "\\";
    if (device.empty()) {
        if (isAbsolute) {
            if (tail.size() > 0)
                return "\\" + tail;
            else
                return "\\";
        } else if (tail.size() > 0) {
            return tail;
        } else {
            return string();
        }
    } else {
        if (isAbsolute) {
            if (tail.size() > 0)
                return device + "\\" + tail;
            else
                return device + "\\";
        } else if (tail.size() > 0) {
            return device + tail;
        } else {
            return device;
        }
    }
}

string CxFileSystem::trimeDots(const string &sPath)
{
    if (sPath.empty()) return string();
    char cPathStyle = getPathStyle(sPath);
    vector<string> ary = CxString::splitByDelimiters(sPath, "/\\");
//    vector<string>::iterator itBegin = ary.begin();
    for (vector<string>::iterator it = ary.begin() ; it != ary.end(); ++it)
    {
        if (*it == ".") {
            ary.erase(it);
        } else if (*it == "..") {
            // If at the start, or previous value is still ..,
            // keep them so that when converted to a path it may
            // still work when converted to a path, even though
            // as an ID it is less than ideal. In larger point
            // releases, may be better to just kick out an error.
//            if (it == ary.begin() || (it == ary.begin()+1 && ary[2] == "..") || ary[i - 1] == "..") {
            if (it == ary.begin() || *(it-1) == "..") {
                continue;
            } else {
                ary.erase(it-1, it+1);
            }
        }
    }
    return CxString::join(ary, cPathStyle);
}
//./a
//../../aa
//./../../aa
//.
//..
//../..
string CxFileSystem::cd(const string &sPathString, const string &sCurrentPath)
{
    if (hasRootPath(sPathString))
    {
        return sPathString;
    }
    else
    {
        string sPath;
        if (hasRootPath(sCurrentPath))
            sPath = sCurrentPath;
        else
            sPath = getCurrentDir();
        string sScanf = CxString::trim(CxString::replace(sPathString, '\\', '/'));
        if (sScanf.size() < 2)
        {
            return sPath;
        }
        if (sScanf[0] == '.' && sScanf[1] == '/')
        {
            sScanf = sScanf.substr(2);
        }
        int iLevel = 0;
        size_t iSize = sScanf.size() - 1;
        size_t i = 0;
        for (; i < iSize; i += 3)
        {
            if (sScanf[i] == '.' && sScanf[i + 1] == '.')
                iLevel++;
            else
                break;
        }
        string rPath;
        if (iLevel > 0 && sScanf[i - 1] != '/')
        {
            iLevel -= 1;
            rPath = sScanf.substr(i - 3);
        }
        else
        {
            rPath = sScanf.substr(i);
        }
        sPath = parentPath(sPath, iLevel);
        return mergeFilePath(sPath, rPath);
    }
}

fd_t CxFileSystem::openFile(const string &sFilePath)
{
    const char *path = sFilePath.c_str();
#ifdef GM_OS_WIN
    SECURITY_ATTRIBUTES sattr;

    sattr.nLength = sizeof(SECURITY_ATTRIBUTES);
    sattr.bInheritHandle = TRUE;
    sattr.lpSecurityDescriptor = NULL;

    fd_t fd = CreateFile(path, GENERIC_WRITE, 0, &sattr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (fd != INVALID_HANDLE_VALUE)
        SetFilePointer(fd, 0, NULL, FILE_END);

    return fd;
#else
    return ::open(path, O_WRONLY | O_CREAT | O_APPEND, 0666);
#endif
}

void CxFileSystem::closeFile(fd_t fd)
{
#ifdef GM_OS_WIN
    CloseHandle(fd);
#else
    ::close(fd);
#endif
}

bool CxFileSystem::createFile(const string &sFilePath)
{
    FILE *pFile;
    pFile = fopen(sFilePath.c_str(), "wb");
    if (pFile == NULL)
    {
        return false;
    }
    fclose(pFile);
    return true;
}

//windows : WIN32_FIND_DATA fd
int64 CxFileSystem::sizeOfFile(const string &sFilePath)
{
    int64 lFileSize = -1;
    FILE *fp;
    if (!(fp = fopen(sFilePath.c_str(), "rb")))
    {
        return lFileSize;
    }
#ifdef GM_OS_WIN
#ifdef _MSC_VER
    //* _MSC_VER
#if _MSC_VER >= 1400
        if ( _fseeki64(fp, (long long)(0), SEEK_END) )
        {
            fclose(fp);
            return (lFileSize);
        }
        lFileSize =_ftelli64(fp);
#else
#error Visual Studio version is less than 8.0(VS 2005) !
#endif
        /***********************/
#else
//* GNU
    if (fseeko64(fp, (int64) (0), SEEK_END))
    {
        fclose(fp);
        return (lFileSize);
    }
    lFileSize = ftello64(fp);
    /***********************/
#endif
#else
    //* UNIX
        if (fseeko(fp, (long long)(0), SEEK_END))
        {
            fclose(fp);
            return (lFileSize);
        }
        lFileSize = ftello(fp);
        /***********************/
#endif
    fclose(fp);
    return lFileSize;
}

int64 CxFileSystem::toSize(const std::string &sSize)
{
    string sSize2 = CxString::toLower(sSize);
    int64 iSize = CxString::toInt64(sSize2);
    if (sSize2.find("mb") != string::npos)
    {
        return iSize * 1024 * 1024;
    }
    else if (sSize2.find("kb") != string::npos)
    {
        return iSize * 1024;
    }
    else if (sSize2.find("gb") != string::npos)
    {
        return iSize * 1024 * 1024 * 1024;
    }
    return iSize;
}

CxDll::CxDll()
{
    ptr = 0;
    error = 0;
}

CxDll::CxDll(const char *path)
{
    ptr = 0;
    error = 0;
    map(path);
}

CxDll::~CxDll()
{
    release();
}

void CxDll::map(const char *path)
{
#ifdef GM_OS_WIN
    error = 0;
    ptr = LoadLibrary(path);
    if (!ptr)
        error = ENOEXEC;
#else
    error = 0;
    ptr = dlopen(path, RTLD_NOW | RTLD_GLOBAL);
    if(ptr == NULL)
        error = ENOEXEC;
#endif
}

void CxDll::release(void)
{
#ifdef GM_OS_WIN
    if (ptr)
        FreeLibrary(ptr);
    ptr = 0;
#else
    if(ptr)
        dlclose(ptr);
    ptr = NULL;
#endif
}

CxDll::addr_t CxDll::find(const char *sym) const
{
#ifdef GM_OS_WIN
    if (ptr == 0)
        return (CxDll::addr_t) NULL;
    return (addr_t) GetProcAddress(ptr, sym);
#else
    if(!ptr)
        return (CxDll::addr_t)NULL;
    return (CxDll::addr_t)dlsym(ptr, (char *)sym);
#endif
}
