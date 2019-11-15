#include <ccxx/ccxx.h>

#include <ccxx/cxcurl.h>

using namespace std;

void fn_test_create(const string& sScanPath)
{
    CxFileSystem::createDirMultiLevel(sScanPath);
    for (int j = 0; j < 20; ++j)
    {
        string sNow = CxTime::currentSystemTimeString('-', 'd', '-');
        string sFilePath = CxFileSystem::mergeFilePath(sScanPath, CxString::format("curl-test-%s-%d.log", sNow.c_str(), j));
        string sText;
        for (int i = 0; i < j; ++i)
        {
            sText += "QWERTYUIOPASDFGHJKL:ZXCVBNM<>!@#$%^&*()abc123-";
        }
        CxFile::save(sFilePath, sText);
        cxPrompt() << "Create File: " << sFilePath;
    }
}

void fn_test_delete(const string& sScanPath)
{
    CxFileSystem::removeDir(sScanPath);
    cxPrompt() << "removeDir: " << sScanPath;
}

void fn_timer_timeout_create_delete(int iInterval)
{
    static int iIndex = 0;
    msepoch_t dtNow = CxTime::currentSystemTime();
    if (iIndex++ % 2)
    {
        fn_test_create("/fff/tmp/curl-test-dir1");
    }
    else
    {
        fn_test_delete("/fff/tmp/curl-test-dir1/log");
    }
    cxPrompt() << "COST TIME(MS): " << CxTime::milliSecondDifferToNow(dtNow);
    cxPrompt() << "";
}



/* <DESC>
 * Get a single file from an FTP server.
 * </DESC>
 */

struct FtpFile {
    const char *filename;
    FILE *stream;
};

static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
{
    struct FtpFile *out = (struct FtpFile *)stream;
    if(out && !out->stream) {
        /* open file for writing */
        out->stream = fopen(out->filename, "wb");
        if(!out->stream)
            return -1; /* failure, can't open file to write */
    }
    return fwrite(buffer, size, nmemb, out->stream);
}

int main1(int argc,const char *argv[])
{
    CxApplication::init(argc, argv);

    cxPrompt() << CxTime::currentSystemTime();
    cxPrompt() << CxTime::currentSystemTimeString();
    cxPrompt() << CxTime::toString(CxTime::currentSystemTime());

    CxTimerManager::startTimer(fn_timer_timeout_create_delete, 1000);

    CxApplication::exec();

    return 0;
}




int main(void)
{
#ifdef GM_OS_WIN
    char sFp[] = "c:/test-curl-002.md";
#else
    char sFp[] = "/tmp/test-curl-002.md";
#endif
    CURL *curl;
    CURLcode res;
    struct FtpFile ftpfile = {
        sFp, /* name to store the file as if successful */
//    "curl.tar.gz", /* name to store the file as if successful */
        NULL
    };

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    if(curl) {
        /*
         * You better replace the URL with one that works!
         */
        curl_easy_setopt(curl, CURLOPT_URL,
                         "http://ftp.gnu.org/gnu/button.14.0.shar");
//                     "ftp://ftp.example.com/curl/curl-7.9.2.tar.gz");
        /* Define our callback to get called when there's data to be written */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_fwrite);
        /* Set a pointer to our struct to pass to the callback */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ftpfile);

        /* Switch on full protocol/debug output */
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        res = curl_easy_perform(curl);

        /* always cleanup */
        curl_easy_cleanup(curl);

        if(CURLE_OK != res) {
            /* we failed */
            fprintf(stderr, "curl told us %d\n", res);
        }
    }

    if(ftpfile.stream)
        fclose(ftpfile.stream); /* close the local file */

    curl_global_cleanup();

    return 0;
}
