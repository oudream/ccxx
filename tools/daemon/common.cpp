
#include <typeinfo>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <bitset>
#include <stack>
#include <queue>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <iso646.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>
#include <wctype.h>

#undef UNICODE
#include <windows.h>
#include <tlhelp32.h>

typedef long long msepoch_t;
typedef long long int64;           /* 64 bit signed */
typedef unsigned long long uint64; /* 64 bit unsigned */

typedef void (*fn_void_int_t)(int);

#define GM_TIME_UTC_MST (-7)
#define GM_TIME_UTC_    (0)
#define GM_TIME_UTC_CCT (+8)

#define GM_TIME_UTC_LOCAL GM_TIME_UTC_CCT

#define GM_TIME_UTC_DIFF_MS (GM_TIME_UTC_LOCAL * 60 * 60 * 100)

using namespace std;

const char CCHexChar16[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

const std::string f_sEmptyString = "";

static char f_sPathCharacter = '\\';
static string f_sApplicationPath = "";
static string f_sApplicationFilePath = "";
static string f_sApplicationConfigFilePath = "";
static map<string, string> f_arguments;
static char f_sSpace = ' ';

string fn_trim(const string &s, const char cDelete)
{
    if (s.empty()) return "";
    char* first = const_cast<char *>( s.data() );
    char* end = first + s.size() - 1;

    char * pchFirst = first;
    char * pchEnd = end;
    while (pchFirst < end)
    {
        if (* pchFirst == cDelete)
            pchFirst ++;
        else
            break;
    }
    while (pchEnd > first)
    {
        if (* pchEnd == cDelete)
            pchEnd --;
        else
            break;
    }
    if (pchFirst == first && pchEnd == end)
    {
        return s;
    }
    else if (pchEnd >= pchFirst)
    {
        return string(pchFirst, pchEnd - pchFirst + 1);
    }
    else
    {
        return string("");
    }
}

std::map<string, string> fn_application_arguments(int argc, char *argv[])
{
    map<string, string> r;
    int i = 1;
    while (i < argc && *argv[i] == '-')
    {
        char * pc = argv[i];
        ++i;
        ++pc;
        string sName = pc;
        string sValue;
        while (i < argc && *argv[i] != '-')
        {
            sValue += argv[i];
            ++i;
            sValue.push_back(' ');
        }
        sValue.resize(sValue.size()-1);
        r[sName] = sValue;
    }
    return r;
}



const string & fn_application_findArgument(const string &sArgumentName)
{
    map<string, string>::const_iterator it = f_arguments.find(sArgumentName);
    if (it != f_arguments.end())
        return it->second;
    else
        return f_sEmptyString;
}

unsigned int julianDayFromGregorianDate1(int year, int month, int day)
{
    // Gregorian calendar starting from October 15, 1582
    // Algorithm from Henry F. Fliegel and Thomas C. Van Flandern
    return (1461 * (year + 4800 + (month - 14) / 12)) / 4
            + (367 * (month - 2 - 12 * ((month - 14) / 12))) / 12
            - (3 * ((year + 4900 + (month - 14) / 12) / 100)) / 4
            + day - 32075;
}

unsigned int julianDayFromDate1(int year, int month, int day)
{
    if (year < 0)
        ++year;

    if (year > 1582 || (year == 1582 && (month > 10 || (month == 10 && day >= 15)))) {
        return julianDayFromGregorianDate1(year, month, day);
    } else if (year < 1582 || (year == 1582 && (month < 10 || (month == 10 && day <= 4)))) {
        // Julian calendar until October 4, 1582
        // Algorithm from Frequently Asked Questions about Calendars by Claus Toendering
        int a = (14 - month) / 12;
        return (153 * (month + (12 * a) - 3) + 2) / 5
                + (1461 * (year + 4800 - a)) / 4
                + day - 32083;
    } else {
        // the day following October 4, 1582 is October 15, 1582
        return 0;
    }
}

void fn_decodeLocalTm(const msepoch_t &dt, int &y, int &m, int &d, int &h, int &mi, int &se, int &ms)
{
    msepoch_t msecs = dt + GM_TIME_UTC_DIFF_MS;
    int ddays = msecs / 86400000;
    msecs %= 86400000;
    if (msecs < 0) {
        // negative
        --ddays;
        msecs += 86400000;
    }

    unsigned int jd = julianDayFromDate1(1970, 1, 1);
    if (ddays >= 0)
        ddays = (jd + ddays >= jd) ? jd + ddays : 0;
    else
        ddays = (jd + ddays < jd) ? jd + ddays : 0;

    if (ddays >= 2299161) {
        // Gregorian calendar starting from October 15, 1582
        // This algorithm is from Henry F. Fliegel and Thomas C. Van Flandern
        msepoch_t ell, n, i, j;
        ell = msepoch_t(ddays) + 68569;
        n = (4 * ell) / 146097;
        ell = ell - (146097 * n + 3) / 4;
        i = (4000 * (ell + 1)) / 1461001;
        ell = ell - (1461 * i) / 4 + 31;
        j = (80 * ell) / 2447;
        d = ell - (2447 * j) / 80;
        ell = j / 11;
        m = j + 2 - (12 * ell);
        y = 100 * (n - 49) + i + ell;
    } else {
        // Julian calendar until October 4, 1582
        // Algorithm from Frequently Asked Questions about Calendars by Claus Toendering
        ddays += 32082;
        int dd = (4 * ddays + 3) / 1461;
        int ee = ddays - (1461 * dd) / 4;
        int mm = ((5 * ee) + 2) / 153;
        d = ee - (153 * mm + 2) / 5 + 1;
        m = mm + 3 - 12 * (mm / 10);
        y = dd - 4800 + (mm / 10);
        if (y <= 0)
            --y;
    }

    int mds = msecs % 86400000;
    if (msecs < 0) {
        // % not well-defined for -ve, but / is.
        int negdays = (86400000 - msecs) / 86400000;
        mds = (msecs + negdays * 86400000) % 86400000;
    }

    h = mds / 3600000;
    mi = (mds % 3600000) / 60000;
    se = (mds / 1000) % 60;
    ms = (mds % 1000);
}

string fn_toString(int y, int m, int d, int h, int mi, int se, int ms)
{
    char dts[23] = {'2', '0', '1', '2', '/', '1', '2', '/', '1', '2', ' ', '1', '2', ':', '1', '2', ':', '1', '2', ':', '1', '2', '2'};

    if (y > -1 && y < 10000 && m > 0 && m < 13 && d > 0 && d < 32 && h > -1 && h < 24 && mi > -1 && mi < 60 && se > -1 && se < 100)
    {
        int hc, lc;

        int yy = y % 100;

        hc = yy / 10;
        lc = yy % 10;
        dts[2] = CCHexChar16[ hc ];
        dts[3] = CCHexChar16[ lc ];

        hc = m / 10;
        lc = m % 10;
        dts[5] = CCHexChar16[ hc ];
        dts[6] = CCHexChar16[ lc ];

        hc = d / 10;
        lc = d % 10;
        dts[8] = CCHexChar16[ hc ];
        dts[9] = CCHexChar16[ lc ];

        hc = h / 10;
        lc = h % 10;
        dts[11] = CCHexChar16[ hc ];
        dts[12] = CCHexChar16[ lc ];

        hc = mi / 10;
        lc = mi % 10;
        dts[14] = CCHexChar16[ hc ];
        dts[15] = CCHexChar16[ lc ];

        hc = se / 10;
        lc = se % 10;
        dts[17] = CCHexChar16[ hc ];
        dts[18] = CCHexChar16[ lc ];

        hc = ms / 100;
        lc = (ms % 100) / 10;
        dts[20] = CCHexChar16[ hc ];
        dts[21] = CCHexChar16[ lc ];
        lc = ms % 10;
        dts[22] = CCHexChar16[ lc ];
    }
    else
    {
    }

    return string(dts);
}

string fn_toString(const msepoch_t &lMsepoch)
{
    int y, m, d, h, mi, se, ms;
    fn_decodeLocalTm(lMsepoch, y, m, d, h, mi, se, ms);
    return fn_toString(y , m, d, h, mi, se, ms);
}

msepoch_t fn_currentMsepoch()
{
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);

    const uint64 iShift = 116444736000000000ULL; // (27111902 << 32) + 3577643008

    union {
        FILETIME as_file_time;
        uint64 as_integer; // 100-nanos since 1601-Jan-01
    } caster;
    caster.as_file_time = ft;

    caster.as_integer -= iShift; // filetime is now 100-nanos since 1970-Jan-01
    uint64 iMicros = caster.as_integer / 10; // truncate to microseconds

    // and cannot be before 1970-Jan-01
    time_t t = static_cast<time_t>(iMicros / 1000000UL); // seconds since epoch
    // microseconds -- static casts supress warnings
    unsigned int sub_sec = static_cast<unsigned int>(iMicros % 1000000UL);
    msepoch_t dt = t;
    return dt * 1000 + sub_sec / 1000;
}

string fn_format(const char *sFormat, ...)
{
    char buffer[512];
    va_list args;
    va_start (args, sFormat);
    vsprintf (buffer, sFormat, args);
    va_end (args);
    return buffer;
}

string fn_extractFileName(const string& sFilePath)
{
    size_t found = sFilePath.find_last_of("/\\");
    if (found == string::npos)
    {
        return sFilePath;
    }
    else
    {
        return sFilePath.substr(found+1);
    }
}

string fn_extractFileNamePrefix(const string& sFileNme)
{
    size_t found = sFileNme.find_last_of(".");
    if (found == string::npos)
    {
        return sFileNme;
    }
    else
    {
        return sFileNme.substr(0,found);
    }
}

string fn_extractFilePath(const string& sFilePath)
{
    size_t found = sFilePath.find_last_of("/\\");
    if (found == string::npos)
    {
        return sFilePath;
    }
    else
    {
        return sFilePath.substr(0,found);
    }
}

vector<string> fn_split(const string &s, char cSplitCharacter)
{
    vector<string> list;
    size_t start = 0;
    size_t end;
    while ((end = s.find(cSplitCharacter, start)) != string::npos)
    {
        if (start != end)
            list.push_back(s.substr(start, end - start));
        start = end + 1;
    }
    if (start != s.size())
        list.push_back(s.substr(start));
    return list;
}

vector<vector<string> > fn_split(const vector<string> & ss, char cHeadCharacter, char cIgnore)
{
    vector<vector<string> > lists;
    vector<string> list;
    for (size_t i = 0; i < ss.size(); ++i)
    {
        const string & s = ss.at(i);
        if (s.size()>0 && s[0] == cIgnore)
        {
            continue;
        }
        if (s.size()>0 && s[0] == cHeadCharacter)
        {
            if (list.size()>0)
            {
                lists.push_back(list);
                list.clear();
            }
            continue;
        }
        if (s.size()>0)
        {
            list.push_back(s);
        }
    }
    if (list.size()>0)
    {
        lists.push_back(list);
        list.clear();
    }
    return lists;
}

map<string, string> fn_splitToMap(const string &s, char sMid, char cSplitCharacter)
{
    std::map<string, string> r;
    size_t start = 0;
    size_t end;
    while ((end = s.find(sMid, start)) != string::npos)
    {
        if (start != end)
        {
            string sContent = s.substr(start, end - start);
            size_t endSplit = sContent.find(cSplitCharacter);
            if (endSplit != string::npos)
            {
                string sKey = sContent.substr(0,endSplit);
                sKey = fn_trim(sKey,f_sSpace);
                string sValue = sContent.substr(endSplit+1);
                sValue = fn_trim(sValue,f_sSpace);
                if(sKey.length()>0)
                {
                   r[sKey] = sValue;
                }
            }
            else
            {
                r[sContent] = string();
            }
        }
        start = end + 1;
    }
    if (start != s.size())
    {
        string sContent = s.substr(start);
        size_t endSplit = sContent.find(cSplitCharacter);
        if (endSplit != string::npos)
        {
            string sKey = sContent.substr(0,endSplit);
            sKey = fn_trim(sKey,f_sSpace);
            string sValue = sContent.substr(endSplit+1);
            sValue = fn_trim(sValue,f_sSpace);

            if(sKey.length()>0)
            {
               r[sKey] = sValue;
            }
        }
        else
        {
            r[sContent] = string();
        }
    }
    return r;
}

map<string, string> fn_splitToMap(const vector<string> &ss, char cSplitCharacter)
{
    std::map<string, string> r;
    for (size_t i = 0; i < ss.size(); ++i)
    {
        const string & sContent = ss.at(i);
        size_t endSplit = sContent.find(cSplitCharacter);
        if (endSplit != string::npos)
        {
            string sKey = sContent.substr(0,endSplit);
            sKey = fn_trim(sKey,f_sSpace);
            string sValue = sContent.substr(endSplit+1);
            sValue = fn_trim(sValue,f_sSpace);
            if(sKey.length()>0)
            {
               r[sKey] = sValue;
            }
        }
        else
        {
            r[sContent] = string();
        }
    }
    return r;
}

//Kill Process
int fn_killProcess(const char *szProcessName)
{
      HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

      PROCESSENTRY32 pe;
      pe.dwSize = sizeof(PROCESSENTRY32);
      if(!Process32First(hSnapShot,&pe))
      {
           return -1;
      }

      int nRet = 0;
      while (Process32Next(hSnapShot,&pe))
      {
            if(strcmp(szProcessName, pe.szExeFile) == 0)
            {
                cout << "kill process:" << pe.th32ProcessID << " Name: " << pe.szExeFile<<endl;

                DWORD dwProcessID = pe.th32ProcessID;
                HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE,FALSE,dwProcessID);
                ::TerminateProcess(hProcess,0);
                CloseHandle(hProcess);
                ::Sleep(1000);
                nRet++;
            }
      }
      return nRet;
}

string fn_toString(int i)
{
    char str[10+1];
    sprintf(str, "%d", i);
    return string(str);
}

string fn_toHexstring(const char *pData, int iLength, bool bHasEmptyChar = true)
{
    if (bHasEmptyChar)
    {
        vector<char> hex(iLength * 3);
        char *hexData = hex.data();
//        const uchar *data = (const uchar *)pData;
        for (int i = 0; i < iLength; ++i) {
            int j = (pData[i] >> 4) & 0xf;
            if (j <= 9)
                hexData[i*3] = (j + '0');
            else
                hexData[i*3] = (j + 'a' - 10);
            j = pData[i] & 0xf;
            if (j <= 9)
                hexData[i*3+1] = (j + '0');
            else
                hexData[i*3+1] = (j + 'a' - 10);
            hexData[i*3+2] = 0x20;
        }
        return string(hex.data(), hex.size());
    }
    else
    {
        vector<char> hex(iLength * 2);
        char *hexData = hex.data();
//        const uchar *data = (const uchar *)pData;
        for (int i = 0; i < iLength; ++i) {
            int j = (pData[i] >> 4) & 0xf;
            if (j <= 9)
                hexData[i*2] = (j + '0');
            else
                hexData[i*2] = (j + 'a' - 10);
            j = pData[i] & 0xf;
            if (j <= 9)
                hexData[i*2+1] = (j + '0');
            else
                hexData[i*2+1] = (j + 'a' - 10);
        }
        return string(hex.data(), hex.size());
    }
}

vector<char> fn_fromHexstring(const string &s)
{
    string s2 = s;
    remove(s2.begin(), s2.end(), ' ');
    vector<char> res((s2.size() + 1)/ 2);
    char *result = res.data() + res.size();

    bool odd_digit = true;
    for (int i = s2.size() - 1; i >= 0; --i) {
        int ch = s2[i];
        int tmp;
        if (ch >= '0' && ch <= '9')
            tmp = ch - '0';
        else if (ch >= 'a' && ch <= 'f')
            tmp = ch - 'a' + 10;
        else if (ch >= 'A' && ch <= 'F')
            tmp = ch - 'A' + 10;
        else
            continue;
        if (odd_digit) {
            --result;
            *result = tmp;
            odd_digit = false;
        } else {
            *result |= tmp << 4;
            odd_digit = true;
        }
    }

    vector<char> rData;
    rData.resize(res.data() + res.size() - result);
    ::copy(result, res.data() + res.size(), rData.begin());
//    res.remove(0, result - res.data());
    return rData;
}

string fn_mergeFilePath(const string &sPath, const string &sFileName)
{
    string r = sPath;
    r.push_back(f_sPathCharacter);
    r.append(sFileName);
    return r;
}

string fn_toLower( const string & s )
{
    string r(s.data(), s.size());
    char * pchBegin = const_cast<char *>( r.data() );
    char * pchEnd = pchBegin + r.size();
    char * pch = pchBegin;
    while ( pch < pchEnd )
    {
        if ( * pch >= 0x41 && * pch <= 0x5A )
            * pch += 0x20;
        pch ++;
    }
    return r;
}

bool fn_isValidHexCharater( const string & s )
{
    const char * pchBegin = s.data();
    const char * pchEnd = pchBegin + s.size();
    const char * pch = pchBegin;
    while ( pch < pchEnd )
    {
        if (! ( ( * pch >= '0' && * pch <= '9' ) || ( * pch >= 'a' && * pch <= 'f' ) || ( * pch >= 'A' && * pch <= 'F' ) ) )
            return false;
        pch ++;
    }
    return true;
}

int fn_fileLoad(const string &sFileName, std::vector<std::string> & sOut, const string &sSplitString)
{
    FILE * pFile;
    size_t iFileSize;
    size_t iOldSize = sOut.size();

    pFile = fopen ( sFileName.data() , "rb" );
    if (pFile==NULL)
    {
        return sOut.size() - iOldSize;
    }

    // obtain file size:
    fseek (pFile , 0 , SEEK_END);
    iFileSize = ftell (pFile);
    if (iFileSize <= 0)
    {
//        fputs ("file is empty",stderr);
        fclose (pFile);
        return sOut.size() - iOldSize;
    }
    rewind (pFile);

    const size_t ci_bufferSize = 512;
    vector<char> buffer(ci_bufferSize, 0);
    char * pBuffer = &(buffer.front());
    string sLine;
    size_t iReadSize = 0;
    while (iReadSize < iFileSize)
    {
        size_t iRemain = iFileSize - iReadSize;
        size_t iReadingLen = (iRemain > ci_bufferSize) ? ci_bufferSize : iRemain;

        // copy the file into the buffer:
        if ( fread (pBuffer,1,iReadingLen,pFile) != iReadingLen )
        {
            // read to file end
            fclose (pFile);
            return sOut.size() - iOldSize;
        }
        else
        {
            sLine.append(string(pBuffer, iReadingLen));

            size_t start = 0;
            size_t end;
//            while ((end = CxString::search(sLine.data()+start, sLine.size()-start, sSplitString.data(), sSplitString.size())) != string::npos)
            while ((end = sLine.find(sSplitString, start)) != string::npos)
            {
                if (start != end)
                    sOut.push_back(sLine.substr(start, end - start));
                start = end + sSplitString.size();
            }
            if (start > 0)
            {
                sLine = sLine.substr(start);
            }
        }
        iReadSize += iReadingLen;
    }
    if (sLine.size() > 0)
    {
        sOut.push_back(sLine);
    }

    /* the whole file is now loaded in the memory buffer. */
    fclose (pFile);
    return sOut.size() - iOldSize;
}
//初始化
void fn_application_init(int argc, char *argv[])
{
    if ( argc > 0 )
    {
        f_sApplicationFilePath = argv[0];
//        f_sApplicationConfigFilePath = f_sApplicationFilePath.append(".ini");

        string sName = fn_extractFileNamePrefix(fn_extractFileName(f_sApplicationFilePath));
        string sPath = fn_extractFilePath(f_sApplicationFilePath);
        f_sApplicationConfigFilePath =  fn_mergeFilePath(sPath,sName.append(".ini"));

        cout<<"ApplicationConfigFilePath:"<<f_sApplicationConfigFilePath<< endl;

        size_t iIndex = f_sApplicationFilePath.find("\\");
        if (iIndex > 0)
            f_sPathCharacter = '\\';
        else
            f_sPathCharacter = '/';
        iIndex = f_sApplicationFilePath.find_last_of("/\\");
        if ( iIndex > 0)
            f_sApplicationPath = f_sApplicationFilePath.substr(0, iIndex);
        else
            f_sApplicationPath = "";
    }
    if ( argc > 1 )
    {
        f_arguments = fn_application_arguments(argc, argv);
    }
}
