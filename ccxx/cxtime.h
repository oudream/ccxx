#ifndef CXTIME_H
#define CXTIME_H

#include "cxglobal.h"

#include "cxstring.h"

#define GM_TIME_UTC_MST (-7)
#define GM_TIME_UTC_    (0)
#define GM_TIME_UTC_CCT (+8)

#define GM_TIME_UTC_LOCAL GM_TIME_UTC_CCT

#define GM_MSEPOCH_ONE_DAY (24 * 60 * 60 * 1000)
#define GM_MSEPOCH_ONE_HOUR (60 * 60 * 1000)
#define GM_MSEPOCH_ONE_MINUTE (60 * 1000)

//*attention
//1)since 1970-01-01 : same to unix time
//2)dataTime string format : 2012/12/12 12:12:12:120
//3)toString and fromString is local UTC
//local VS current : local is system ( utc bejing ), current is the world
class GM_CCXX_CORE_API CxTime
{
public:
    /**
     * @brief os.time count ms(since 1970-01-01 's count ms)
     * @return
     */
    static msepoch_t currentSystemTime();

    /**
     * @brief os.time : 2012-12-12 12:12:000
     * '2', '0', '1', '2', sSplit1, '1', '2', sSplit1, '1', '2', sSplit2, '1', '2', sSplit3, '1', '2', sSplit3, '1', '2', sSplit3, '1', '2', '2', '\0'
     * @param sSplit1
     * @param sSplit2
     * @param sSplit3
     * @return
     */
    inline static std::string currentSystemTimeString(char sSplit1 = '/', char sSplit2 = ' ', char sSplit3 = ':') { return toString(currentSystemTime(), sSplit1, sSplit2, sSplit3); }

    /**
     * @brief os.time count ms:(since 1970-01-01 's count ms)
     * @return
     */
    static msepoch_t currentMsepoch();

    inline static std::string currentMsepochString(char sSplit1 = '/', char sSplit2 = ' ', char sSplit3 = ':') { return toString(currentMsepoch(), sSplit1, sSplit2, sSplit3); }

    /**
     * @brief os.time count seconds(since 1970-01-01 's count ms)
     * @return
     */
    static sepoch_t currentSepoch();

    static inline std::string currentSepochString(char sSplit1 = '/', char sSplit2 = ' ', char sSplit3 = ':') { return toStringSepoch(currentSepoch(), sSplit1, sSplit2, sSplit3); }

    /**
     * @brief os.date : 2012-12-12
     * @return
     */
    static inline std::string currentDayString(char sSplit1 = '/') { return toStringDay(currentSepoch(), sSplit1); }

    /**
     * @brief os.UTC
     * @return
     */
    static tm currentUtc();

    /**
     * @brief os.TM
     * @return
     */
    static tm localTm();

    /**
     * @brief toString
     * @param lMsepoch
     * @param sSplit1
     * @param sSplit2
     * @param sSplit3
     * @return
     */
    static std::string toString(const msepoch_t& lMsepoch, char sSplit1 = '/', char sSplit2 = ' ', char sSplit3 = ':');

    static std::string toString(const tm& dtTm, char sSplit1 = '/', char sSplit2 = ' ', char sSplit3 = ':');

    static std::string toStringSepoch(const sepoch_t& iSepoch, char sSplit1 = '/', char sSplit2 = ' ', char sSplit3 = ':');

    static std::string toStringDay(const sepoch_t& iSepoch, char sSplit1 = '/');

    static std::string toString(int y, int m, int d, char sSplit1 = '/');

    static std::string toString(int y, int m, int d, int h, int mi, int se, char sSplit1 = '/', char sSplit2 = ' ', char sSplit3 = ':');

    static std::string toString(int y, int m, int d, int h, int mi, int se, int ms, char sSplit1 = '/', char sSplit2 = ' ', char sSplit3 = ':');

    /**
     * @brief fromString
     * @param sDateTime
     * @param lMsepoch
     * @return
     */
    static bool fromString(const std::string& sDateTime, msepoch_t& lMsepoch);

    static msepoch_t fromString(const std::string& sDateTime, bool * bOk = NULL);

    static bool fromStringSepoch(const std::string& sDateTime, sepoch_t& iSepoch);

    static bool fromString(const std::string& sDateTime, tm& dtTm);

    static tm toTm(const std::string& sDateTime);

    /**
     *
     * @param sDateTime
     * @return
     * @case1 : 30d, 30h, 30mi, 30se, 30ms
     */
    static mslong_t toMslong(const std::string& sTimeLong);

    inline static secondlong_t toSecondlong(const std::string& sTimeLong) { return toMslong(sTimeLong) / 1000; }

    /**
     * @brief decodeUtcTm
     * @param dt
     * @param y
     * @param m
     * @param d
     * @param h
     * @param mi
     * @param se
     */
    static void decodeUtcTm(const msepoch_t& dt, int& y, int& m, int& d, int& h, int& mi, int& se);

    static void decodeUtcTm(const msepoch_t& dt, int& y, int& m, int& d, int& h, int& mi, int& se, int& ms);

    /**
     * @brief decodeLocalTm GM_TIME_UTC_LOCAL (+8)
     * @param dt
     * @param y
     * @param m
     * @param d
     * @param h
     * @param mi
     * @param se
     * @param ms
     */
    static void decodeLocalTm(const msepoch_t& dt, int& y, int& m, int& d, int& h, int& mi, int& se, int& ms);

    /**
     * @brief decodeDataTime
     * @param sDateTime : "2012 12 12" , "2012 12 12 12 12 12" , "2012 12 12 12 12 12 122"
     * @param sDateTime : "2012-12-12 12:12:12" , "2012-12-12 12:12:12:122"
     * @param y
     * @param m
     * @param d
     * @param h
     * @param mi
     * @param se
     * @param ms
     * @return
     */
    static bool decodeDataTime(const std::string& sDateTime, int& y, int& m, int& d, int& h, int& mi, int& se, int& ms);

    static void decodeDataTime2(const std::string& sDateTime, int& y, int& m, int& d, int& h, int& mi, int& se, int& ms);

    static void decodeTimeLong(const std::string& sTimeLong, int& y, int& m, int& d, int& h, int& mi, int& se, int& ms);

    /**
     * @brief encodeDateTime
     * @param y
     * @param m
     * @param d
     * @param h
     * @param mi
     * @param se
     * @param ms
     * @return
     */
    static msepoch_t encodeDateTime(int y, int m, int d, int h, int mi, int se, int ms);

    inline static long long milliSecondDifferToNow(const msepoch_t & dt) { return currentSystemTime() - dt; }

    static msepoch_t currentDayEnd();

};


/***
 *
time                        clib <1s
clcok                       clib <10ms
gettimeofday                linux env clib <0.1ms
clock_gettime               linux env clib <0.1ms
timeGetTime                 Windows API	middle<1ms
QueryPerformanceCounter     Windows API	high<0.1ms
QueryPerformanceFrequency   Windows API	high<0.1ms
GetTickCount                Windows API	middle<1ms
GetLocalTime                Windows API	middle<1ms
GetTickCount
GetSystemTimeAsFileTime
RDTSC                       Assemble instructions high<0.1ms
 */
#endif // CXTIME_H
