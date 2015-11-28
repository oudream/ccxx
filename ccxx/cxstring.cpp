#include "cxstring.h"


using namespace std;

const char f_cHexChar16[16] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

const bool f_cPathChar[128] = {
    false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
    false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
    false, true , false, true , true , true , true , true , true , true , false, true , true , true , true , false,
    true , true , true , true , true , true , true , true , true , true , false, true , false, true , false, false,
    true , true , true , true , true , true , true , true , true , true , true , true , true , true , true , true ,
    true , true , true , true , true , true , true , true , true , true , true , true , false, true , true , true ,
    true , true , true , true , true , true , true , true , true , true , true , true , true , true , true , true ,
    true , true , true , true , true , true , true , true , true , true , true , true , false, true , true , false
};



/**
 * *** std::string begin ********************************************************
 */

string CxString::toString(const bool& b)
{
    char str[1+1];
    sprintf(str, "%d", b);
    return string(str);
}

string CxString::toString(const int &i)
{
    char str[10+1];
    sprintf(str, "%d", i);
    return string(str);
}

string CxString::toString(const uint32 &n)
{
    char str[10+1];
    sprintf(str, "%d", n);
    return string(str);
}

string CxString::toString(const long &n)
{
    stringstream ss;
    ss << n;
    return ss.str();
}

string CxString::toString(const ulong &n)
{
    stringstream ss;
    ss << n;
    return ss.str();
}

string CxString::toString(const int64 &n)
{
    stringstream ss;
    ss << n;
    return ss.str();
}

string CxString::toString(const uint64 &n)
{
    stringstream ss;
    ss << n;
    return ss.str();
}

string CxString::toString(const float &f)
{
    stringstream ss;
    ss << f;
    return ss.str();
}

string CxString::toString(const double &d)
{
    stringstream ss;
    ss << d;
    return ss.str();
//    char str[256];
//    sprintf(str, "%lf", d);
//    return string(str);
}

string CxString::toString(const string &s)
{
    return s;
}

bool CxString::toBoolean(const string &sSource)
{
    return atoi(sSource.data()) != 0;
}

int CxString::toInt32(const string &sSource)
{
    if (sSource.size()>2 && (sSource[1] == 'x' || sSource[1] == 'X'))
    {
        int n;
        sscanf(sSource.data(), "%x", &n);
        return n;
    }
    else
    {
        return atoi(sSource.data());
    }
}

uint CxString::toUint32(const string &sSource)
{
    if (sSource.size()>2 && (sSource[1] == 'x' || sSource[1] == 'X'))
    {
        uint n;
        sscanf(sSource.data(), "%x", &n);
        return n;
    }
    else
    {
        uint n;
        n = atoi(sSource.data());
        return n;
    }
}

int64 CxString::toInt64(const string &sSource)
{
    if (sSource.size()>2 && (sSource[1] == 'x' || sSource[1] == 'X'))
    {
        long unsigned int n;
        sscanf(sSource.data(), "%lx", &n);
        return n;
    }
    else
    {
        int64 n;
        stringstream ss(sSource);
        ss >> n;
        return n;
    }
}

uint64 CxString::toUint64(const string &sSource)
{
    if (sSource.size()>2 && (sSource[1] == 'x' || sSource[1] == 'X'))
    {
        long unsigned int n;
        sscanf(sSource.data(), "%lx", &n);
        return n;
    }
    else
    {
        uint64 n;
        stringstream ss(sSource);
        ss >> n;
        return n;
    }
}

double CxString::toDouble(const string &sSource)
{
    return strtod(sSource.data(), NULL);
}

float CxString::toFloat(const string &sSource)
{
    return strtod(sSource.data(), NULL);
}

bool CxString::toBoolean(const string &sSource, bool *bOk)
{
    if (sSource.size() == 1)
    {
        bool r = sSource.at(0) != '0';
        if (bOk) * bOk = true;
        return r;
    }
    else
    {
        bool r = (sSource.size() == 5) && (toLower(sSource) == "false");
        if (bOk) * bOk = true;
        return ! r;
    }
}

int CxString::toInt32(const string &sSource, bool *bOk)
{
    if (sSource.size()>2 && (sSource[1] == 'x' || sSource[1] == 'X'))
    {
        int n;
        sscanf(sSource.data(), "%x", &n);
        if (bOk) * bOk = true;
        return n;
    }
    else
    {
        if (sSource.size() == 1 && sSource.at(0) == '0')
        {
            if (bOk) * bOk = true;
            return 0;
        }
        else
        {
            int i = atoi(sSource.data());
            if (bOk) * bOk = i != 0;
            return i;
        }
    }
}

uint CxString::toUint32(const string &sSource, bool *bOk)
{
    if (sSource.size()>2 && (sSource[1] == 'x' || sSource[1] == 'X'))
    {
        uint n;
        sscanf(sSource.data(), "%x", &n);
        if (bOk) * bOk = true;
        return n;
    }
    else
    {
        if (sSource.size() == 1 && sSource.at(0) == '0')
        {
            if (bOk) * bOk = true;
            return 0;
        }
        else
        {
            uint i = atoi(sSource.data());
            if (bOk) * bOk = i != 0;
            return i;
        }
    }
}

int64 CxString::toInt64(const string &sSource, bool *bOk)
{
    if (sSource.size()>2 && (sSource[1] == 'x' || sSource[1] == 'X'))
    {
        long unsigned int n;
        sscanf(sSource.data(), "%lx", &n);
        if (bOk) * bOk = true;
        return n;
    }
    else
    {
        if (sSource.size() == 1 && sSource.at(0) == '0')
        {
            if (bOk) * bOk = true;
            return 0;
        }
        else
        {
            int64 n;
            stringstream ss(sSource);
            ss >> n;
            if (bOk) * bOk = n != 0;
            return n;
        }
    }
}

uint64 CxString::toUint64(const string &sSource, bool *bOk)
{
    if (sSource.size()>2 && (sSource[1] == 'x' || sSource[1] == 'X'))
    {
        long unsigned int n;
        sscanf(sSource.data(), "%lx", &n);
        return n;
    }
    else
    {
        if (sSource.size() == 1 && sSource.at(0) == '0')
        {
            if (bOk) * bOk = true;
            return 0;
        }
        else
        {
            uint64 n;
            stringstream ss(sSource);
            ss >> n;
            if (bOk) * bOk = n != 0;
            return n;
        }
    }
}

double CxString::toDouble(const string &sSource, bool *bOk)
{
    double d= strtod(sSource.data(), NULL);
    if (d != 0)
    {
        if (bOk) * bOk = true;
        return d;
    }
    else
    {
        if (bOk) * bOk = isvalidDouble(sSource);
        return 0;
    }
}

float CxString::toFloat(const string &sSource, bool *bOk)
{
    float d= strtod(sSource.data(), NULL);
    if (d != 0)
    {
        if (bOk) * bOk = true;
        return d;
    }
    else
    {
        if (bOk) * bOk = isvalidFloat(sSource);
        return 0;
    }
}

bool CxString::fromString(const string &sSource, const bool &b, bool * bOk)
{
    if (sSource.size() == 1)
    {
        bool r = sSource.at(0) != '0';
        if (bOk) * bOk = true;
        return r;
    }
    else
    {
        bool r = (sSource.size() == 5) && (toLower(sSource) == "false");
        if (bOk) * bOk = true;
        return r;
    }
}

int CxString::fromString(const string &sSource, const int &i, bool * bOk)
{
    if (sSource.size()>2 && (sSource[1] == 'x' || sSource[1] == 'X'))
    {
        int r;
        sscanf(sSource.data(), "%x", &r);
        if (bOk) * bOk = true;
        return r;
    }
    else
    {
        if (sSource.size() == 1 && sSource.at(0) == '0')
        {
            if (bOk) * bOk = true;
            return 0;
        }
        else
        {
            int r = atoi(sSource.data());
            if (r != 0)
            {
                if (bOk) * bOk = true;
                return r;
            }
            else
            {
                if (bOk) * bOk = false;
                return i;
            }
        }
    }
}

uint32 CxString::fromString(const string &sSource, const uint32 &n, bool * bOk)
{
    if (sSource.size()>2 && (sSource[1] == 'x' || sSource[1] == 'X'))
    {
        uint32 r;
        sscanf(sSource.data(), "%x", &r);
        if (bOk) * bOk = true;
        return r;
    }
    else
    {
        if (sSource.size() == 1 && sSource.at(0) == '0')
        {
            if (bOk) * bOk = true;
            return 0;
        }
        else
        {
            uint32 r = atoi(sSource.data());
            if (r != 0)
            {
                if (bOk) * bOk = true;
                return r;
            }
            else
            {
                if (bOk) * bOk = false;
                return n;
            }
        }
    }
}

int64 CxString::fromString(const string &sSource, const int64 &n, bool * bOk)
{
    if (sSource.size()>2 && (sSource[1] == 'x' || sSource[1] == 'X'))
    {
        long unsigned int r;
        sscanf(sSource.data(), "%lx", &r);
        if (bOk) * bOk = true;
        return r;
    }
    else
    {
        if (sSource.size() == 1 && sSource.at(0) == '0')
        {
            if (bOk) * bOk = true;
            return 0;
        }
        else
        {
            int64 r;
            stringstream ss(sSource);
            ss >> r;
            if (r != 0)
            {
                if (bOk) * bOk = true;
                return r;
            }
            else
            {
                if (bOk) * bOk = false;
                return n;
            }
        }
    }
}

uint64 CxString::fromString(const string &sSource, const uint64 &n, bool * bOk)
{
    if (sSource.size()>2 && (sSource[1] == 'x' || sSource[1] == 'X'))
    {
        long unsigned int r;
        sscanf(sSource.data(), "%lx", &r);
        if (bOk) * bOk = true;
        return r;
    }
    else
    {
        if (sSource.size() == 1 && sSource.at(0) == '0')
        {
            if (bOk) * bOk = true;
            return 0;
        }
        else
        {
            uint64 r;
            stringstream ss(sSource);
            ss >> r;
            if (r != 0)
            {
                if (bOk) * bOk = true;
                return r;
            }
            else
            {
                if (bOk) * bOk = false;
                return n;
            }
        }
    }
}

double CxString::fromString(const string &sSource, const double &d, bool * bOk)
{
    double r= strtod(sSource.data(), NULL);
    if (r != 0)
    {
        if (bOk) * bOk = true;
        return r;
    }
    else
    {
        if (isvalidDouble(sSource))
        {
            if (bOk) * bOk = true;
            return 0;
        }
        else
        {
            if (bOk) * bOk = false;
            return d;
        }
    }
}

float CxString::fromString(const string &sSource, const float &f, bool * bOk)
{
    float r = strtod(sSource.data(), NULL);
    if (r != 0)
    {
        if (bOk) * bOk = true;
        return r;
    }
    else
    {
        if (isvalidFloat(sSource))
        {
            if (bOk) * bOk = true;
            return 0;
        }
        else
        {
            if (bOk) * bOk = false;
            return f;
        }
    }
}

string CxString::fromString(const string &sSource, const string &s, bool * bOk)
{
    if (bOk) * bOk = true;
    return sSource;
}

bool CxString::setFromString(const string &sSource, bool &b)
{
    if (sSource.size() == 1)
    {
        b = sSource.at(0) != '0';
        return true;
    }
    else
    {
        b = (sSource.size() == 5) && (toLower(sSource) == "false");
        return true;
    }
}

bool CxString::setFromString(const string &sSource, int &i)
{
    if (sSource.size()>2 && (sSource[1] == 'x' || sSource[1] == 'X'))
    {
        sscanf(sSource.data(), "%x", &i);
        return true;
    }
    else
    {
        if (sSource.size() == 1 && sSource.at(0) == '0')
        {
            i = 0;
            return true;
        }
        else
        {
            int r = atoi(sSource.data());
            if (r != 0)
            {
                i = r;
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}

bool CxString::setFromString(const string &sSource, uint32 &n)
{
    if (sSource.size()>2 && (sSource[1] == 'x' || sSource[1] == 'X'))
    {
        sscanf(sSource.data(), "%x", &n);
        return true;
    }
    else
    {
        if (sSource.size() == 1 && sSource.at(0) == '0')
        {
            n = 0;
            return true;
        }
        else
        {
            uint32 r = atoi(sSource.data());
            if (r != 0)
            {
                n = r;
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}

bool CxString::setFromString(const string &sSource, int64 &n)
{
    if (sSource.size()>2 && (sSource[1] == 'x' || sSource[1] == 'X'))
    {
        long unsigned int r;
        sscanf(sSource.data(), "%lx", &r);
        n = r;
        return true;
    }
    else
    {
        if (sSource.size() == 1 && sSource.at(0) == '0')
        {
            n = 0;
            return true;
        }
        else
        {
            int64 r;
            stringstream ss(sSource);
            ss >> r;
            if (r != 0)
            {
                n = r;
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}

bool CxString::setFromString(const string &sSource, uint64 &n)
{
    if (sSource.size()>2 && (sSource[1] == 'x' || sSource[1] == 'X'))
    {
        long unsigned int r;
        sscanf(sSource.data(), "%lx", &r);
        n = r;
        return true;
    }
    else
    {
        if (sSource.size() == 1 && sSource.at(0) == '0')
        {
            n = 0;
            return true;
        }
        else
        {
            uint64 r;
            stringstream ss(sSource);
            ss >> r;
            if (r != 0)
            {
                n = r;
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}

bool CxString::setFromString(const string &sSource, double &d)
{
    double r= strtod(sSource.data(), NULL);
    if (r != 0)
    {
        d = r;
        return true;
    }
    else
    {
        if (isvalidDouble(sSource))
        {
            d = 0;
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool CxString::setFromString(const string &sSource, float &f)
{
    float r = strtod(sSource.data(), NULL);
    if (r != 0)
    {
        f = r;
        return true;
    }
    else
    {
        if (isvalidFloat(sSource))
        {
            f = 0;
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool CxString::setFromString(const string &sSource, string &s)
{
    s = sSource;
    return true;
}

bool CxString::isvalidBoolean(const string &sSource)
{
    return sSource.size() > 0 && sSource[0] >= 0x30 && sSource[0] <= 0x39;
}

bool CxString::isvalidInteger(const string &sSource)
{   
    if (sSource.size()>2 && (sSource[1] == 'x' || sSource[1] == 'X'))
    {
        if (sSource.size() > 2 && sSource.size() < 11)
        {
            char const * pchEnd = sSource.data() + sSource.size();
            const char* pch = sSource.data()+2;
            while (pch < pchEnd)
            {
                if ( (*pch >= '0' && *pch <= '9') || (*pch >= 'a' && *pch <= 'f') || (*pch >= 'A' && *pch <= 'F') )
                {
                    pch++;
                    continue;
                }
                return false;
            }
            return true;
        }
        return false;
    }
    else
    {
        bool r = sSource.size() > 0 && sSource.size() < 11;
        if (r)
        {
            for (size_t i = 0; i < sSource.size(); ++i)
            {
                if (sSource[i] < 0x30 || sSource[i] > 0x39)
                {
                    return false;
                }
            }
            return true;
        }
        return false;
    }
}

bool CxString::isvalidLong(const string &sSource)
{
    bool r = sSource.size() > 0 && sSource.size() < 19;
    if (! r)
        return r;
    for (size_t i = 0; i < sSource.size(); ++i)
    {
        if (sSource[i] < 0x30 || sSource[i] > 0x39)
        {
            return false;
        }
    }
    return true;
}

bool CxString::isvalidFloat(const string &sSource)
{
    bool r = sSource.size() > 0 && sSource.size() < 9;
    if (! r)
        return r;
    int iDot = 0;
    for (size_t i = 0; i < sSource.size(); ++i)
    {
        if (sSource[i] == 0x2E)
        {
            ++ iDot;
            if (iDot > 1)
                return false;
        }
        else if (sSource[i] < 0x30 || sSource[i] > 0x39)
        {
            return false;
        }
    }
    return true;
}

bool CxString::isvalidDouble(const string &sSource)
{
    bool r = sSource.size() > 0 && sSource.size() < 17;
    if (! r)
        return r;
    int iDot = 0;
    for (size_t i = 0; i < sSource.size(); ++i)
    {
        if (sSource[i] == 0x2E)
        {
            ++ iDot;
            if (iDot > 1)
                return false;
        }
        else if (sSource[i] < 0x30 || sSource[i] > 0x39)
        {
            return false;
        }
    }
    return true;
}

string CxString::toHexstring(const char *pData, int iLength, bool bHasEmptyChar)
{
    if (bHasEmptyChar)
    {
        string sHex(iLength * 3, 0);
        char *hexData = const_cast<char *>( sHex.data() );
        const uchar *data = (const uchar *)pData;
        for (int i = 0; i < iLength; ++i) {
            int j = (data[i] >> 4) & 0xf;
            if (j <= 9)
                hexData[i*3] = (j + '0');
            else
                hexData[i*3] = (j + 'A' - 10);
            j = data[i] & 0xf;
            if (j <= 9)
                hexData[i*3+1] = (j + '0');
            else
                hexData[i*3+1] = (j + 'A' - 10);
            hexData[i*3+2] = 0x20;
        }
        return sHex;
    }
    else
    {
        string sHex(iLength * 2, 0);
        char *hexData = const_cast<char *>( sHex.data() );
        const uchar *data = (const uchar *)pData;
        for (int i = 0; i < iLength; ++i) {
            int j = (data[i] >> 4) & 0xf;
            if (j <= 9)
                hexData[i*2] = (j + '0');
            else
                hexData[i*2] = (j + 'A' - 10);
            j = data[i] & 0xf;
            if (j <= 9)
                hexData[i*2+1] = (j + '0');
            else
                hexData[i*2+1] = (j + 'A' - 10);
        }
        return string(sHex.data(), sHex.size());
    }
}

string CxString::toHexstring(const uchar *pData, int iLength, bool bHasEmptyChar)
{
    if (bHasEmptyChar)
    {
        string sHex(iLength * 3, 0);
        char *hexData = const_cast<char *>( sHex.data() );
        const uchar *data = (const uchar *)pData;
        for (int i = 0; i < iLength; ++i) {
            int j = (data[i] >> 4) & 0xf;
            if (j <= 9)
                hexData[i*3] = (j + '0');
            else
                hexData[i*3] = (j + 'A' - 10);
            j = data[i] & 0xf;
            if (j <= 9)
                hexData[i*3+1] = (j + '0');
            else
                hexData[i*3+1] = (j + 'A' - 10);
            hexData[i*3+2] = 0x20;
        }
        return sHex;
    }
    else
    {
        string sHex(iLength * 2, 0);
        char *hexData = const_cast<char *>( sHex.data() );
        const uchar *data = (const uchar *)pData;
        for (int i = 0; i < iLength; ++i) {
            int j = (data[i] >> 4) & 0xf;
            if (j <= 9)
                hexData[i*2] = (j + '0');
            else
                hexData[i*2] = (j + 'A' - 10);
            j = data[i] & 0xf;
            if (j <= 9)
                hexData[i*2+1] = (j + '0');
            else
                hexData[i*2+1] = (j + 'A' - 10);
        }
        return string(sHex.data(), sHex.size());
    }
}

string CxString::toHexstring(const vector<char> & data, bool bHasEmptyChar)
{
    if (data.size() > 0)
        return toHexstring((& data.front()), data.size(), bHasEmptyChar);
    else
        return std::string();
}

string CxString::toHexstring(const vector<uchar> &data, bool bHasEmptyChar)
{
    if (data.size() > 0)
        return toHexstring((& data.front()), data.size(), bHasEmptyChar);
    else
        return std::string();
}

string CxString::toHexstring(const char& v)
{
    string s = "0x00";
    char* data =  const_cast<char *>( s.data() );
    int j = (v >> 4) & 0xf;
    data[2] = f_cHexChar16[j];
    j = v & 0xf;
    data[3] = f_cHexChar16[j];
    return s;
}

string CxString::toHexstring(const uchar& v)
{
    string s = "0x00";
    char* data = const_cast<char *>( s.data() );
    int j = (v >> 4) & 0xf;
    data[2] = f_cHexChar16[j];
    j = v & 0xf;
    data[3] = f_cHexChar16[j];
    return s;
}

string CxString::toHexstring(const short& v)
{
    string s = "0x0000";
    char* data = const_cast<char *>( s.data() );
    int j = (v >> 12) & 0xf;
    data[2] = f_cHexChar16[j];
    j = (v >> 8) & 0xf;
    data[3] = f_cHexChar16[j];
    j = (v >> 4) & 0xf;
    data[4] = f_cHexChar16[j];
    j = v & 0xf;
    data[5] = f_cHexChar16[j];
    return s;
}

string CxString::toHexstring(const ushort& v)
{
    string s = "0x0000";
    char* data = const_cast<char *>( s.data() );
    int j = (v >> 12) & 0xf;
    data[2] = f_cHexChar16[j];
    j = (v >> 8) & 0xf;
    data[3] = f_cHexChar16[j];
    j = (v >> 4) & 0xf;
    data[4] = f_cHexChar16[j];
    j = v & 0xf;
    data[5] = f_cHexChar16[j];
    return s;
}

string CxString::toHexstring(const int& v)
{
    string s = "0x00000000";
    char* data = const_cast<char *>( s.data() );
    int j = (v >> 28) & 0xf;
    data[2] = f_cHexChar16[j];
    j = (v >> 24) & 0xf;
    data[3] = f_cHexChar16[j];
    j = (v >> 20) & 0xf;
    data[4] = f_cHexChar16[j];
    j = (v >> 16) & 0xf;
    data[5] = f_cHexChar16[j];
    j = (v >> 12) & 0xf;
    data[6] = f_cHexChar16[j];
    j = (v >> 8) & 0xf;
    data[7] = f_cHexChar16[j];
    j = (v >> 4) & 0xf;
    data[8] = f_cHexChar16[j];
    j = v & 0xf;
    data[9] = f_cHexChar16[j];
    return s;
}

string CxString::toHexstring(const uint32& v)
{
    string s = "0x00000000";
    char* data = const_cast<char *>( s.data() );
    int j = (v >> 28) & 0xf;
    data[2] = f_cHexChar16[j];
    j = (v >> 24) & 0xf;
    data[3] = f_cHexChar16[j];
    j = (v >> 20) & 0xf;
    data[4] = f_cHexChar16[j];
    j = (v >> 16) & 0xf;
    data[5] = f_cHexChar16[j];
    j = (v >> 12) & 0xf;
    data[6] = f_cHexChar16[j];
    j = (v >> 8) & 0xf;
    data[7] = f_cHexChar16[j];
    j = (v >> 4) & 0xf;
    data[8] = f_cHexChar16[j];
    j = v & 0xf;
    data[9] = f_cHexChar16[j];
    return s;
}

string CxString::toHexstring(const float& v)
{
    char fData[4];
    memcpy(fData, &v, 4);
    return toHexstring(fData, 4);
}

string CxString::toHexstring(const double& v)
{
    char fData[8];
    memcpy(fData, &v, 8);
    return toHexstring(fData, 8);
}

string CxString::toHexstring(const int &v, bool bHasHead)
{
    string s = "00000000";
    char* data = const_cast<char *>( s.data() );
    int j = (v >> 28) & 0xf;
    data[0] = f_cHexChar16[j];
    j = (v >> 24) & 0xf;
    data[1] = f_cHexChar16[j];
    j = (v >> 20) & 0xf;
    data[2] = f_cHexChar16[j];
    j = (v >> 16) & 0xf;
    data[3] = f_cHexChar16[j];
    j = (v >> 12) & 0xf;
    data[4] = f_cHexChar16[j];
    j = (v >> 8) & 0xf;
    data[5] = f_cHexChar16[j];
    j = (v >> 4) & 0xf;
    data[6] = f_cHexChar16[j];
    j = v & 0xf;
    data[7] = f_cHexChar16[j];
    if (bHasHead)
        return string("0x") + s;
    return s;
}

string CxString::toHexstring(const uint32 &v, bool bHasHead)
{
    string s = "00000000";
    char* data = const_cast<char *>( s.data() );
    int j = (v >> 28) & 0xf;
    data[0] = f_cHexChar16[j];
    j = (v >> 24) & 0xf;
    data[1] = f_cHexChar16[j];
    j = (v >> 20) & 0xf;
    data[2] = f_cHexChar16[j];
    j = (v >> 16) & 0xf;
    data[3] = f_cHexChar16[j];
    j = (v >> 12) & 0xf;
    data[4] = f_cHexChar16[j];
    j = (v >> 8) & 0xf;
    data[5] = f_cHexChar16[j];
    j = (v >> 4) & 0xf;
    data[6] = f_cHexChar16[j];
    j = v & 0xf;
    data[7] = f_cHexChar16[j];
    if (bHasHead)
        return string("0x") + s;
    return s;
}

template<typename TType>
vector<TType> f_fromHexstring(const string &s)
{
    vector<TType> rData;
    if (s.size() > 1)
    {
        string s2 = s;
        CxString::remove(s2, ' ');
        vector<TType> res((s2.size() + 1)/ 2);
        TType * result = (& res.front()) + res.size();

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

        rData.resize((& res.front()) + res.size() - result);
        ::copy(result, (& res.front()) + res.size(), rData.begin());
        //    res.remove(0, result - res.data());
    }
    return rData;
}

std::vector<uchar> CxString::fromHexstring(const string &s)
{
    return f_fromHexstring<uchar>(s);
}

std::vector<char> CxString::hexToData(const string &s)
{
    return f_fromHexstring<char>(s);
}

int CxString::hexToInt32(const string &s, bool * bOk)
{
    int r;
    int iCount = sscanf(s.data(), "%x", &r);
    if (bOk) *bOk = (iCount == 1);
    return r;
}

uint CxString::hexToUint32(const string &s, bool * bOk)
{
    uint r;
    int iCount = sscanf(s.data(), "%x", &r);
    if (bOk) *bOk = (iCount == 1);
    return r;
}

int64 CxString::hexToInt64(const string &s, bool * bOk)
{
    long unsigned int r;
    int iCount = sscanf(s.data(), "%lx", &r);
    if (bOk) *bOk = (iCount == 1);
    return r;
}

uint64 CxString::hexToUint64(const string &s, bool * bOk)
{
    long unsigned int r;
    int iCount = sscanf(s.data(), "%lx", &r);
    if (bOk) *bOk = (iCount == 1);
    return r;
}

void CxString::remove(string &s, char c)
{
    string::iterator itRemove = std::remove(s.begin(), s.end(), c);
    if (itRemove != s.end())
    {
        s.erase(itRemove, s.end());
    }
//    for (string::iterator it = s.begin(); it != s.end(); )
//    {
//        if (*it == c)
//        {
//            s.erase(it);
//            continue;
//        }
//        ++it;
//    }
}

vector<string> CxString::split(const string &ss, char cSplitCharacter, bool bHasEmptyString)
{
    vector<string> list;
    size_t start = 0;
    size_t end;
    while ((end = ss.find(cSplitCharacter, start)) != string::npos)
    {
        if (start != end)
            list.push_back(ss.substr(start, end - start));
        else if (bHasEmptyString)
            list.push_back("");
        start = end + 1;
    }
    if (start != ss.size())
        list.push_back(ss.substr(start));
    return list;
}

vector<string> fn_split(const string &s, const string &sSplitString)
{
    vector<string> ss;
    char const * pchEnd = s.data() + s.size() - 1;
    const char* pch = s.data();
    const char* pchStart = s.data();
    while (pch <= pchEnd)
    {
        if (sSplitString.find(*pch) == string::npos)
        {
            pchStart = pch;
            break;
        }
        pch ++;
    }
    pch = pchStart + 1;
    const char* pchOldStart;
    while (pch <= pchEnd)
    {
        size_t i = sSplitString.find(*pch);
        if (i != string::npos && pch > pchStart)
        {
            ss.push_back(string(pchStart, pch-pchStart));
            pchOldStart = pchStart;
            while (pch <= pchEnd)
            {
                pch ++;
                if (sSplitString.find(*pch) == string::npos)
                {
                    pchStart = pch;
                    break;
                }
            }
            if ( pchOldStart == pchStart )
                pchStart = pchEnd + 1;
        }
        pch ++;
    }
    if (pchStart <= pchEnd)
    {
        ss.push_back(string(pchStart, pch-pchStart));
    }
    return ss;
}

vector<string> CxString::split(const string &ss, const string &sSplitString, bool bHasEmptyString)
{
    vector<string> list;
    size_t start = 0;
    size_t end;
    while ((end = ss.find(sSplitString, start)) != string::npos)
    {
        if (start != end)
            list.push_back(ss.substr(start, end - start));
        else if (bHasEmptyString)
            list.push_back("");
        start = end + sSplitString.size();
    }
    if (start != ss.size())
        list.push_back(ss.substr(start));
    return list;
}

std::vector<string> CxString::split(const std::vector<string> &ss, const string &sSplitString, bool bHasEmptyString)
{
    string r(sizeOf(ss), 0);
    char * pch = (char*)r.data();
    for (size_t i = 0; i < ss.size(); ++i)
    {
        const string & s = ss.at(i);
        memcpy(pch, s.data(), s.size()); pch += s.size();
    }
    return split(r, sSplitString, bHasEmptyString);
}

vector<string> CxString::splitCase(const string &ss, const string &sSplitString, bool bHasEmptyString)
{
    string ls = toLower(ss);
    string lSplitString = toLower(sSplitString);

    vector<string> list;
    size_t start = 0;
    size_t end;
    while ((end = ls.find(lSplitString, start)) != string::npos)
    {
        if (start != end)
            list.push_back(ss.substr(start, end - start));
        else if (bHasEmptyString)
            list.push_back("");
        start = end + sSplitString.size();
    }
    if (start != ls.size())
        list.push_back(ss.substr(start));
    return list;
}

void CxString::splitCase(int mode, char *pBuf, int num, string &sVal, string &split)
{
    if(pBuf==NULL) return;
    if(sVal.length()>0)
    {
        std::vector<string> v = splitCase(sVal,split);

        if(v.size()==num)
        {
            switch(mode)
            {
            case 0://整形
            {
                int *pw = (int*)pBuf;
                for(int i=0;i<num;i++)
                {
                  pw[i] = CxString::toInt32(v.at(i));
                }
            }
                break;
            case 1://双精度
            {
                double *pd = (double *)pBuf;
                for(int i=0;i<num;i++)
                {
                    pd[i] =  CxString::toDouble(v.at(i));
                }
            }
                break;
            default:
                break;
            }
        }
    }
}

void CxString::splitCase(std::vector<std::vector<char> > &v, string &sVal, string &split)
{
    if(sVal.length()>0)
    {
        vector<string> v1 = splitCase(sVal,split);
        for(size_t i=0;i<v1.size();i++)
        {
           vector<char> v2 = hexToData(v1.at(i));
           if(v2.size()>0) v.push_back(v2);
        }
    }
}

std::map<string, string> CxString::splitToMap(const string &ss, char cMid, char cSplitCharacter, bool bTrim)
{
    if (bTrim)
        return doSplitToMap_trim(ss, cMid, cSplitCharacter);
    else
        return doSplitToMap_notrim(ss, cMid, cSplitCharacter);
}

std::map<string, string> CxString::splitToMap_mix(const string &ss, char cMid, char cSplitCharacter)
{
    std::map<string, string> r;
    const char * pchBeing = ss.data();
    const char * pchEnd = pchBeing+ss.size();
    const char * pchIndex = pchBeing;
    while ((*pchIndex == cMid) || (*pchIndex == cSplitCharacter))
    {
        pchIndex++;
    }
    const char * pchMid = pchIndex-1;
    const char * pchSplit = pchIndex-1;
    const char * pchKeyBeing;
    const char * pchKeyEnd;
    const char * pchValueBeing;
    const char * pchValueEnd;
//    bool bHasShift = false;
    while (pchIndex < pchEnd)
    {
        if (*pchIndex == cMid)
        {
            if ( (pchMid <= pchSplit) )
                pchMid = pchIndex;
//            if ( (*(pchIndex+1) == '\\') )
//            {
//                bHasShift = true;
//            }
//            else
//            {
//            }
        }
        else if (*pchIndex == cSplitCharacter)
        {
            if ((*(pchIndex+1) == '\\'))
            {
//                bHasShift = true;
            }
            else
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
//                bHasShift = false;
            }
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

std::map<string, string> CxString::splitToMap_reverse(const string &ss, char cMid, char cSplitCharacter, bool bTrim)
{
    std::map<string, string> r;
    size_t start = 0;
    size_t endMid;
    size_t endSplit;
    while (1)
    {
        string sKey;
        string sValue;
        endMid = ss.find(cMid, start);
        if (endMid != string::npos)
        {
            sKey = ss.substr(start, endMid - start);
            sKey = trim(sKey,CxGlobal::spaceChar);
            start = endMid + 1;

            endSplit = ss.find(cSplitCharacter, start);
            if (endSplit != string::npos)
            {
                sValue = ss.substr(start, endSplit - start);
                sValue = trim(sValue,CxGlobal::spaceChar);
                start = endSplit + 1;
                r[sValue] = sKey;
            }
            else
            {
                if (start != ss.size())
                {
                    sValue = ss.substr(start);
                    sValue = trim(sValue,CxGlobal::spaceChar);
                    r[sValue] = sKey;
                }
                break;
            }
        }
        else
        {
            break;
        }
    }
    return r;
}

std::vector<std::vector<string> > CxString::splitToLines(const string &ss, char cMid, char cSplitCharacter)
{
    vector<string> rs = split(ss, cSplitCharacter);
    std::vector<std::vector<string> > r;
    for (size_t i = 0; i < rs.size(); ++i)
    {
        const string & line = rs.at(i);
        r.push_back(split(line, cMid));
    }
    return r;
}

std::vector<std::map<string, string> > CxString::splitToLines(const std::vector<string> &ss, char cMid, char cSplitCharacter)
{
    std::vector<std::map<string, string> > r;
    for (size_t i = 0; i < ss.size(); ++i)
    {
        const std::string & sRow = ss.at(i);
        r.push_back(splitToMap(sRow, cMid, cSplitCharacter));
    }
    return r;
}

std::vector<std::map<string, string> > CxString::sortToLines(const std::map<string, std::map<string, string> > &ss)
{
    std::vector<string> sSorts;
    for (map<string, map<string, string> >::const_iterator itSection = ss.begin(); itSection != ss.end(); ++itSection)
    {
        sSorts.push_back(itSection->first);
    }
    std::sort(sSorts.begin(), sSorts.end());
    std::vector<std::map<string, string> > sRows;
    for (size_t i = 0; i < sSorts.size(); ++i)
    {
        const string & sSort = sSorts.at(i);
        std::map<string, std::map<string, string> >::const_iterator it = ss.find(sSort);
        if (it != ss.end())
            sRows.push_back(it->second);
    }
    return sRows;
}

string CxString::join(const std::vector<string> &ss, char cJoin)
{
    if (cJoin == 0)
    {
        string r(sizeOf(ss), 0);
        char * pch = (char*)r.data();
        for (size_t i = 0; i < ss.size(); ++i)
        {
            const string & s = ss.at(i);
            memcpy(pch, s.data(), s.size()); pch += s.size();
        }
        return r;
    }
    else
    {
        string r(sizeOf(ss) + ss.size(), 0);
        char * pch = (char*)r.data();
        for (size_t i = 0; i < ss.size(); ++i)
        {
            const string & s = ss.at(i);
            memcpy(pch, s.data(), s.size()); pch += s.size();
            * pch = cJoin; ++ pch;
        }
        if (r.size() > 0)
        {
            return r.substr(0, r.size()-1);
        }
        return r;
    }
}

string CxString::join(const vector<string> &ss, const string &sJoin)
{
    string r;
    size_t iSize = ss.size();
    size_t iSize_1 = iSize - 1;
    for (size_t i = 0; i < iSize; ++i)
    {
        r.append(ss.at(i));
        if (i != iSize_1)
        {
            r.append(sJoin);
        }
    }
    return r;
}

string CxString::join(const map<string, string> &pairs, const string &sMid, const string &sJoin)
{
    string r;
    for (map<string, string>::const_iterator it = pairs.begin(); it != pairs.end(); ++it)
    {
        r.append(it->first + sMid + it->second + sJoin);
    }
    if (r.size()>0) r.resize(r.size()-sJoin.size());
    return r;
}

vector<string> CxString::joinToStringList(const map<string, string> &pairs, const string &sMid)
{
    vector<string> rs;
    for (map<string, string>::const_iterator it = pairs.begin(); it != pairs.end(); ++it)
    {
        rs.push_back(it->first + sMid + it->second);
    }
    return rs;
}

string CxString::join(const std::vector<std::vector<string> > &lines, char cMid, char cSplitCharacter)
{
    vector<string> rs;
    for (size_t i = 0; i < lines.size(); ++i)
    {
        const vector<string> & line = lines.at(i);
        rs.push_back(join(line, cMid));
    }
    return join(rs, cSplitCharacter);
}

string CxString::replace(const string &strBase, char cSrc, char cDes)
{
    string r(strBase.data(), strBase.size());
    uchar * first = (uchar *)r.data();
    uchar * end = first + r.size() - 1;

    uchar * pch = first;
    while (pch < end)
    {
        if (* pch == cSrc)
            * pch = cDes;
        ++pch;
    }
    return r;
}

string CxString::replace(const string &strBase, const string &strSrc, const string &strDes)
{
    string sResult(strBase.data(), strBase.size());
    string::size_type pos = 0;
    string::size_type srcLen = strSrc.size();
    string::size_type desLen = strDes.size();
    pos=sResult.find(strSrc, pos);
    while ((pos != string::npos))
    {
        sResult.replace(pos, srcLen, strDes);
        pos=sResult.find(strSrc, (pos+desLen));
    }
    return sResult;
}

string CxString::replaceCase(const string &strBase, const string &strSrc, const string &strDes)
{
    vector<string> ss = splitCase(strBase, strSrc);
    if (ss.size() > 0)
        return join(ss, strDes);
    else
        return strBase;
}

string CxString::trim(const string &s)
{
    string r(s.data(), s.size());
    if (r.empty()) return r;
    uchar * first = (uchar *)r.data();
    uchar * end = first + r.size() - 1;

    uchar * pchFirst = first;
    uchar * pchEnd = end;
    while (pchFirst < end)
    {
        if (* pchFirst < '!')
            pchFirst ++;
        else
            break;
    }
    while (pchEnd > first)
    {
        if (* pchEnd < '!')
            pchEnd --;
        else
            break;
    }
    if (pchFirst == first && pchEnd == end)
    {
        return r;
    }
    else if (pchEnd >= pchFirst)
    {
        return string((char*)pchFirst, pchEnd - pchFirst + 1);
    }
    else
    {
        return string("");
    }
}

string CxString::trim(const string &s, const char cDelete)
{
    if (s.empty()) return std::string();
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

string CxString::trim(const string &s, const string &sDelete)
{
    if (s.empty()) return std::string();
    char* first = const_cast<char *>( s.data() );
    char* end = first + s.size() - 1;
    int iSize = sDelete.size();
    char * pchFirst = first;
    char * pchEnd = end;
    while (pchFirst < end)
    {
        if (pchFirst + iSize <= end && strncmp(pchFirst, sDelete.data(), iSize) == 0)
            pchFirst += iSize;
        else
            break;
    }
    while (pchEnd > first)
    {
        if (pchEnd - iSize >= first && strncmp(pchEnd-iSize, sDelete.data(), iSize) == 0)
            pchEnd -= iSize;
        else
            break;
    }
    if (pchEnd >= pchFirst)
        return string(pchFirst, pchEnd - pchFirst + 1);
    else
        return string("");
}

void fn_lower(char *str)
{
    while(str && *str)
    {
        *str = ::tolower(*str);
        ++str;
    }
}

void CxString::toLowerSelf( string & r )
{
    char * pchBegin = const_cast<char *>( r.data() );
    char * pchEnd = pchBegin + r.size();
    char * pch = pchBegin;
    while ( pch < pchEnd )
    {
        if ( * pch >= 0x41 && * pch <= 0x5A )
            * pch += 0x20;
        pch ++;
    }
}

string CxString::toLower( const string & s )
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

void fn_upper(char *str)
{
    while(str && *str)
    {
        *str = ::toupper(*str);
        ++str;
    }
}

void CxString::toUpperSelf( string & r )
{
    char * pchBegin = const_cast<char *>( r.data() );
    char * pchEnd = pchBegin + r.size();
    char * pch = pchBegin;
    while ( pch < pchEnd )
    {
        if ( * pch >= 0x61 && * pch <= 0x7A )
            * pch -= 0x20;
        pch ++;
    }
}

string CxString::toUpper( const string& s )
{
    string r(s.data(), s.size());
    char * pchBegin = const_cast<char *>( r.data() );
    char * pchEnd = pchBegin + r.size();
    char * pch = pchBegin;
    while ( pch < pchEnd )
    {
        if ( * pch >= 0x61 && * pch <= 0x7A )
            * pch -= 0x20;
        pch ++;
    }
    return r;
}

bool CxString::equalCase(const string &s1, const string &s2)
{
    string str1 = toLower(s1);
    string str2 = toLower(s2);
    return str1 == str2;
}

bool CxString::equalIgnoreAll(const string &s1, const string &s2)
{
    string str1 = trim(toLower(s1));
    string str2 = trim(toLower(s2));
    return str1 == str2;
}

size_t CxString::findLeftCase(const string &sMaster, const string &sSub)
{
    string str1 = toLower(sMaster);
    string str2 = toLower(sSub);
    return str1.find(str2);
}

size_t CxString::findLeftCase(const string &sMaster, const std::vector<string> &sSubs)
{
    string str1 = toLower(sMaster);
    for (size_t i = 0; i < sSubs.size(); ++i)
    {
        string str2 = toLower(sSubs.at(i));
        size_t iPos = str1.find(str2);
        if (iPos != string::npos)
        {
            return iPos;
        }
    }
    return string::npos;
}

size_t CxString::findRightCase(const string &sMaster, const string &sSub)
{
    string str1 = toLower(sMaster);
    string str2 = toLower(sSub);
    return str1.rfind(str2);
}

bool CxString::exist(const string &sMaster, const string &sSub)
{
    return sMaster.find(sSub) != string::npos;
}

bool CxString::existCase(const string &sMaster, const string &sSub)
{
    return findLeftCase(sMaster, sSub) != string::npos;
}

bool CxString::existCase(const string &sMaster, const std::vector<string> &sSubs)
{
    return findLeftCase(sMaster, sSubs) != string::npos;
}

bool CxString::beginWith(const string &sMaster, const string &sSub)
{
    return sMaster.find(sSub) == 0;
}

bool CxString::beginWithCase(const string &sMaster, const string &sSub)
{
    return findLeftCase(sMaster, sSub) == 0;
}

bool CxString::endWith(const string &sMaster, const string &sSub)
{
    int nPos = sMaster.rfind(sSub);
    if(nPos!= string::npos)
    {
        return nPos == (sMaster.size() - sSub.size());
    }
    return false;
}

bool CxString::endWithCase(const string &sMaster, const string &sSub)
{
    size_t nPos = findRightCase(sMaster, sSub);
    if(nPos!= string::npos)
    {
        return nPos == (sMaster.size() - sSub.size());
    }
    return false;
}

string CxString::format(const char *sFormat, ...)
{
    char buffer[512];
    va_list args;
    va_start (args, sFormat);
//    vsprintf (buffer, sFormat, args); //会有内存溢出 llb 20140427
    vsnprintf(buffer,sizeof(buffer),sFormat, args);

//    perror (buffer);
    va_end (args);
    return buffer;
}

string CxString::token(string &s, char cToken, bool *ok)
{
    string r;
    size_t iIndex = s.find(cToken);
    if (iIndex != string::npos)
    {
        r = s.substr(0, iIndex);
        s = s.substr(iIndex+1);
        if (ok) *ok = true;
    }
    else
    {
        if (ok) *ok = false;
    }
    return r;
}

string CxString::token(string &s, const string &sToken, bool *ok)
{
    string r;
    size_t iIndex = s.find(sToken);
    if (iIndex != string::npos)
    {
        r = s.substr(0, iIndex);
        s = s.substr(iIndex+sToken.size());
        if (ok) *ok = true;
    }
    else
    {
        if (ok) *ok = false;
    }
    return r;
}

string CxString::tokenLeft(string &s, char cToken, bool *ok)
{
    string r;
    size_t iIndex = s.find(cToken);
    if (iIndex != string::npos)
    {
        r = s.substr(iIndex+1);
        s = s.substr(0, iIndex);
        if (ok) *ok = true;
    }
    else
    {
        if (ok) *ok = false;
    }
    return r;
}

string CxString::tokenLeft(string &s, const string &sToken, bool *ok)
{
    string r;
    size_t iIndex = s.find(sToken);
    if (iIndex != string::npos)
    {
        r = s.substr(iIndex+sToken.size());
        s = s.substr(0, iIndex);
        if (ok) *ok = true;
    }
    else
    {
        if (ok) *ok = false;
    }
    return r;
}

string CxString::tokenRight(string &s, char cToken, bool *ok)
{
    string r;
    size_t iIndex = s.rfind(cToken);
    if (iIndex != string::npos)
    {
        r = s.substr(iIndex+1);
        s.resize(iIndex);
        if (ok) *ok = true;
    }
    else
    {
        if (ok) *ok = false;
    }
    return r;
}

string CxString::tokenRight(string &s, const string &sToken, bool *ok)
{
    string r;
    size_t iIndex = s.rfind(sToken);
    if (iIndex != string::npos)
    {
        r = s.substr(iIndex+sToken.size());
        s.resize(iIndex);
        if (ok) *ok = true;
    }
    else
    {
        if (ok) *ok = false;
    }
    return r;
}

string CxString::unquote(const string &s, char quote_l, char quote_r)
{
    uchar* first = (uchar *)s.data();
    uchar* end = first + s.size() - 1;

    uchar * pchFirst = first;
    uchar * pchEnd = end;
    bool bHas = false;
    while (pchFirst < end)
    {
        if (bHas)
        {
            if (* pchFirst > ' ')
                break;
        }
        else
        {
            if (* pchFirst == quote_l)
                bHas = true;
        }
        pchFirst ++;
    }
    if (! bHas)
    {
        return std::string();
    }
    bHas = false;
    while (pchEnd > first)
    {
        if (bHas)
        {
            if (* pchEnd > ' ')
                break;
        }
        else
        {
            if (* pchEnd == quote_r)
                bHas = true;
        }
        pchEnd --;
    }
    if (! bHas)
    {
        return std::string();
    }
    if (pchEnd >= pchFirst)
    {
        return string((char*)pchFirst, pchEnd - pchFirst + 1);
    }
    else
    {
        return std::string();
    }
}

string CxString::unquote(const string &s, char c)
{
    string r;
    size_t iLIndex = s.find(c);
    if (iLIndex != string::npos)
    {
        size_t iRIndex = s.rfind(c);
        if (iRIndex != string::npos)
        {
            int iLength = iRIndex - iLIndex;
            if (iLength > 1)
            r = string(s.data()+iLIndex+1, iLength-1);
        }
    }
    return r;
}

string CxString::unquote(const string &s, const string &quote_l, const string &quote_r)
{
    string r;
    if (quote_l.empty() || quote_r.empty())
    {
        return r;
    }
    size_t iIndexLeft = s.find(quote_l);
    if (iIndexLeft != string::npos)
    {
        size_t iPosLeft = iIndexLeft+quote_l.size();
        size_t iIndexRight = s.find(quote_r, iPosLeft);
        if (iIndexRight != string::npos)
        {
            r = s.substr(iPosLeft, iIndexRight-iPosLeft);
        }
    }
    return r;
}

int CxString::lengthCString(int n, ...)
{
    int rSize = 0;
    va_list vl;
    va_start(vl,n);
    for (int i = 0; i < n; ++i)
    {
        char * pch = va_arg(vl, char *);
        rSize += strlen(pch);
    }
    va_end(vl);
    return rSize + n;
}

int CxString::copyCString(char *dest, int n, ...)
{
    int rSize = 0;
    int iSize = 0;
    va_list vl;
    va_start(vl,n);
    for (int i = 0; i < n; ++i)
    {
        char * pch = va_arg(vl, char *);
        iSize = strlen(pch) + 1;
        memcpy(dest, pch, iSize);
        dest += iSize;
        rSize += iSize;
    }
    va_end(vl);
    return rSize;
}

uint32 CxString::toUint32(char *pBuff,int iLenth,bool bInverse)
{
    uint32 val = 0;
    if(bInverse)
    {
        switch(iLenth)
        {
        case 1:
            val = pBuff[0];
            break;
        case 2:
            val = pBuff[0]|(pBuff[1]<<8);
            break;
        case 3:
            val = pBuff[0]|(pBuff[1]<<8)|(pBuff[2]<<16);
            break;
        case 4:
            val = pBuff[0]|(pBuff[1]<<8)|(pBuff[2]<<16)|(pBuff[4]<<24);
            break;
        default:
            break;
        }

    }else{
        switch(iLenth)
        {
        case 1:
            val = pBuff[0];
            break;
        case 2:
            val = (pBuff[0]<<8)|(pBuff[1]);
            break;
        case 3:
            val = (pBuff[0]<<16)|(pBuff[1]<<8)|(pBuff[2]);
            break;
        case 4:
            val = (pBuff[0]<<24)|(pBuff[1]<<16)|(pBuff[2]<<8)|(pBuff[4]);
            break;
        default:
            break;
        }
    }
    return val;
}

uint32 CxString::toUint32(uchar *pBuff,int iLenth,bool bInverse)
{
    uint32 val = 0;
    if(bInverse)
    {
        switch(iLenth)
        {
        case 1:
            val = pBuff[0];
            break;
        case 2:
            val = pBuff[0]|(pBuff[1]<<8);
            break;
        case 3:
            val = pBuff[0]|(pBuff[1]<<8)|(pBuff[2]<<16);
            break;
        case 4:
            val = pBuff[0]|(pBuff[1]<<8)|(pBuff[2]<<16)|(pBuff[4]<<24);
            break;
        default:
            break;
        }

    }else{
        switch(iLenth)
        {
        case 1:
            val = pBuff[0];
            break;
        case 2:
            val = (pBuff[0]<<8)|(pBuff[1]);
            break;
        case 3:
            val = (pBuff[0]<<16)|(pBuff[1]<<8)|(pBuff[2]);
            break;
        case 4:
            val = (pBuff[0]<<24)|(pBuff[1]<<16)|(pBuff[2]<<8)|(pBuff[4]);
            break;
        default:
            break;
        }
    }
    return val;
}

/*
uint32 CxString::toUint32(std::string szBuf)
{
    uint32 val = 0;
    string tmp="";
    size_t i=0;
    //消除空格
    for(i=0;i<szBuf.length();i++)
    {
        if(szBuf.substr(i,1)!=" ")
        {
            tmp+=szBuf.substr(i,1);
        }
    }
    //消除开始字符
    if((tmp.substr(0,2)=="0x")||(tmp.substr(0,2)=="0X"))
    {
        tmp = tmp.substr(2,tmp.length()-2);
        vector<uchar> v = fromHexstring(tmp);

        size_t count = v.size();
        if(count<1) return 0;
        if(count>4) count = 4;

        for(i=0;i<count;i++)
        {
            val |= (v[i]<<((count-i-1)*8));
        }
    }
    else //十进制
    {
        val = atoi(szBuf.data());
    }
    return val;
}
*/
//Ip 地址转寄存器
//std::string CxString::ipToString(uint32 val)
//{
//    char*pBuf = (char*)&val;
//    char cTmp[20];
//    memset(cTmp, 0, sizeof(cTmp));
//    sprintf(cTmp,"%d.%d.%d.%d",pBuf[0],pBuf[1],pBuf[2],pBuf[3]);
//    return std::string(cTmp);
//}

//uint32 CxString::ipToUint32(std::string val)
//{
//    //needtodo
//    return 0;
//}

bool CxString::isValidIp(const std::string &sIp)
{
   vector<string> v = split(sIp,'.');
   if(v.size()<4 || v.size() > 6) return false;

   int n[6];
   bool bOk;
   for(size_t i=0;i<v.size();i++)
   {
      n[i] = toInt32(v.at(i),&bOk);
      if(n[i]>255 || ! bOk) return false;
   }

   return true;
}

bool CxString::isValidPort(int iPort)
{
    return iPort > 0x0000 && iPort < 0xffff;
}

bool CxString::isValidPath(const string &sPath)
{
    string s = trim(sPath);
    if (s.empty())
        return false;
    if (s[0] == '/' || (s.size()>1 && s[1]==':'))
        return isValidFullPath(sPath);
    const char* pch = s.data();
    const char* pchEnd = s.data() + s.size();
    while (pch < pchEnd)
    {
        if (*pch=='/' || *pch=='\\')
        {
            ++pch;
            continue;
        }
        if (*pch > 0x20 && f_cPathChar[*pch])
            ++pch;
        else
            return false;
    }
    return true;
}

bool CxString::isValidFullPath(const string &sPath)
{
    string s = trim(sPath);
    if (s.empty())
        return false;
    char cRoot = s[0];
    const char* pch;
    const char* pchEnd = s.data() + s.size();
    if (cRoot != '/')
    {
        if (s.size()<2)
            return false;
        if (s[1] == ':')
        {
             bool r = (cRoot>'a' && cRoot<'z') || (cRoot>'A' && cRoot<'Z');
             if (! r)
                 return false;
        }
        else
        {
            return false;
        }
        pch = s.data()+2;
    }
    else
    {
        pch = s.data()+1;
    }

    while (pch < pchEnd)
    {
        if (*pch=='/' || *pch=='\\')
        {
            ++pch;
            continue;
        }
        if (*pch > 0x20 && f_cPathChar[*pch])
            ++pch;
        else
            return false;
    }
    return true;
}

bool CxString::isValidFileName(const string &sPath)
{
    const char* pch = sPath.data();
    const char* pchEnd = sPath.data() + sPath.size();
    while (pch < pchEnd)
    {
        if (*pch > 0x20 && f_cPathChar[*pch])
            ++pch;
        else
            return false;
    }
    return true;
}

bool CxString::isValidPathName(const string &sPath)
{
    const char* pch = sPath.data();
    const char* pchEnd = sPath.data() + sPath.size();
    while (pch < pchEnd)
    {
        if (*pch > 0x20 && f_cPathChar[*pch])
            ++pch;
        else
            return false;
    }
    return true;
}

bool CxString::isValidCodeName(const string &sName)
{
    const char* pch = sName.data();
    const char* pchEnd = sName.data() + sName.size();
    while (pch < pchEnd)
    {
        if ((*pch >= 'a' && *pch <= 'z') || (*pch >= 'A' && *pch <= 'Z') || (*pch >= '0' && *pch <= '9') || *pch == '-' || *pch == '_')
            ++pch;
        else
            return false;
    }
    return true;
}

int CxString::copyTo(const std::vector<string> &sLines, char *dest, int iLength)
{
    int iSize = sizeOf(sLines) + sLines.size();
    if (iSize > iLength)
        return 0;
    memset(dest, 0, iLength);
    for (size_t i = 0; i < sLines.size(); ++i)
    {
        const string & sLine = sLines.at(i);
        memcpy(dest, sLine.data(), sLine.size()); dest += sLine.size();
        * dest = '\n'; ++ dest;
    }
    return iSize;
}

int CxString::sizeOf(const std::vector<string> &sLines)
{
    int r = 0;
    for (size_t i = 0; i < sLines.size(); ++i)
    {
        const string & sLine = sLines.at(i);
        r += sLine.size();
    }
    return r;
}

bool CxString::find(const string &src, const string &start, const string &end, int &pos, int &len, string &des)
{
    bool bRet = false;

    size_t ns = src.rfind(start);
    size_t ne = src.rfind(end);

    if(ns!=string::npos && ne!=string::npos && ne>(ns+start.length()))
    {
        pos = ns;
        len = ne-ns+end.length();
        des = src.substr(ns+start.length(),ne-ns-start.length());
        bRet = true;
    }
    return bRet;
}

int CxString::countOfString(const string &s, const string &sSplitString)
{
    int iCount = 0;
    size_t start = 0;
    size_t end;
    while ((end = s.find(sSplitString, start)) != string::npos)
    {
        start = end + sSplitString.size();
        iCount++;
    }
    return iCount;
}

int CxString::countOfString(const string &s, char sub)
{
    int iCount = 0;
    const char* pch = s.data();
    const char* pchEnd = s.data() + s.size();
    while (pch < pchEnd)
    {
        if (*pch == sub)
            iCount++;
        ++pch;
    }
    return iCount;
}

std::map<string, string> CxString::doSplitToMap_trim(const string &s, char cMid, char cSplitCharacter)
{
    std::map<string, string> r;
    size_t start = 0;
    size_t endMid;
    size_t endSplit;
    while (1)
    {
        string sKey;
        string sValue;
        endMid = s.find(cMid, start);
        if (endMid != string::npos)
        {
            sKey = s.substr(start, endMid - start);
            sKey = trim(sKey,CxGlobal::spaceChar);
            start = endMid + 1;

            endSplit = s.find(cSplitCharacter, start);
            if (endSplit != string::npos)
            {
                sValue = s.substr(start, endSplit - start);
                sValue = trim(sValue,CxGlobal::spaceChar);
                start = endSplit + 1;
                r[sKey] = sValue;
            }
            else
            {
                if (start != s.size())
                {
                    sValue = s.substr(start);
                    sValue = trim(sValue,CxGlobal::spaceChar);
                    r[sKey] = sValue;
                }
                break;
            }
        }
        else
        {
            break;
        }
    }
    return r;
}

std::map<string, string> CxString::doSplitToMap_notrim(const string &s, char cMid, char cSplitCharacter)
{
    std::map<string, string> r;
    size_t start = 0;
    size_t endMid;
    size_t endSplit;
    while (1)
    {
        string sKey;
        string sValue;
        endMid = s.find(cMid, start);
        if (endMid != string::npos)
        {
            sKey = s.substr(start, endMid - start);
            start = endMid + 1;

            endSplit = s.find(cSplitCharacter, start);
            if (endSplit != string::npos)
            {
                sValue = s.substr(start, endSplit - start);
                start = endSplit + 1;
                r[sKey] = sValue;
            }
            else
            {
                if (start != s.size())
                {
                    sValue = s.substr(start);
                    r[sKey] = sValue;
                }
                break;
            }
        }
        else
        {
            break;
        }
    }
    return r;
}

std::map<string, string> CxString::doSplitToMap_reverse_trim(const string &s, char cMid, char cSplitCharacter)
{
    std::map<string, string> r;
    size_t start = 0;
    size_t endMid;
    size_t endSplit;
    while (1)
    {
        string sKey;
        string sValue;
        endMid = s.find(cMid, start);
        if (endMid != string::npos)
        {
            sKey = s.substr(start, endMid - start);
            sKey = trim(sKey,CxGlobal::spaceChar);
            start = endMid + 1;

            endSplit = s.find(cSplitCharacter, start);
            if (endSplit != string::npos)
            {
                sValue = s.substr(start, endSplit - start);
                sValue = trim(sValue,CxGlobal::spaceChar);
                start = endSplit + 1;
                r[sValue] = sKey;
            }
            else
            {
                if (start != s.size())
                {
                    sValue = s.substr(start);
                    sValue = trim(sValue,CxGlobal::spaceChar);
                    r[sValue] = sKey;
                }
                break;
            }
        }
        else
        {
            break;
        }
    }
    return r;
}

std::map<string, string> CxString::doSplitToMap_reverse_notrim(const string &s, char cMid, char cSplitCharacter)
{
    std::map<string, string> r;
    size_t start = 0;
    size_t endMid;
    size_t endSplit;
    while (1)
    {
        string sKey;
        string sValue;
        endMid = s.find(cMid, start);
        if (endMid != string::npos)
        {
            sKey = s.substr(start, endMid - start);
            start = endMid + 1;

            endSplit = s.find(cSplitCharacter, start);
            if (endSplit != string::npos)
            {
                sValue = s.substr(start, endSplit - start);
                start = endSplit + 1;
                r[sValue] = sKey;
            }
            else
            {
                if (start != s.size())
                {
                    sValue = s.substr(start);
                    r[sValue] = sKey;
                }
                break;
            }
        }
        else
        {
            break;
        }
    }
    return r;
}




/**
 * --- std::string end --------------------------------------------------------
 */



/**
 * *** c string begin *********************************************************
 */

char * CxStringC::create(const char *cp)
{
    char *mem;

    if(!cp)
        return NULL;

    size_t len = strlen(cp) + 1;
    mem = (char *)malloc(len);
    GM_ASSERT_IFNOT( mem != NULL, "string dup allocation error");
    set(mem, len, cp);
    return mem;
}

char * CxStringC::createLeft(const char *cp, size_t size)
{
    char *mem;

    if(!cp)
        return NULL;

    if(!size)
        size = strlen(cp);

    mem = (char *)malloc(++size);
    GM_ASSERT_IFNOT( mem != NULL, "string dup allocation error" );
    set(mem, size, cp);
    return mem;
}

char * CxStringC::set(char *str, size_t size, const char *s)
{
    if(!str)
        return NULL;

    if(size < 2)
        return str;

    if(!s)
        s = "";

    size_t l = strlen(s);

    if(l >= size)
        l = size - 1;

    if(!l) {
        *str = 0;
        return str;
    }

    memmove(str, s, l);
    str[l] = 0;
    return str;
}

char * CxStringC::set(char *str, size_t size, const char *s, size_t len)
{
    if(!str)
        return NULL;

    if(size < 2)
        return str;

    if(!s)
        s = "";

    size_t l = strlen(s);
    if(l >= size)
        l = size - 1;

    if(l > len)
        l = len;

    if(!l) {
        *str = 0;
        return str;
    }

    memmove(str, s, l);
    str[l] = 0;
    return str;
}

char * CxStringC::token(char *text, char **token, const char *clist, const char *quote, const char *eol)
{
    char *result;

    if(!eol)
        eol = "";

    if(!token || !clist)
        return NULL;

    if(!*token)
        *token = text;

    if(!**token) {
        *token = text;
        return NULL;
    }

    while(**token && strchr(clist, **token))
        ++*token;

    result = *token;

    if(*result && *eol && NULL != (eol = strchr(eol, *result))) {
        if(eol[0] != eol[1] || *result == eol[1]) {
            *token = text;
            return NULL;
        }
    }

    if(!*result) {
        *token = text;
        return NULL;
    }

    while(quote && *quote && *result != *quote) {
        quote += 2;
    }

    if(quote && *quote) {
        ++result;
        ++quote;
        *token = strchr(result, *quote);
        if(!*token)
            *token = result + strlen(result);
        else {
            **token = 0;
            ++(*token);
        }
        return result;
    }

    while(**token && !strchr(clist, **token))
        ++(*token);

    if(**token) {
        **token = 0;
        ++(*token);
    }

    return result;
}

char *CxStringC::unquote(char *str, const char *clist)
{
    assert(clist != NULL);

    size_t len = strlen(str);
    if(!len || !str)
        return NULL;

    while(clist[0]) {
        if(*str == clist[0] && str[len - 1] == clist[1]) {
            str[len - 1] = 0;
            return ++str;
        }
        clist += 2;
    }
    return str;
}

int CxStringC::compareCase(const char *s1, const char *s2)
{
    if(!s1)
        s1 = "";

    if(!s2)
        s2 = "";

#ifdef GM_OS_WIN
    return _stricmp(s1, s2);
#else
    return strcasecmp(s1, s2);
#endif
}

char * CxStringC::trimLeft(char *str, const char *clist)
{
    if(!str)
        return NULL;

    if(!clist)
        return str;

    while(*str && strchr(clist, *str))
        ++str;

    return str;
}

char * CxStringC::trimRight(char *str, const char *clist)
{
    if(!str)
        return NULL;

    if(!clist)
        return str;

    size_t offset = strlen(str);
    while(offset && strchr(clist, str[offset - 1]))
        str[--offset] = 0;
    return str;
}

char * CxStringC::trim(char *str, const char *clist)
{
    str = trimLeft(str, clist);
    trimRight(str, clist);
    return str;
}

size_t CxStringC::strcspnRight(char *str, const char *clist)
{
    size_t size = strlen(str);

    if(!str)
        return 0;

    if(!clist)
        return size;

    size_t pos = size - 1;
    while(str[pos]) {
        if(strchr(clist, str[pos]))
            return pos;
        --pos;
    }
    return size;
}

char * CxStringC::inside(char *str, const char *clist)
{
    if(!str || !clist)
        return NULL;

    while(* str) {
        if(strchr(clist, *str))
            return str;
        ++str;
    }
    return NULL;
}

char * CxStringC::insideRight(char *str, const char *clist)
{
    if(!str || !clist)
        return NULL;

    str += strlen(str) - 1;
    while(* str) {
        if(strchr(clist, *str))
            return str;
        --str;
    }
    return NULL;
}

char * CxStringC::skip(char *str, const char *clist)
{
    if(!str || !clist)
        return NULL;

    while(*str && strchr(clist, *str))
        ++str;

    if(*str)
        return str;

    return NULL;
}

char * CxStringC::skipRight(char *str, const char *clist)
{
    size_t len = strlen(str);

    if(!len || !clist)
        return NULL;

    while(len > 0) {
        if(!strchr(clist, str[--len]))
            return str;
    }
    return NULL;
}

unsigned CxStringC::hexsize(const char *format)
{
    unsigned count = 0;
    char *ep;
    unsigned skip;

    while(format && *format) {
        while(*format && !isdigit(*format)) {
            ++format;
            ++count;
        }
        if(isdigit(*format)) {
            skip = (unsigned)strtol(format, &ep, 10);
            format = ep;
            count += skip * 2;
        }
    }
    return count;
}

unsigned CxStringC::hexdump(const unsigned char *binary, char *string, const char *format)
{
    unsigned count = 0;
    char *ep;
    unsigned skip;

    while(format && *format) {
        while(*format && !isdigit(*format)) {
            *(string++) = *(format++);
            ++count;
        }
        if(isdigit(*format)) {
            skip = (unsigned)strtol(format, &ep, 10);
            format = ep;
            count += skip * 2;
            while(skip--) {
                snprintf(string, 3, "%02x", *(binary++));
                string += 2;
            }
        }
    }
    *string = 0;
    return count;
}

static unsigned hex(char ch)
{
    if(ch >= '0' && ch <= '9')
        return ch - '0';
    else
        return toupper(ch) - 'A' + 10;
}

unsigned CxStringC::hexpack(unsigned char *binary, const char *string, const char *format)
{
    unsigned count = 0;
    char *ep;
    unsigned skip;

    while(format && *format) {
        while(*format && !isdigit(*format)) {
            if(*(string++) != *(format++))
                return count;
            ++count;
        }
        if(isdigit(*format)) {
            skip = (unsigned)strtol(format, &ep, 10);
            format = ep;
            count += skip * 2;
            while(skip--) {
                *(binary++) = hex(string[0]) * 16 + hex(string[1]);
                string += 2;
            }
        }
    }
    return count;
}


static const unsigned char alphabet[65] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

size_t CxStringC::b64encode(char *dest, const uint8 *bin, size_t size, size_t dsize)
{
    assert(dest != NULL && bin != NULL);

    size_t count = 0;

    if(!dsize)
        dsize = (size * 4 / 3) + 1;

    if (!dsize || !size)
        goto end;

    unsigned bits;

    while(size >= 3 && dsize > 4) {
        bits = (((unsigned)bin[0])<<16) | (((unsigned)bin[1])<<8)
            | ((unsigned)bin[2]);
        bin += 3;
        size -= 3;
        count += 3;
        *(dest++) = alphabet[bits >> 18];
        *(dest++) = alphabet[(bits >> 12) & 0x3f];
        *(dest++) = alphabet[(bits >> 6) & 0x3f];
        *(dest++) = alphabet[bits & 0x3f];
        dsize -= 4;
    }

    if (size && dsize > 4) {
        bits = ((unsigned)bin[0])<<16;
        *(dest++) = alphabet[bits >> 18];
        ++count;
        if (size == 1) {
            *(dest++) = alphabet[(bits >> 12) & 0x3f];
            *(dest++) = '=';
        }
        else {
            ++count;
            bits |= ((unsigned)bin[1])<<8;
            *(dest++) = alphabet[(bits >> 12) & 0x3f];
            *(dest++) = alphabet[(bits >> 6) & 0x3f];
        }
        *(dest++) = '=';
    }

end:
    *dest = 0;
    return count;
}

size_t CxStringC::b64decode(uint8 *dest, const char *src, size_t size)
{
    char decoder[256];
    unsigned long bits;
    uint8 c;
    unsigned i;
    size_t count = 0;

    for (i = 0; i < 256; ++i)
        decoder[i] = 64;

    for (i = 0; i < 64 ; ++i)
        decoder[alphabet[i]] = i;

    bits = 1;

    while(*src) {
        c = (uint8)(*(src++));
        if (c == '=') {
            if (bits & 0x40000) {
                if (size < 2)
                    break;
                *(dest++) = (uint8)((bits >> 10) & 0xff);
                *(dest++) = (uint8)((bits >> 2) & 0xff);
                count += 2;
                break;
            }
            if ((bits & 0x1000) && size) {
                *(dest++) = (uint8)((bits >> 4) & 0xff);
                ++count;
            }
            break;
        }
        // end on invalid chars
        if (decoder[c] == 64)
            break;
        bits = (bits << 6) + decoder[c];
        if (bits & 0x1000000) {
            if (size < 3)
                break;
            *(dest++) = (uint8)((bits >> 16) & 0xff);
            *(dest++) = (uint8)((bits >> 8) & 0xff);
            *(dest++) = (uint8)((bits & 0xff));
            bits = 1;
            size -= 3;
            count += 3;
        }
    }
    return count;
}

const char * CxStringC::pos(const char *cp, ssize_t offset)
{
    if(!cp)
        return NULL;

    size_t len = strlen(cp);
    if(!len)
        return cp;

    if(offset >= 0) {
        if((size_t)offset > len)
            offset = len;
        return cp + offset;
    }

    offset = -offset;
    if((size_t)offset >= len)
        return cp;

    return cp + len - offset;
}

/**
 * --- c string end --------------------------------------------------------
 */

