#include "cxcontext.h"

#include "cxtime.h"
#include "cxstring.h"

using namespace std;


CxGetSkvMap::CxGetSkvMap(const std::map<std::string, std::string> &sEntryValues)
{
    _defaultEntryValues = sEntryValues;
}

int CxGetSkvMap::getValue(const string& sEntry, int iDefaultValue) const
{
    map<string, string>::const_iterator it = _defaultEntryValues.find(sEntry);
    if (it != _defaultEntryValues.end())
    {
        return CxString::fromString(it->second, iDefaultValue);
    }
    return iDefaultValue;
}

bool CxGetSkvMap::getValue(const string& sEntry, bool bDefaultValue) const
{
    map<string, string>::const_iterator it = _defaultEntryValues.find(sEntry);
    if (it != _defaultEntryValues.end())
    {
        return CxString::fromString(it->second, bDefaultValue);
    }
    return bDefaultValue;
}

double CxGetSkvMap::getValue(const string& sEntry, double dDefaultValue) const
{
    map<string, string>::const_iterator it = _defaultEntryValues.find(sEntry);
    if (it != _defaultEntryValues.end())
    {
        return CxString::fromString(it->second, dDefaultValue);
    }
    return dDefaultValue;
}

msepoch_t CxGetSkvMap::getValue(const string& sEntry, const msepoch_t& dtDefaultValue) const
{
    msepoch_t r = dtDefaultValue;
    map<string, string>::const_iterator it = _defaultEntryValues.find(sEntry);
    if (it != _defaultEntryValues.end())
    {
        CxTime::fromString(it->second, r);
    }
    return r;
}

string CxGetSkvMap::getValue(const string& sEntry, const string& sDefaultValue) const
{
    map<string, string>::const_iterator it = _defaultEntryValues.find(sEntry);
    if (it != _defaultEntryValues.end())
    {
        return it->second;
    }
    return sDefaultValue;
}

bool CxGetSkvMap::hasEntry(const string &sEntry) const
{
    return _defaultEntryValues.find(sEntry) != _defaultEntryValues.end();
}

