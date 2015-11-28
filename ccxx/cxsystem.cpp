#include "cxsystem.h"

#ifndef GM_OS_WIN
#include <sys/utsname.h>  // For uname.
#include <sys/stat.h>
#endif

#include "cxapplication.h"


using namespace std;



bool CxSystem::sendEmailInternal(const char*dest, const char *subject, const char*body)
{
    if (dest && *dest)
    {
        fprintf(stderr, "Trying to send TITLE: %s BODY: %s to %s\n", subject, body, dest);
        string cmd = string("/bin/mail -s\"") + subject + "\" " + dest;
        FILE* pipe = fopen(cmd.c_str(), "w");
        if (pipe != NULL)
        {
            // Add the body if we have one
            if (body)
            {
                fwrite(body, sizeof(char), strlen(body), pipe);
            }
            bool ok = fclose(pipe) != -1;
            if ( !ok )
            {
                fprintf(stderr, "Problems sending mail to %s: %s\n", dest, strerror(errno));
            }
            return ok;
        }
    }
    fprintf(stderr, "Unable to send mail to %s\n", dest);
    return false;
}

std::vector<string> CxSystem::getTempDirectories()
{
    std::vector<string> sTempDirectories;
#ifdef GM_OS_WIN
    // On windows we'll try to find a directory in this order:
    //   C:/Documents & Settings/whomever/TEMP (or whatever GetTempPath() is)
    //   C:/TMP/
    //   C:/TEMP/
    //   C:/WINDOWS/ or C:/WINNT/
    //   .
    char tmp[MAX_PATH];
    if (GetTempPathA(MAX_PATH, tmp))
        sTempDirectories.push_back( string(tmp) );
    sTempDirectories.push_back("C:\\tmp\\");
    sTempDirectories.push_back("C:\\temp\\");
#else
    // Directories, in order of preference. If we find a dir that
    // exists, we stop adding other less-preferred dirs
    const char * candidates[] = {
        // Non-null only during unittest/regtest
        getenv("TEST_TMPDIR"),

        // Explicitly-supplied temp dirs
        getenv("TMPDIR"), getenv("TMP"),

        // If all else fails
        "/tmp",
    };

    for (size_t i = 0; i < (sizeof(candidates) / sizeof(*(candidates))); i++) {
        const char *d = candidates[i];
        if (!d) continue;  // Empty env var

        // Make sure we don't surprise anyone who's expecting a '/'
        string dstr = d;
        if (dstr[dstr.size() - 1] != '/') {
            dstr += "/";
        }
        sTempDirectories.push_back(dstr);

        struct stat statbuf;
        if (!stat(d, &statbuf) && S_ISDIR(statbuf.st_mode)) {
            // We found a dir that exists - we're done.
            return;
        }
    }

#endif
    return sTempDirectories;
}

string CxSystem::getTempDirectorie()
{
    vector<string> sTempDirectories = getTempDirectories();
    if (sTempDirectories.size()>0)
        return sTempDirectories[0];
    else
        return std::string();
}

void CxSystem::setAutoStart(bool value)
{
#ifdef GM_OS_WIN32
//    if (value)
//    {
//        QSettings reg ("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
//        QString appName = "ProtocolConvertor";
//        QString appFilePath = CommonFile::FilePath(ProgramPath::AppPath(), appName);
//        reg.setValue(appName, appFilePath);
//    }
//    else
//    {
//        QSettings reg ("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
//        QString appName = "ProtocolConvertor";
//        reg.remove(appName);
//    }
#else
#endif
}

