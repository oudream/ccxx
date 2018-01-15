#include "cxtime.h"

#ifdef GM_OS_WIN
#include <windows.h>
#else
#include <sys/time.h>
#endif

using namespace std;


//2208988800.0
//2208988800是1900到【GMT】1970的秒数 不对
//2209161600 是1900到【GMT】1970的秒数
//25569.0; // 1970/01/01


#define GM_TIME_UTC_DIFF_MS (GM_TIME_UTC_LOCAL * 60 * 60 * 1000)
#define GM_TIME_UTC_DIFF_SECOND (GM_TIME_UTC_LOCAL * 60 * 60)

const char CCHexChar16[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

const char CCNumberCharZero127[128] =
    {
        '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
        '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
        '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, '0', '0', '0', '0', '0', '0',
        '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
        '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
        '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
        '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'
    };

msepoch_t CxTime::currentSystemTime()
{
#ifdef GM_OS_WIN
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);

    const uint64 iShift = 116444736000000000ULL; // (27111902 << 32) + 3577643008

    union {
        FILETIME as_file_time;
        msepoch_t as_integer;   // 100-nanos since 1601-Jan-01
    } caster;
    caster.as_file_time = ft;
    caster.as_integer -= iShift; // filetime is now 100-nanos since 1970-Jan-01
    return caster.as_integer / 10000LL; // truncate to microseconds
#else
    timeval tv;
    gettimeofday(&tv, 0); //gettimeofday does not support TZ adjust on Linux.
    time_t t = tv.tv_sec;
    uint32 sub_sec = tv.tv_usec;
    msepoch_t dt = t;
    return dt * 1000 + sub_sec / 1000;
//#else
//#error Now however neither gettimeofday nor FILETIME support is detected.
#endif
}

msepoch_t CxTime::currentMsepoch()
{
#ifdef GM_OS_WIN
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);

    const uint64 iShift = 116444736000000000ULL; // (27111902 << 32) + 3577643008

    union {
        FILETIME as_file_time;
        msepoch_t as_integer;   // 100-nanos since 1601-Jan-01
    } caster;
    caster.as_file_time = ft;
    caster.as_integer -= iShift; // filetime is now 100-nanos since 1970-Jan-01
    return caster.as_integer / 10000LL; // truncate to microseconds
#else
    timeval tv;
    gettimeofday(&tv, 0); //gettimeofday does not support TZ adjust on Linux.
    time_t t = tv.tv_sec;
    uint32 sub_sec = tv.tv_usec;
    msepoch_t dt = t;
    return dt * 1000 + sub_sec / 1000;
//#else
//#error Now however neither gettimeofday nor FILETIME support is detected.
#endif
}

//in valid
//msepoch_t CxTime::currentMsepoch()
//{
//    msepoch_t r = clock() * 1000;
//    time_t t;
//    time(&t);
//    return (msepoch_t)t * 1000 + (r / CLOCKS_PER_SEC) % 1000;
//}

sepoch_t CxTime::currentSepoch()
{
    time_t t;
    time(&t);
    return t;
}

tm CxTime::currentUtc()
{
    time_t t;
    time(&t);
    return *gmtime(&t);
}

tm CxTime::localTm()
{
    time_t t;
    time(&t);
    return *localtime(&t);
}

inline uint32 julianDayFromGregorianDate1(int year, int month, int day)
{
    // Gregorian calendar starting from October 15, 1582
    // Algorithm from Henry F. Fliegel and Thomas C. Van Flandern
    return (1461 * (year + 4800 + (month - 14) / 12)) / 4
           + (367 * (month - 2 - 12 * ((month - 14) / 12))) / 12
           - (3 * ((year + 4900 + (month - 14) / 12) / 100)) / 4
           + day - 32075;
}

inline uint32 julianDayFromDate1(int year, int month, int day)
{
    if (year < 0)
        ++year;

    if (year > 1582 || (year == 1582 && (month > 10 || (month == 10 && day >= 15))))
    {
        return julianDayFromGregorianDate1(year, month, day);
    }
    else if (year < 1582 || (year == 1582 && (month < 10 || (month == 10 && day <= 4))))
    {
        // Julian calendar until October 4, 1582
        // Algorithm from Frequently Asked Questions about Calendars by Claus Toendering
        int a = (14 - month) / 12;
        return (153 * (month + (12 * a) - 3) + 2) / 5
               + (1461 * (year + 4800 - a)) / 4
               + day - 32083;
    }
    else
    {
        // the day following October 4, 1582 is October 15, 1582
        return 0;
    }
}

inline uint msecsFromDecomposed1(int hour, int minute, int sec, int msec = 0)
{
    return 3600000 * hour + 60000 * minute + 1000 * sec + msec;
}

string CxTime::toString(const msepoch_t &lMsepoch, char sSplit1, char sSplit2, char sSplit3)
{
    int y, m, d, h, mi, se, ms;
    decodeLocalTm(lMsepoch, y, m, d, h, mi, se, ms);
    return toString(y, m, d, h, mi, se, ms, sSplit1, sSplit2, sSplit3);
}

string CxTime::toStringSepoch(const sepoch_t &iSepoch, char sSplit1, char sSplit2, char sSplit3)
{
    int y, m, d, h, mi, se, ms;
    decodeLocalTm(iSepoch * 1000LL, y, m, d, h, mi, se, ms);
    return toString(y, m, d, h, mi, se, sSplit1, sSplit2, sSplit3);
}

string CxTime::toStringDay(const sepoch_t &iSepoch, char sSplit1)
{
    int y, m, d, h, mi, se, ms;
    decodeLocalTm(iSepoch * 1000LL, y, m, d, h, mi, se, ms);
    return toString(y, m, d, sSplit1);
}

string CxTime::toString(const tm &dtTm, char sSplit1, char sSplit2, char sSplit3)
{
    return toString(dtTm.tm_year,
                    dtTm.tm_mon + 1, dtTm.tm_mday, dtTm.tm_hour, dtTm.tm_min, dtTm.tm_sec, sSplit1, sSplit2, sSplit3);
}

string CxTime::toString(int y, int m, int d, char sSplit1)
{
    if (sSplit1 < ' ')
    {
        char dts[9] = {'2', '0', '1', '2', '1', '2', '1', '2', '\0'};

        if (y > -1 && y < 10000 && m > 0 && m < 13 && d > 0 && d < 32)
        {
            int hc, lc;

            int yy = y % 100;

            hc = yy / 10;
            lc = yy % 10;
            dts[2] = CCHexChar16[hc];
            dts[3] = CCHexChar16[lc];

            hc = m / 10;
            lc = m % 10;
            dts[4] = CCHexChar16[hc];
            dts[5] = CCHexChar16[lc];

            hc = d / 10;
            lc = d % 10;
            dts[6] = CCHexChar16[hc];
            dts[7] = CCHexChar16[lc];
        }
        else
        {
        }

        return string(dts);
    }
    else
    {
        char dts[11] = {'2', '0', '1', '2', sSplit1, '1', '2', sSplit1, '1', '2', '\0'};

        if (y > -1 && y < 10000 && m > 0 && m < 13 && d > 0 && d < 32)
        {
            int hc, lc;

            int yy = y % 100;

            hc = yy / 10;
            lc = yy % 10;
            dts[2] = CCHexChar16[hc];
            dts[3] = CCHexChar16[lc];

            hc = m / 10;
            lc = m % 10;
            dts[5] = CCHexChar16[hc];
            dts[6] = CCHexChar16[lc];

            hc = d / 10;
            lc = d % 10;
            dts[8] = CCHexChar16[hc];
            dts[9] = CCHexChar16[lc];
        }
        else
        {
        }

        return string(dts);
    }
}

string CxTime::toString(int y, int m, int d, int h, int mi, int se, char sSplit1, char sSplit2, char sSplit3)
{
    if (sSplit1 < ' ' || sSplit3 < ' ' || sSplit3 < ' ')
    {
        char dts[15] = {'2', '0', '1', '2', '1', '2', '1', '2', '1', '2', '1', '2', '1', '2', '\0'};

        if (y > -1 && y < 10000 && m > 0 && m < 13 && d > 0 && d < 32 && h > -1 && h < 24 && mi > -1 && mi < 60
            && se > -1 && se < 100)
        {
            int hc, lc;

            int yy = y % 100;

            hc = yy / 10;
            lc = yy % 10;
            dts[2] = CCHexChar16[hc];
            dts[3] = CCHexChar16[lc];

            hc = m / 10;
            lc = m % 10;
            dts[4] = CCHexChar16[hc];
            dts[5] = CCHexChar16[lc];

            hc = d / 10;
            lc = d % 10;
            dts[6] = CCHexChar16[hc];
            dts[7] = CCHexChar16[lc];

            hc = h / 10;
            lc = h % 10;
            dts[8] = CCHexChar16[hc];
            dts[9] = CCHexChar16[lc];

            hc = mi / 10;
            lc = mi % 10;
            dts[10] = CCHexChar16[hc];
            dts[11] = CCHexChar16[lc];

            hc = se / 10;
            lc = se % 10;
            dts[12] = CCHexChar16[hc];
            dts[13] = CCHexChar16[lc];
        }
        else
        {
        }

        return string(dts);
    }
    else
    {
        char dts[20] = {'2', '0', '1', '2', sSplit1, '1', '2', sSplit1, '1', '2', sSplit2, '1', '2', sSplit3, '1', '2',
                        sSplit3, '1', '2', '\0'};

        if (y > -1 && y < 10000 && m > 0 && m < 13 && d > 0 && d < 32 && h > -1 && h < 24 && mi > -1 && mi < 60
            && se > -1 && se < 100)
        {
            int hc, lc;

            int yy = y % 100;

            hc = yy / 10;
            lc = yy % 10;
            dts[2] = CCHexChar16[hc];
            dts[3] = CCHexChar16[lc];

            hc = m / 10;
            lc = m % 10;
            dts[5] = CCHexChar16[hc];
            dts[6] = CCHexChar16[lc];

            hc = d / 10;
            lc = d % 10;
            dts[8] = CCHexChar16[hc];
            dts[9] = CCHexChar16[lc];

            hc = h / 10;
            lc = h % 10;
            dts[11] = CCHexChar16[hc];
            dts[12] = CCHexChar16[lc];

            hc = mi / 10;
            lc = mi % 10;
            dts[14] = CCHexChar16[hc];
            dts[15] = CCHexChar16[lc];

            hc = se / 10;
            lc = se % 10;
            dts[17] = CCHexChar16[hc];
            dts[18] = CCHexChar16[lc];
        }
        else
        {
        }

        return string(dts);
    }
}

string CxTime::toString(int y, int m, int d, int h, int mi, int se, int ms, char sSplit1, char sSplit2, char sSplit3)
{
    if (sSplit1 < ' ' || sSplit3 < ' ' || sSplit3 < ' ')
    {
        char dts[18] = {'2', '0', '1', '2', '1', '2', '1', '2', '1', '2', '1', '2', '1', '2', '1', '2', '2', '\0'};

        if (y > -1 && y < 10000 && m > 0 && m < 13 && d > 0 && d < 32 && h > -1 && h < 24 && mi > -1 && mi < 60
            && se > -1 && se < 100)
        {
            int hc, lc;

            div_t y1 = div(y, 1000);
            div_t y2 = div(y1.rem, 100);
            div_t y3 = div(y2.rem, 10);
            dts[0] = CCHexChar16[y1.quot];
            dts[1] = CCHexChar16[y2.quot];
            dts[2] = CCHexChar16[y3.quot];
            dts[3] = CCHexChar16[y3.rem];

            hc = m / 10;
            lc = m % 10;
            dts[4] = CCHexChar16[hc];
            dts[5] = CCHexChar16[lc];

            hc = d / 10;
            lc = d % 10;
            dts[6] = CCHexChar16[hc];
            dts[7] = CCHexChar16[lc];

            hc = h / 10;
            lc = h % 10;
            dts[8] = CCHexChar16[hc];
            dts[9] = CCHexChar16[lc];

            hc = mi / 10;
            lc = mi % 10;
            dts[10] = CCHexChar16[hc];
            dts[11] = CCHexChar16[lc];

            hc = se / 10;
            lc = se % 10;
            dts[12] = CCHexChar16[hc];
            dts[13] = CCHexChar16[lc];

            hc = ms / 100;
            lc = (ms % 100) / 10;
            dts[14] = CCHexChar16[hc];
            dts[15] = CCHexChar16[lc];
            lc = ms % 10;
            dts[16] = CCHexChar16[lc];
        }
        else
        {
        }

        return string(dts);
    }
    else
    {
        char dts[24] = {'2', '0', '1', '2', sSplit1, '1', '2', sSplit1, '1', '2', sSplit2, '1', '2', sSplit3, '1', '2',
                        sSplit3, '1', '2', sSplit3, '1', '2', '2', '\0'};

        if (y > -1 && y < 10000 && m > 0 && m < 13 && d > 0 && d < 32 && h > -1 && h < 24 && mi > -1 && mi < 60
            && se > -1 && se < 100)
        {
            int hc, lc;

            div_t y1 = div(y, 1000);
            div_t y2 = div(y1.rem, 100);
            div_t y3 = div(y2.rem, 10);
            dts[0] = CCHexChar16[y1.quot];
            dts[1] = CCHexChar16[y2.quot];
            dts[2] = CCHexChar16[y3.quot];
            dts[3] = CCHexChar16[y3.rem];
//            int yy = y % 100;

//            hc = yy / 10;
//            lc = yy % 10;
//            dts[2] = CCHexChar16[ hc ];
//            dts[3] = CCHexChar16[ lc ];

            hc = m / 10;
            lc = m % 10;
            dts[5] = CCHexChar16[hc];
            dts[6] = CCHexChar16[lc];

            hc = d / 10;
            lc = d % 10;
            dts[8] = CCHexChar16[hc];
            dts[9] = CCHexChar16[lc];

            hc = h / 10;
            lc = h % 10;
            dts[11] = CCHexChar16[hc];
            dts[12] = CCHexChar16[lc];

            hc = mi / 10;
            lc = mi % 10;
            dts[14] = CCHexChar16[hc];
            dts[15] = CCHexChar16[lc];

            hc = se / 10;
            lc = se % 10;
            dts[17] = CCHexChar16[hc];
            dts[18] = CCHexChar16[lc];

            hc = ms / 100;
            lc = (ms % 100) / 10;
            dts[20] = CCHexChar16[hc];
            dts[21] = CCHexChar16[lc];
            lc = ms % 10;
            dts[22] = CCHexChar16[lc];
        }
        else
        {
        }

        return string(dts);
    }
}

bool CxTime::fromString(const string &sDateTime, msepoch_t &lMsepoch)
{
    int y, m, d, h, mi, se, ms = 0;
    if (decodeDataTime(sDateTime, y, m, d, h, mi, se, ms))
    {
        msepoch_t dt;
        dt = msecsFromDecomposed1(h, mi, se, ms);
        dt += msepoch_t(julianDayFromGregorianDate1(y, m, d) - julianDayFromGregorianDate1(1970, 1, 1))
              * GM_INT64_C(86400000);
        lMsepoch = dt - GM_TIME_UTC_DIFF_MS;
        return true;
    }
    return false;
}

msepoch_t CxTime::fromString(const string &sDateTime, bool *bOk)
{
    int y, m, d, h, mi, se, ms = 0;
    if (decodeDataTime(sDateTime, y, m, d, h, mi, se, ms))
    {
        if (bOk) *bOk = true;
        msepoch_t dt;
        dt = msecsFromDecomposed1(h, mi, se, ms);
        dt += msepoch_t(julianDayFromGregorianDate1(y, m, d) - julianDayFromGregorianDate1(1970, 1, 1))
              * GM_INT64_C(86400000);
        return dt - GM_TIME_UTC_DIFF_MS;
    }
    if (bOk) *bOk = false;
    return 0;
}

bool CxTime::fromStringSepoch(const string &sDateTime, sepoch_t &iSepoch)
{
    int y, m, d, h, mi, se, ms = 0;
    if (decodeDataTime(sDateTime, y, m, d, h, mi, se, ms))
    {
        tm tm1;
        tm1.tm_year = y - 1900;
        tm1.tm_mon = m - 1;
        tm1.tm_mday = d;
        tm1.tm_hour = h;
        tm1.tm_min = mi;
        tm1.tm_sec = se;
        tm1.tm_isdst = 0;
        iSepoch = mktime(&tm1);
        return true;
    }
    return false;
}

bool CxTime::fromString(const string &sDateTime, tm &dtTm)
{
    int y, m, d, h, mi, se, ms = 0;
    if (decodeDataTime(sDateTime, y, m, d, h, mi, se, ms))
    {
        dtTm.tm_year = y - 1900;
        dtTm.tm_mon = m - 1;
        dtTm.tm_mday = d;
        dtTm.tm_hour = h;
        dtTm.tm_min = mi;
        dtTm.tm_sec = se;
        dtTm.tm_isdst = 0;
        return true;
    }
    return false;
}

tm CxTime::toTm(const string &sDateTime)
{
    tm rTm;
    int y, m, d, h, mi, se, ms;
    decodeDataTime2(sDateTime, y, m, d, h, mi, se, ms);
    rTm.tm_year = y - 1900;
    rTm.tm_mon = m - 1;
    rTm.tm_mday = d;
    rTm.tm_hour = h;
    rTm.tm_min = mi;
    rTm.tm_sec = se;
    rTm.tm_isdst = 0;
    return rTm;
}

inline mslong_t fn_toMslong(const std::string& sLong, const std::string& sUnit) {
    if (sUnit.find("d") != std::string::npos)
    {
        return CxString::toInt64(sLong) * GM_MSEPOCH_ONE_DAY;
    }
    else if (sUnit.find("h") != std::string::npos)
    {
        return CxString::toInt64(sLong) * GM_MSEPOCH_ONE_HOUR;
    }
    else if (sUnit.find("mi") != std::string::npos)
    {
        return CxString::toInt64(sLong) * GM_MSEPOCH_ONE_MINUTE;
    }
    else if (sUnit.find("se") != std::string::npos)
    {
        return CxString::toInt64(sLong) * 1000;
    }
    else if (sUnit.find("ms") != std::string::npos)
    {
        return CxString::toInt64(sLong);
    }
    return 0;
}

mslong_t CxTime::toMslong(const std::string &sTimeLong)
{
    mslong_t r = 0;
    if (sTimeLong.empty()) return r;
    char * pchBegin = const_cast<char *>( sTimeLong.data() );
    char * pchEnd = pchBegin + sTimeLong.size();
    char * pch = pchBegin;
    string sLong;
    char * pLong = pchBegin;
    string sUnit;
    char * pUnit = NULL;
    bool bNum = true;
    while ( pch < pchEnd )
    {
        if ( * pch > 57 || * pch < 48 )
        {
            if (! bNum)
            {
                pUnit = pch;
                bNum = true;
            }
        }
        else
        {
            if (bNum)
            {
                if (pUnit)
                {
                    sLong = string(pLong, pUnit-pLong);
                    sUnit = string(pUnit, pch-pUnit);
                    pUnit = NULL;
                    r += fn_toMslong(sLong, sUnit);
                }
                pLong = pch;
                bNum = false;
            }
        }
        pch ++;
    }
    if (pUnit)
    {
        sLong = string(pLong, pUnit-pLong);
        sUnit = string(pUnit, pch-pUnit);
        r += fn_toMslong(sLong, sUnit);
    }

    return r;
}

void CxTime::decodeUtcTm(const msepoch_t &ms, int &y, int &m, int &d, int &h, int &mi, int &se)
{
    msepoch_t msecs = ms;
    int ddays = msecs / 86400000;
    msecs %= 86400000;
    if (msecs < 0)
    {
        // negative
        --ddays;
        msecs += 86400000;
    }

    uint32 jd = julianDayFromDate1(1970, 1, 1);
    if (ddays >= 0)
        ddays = (jd + ddays >= jd) ? jd + ddays : 0;
    else
        ddays = (jd + ddays < jd) ? jd + ddays : 0;

    if (ddays >= 2299161)
    {
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
    }
    else
    {
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
    if (msecs < 0)
    {
        // % not well-defined for -ve, but / is.
        int negdays = (86400000 - msecs) / 86400000;
        mds = (msecs + negdays * 86400000) % 86400000;
    }

    h = mds / 3600000;
    mi = (mds % 3600000) / 60000;
    se = (mds / 1000) % 60;
}

void CxTime::decodeUtcTm(const msepoch_t &dt, int &y, int &m, int &d, int &h, int &mi, int &se, int &ms)
{
    msepoch_t msecs = dt;
    int ddays = msecs / 86400000;
    msecs %= 86400000;
    if (msecs < 0)
    {
        // negative
        --ddays;
        msecs += 86400000;
    }

    uint32 jd = julianDayFromDate1(1970, 1, 1);
    if (ddays >= 0)
        ddays = (jd + ddays >= jd) ? jd + ddays : 0;
    else
        ddays = (jd + ddays < jd) ? jd + ddays : 0;

    if (ddays >= 2299161)
    {
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
    }
    else
    {
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
    if (msecs < 0)
    {
        // % not well-defined for -ve, but / is.
        int negdays = (86400000 - msecs) / 86400000;
        mds = (msecs + negdays * 86400000) % 86400000;
    }

    h = mds / 3600000;
    mi = (mds % 3600000) / 60000;
    se = (mds / 1000) % 60;
    ms = (mds % 1000);
}

void CxTime::decodeLocalTm(const msepoch_t &dt, int &y, int &m, int &d, int &h, int &mi, int &se, int &ms)
{
    msepoch_t msecs = dt + GM_TIME_UTC_DIFF_MS;
    int ddays = msecs / 86400000;
    msecs %= 86400000;
    if (msecs < 0)
    {
        // negative
        --ddays;
        msecs += 86400000;
    }

    uint32 jd = julianDayFromDate1(1970, 1, 1);
    if (ddays >= 0)
        ddays = (jd + ddays >= jd) ? jd + ddays : 0;
    else
        ddays = (jd + ddays < jd) ? jd + ddays : 0;

    if (ddays >= 2299161)
    {
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
    }
    else
    {
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
    if (msecs < 0)
    {
        // % not well-defined for -ve, but / is.
        int negdays = (86400000 - msecs) / 86400000;
        mds = (msecs + negdays * 86400000) % 86400000;
    }

    h = mds / 3600000;
    mi = (mds % 3600000) / 60000;
    se = (mds / 1000) % 60;
    ms = (mds % 1000);
}

bool CxTime::decodeDataTime(const string &sDateTime, int &y, int &m, int &d, int &h, int &mi, int &se, int &ms)
{
    //48 49 50 51 .. 57
    // 0  1  2  3     9
    string sDateTime2 = CxString::trim(sDateTime);
    const unsigned char *dts = (const unsigned char *) sDateTime2.data();;
    if (sDateTime2.size() > 9)
    {
        y = CCNumberCharZero127[dts[0]] * 1000 + CCNumberCharZero127[dts[1]] * 100 + CCNumberCharZero127[dts[2]] * 10
            + CCNumberCharZero127[dts[3]];
        m = CCNumberCharZero127[dts[5]] * 10 + CCNumberCharZero127[dts[6]];
        d = CCNumberCharZero127[dts[8]] * 10 + CCNumberCharZero127[dts[9]];
        if (m > 12) return false;
        if (d > 31) return false;
    }
    h = 0;
    mi = 0;
    se = 0;
    ms = 0;
    if (sDateTime2.size() > 18)
    {
        h = CCNumberCharZero127[dts[11]] * 10 + CCNumberCharZero127[dts[12]];
        mi = CCNumberCharZero127[dts[14]] * 10 + CCNumberCharZero127[dts[15]];
        se = CCNumberCharZero127[dts[17]] * 10 + CCNumberCharZero127[dts[18]];
        if (h > 23) return false;
        if (mi > 59) return false;
        if (se > 59) return false;
    }
    else if (sDateTime2.size() > 22)
    {
        ms = CCNumberCharZero127[dts[20]] * 100 + CCNumberCharZero127[dts[21]] * 10 + CCNumberCharZero127[dts[22]];
        if (ms > 999) return false;
    }
    else
    {
        return false;
    }
    return true;
}

void CxTime::decodeDataTime2(const string &sDateTime, int &y, int &m, int &d, int &h, int &mi, int &se, int &ms)
{
    char buffer[256];
    if (sDateTime.size() > sizeof(buffer))
        return;
    memcpy(buffer, sDateTime.c_str(), sDateTime.size());
    buffer[sDateTime.size()] = 0;
    char *pch = buffer;
    char *pchEnd = buffer + sDateTime.size();
    char *part[8];
    int iIndex = -1;
    bool bNotNumber = true;
    while (pch < pchEnd)
    {
        if (*pch > '9' || *pch < '0')
        {
            *pch = 0;
            bNotNumber = true;
        }
        else
        {
            if (bNotNumber)
            {
                bNotNumber = false;
                ++iIndex;
                if (iIndex < 7)
                    part[iIndex] = pch;
                else
                    break;
            }
        }
        ++pch;
    }

    if (iIndex > -1)
        y = atoi(part[0]);
    if (iIndex > 0)
        m = atoi(part[1]);
    if (iIndex > 1)
        d = atoi(part[2]);
    if (iIndex > 2)
        h = atoi(part[3]);
    if (iIndex > 3)
        mi = atoi(part[4]);
    if (iIndex > 4)
        se = atoi(part[5]);
    if (iIndex > 5)
        ms = atoi(part[6]);
}


void CxTime::decodeTimeLong(const std::string &sTimeLong, int &y, int &m, int &d, int &h, int &mi, int &se, int &ms)
{
    //48 49 50 51 .. 57
    // 0  1  2  3     9
    string sDateTime2 = CxString::trim(sTimeLong);
    const unsigned char *dts = (const unsigned char *) sDateTime2.data();;
    if (sDateTime2.size() > 9)
    {
        y = CCNumberCharZero127[dts[0]] * 1000 + CCNumberCharZero127[dts[1]] * 100 + CCNumberCharZero127[dts[2]] * 10
            + CCNumberCharZero127[dts[3]];
        m = CCNumberCharZero127[dts[5]] * 10 + CCNumberCharZero127[dts[6]];
        d = CCNumberCharZero127[dts[8]] * 10 + CCNumberCharZero127[dts[9]];
    }
    h = 0;
    mi = 0;
    se = 0;
    ms = 0;
    if (sDateTime2.size() > 18)
    {
        h = CCNumberCharZero127[dts[11]] * 10 + CCNumberCharZero127[dts[12]];
        mi = CCNumberCharZero127[dts[14]] * 10 + CCNumberCharZero127[dts[15]];
        se = CCNumberCharZero127[dts[17]] * 10 + CCNumberCharZero127[dts[18]];
    }
    else if (sDateTime2.size() > 22)
    {
        ms = CCNumberCharZero127[dts[20]] * 100 + CCNumberCharZero127[dts[21]] * 10 + CCNumberCharZero127[dts[22]];
    }
}

msepoch_t CxTime::encodeDateTime(int y, int m, int d, int h, int mi, int se, int ms)
{
    tm dt;
    dt.tm_year = y - 1900;
    dt.tm_mon = m - 1;
    dt.tm_mday = d;
    dt.tm_hour = h;
    dt.tm_min = mi;
    dt.tm_sec = se;
    dt.tm_isdst = 0;
    dt.tm_wday = 0;
    dt.tm_yday = 0;

    msepoch_t dtResult = mktime(&dt);
    return dtResult * 1000 + ms;
}

msepoch_t CxTime::currentDayEnd()
{
    msepoch_t dt = currentSystemTime();
    int y, m, d, h, mi, se, ms;
    decodeLocalTm(dt, y, m, d, h, mi, se, ms);
    h = 23;
    mi = 59;
    se = 59;
    ms = 999;
    return encodeDateTime(y, m, d, h, mi, se, ms);
}
