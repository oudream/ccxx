//
// Created by oudream on 2017/5/27.
//

#include <ccxx/ccxx.h>

using namespace std;

int testString11()
{
    string s1 = "0x02f0 -t 5s -e 门禁1#，火车设备厂2# -p tonga";
    string s2 = " -t 5s -p tonga -e 门禁1#，火车设备厂2# 0x02f0 ";
    vector<string> ss1 = CxString::split(s1, ' ');
    vector<string> ss2 = CxString::split(s2, ' ');

    msepoch_t dtNow1 = CxTime::currentSystemTime();

    for (int i = 0; i < 1024 * 10; ++i)
    {
        CxString::equal(ss1, ss2, true, true, true);
    }
    cxPrompt() << "IgnoreOrder ss1 same to ss2: " << CxString::equal(ss1, ss2, true, true, true);
    cxPrompt() << "No IgnoreOrder ss1 same to ss2: " << CxString::equal(ss1, ss2, false, true, true);
    cxPrompt() << "dtNow2 - dtNow1: " << int(CxTime::milliSecondDifferToNow(dtNow1));

    return 0;
}

int testString12()
{
    string s1 = "0x02f0 -t 5s -e -p tonga";

    cxPrompt() << s1;
    cxPrompt() << CxString::replace(s1, "t", "tt");

    string s2;

    cxPrompt() << CxString::replace(s2, "", "tt");

    return 0;
}