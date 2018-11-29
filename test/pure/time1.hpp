//
// Created by oudream on 2017/5/27.
//

#include <ccxx/ccxx.h>

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

int testCxTime_toMslong1()
{
    for (int i = 0; i < 100; ++i)
    {
        cout << CxTime::toMslong("30d") << endl;
        cout << CxTime::toMslong("30ms") << endl;
        cout << CxTime::toMslong("1h2d3ms") << endl;
        cout << CxTime::toMslong("1h 2d3ms") << endl;
        cout << CxTime::toMslong("11") << endl;
        cout << CxTime::toMslong("a") << endl;
        cout << CxTime::toMslong("") << endl;
    }
    return 0;
}