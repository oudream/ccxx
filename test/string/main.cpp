#include <ccxx/cxapplication.h>
#include <ccxx/cxstring.h>
#include <ccxx/cxfile.h>
#include <ccxx/cxinterinfo.h>

//#include "temp1.cpp"
#include "testString1.cpp"
#include "testString2.cpp"

using namespace std;

size_t fn_printf(const char *format, ...)
{
    char buf[1024];
    va_list args;

    va_start(args, format);
    vsnprintf(buf, sizeof(buf), format, args);
    va_end(args);

    return printf(buf);
}

void testString001_split()
{
    string s = "\n\n\n";
    vector<string> sLines = CxString::split(s, '\n', true);
    for (size_t i = 0; i < sLines.size(); ++i)
    {
        cout << sLines.at(i) << endl;
    }
}

void testString002_token()
{
    string s1 = "aaaaa.12341234.bbbbbbbbb";
    string r1 = CxString::token(s1, '.');
    cout << "token result=" << r1 << " input string=" << s1 << endl;
}

void testString002_tokenLeft()
{
    string s1 = "aaaaa.12341234.bbbbbbbbb";
    string r1 = CxString::tokenLeft(s1, '.');
    cout << "tokenLeft result=" << r1 << " input string=" << s1 << endl;
}

void testString002_tokenRight()
{
    string s1 = "aaaaa.12341234.bbbbbbbbb";    string r1 = CxString::tokenRight(s1, '.');
    cout << "tokenRight result=" << r1 << " input string=" << s1 << endl;
}


std::map<string, string> fn_doSplitToMap_trim(const string &s, char cMid, char cSplitCharacter)
{
    std::map<string, string> r;

    size_t start = 0;
    size_t end;
    char sMid[2] = {cMid, 0};
    while ((end = s.find(cSplitCharacter, start)) != string::npos)
    {
        size_t iMid = s.find(sMid, start, end - start);
        if (iMid != string::npos)
        {
            string sKey = s.substr(start, iMid);
            string sValue = s.substr(iMid, end - start);
            r[sKey] = sValue;
        }
        else
        {
            string sKey = s.substr(start, end - start);
            string sValue;
            r[sKey] = sValue;
        }
        start = end + 1;
    }
    if (start != s.size())
    {
        size_t iMid = s.find(cMid, start);
        if (iMid != string::npos)
        {
            string sKey = s.substr(start, iMid);
            string sValue = s.substr(iMid);
            r[sKey] = sValue;
        }
        else
        {
            string sKey = s.substr(start);
            string sValue;
            r[sKey] = sValue;
        }
    }
    return r;
}

std::map<string, string> fn_doSplitToMap(const string &s, char cMid, char cSplitCharacter)
{
    std::map<string, string> r;
    const char * pchBeing = s.data();
    const char * pchEnd = pchBeing+s.size();
    const char * pchIndex = pchBeing;
    while ((*pchIndex == cMid) || (*pchIndex == cSplitCharacter))
    {
        pchIndex++;
    }
    const char * pchMid = pchBeing;
    const char * pchSplit = pchBeing;
    const char * pchKeyBeing;
    const char * pchKeyEnd;
    const char * pchValueBeing;
    const char * pchValueEnd;
    while (pchIndex < pchEnd)
    {
        if ((*pchIndex == cMid))
        {
            if ( (*(pchIndex+1) != '\\') && (pchMid <= pchSplit) )
                pchMid = pchIndex;
        }
        else if ((*pchIndex == cSplitCharacter ) && (*(pchIndex+1) != '\\'))
        {
            if (pchMid > pchSplit)
            {
                pchKeyBeing = pchSplit+1;
                pchKeyEnd = pchMid;
                pchValueBeing = pchMid+1;
                pchValueEnd = pchIndex;
                if (pchKeyEnd > pchKeyBeing)
                {
                    string sKey = CxString::trim(string(pchKeyBeing, pchKeyEnd));
                    if (pchValueEnd > pchValueBeing)
                        r[sKey] = string(pchValueBeing, pchValueEnd);
                    else
                        r[sKey] = string();
                }
            }
            else
            {
                pchKeyBeing = pchSplit+1;
                pchKeyEnd = pchIndex;
                if (pchKeyEnd > pchKeyBeing)
                {
                    r[CxString::trim(string(pchKeyBeing, pchKeyEnd))] = string();
                }
            }
            pchSplit = pchIndex;
        }
        pchIndex++;
    }
    if (pchSplit != pchEnd-1)
    {
        if (pchMid > pchSplit)
        {
            pchKeyBeing = pchSplit+1;
            pchKeyEnd = pchMid;
            pchValueBeing = pchMid+1;
            pchValueEnd = pchIndex;
            if (pchKeyEnd > pchKeyBeing)
            {
                string sKey = CxString::trim(string(pchKeyBeing, pchKeyEnd));
                if (pchValueEnd > pchValueBeing)
                    r[sKey] = string(pchValueBeing, pchValueEnd);
                else
                    r[sKey] = string();
            }
        }
        else
        {
            pchKeyBeing = pchSplit+1;
            pchKeyEnd = pchIndex;
            if (pchKeyEnd > pchKeyBeing)
            {
                string sKey = CxString::trim(string(pchKeyBeing, pchKeyEnd));
                r[sKey] = string();
            }
        }
    }
    return r;
}


void testString003_tomap()
{
    string sText;
    if ( CxFile::load("c:/0.txt", sText) )
    {
        clock_t dtNow = clock();
        std::map<string, string> r;
        for (size_t i = 0; i < 1000 * 100; ++i)
        {
            r = CxString::splitToMap(sText , '=', ';');
        }
        cout << clock() - dtNow << " count=" << r.size() << endl;
        cxDebug() << r;

//        dtNow = clock();
//        for (size_t i = 0; i < 1000 * 10; ++i)
//        {
//            r = fn_doSplitToMap_trim(sText , '=', ';');
//        }
//        cout << clock() - dtNow << " count=" << r.size() << endl;
//        cxDebug() << r;

        dtNow = clock();
        for (size_t i = 0; i < 1000 * 100; ++i)
        {
            r = fn_doSplitToMap(sText , '=', ';');
        }
        cout << clock() - dtNow << " count=" << r.size() << endl;
        cxDebug() << r;
    }
}

void testString004_isValidPath()
{
    vector<string> ss;
    ss.push_back(string("c:/a.a"));
    ss.push_back(string("c:\\a.a"));
    ss.push_back(string("\\temp\\a.a"));
    ss.push_back(string("\\c:\\temp\\a.a"));
    ss.push_back(string("c:\\temp\\a.a"));
    ss.push_back(string("c:\\temp\\a.a"));
    ss.push_back(string());

    for (size_t i = 0; i < ss.size(); ++i)
    {
        const string & s = ss.at(i);
        cout << i << " return " << CxString::isValidPath(s) << endl;
    }

}

#include <ccxx/cxvalue.h>

void testString005_toString()
{
    double dMin = CxValueType::minValue<double>()/*GM_DOUBLE_MIN*/;
    double dMax = CxValueType::maxValue<double>();

    cout << dMin << endl;
    cout << dMax << endl;

    string sdMin = CxString::toString(dMin);
    string sdMax = CxString::toString(dMax);

    cout << sdMin << endl;
    cout << sdMax << endl;

    double d;
    d = CxString::fromString(sdMin, d);
    cout << (d == dMin ? "yes" : "no") << endl;
}

void testString006_toString()
{
    longlong dMin = GM_MSEPOCH_MIN/*GM_DOUBLE_MIN*/;
    longlong dMax = GM_MSEPOCH_MAX;

    cout << dMin << endl;
    cout << dMax << endl;

    string sdMin = CxString::toString(dMin);
    string sdMax = CxString::toString(dMax);

    cout << sdMin << endl;
    cout << sdMax << endl;

    longlong d;
    d = CxString::fromString(sdMin, d);
    cout << (d == dMin ? "yes" : "no") << endl;
}

void testString007_toString()
{
    msepoch_t dt = 1000 * 60 * 60;
    dt = dt * 24 * 365 * 1000;
    msepoch_t dMin = CxTime::currentSystemTime() - dt/*GM_DOUBLE_MIN*/;
    msepoch_t dMax = CxTime::currentSystemTime() + dt;

    cout << dMin << endl;
    cout << dMax << endl;

    string sdMin = CxTime::toString(dMin);
    string sdMax = CxTime::toString(dMax);

    cout << sdMin << endl;
    cout << sdMax << endl;

    msepoch_t d;
    d = CxTime::fromString(sdMin, d);
    cout << (d == dMin ? "yes" : "no") << endl;
}

void testString008_toString()
{
    msepoch_t dMin ;
    msepoch_t dMax ;
    CxTime::fromString("0001-01-01 00:00:000", dMin)/*GM_DOUBLE_MIN*/;
    CxTime::fromString("1969-12-12 23:59:999", dMax)/*GM_DOUBLE_MIN*/;

    cout << dMin << endl;
    cout << dMax << endl;

    msepoch_t dt = CxTime::currentSystemTime();
    cout << dt << endl;

    cout << CxTime::toString(dMin) << endl;
    cout << CxTime::toString(dMax) << endl;
}

void testString009_unquote()
{
    string sSqlCommand = "post.sql.select..v1";
    string sDbname = CxString::unquote(sSqlCommand, "post.sql.select.", ".");
    cout << sDbname << endl;

    string s20 = "<123 456=789>";
    string s21 = CxString::unquote(s20, "<", ">");
    cout << s21 << endl;
}

int main(int argc, char *argv[])
{
    CxApplication::init(argc, argv);

    testString009_unquote();

    cout << "--" << endl;
    cout << "--" << endl;
    cout << "--" << endl;

    return 0;
}
