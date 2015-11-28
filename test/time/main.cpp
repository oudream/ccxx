
#include <ccxx/ccxx.h>

using namespace std;

#ifdef GM_OS_WIN
#include <windows.h>
#endif

void testTime1()
{
    vector<msepoch_t> ls1;
    vector<msepoch_t> ls2;
    msepoch_t l1;
    msepoch_t l2;
    for (size_t i = 0; i < 1000 * 100; ++i)
    {
        l1 = CxTime::currentSystemTime();
        l2 = CxTime::currentSystemTime();
        ls1.push_back(l1);
        ls2.push_back(l2);
        if (i % 1000 == 0)
            CxThread::sleep(1);
    }

    vector<string> ls3;
    for (size_t i = 0; i < ls1.size(); ++i)
    {
        ls3.push_back(CxString::toString(ls1[i]));
        ls3.push_back(CxString::toString(ls2[i]));
    }

    CxFile::save(CxApplication::applicationPath() + "/001.txt", ls3);
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

    CxFile::save(CxApplication::applicationPath() + "/001.txt", ls3);
}

void testTime3()
{
    for (int i = 0; i < 1000; ++i)
    {
        cout << CxTime::currentSystemTimeString() << endl;
        CxThread::sleep(100);
    }
}

void testTime4()
{
    for (int i = 0; i < 1000; ++i)
    {
        cout << CxTime::toString(CxTime::localTm()) << endl;
        CxThread::sleep(100);
    }
}

void testTime5()
{
    for (int i = 0; i < 1000; ++i)
    {
        cout << CxTime::currentSepochString() << endl;
        CxThread::sleep(100);
    }
}

int main(int argc, char * argv[])
{
//    CxApplication::init(argc, argv);

//    testTime5();

    string sDt = "2015年10月20日,3点40分50秒";

    clock_t dtNow1 = clock();

    for (int i = 0; i < 1000 * 1000 * 10; ++i)
    {
        tm dt = CxTime::toTm(sDt);
    }

    clock_t dtNow2 = clock();

    cout << "CxTime::toTm cost time : " << dtNow2 - dtNow1 << endl;

    for (int i = 0; i < 1000 * 1000 * 10; ++i)
    {
        tm dt;
        CxTime::fromString(sDt, dt);
    }

    clock_t dtNow3 = clock();

    cout << "CxTime::fromString cost time : " << dtNow3 - dtNow2 << endl;

    return 0;
}

