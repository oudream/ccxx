#ifndef CXFILE_H
#define CXFILE_H

#include "cxglobal.h"

#include "cxstring.h"
#include "cxtime.h"


#ifdef GM_OS_WIN
#define	GM_FILE_MODE_IEXEC	0x0040
#define	GM_FILE_MODE_IWRITE	0x0080
#define	GM_FILE_MODE_IREAD	0x0100
#define	GM_FILE_MODE_IRWXU	(GM_FILE_MODE_IREAD | GM_FILE_MODE_IWRITE | GM_FILE_MODE_IEXEC)
#else
#define	GM_FILE_MODE_IEXEC	S_IEXEC
#define	GM_FILE_MODE_IWRITE	S_IWRITE
#define	GM_FILE_MODE_IREAD	S_IREAD
#define	GM_FILE_MODE_IRWXU	S_IRWXU
#endif


class CxFile
{
public:
    /**
     * @brief load : Read file to byte array (string).
     * @param sFileName
     * @param sOut
     * @return
     */
    static inline bool load(const std::string& sFileName, std::string& sOut) { return load(sFileName, sOut, 1024 * 1024 * 300); }

    static bool load(const std::string& sFileName, std::string& sOut, size_t iMaxSize);

    /**
     * @brief load : Read file to byte array (string) by split character.
     * @param sFileName
     * @param sOut
     * @param sSplitString
     * @return
     */
    static int load(const std::string& sFileName, std::vector<std::string> & sOut, const std::string& sSplitString = CxGlobal::lineString);

    /**
     * @brief load
     * @param sFileName
     * @param sOut
     * @param cSplit
     * @return
     */
    static int load(const std::string& sFileName, std::vector<std::string> & sOut, char cSplit);

    /**
     * @brief load
     * @param sFileName
     * @param sOut
     * @param iSectionLength
     * @return
     */
    static int load(const std::string& sFileName, std::vector<std::string> & sOut, int iSectionLength);

    /**
     * @brief load
     * @param sFilePath
     * @param oObject
     * @param fn : call back to deal
     * @return 0 on false
     */
    static int load(const std::string& sFilePath, void * oObject, fn_int_object_tlv_t fn);


    /**
     * @brief save : Write file from byte array (string).
     * @param sFileName
     * @param sBuffer
     * @return
     */
    static bool save(const std::string& sFilePath, const std::string& sBuffer);

    /**
     * @brief save : Write file from byte array (string).
     * @param sFilePath
     * @param sStrings
     * @param sSplitString
     * @return
     */
    static bool save(const std::string &sFilePath, const std::vector<std::string>& sStrings, const std::string& sSplitString = CxGlobal::lineString, bool saveReturn = false);

    /**
     * @brief md5
     * @param sFilePath
     * @param iCodeType : 0=data ; 1=hex
     * @return : empty=file is empty or error
     */
    static std::string md5(const std::string &sFilePath, int iCodeType = 1);

    static bool isSameFileData(const std::string &sFilePath1, const std::string &sFilePath2);

};

class CxFileSystem
{
public:
    typedef enum {
        PathTypeNone,
        PathTypeFile,       //file
        PathTypeDir,        //directory
        PathTypeSoftlink,   //link
        PathTypeDevChar,    //io
        PathTypeDevBlock,   //disk
        PathTypeSystem      //fifo , socket
    } PathTypeEnum;

    class PathInfo
    {
    public:
        PathInfo() : pathType(PathTypeNone), fileSize(0), creationTime(0), lastWriteTime(0) {}
        ~PathInfo() {}
        inline PathInfo(const PathInfo &o) : pathType(o.pathType), path(o.path), fileName(o.fileName), fileSize(o.fileSize), creationTime(o.creationTime), lastWriteTime(o.lastWriteTime) { }
        inline PathInfo &operator=(const PathInfo &other)
        {
            if (this != &other)
            {
                pathType = other.pathType;
                path = other.path;
                fileName = other.fileName;
                fileSize = other.fileSize;
                creationTime = other.creationTime;
                lastWriteTime = other.lastWriteTime;
            }
            return *this;
        }

        inline std::string filePath() const
        {
            return mergeFilePath(path, fileName);
        }

        PathTypeEnum pathType;
        std::string path;
        std::string fileName;
        int64 fileSize;
        msepoch_t creationTime;
        msepoch_t lastWriteTime;
    };

    /**
     * @brief getFileLastError
     * @return
     */
    static int getFileLastError();

    /**
     * Set directory prefix (chdir).
     * @param path to change to.
     * @return true on success.
     */
    static bool setCurrentDir(const std::string& sDir);

    /**
     * @brief getCurrentDir
     * @return
     */
    static std::string getCurrentDir();

    /**
     * @brief openFile
     * @param sFilePath
     * @return
     */
    static fd_t openFile(const std::string& sFilePath);

    /**
     * @brief release
     * @param fd
     */
    static void closeFile(fd_t fd);

    /**
     * @brief createFile
     * @param sFilePath
     * @return
     */
    static bool createFile(const std::string& sFilePath);

    /**
     * Erase (remove) a file only.
     * @param path of file.
     * @return true on success.
     */
    static bool deleteFile(const std::string& sPath);

    /**
     * Copy a file.
     * @param source file.
     * @param target file.
     * @param size of buffer.
     * @return copied size.
     */
    static int copyFile(const std::string& sSource, std::string& sTarget, bool bOvert = true, int * iSourceSize = NULL);

    /**
     * Rename a file.
     * @param oldpath to rename from.
     * @param newpath to rename to.
     * @return true on success.
     */
    static bool renameFile(const std::string& sOldPath, const std::string& sNewPath);

    /**
     * @brief sizeOfFile
     * @param sFilePath
     * @return
     */
    static int64 sizeOfFile(const std::string& sFilePath);

    /**
     * Change file access mode.
     * @param path to change.
     * @param value of mode to assign. _S_IREAD | _S_IWRITE | _S_IEXEC S_IRWXU
     * @return true on success.
     */
    static bool changeMode(const std::string& sPath, unsigned value);

    /**
     * check if path exists.
     * @param path to test.
     * @return if true.
     */
    static bool isExist(const std::string& sPath);

    /**
     * @brief isExist
     * @param sPath
     * @param sDir
     * @return
     */
    static bool isExist(const std::string& sPath, const std::string & sDir);

    /**
     * @brief isExist
     * @param sPath
     * @param sDirs
     * @return
     */
    static bool isExist(const std::string& sPath, const std::vector<std::string> & sDirs);

    /**
     * Test if path readable.
     * @param path to test.
     * @return if true.
     */
    static bool isReadable(const std::string& sPath);

    /**
     * Test if path writable.
     * @param path to test.
     * @return if true.
     */
    static bool isWritable(const std::string& sPath);

    /**
     * Test if path is executable.
     * @param path to test.
     * @return if true.
     */
    static bool isExecutable(const std::string& sPath);

    /**
     * Test if path is a file.
     * @param path to test.
     * @return true if exists and is file.
     */
    static bool isFile(const std::string& sPath);

    /**
     * Test if path is a directory.
     * @param path to test.
     * @return true if exists and is directory.
     */
    static bool isDir(const std::string& sPath);

    /**
     * Test if path is a symlink.
     * @param path to test.
     * @return true if exists and is symlink.
     */
    static bool isLink(const std::string& sPath);

    /**
     * Test if path is a device path.
     * @param path to test.
     * @return true of is a device path.
     */
    static bool isDevice(const std::string& sPath);

    /**
     * Test if path is a hidden file.
     * @param path to test.
     * @return true if exists and is hidden.
     */
    static bool isHidden(const std::string& sPath);

    /**
     * Stat a file.
     * @param path of file to stat.
     * @param buffer to save stat info.
     * @return success or fail.
     */
    static bool getPathInfo(const std::string & sFilePath, PathInfo * pathInfo);

    /**
     * Stat a file.
     * @param path of file to stat.
     * @param buffer to save stat info.
     * @return success or fail.
     */
    static PathInfo getPathInfo(const std::string & sFilePath);

    /**
     * Create a symbolic link.
     * @param path to create.
     * @param target of link.
     * @return true on success.
     */
    static bool createLink(const std::string& sPath, const std::string& sTarget);

    /**
     * Remove a symbolic link explicitly.  Other kinds of files are also
     * deleted.  This should be used when uncertain about symlinks requiring
     * special support.
     * @param path to remove.
     * @return true on success.
     */
    static bool unlink(const std::string& sPath);

    /**
     * Create a hard link.
     * @param path to create link to.
     * @param target of link.
     * @return error number or 0 on success.
     */
    static bool createHardlink(const std::string& sPath, const std::string& sTarget);

    /**
     * Read a symbolic link to get it's target.
     * @param path of link.
     * @param buffer to save target into.
     * @param size of buffer.
     */
    static int getLinkinfo(const std::string& sPath, char * buffer, size_t size);

    /**
     * Create pipe.  These are created inheritable by default.
     * @param input descriptor.
     * @param output descriptor.
     * @param size of buffer if supported.
     * @return 0 or error code.
     */
    static int createPipe(fd_t& input, fd_t& output, size_t size = 0);

    /**
     * load a library into memory.
     * @param path to plugin.
     * @return 0 on success, else error.
     */
    static bool dllCanLoad(const std::string& sPath);

    /**
     * Simple direct method to create a directory.
     * @param path of directory to create.
     * @param mode of directory.
     * @return error number or 0 on success.
     */
    static bool createDir(const std::string& sPath, unsigned model = GM_FILE_MODE_IRWXU);

    /**
     * Simple direct method to create a directory.
     * @param path of directory to create.
     * @param mode of directory.
     * @return error number or 0 on success.
     */
    static int createDirMultiLevel(const std::string& sPath, unsigned model = GM_FILE_MODE_IRWXU);

    /**
     * Remove an empty directory.
     * @param path of directory.
     * @return error number or 0 on success.
     */
    static int removeDir(const std::string& sPath);

    /**
     * @brief scanDir
     * @param path
     * @param pathInfos
     * @param includeDir
     */
    static void scanDir(const std::string& sPath, std::vector<PathInfo>& pathInfos, bool includeDir = false);

    static void scanPath(const std::string& sPath, std::vector<PathInfo>& pathInfos, bool includeDir = false);

    /**
     * @brief sizeOfDir
     * @param sPath
     * @return
     */
    static int64 sizeOfDir(const std::string& sPath);

    /**
     * @brief mergeFilePath
     * @param sPath
     * @param sFileName
     * @return
     */
    static std::string mergeFilePath(const std::string& sPath, const std::string& sFileName);

    /**
     * @brief relativeFilePath
     * @param sPath
     * @param sFilePath
     * @return
     */
    static std::string relativeFilePath(const std::string& sPath, const std::string& sFilePath, bool * bOk = NULL);

    /**
     * @brief hasRootPath
     * @param sFilePath
     * @return
     */
    static bool hasRootPath(const std::string& sFilePath);

    /**
     * @brief fullFilePath
     * @param sPath
     * @param sFilePath
     * @return
     */
    static std::string fullFilePath(const std::string& sPath, const std::string& sFilePath, bool * bOk = NULL);

    /**
     * @brief fullFilePath
     * @param sRootPath
     * @param sRelativePath
     * @param sFilePath
     * @param pStrategy
     * @return
     */
    static std::string fullFilePath(const std::string& sRootPath, const std::string& sRelativePath, const std::string& sFilePath);

    /**
     * @brief extractFileName
     * @param sFilePath
     * @return
     */
    static std::string extractFileName(const std::string& sFilePath);

    /**
     * @brief extractPath
     * @param sFilePath
     * @return
     */
    static std::string extractPath(const std::string& sFilePath);

    /**
     * @brief parentPath
     * @param sPath
     * @return
     */
    static std::string parentPath(const std::string& sPath);
    
    /**
     * @brief parentPath
     * @param sPath
     * @param iLevel back level : ../../../=3
     * @return 
     */
    static std::string parentPath(const std::string& sPath, int iLevel);

    /**
     * @brief extractFilePrefixName
     * @param value
     * @return
     * example : sFilePath = \tmp\dir\filename.suffix; return filename
     */
    static std::string extractFilePrefixName(const std::string& sFilePath);

    /**
     * @brief extractFileSuffixName
     * @param value
     * @return
     * example : sFilePath = \tmp\dir\filename.suffix; return .suffix
     */
    static std::string extractFileSuffixName(const std::string& sPath);

    /**
     * @brief isWin32PathStyle
     * @param sPath
     * @return
     */
    static bool isWin32PathStyle(const std::string & sPath);
    
    /**
     * @brief convertPathStyle
     * @param sPath
     * @param iStype 0==unixStyle , 1==win32Style
     * @return
     */
    static std::string convertPathStyle(const std::string & sPath, int iStype = 0);

    /**
     * @brief refine
     * @param sPath
     * @return 
     * @sample :/temp/a\b return : /temp/a/b
     */
    static std::string refine(const std::string & sPath);

    /**
     * @brief calcLevel
     * @param sPath
     * @return 
     * @sample :/temp/a\b return : 3
     */
    static int calcLevel(const std::string & sPath);
    
    /**
     * @brief cd
     * @param sPathString
     * @param sCurrentPath
     * @return 
     * @sample : cd("./..", "/temp/a") return "/temp
     */
    static std::string cd(const std::string& sPathString, const std::string& sCurrentPath=std::string());



};

typedef CxFileSystem::PathInfo CxFilePathInfo;


/**
 * Convenience class for library plugins.
 * @author David Sugar <dyfet@gnutelephony.org>
 */
class CxDll
{
private:
    friend class CxFileSystem;

#ifdef GM_OS_WIN
    HINSTANCE   ptr;
#else
    void    *ptr;
#endif
    int     error;

public:
#ifdef  GM_OS_WIN
    typedef int (FAR WINAPI *addr_t)();
#else
    typedef void *addr_t;
#endif

    /**
     * Create CxDll object for use by load functions.
     */
    CxDll();

    /**
     * Create and map a CxDll object.
     * @param path of library to map.
     */
    CxDll(const char *path);

    /**
     * Destroy CxDll and release library.
     */
    ~CxDll();

    /**
     * Map library object with library.
     * @param name of library to load.
     */
    void map(const char *path);

    inline bool hadLoad() const { return ptr; }

    /**
     * Release loaded library.
     */
    void release(void);

    /**
     * Find symbol in loaded module.
     * @param module to search.
     * @param symbol to search for.
     * @return address of symbol or NULL if not found.
     */
    addr_t find(const char *symbol) const;

    inline int err(void) const
        {return error;}

    inline addr_t operator[](const char *symbol) const
        {return find(symbol);}

    inline addr_t operator()(const char *symbol) const
        {return find(symbol);}

    inline operator bool()
        {return ptr != NULL;}

    inline bool operator!()
        {return ptr == NULL;}
};

#endif // CXFILE_H
