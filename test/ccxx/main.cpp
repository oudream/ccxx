
#include <cxapplication.h>
#include <cxtime.h>
#include <cxstring.h>
#include <cxfile.h>

//#include <ucommon/file.h>
//#include <ucommon/fsys.h>

using namespace std;


void testTime1()
{
    vector<msepoch> ls1;
    vector<msepoch> ls2;
    msepoch l1;
    msepoch l2;
    for (size_t i = 0; i < 1000 * 100; ++i)
    {
        l1 = CxTime::currentMsepoch();
        l2 = CxTime::currentSystemTime();
        ls1.push_back(l1);
        ls2.push_back(l2);
    }

    vector<string> ls3;
    for (size_t i = 0; i < ls1.size(); ++i)
    {
        ls3.push_back(CxString::toString(ls1[i]));
        ls3.push_back(CxString::toString(ls2[i]));
    }

    CxFile::writeFromStrings(CxApplication::applicationPath() + "/001.txt", ls3);
}

void testTime2()
{
    vector<time_t> ls1;
    for (size_t i = 0; i < 1000 * 100; ++i)
    {
        ls1.push_back(clock());
    }

    vector<string> ls3;
    for (size_t i = 0; i < ls1.size(); ++i)
    {
        ls3.push_back(CxString::toString(ls1[i]));
    }

    CxFile::writeFromStrings(CxApplication::applicationPath() + "/001.txt", ls3);
}

int testFileSystem1()
{
//    ucommon::dir d("F:\\Tools\\Designer\\mingw\\include");
    char buffer[4096];
//    int iR = d.read(buffer, 4096);
    printf("%s", buffer);
}

int main(int argc, char * argv[])
{
    CxApplication::init(argc, argv);

    testFileSystem1();

    return 0;
}

