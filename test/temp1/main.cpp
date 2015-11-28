
#include <cxApplication.h>
#include <cxTime.h>
#include <cxString.h>
#include <cxFile.h>

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

    CxFile::saveToFile(CxApplication::applicationPath() + "/001.txt", ls3);
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

    CxFile::saveToFile(CxApplication::applicationPath() + "/001.txt", ls3);
}

int main(int argc, char * argv[])
{
    CxApplication::init(argc, argv);

    testTime2();

    return 0;
}

