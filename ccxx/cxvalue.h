#ifndef CXVALUE_H
#define CXVALUE_H


#include "cxglobal.h"
#include "cxstring.h"

//const std::string CSInteger   = "Integer";
//const std::string CSBoolean   = "Boolean";
//const std::string CSDouble    = "Double";
//const std::string CSString    = "String";
//const std::string CSDateTime  = "DateTime";
//const std::string CSEnum      = "Enum";


class CxValueType
{
public:
    enum ValueTypeEnum{ ValueType_Integer, ValueType_Enum, ValueType_Boolean,
                        ValueType_Double, ValueType_DateTime, ValueType_String };

    template<typename T>
    static ValueTypeEnum id();

    template<typename T>
    static std::string simpleName();

    template<typename T>
    static T defaultValue();

    //std::numeric_limits<T>::min()
    template<typename T>
    static T minValue();

    //std::numeric_limits<T>::max()
    template<typename T>
    static T maxValue();

    template<typename T>
    static T originalValue();

    template<typename T1, typename T2>
    static T2 valueTo(const T1 & t1, const T2 & defaultT2);

};

template<typename T> inline CxValueType::ValueTypeEnum CxValueType::id(){ return ValueType_Enum; }
template<> inline CxValueType::ValueTypeEnum CxValueType::id<int>(){ return ValueType_Integer; }
template<> inline CxValueType::ValueTypeEnum CxValueType::id<bool>(){ return ValueType_Boolean; }
template<> inline CxValueType::ValueTypeEnum CxValueType::id<double>(){ return ValueType_Double; }
template<> inline CxValueType::ValueTypeEnum CxValueType::id<std::string>(){ return ValueType_String; }
template<> inline CxValueType::ValueTypeEnum CxValueType::id<msepoch_t>(){ return ValueType_DateTime; }


template<typename T> inline std::string CxValueType::simpleName()   { return std::string(); }
template<> inline std::string CxValueType::simpleName<int>()        { return "Integer"; }
template<> inline std::string CxValueType::simpleName<bool>()       { return "Boolean"; }
template<> inline std::string CxValueType::simpleName<double>()     { return "Double"; }
template<> inline std::string CxValueType::simpleName<std::string>(){ return "String"; }
template<> inline std::string CxValueType::simpleName<msepoch_t>()  { return "DateTime"; }
//                                                                             "Enum";

template<typename T> inline T CxValueType::defaultValue()         { T t; return t; }
template<> inline int CxValueType::defaultValue<int>()            { return 0; }
template<> inline bool CxValueType::defaultValue<bool>()          { return false; }
template<> inline uchar CxValueType::defaultValue<uchar>()        { return 0x00; }
template<> inline double CxValueType::defaultValue<double>()      { return 0; }
template<> inline msepoch_t CxValueType::defaultValue<msepoch_t>(){ return 0; }
template<> inline std::string CxValueType::defaultValue<std::string>()    { return std::string(); }
template<> inline void * CxValueType::defaultValue<void *>()      { return NULL; }


template<typename T> inline T CxValueType::minValue()         { T t; return t; }
template<> inline bool CxValueType::minValue<bool>()          { return false; }
template<> inline char CxValueType::minValue<char>()          { return (std::numeric_limits<char>::min)(); }
template<> inline uchar CxValueType::minValue<uchar>()        { return (std::numeric_limits<unsigned char>::min)(); }
template<> inline short CxValueType::minValue<short>()        { return (std::numeric_limits<short>::min)(); }
template<> inline ushort CxValueType::minValue<ushort>()      { return (std::numeric_limits<unsigned short>::min)(); }
template<> inline int CxValueType::minValue<int>()            { return (std::numeric_limits<int>::min)(); }
template<> inline uint CxValueType::minValue<uint>()          { return (std::numeric_limits<unsigned int>::min)(); }
template<> inline float CxValueType::minValue<float>()        { return (std::numeric_limits<float>::min)(); }
template<> inline double CxValueType::minValue<double>()      { return (std::numeric_limits<double>::min)(); }
template<> inline msepoch_t CxValueType::minValue<msepoch_t>(){ return GM_MSEPOCH_MIN; }
template<> inline std::string CxValueType::minValue<std::string>()    { return std::string(); }


template<typename T> inline T CxValueType::maxValue()         { T t; return t; }
template<> inline bool CxValueType::maxValue<bool>()          { return false; }
template<> inline char CxValueType::maxValue<char>()          { return (std::numeric_limits<char>::max)(); }
template<> inline uchar CxValueType::maxValue<uchar>()        { return (std::numeric_limits<unsigned char>::max)(); }
template<> inline short CxValueType::maxValue<short>()        { return (std::numeric_limits<short>::max)(); }
template<> inline ushort CxValueType::maxValue<ushort>()      { return (std::numeric_limits<unsigned short>::max)(); }
template<> inline int CxValueType::maxValue<int>()            { return (std::numeric_limits<int>::max)(); }
template<> inline uint CxValueType::maxValue<uint>()          { return (std::numeric_limits<unsigned int>::max)(); }
template<> inline float CxValueType::maxValue<float>()        { return (std::numeric_limits<float>::max)(); }
template<> inline double CxValueType::maxValue<double>()      { return (std::numeric_limits<double>::max)(); }
template<> inline msepoch_t CxValueType::maxValue<msepoch_t>(){ return GM_MSEPOCH_MAX; }
template<> inline std::string CxValueType::maxValue<std::string>()    { return std::string(); }


template<typename T> inline T CxValueType::originalValue()         { T t; return t; }
template<> inline int CxValueType::originalValue<int>()            { return -1; }
template<> inline bool CxValueType::originalValue<bool>()          { return false; }
template<> inline uchar CxValueType::originalValue<uchar>()        { return 0x00; }
template<> inline double CxValueType::originalValue<double>()      { return -1; }
template<> inline msepoch_t CxValueType::originalValue<msepoch_t>(){ return 0; }
template<> inline std::string CxValueType::originalValue<std::string>()    { return std::string(); }


template<typename T1, typename T2> inline T2 CxValueType::valueTo(const T1 & t1, const T2 & defaultT2) { return CxString::fromString(t1, defaultT2); }


#endif // CXVALUE_H
