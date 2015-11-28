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

#include <stddef.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <signal.h>
#include <ctype.h>
#include <wchar.h>
#include <wctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>

using namespace std;

void gprint(std::string msg)
{
    std::cout << msg << std::endl;
}

std::string ginput()
{
    std::string msg;
    std::cin >> msg;
    return msg;
}

inline void TestVector21()
{
    string str;
    do
    {
        str = ginput();

        if (str[0] == 'C')
        {
            if (str[1] == 'B')
            {
                if (str[2] == '0')
                {

                }
            }
            else if (str[1] == 'E')
            {
            }
            else if (str[1] == 'T')
            {
            }
            else if (str[1] == 'R')
            {
            }
            else if (str[1] == 'C')
            {
                break;
            }
        }
        else
        {
        }
    } while (str != "CC");
}

#include <windows.h>

int main1(int argc, char*agrv[])
{
    gprint("hello begin:");

    while (1)
    {
        std::cout << "tick : " << clock() << std::endl;
        ::Sleep(1000);
    }

    gprint("hello end!");
}

#include <limits>

typedef long long msepoch_t;

const std::string CSInteger   = "Integer";
const std::string CSBoolean   = "Boolean";
const std::string CSDouble    = "Double";
const std::string CSString    = "String";
const std::string CSDateTime  = "DateTime";
const std::string CSEnum      = "Enum";

typedef unsigned char       uchar       ;   /* 8 bit unsigned */

class CxValue
{
public:
	enum ValueTypeEnum{ ValueType_Integer, ValueType_Enum, ValueType_Boolean,
		ValueType_Double, ValueType_DateTime, ValueType_String };

	template<typename T>
	static ValueTypeEnum type();

	template<typename T>
	static std::string typeSimpleName();

	template<typename T>
	static T defaultValue();

	template<typename T>
	static T minValue();



};

template<typename T> inline CxValue::ValueTypeEnum CxValue::type(){ return ValueType_Enum; }
template<> inline CxValue::ValueTypeEnum CxValue::type<int>(){ return ValueType_Integer; }
template<> inline CxValue::ValueTypeEnum CxValue::type<bool>(){ return ValueType_Boolean; }
template<> inline CxValue::ValueTypeEnum CxValue::type<double>(){ return ValueType_Double; }
template<> inline CxValue::ValueTypeEnum CxValue::type<std::string>(){ return ValueType_String; }
template<> inline CxValue::ValueTypeEnum CxValue::type<msepoch_t>(){ return ValueType_DateTime; }


template<typename T> inline std::string CxValue::typeSimpleName()  { return ""; }
template<> inline std::string CxValue::typeSimpleName<int>()       { return CSInteger; }
template<> inline std::string CxValue::typeSimpleName<bool>()      { return CSBoolean; }
template<> inline std::string CxValue::typeSimpleName<double>()    { return CSDouble; }
template<> inline std::string CxValue::typeSimpleName<std::string>()   { return CSString; }
template<> inline std::string CxValue::typeSimpleName<msepoch_t>() { return CSDateTime; }


template<typename T> inline T CxValue::defaultValue()         { T t; return t; }
template<> inline int CxValue::defaultValue<int>()            { return 0; }
template<> inline bool CxValue::defaultValue<bool>()          { return false; }
template<> inline uchar CxValue::defaultValue<uchar>()        { return 0x00; }
template<> inline double CxValue::defaultValue<double>()      { return 0; }
template<> inline msepoch_t CxValue::defaultValue<msepoch_t>(){ return 0; }
template<> inline std::string CxValue::defaultValue<std::string>()    { return ""; }


template<typename T> inline T CxValue::minValue()         { T t; return t; }
/*
template<> 
inline bool CxValue::minValue<bool>()          { return false; }

template<> 
inline char CxValue::minValue<char>()          
{ 
	return std::numeric_limits<char>::min(); 
}

template<> 
inline uchar CxValue::minValue<uchar>()        { return std::numeric_limits<unsigned char>::min(); }

template<> 
inline short CxValue::minValue<short>()        { return std::numeric_limits<short>::min(); }

template<> 
inline ushort CxValue::minValue<ushort>()      { return std::numeric_limits<unsigned short>::min(); }
*/

int main(int argc, char*agrv[])
{
	gprint("hello begin:");

	cout << sizeof(time_t) << endl;

	cout << (std::numeric_limits <int>::min)() << endl;

	system("pause");

	gprint("hello end!");
}


