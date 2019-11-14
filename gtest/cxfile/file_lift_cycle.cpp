#include <gtest/gtest.h>

#include <ccxx/cxfile.h>

using namespace std;

namespace {
    std::vector<string> fn_getTempDirectories()
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
                break;
            }
        }

#endif
        return sTempDirectories;
    }

    string fn_getTempDirectorie()
    {
        vector<string> sTempDirectories = fn_getTempDirectories();
        if (sTempDirectories.size()>0)
            return sTempDirectories[0];
        else
            return std::string();
    }

    class CxFileLiftCycleTest : public testing::Test {
    protected:
        void SetUp() override {
            _p = fn_getTempDirectorie();
            _fp = CxFileSystem::mergeFilePath(_p, "cxfile_lift_cycle_test_file.txt");
            _ftext = "asdfaldsfjsd;fqrqweriupqer[1249213775=`=48`=332-402834!@#$%^&*()_+QWERTYUIOP{ASDFGHJKL:\"ZXCVBNM<>?zxcvbnm,asdfghjkl;qwertyuiop[234567890\n\r\t]";
        }

        void TearDown() override {

        }

        string _p;
        string _fp;
        string _ftext;
    };

    TEST_F(CxFileLiftCycleTest, DeleteFile) {
        CxFileSystem::deleteFile(_fp);
        EXPECT_EQ(false, CxFileSystem::isExist(_fp));
    }

    TEST_F(CxFileLiftCycleTest, CreateFile) {
        CxFileSystem::createFile(_fp);
        EXPECT_EQ(true, CxFileSystem::isExist(_fp));
    }

    TEST_F(CxFileLiftCycleTest, SaveFile) {
        CxFile::save(_fp, _ftext);
        EXPECT_EQ(_ftext.size(), CxFileSystem::sizeOfFile(_fp));
    }

    TEST_F(CxFileLiftCycleTest, LoadFile) {
        string s = CxFile::load(_fp);
        EXPECT_EQ(_ftext.size(), s.size());
        CxFileSystem::deleteFile(_fp);
    }

}  // namespace
