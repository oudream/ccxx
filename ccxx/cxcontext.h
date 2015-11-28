#ifndef CXCONTEXT_H
#define CXCONTEXT_H

#include "cxglobal.h"

// CxIGetSkv : special get value
class CxIGetSkv
{

public:
    uint getValue(const std::string& sEntry, uint iDefaultValue) const { return getValue(sEntry, (int)iDefaultValue ); }
    uint getValue(const std::string& sSection, const std::string& sEntry, uint iDefaultValue) const { return getValue(sSection, sEntry, (int)iDefaultValue ); }

    virtual int getValue(const std::string& sEntry, int iDefaultValue) const = 0;
    virtual bool getValue(const std::string& sEntry, bool bDefaultValue) const = 0;
    virtual double getValue(const std::string& sEntry, double dDefaultValue) const = 0;
    virtual msepoch_t getValue(const std::string& sEntry, const msepoch_t& dtDefaultValue) const = 0;
    virtual std::string getValue(const std::string& sEntry, const std::string& sDefaultValue) const = 0;

    virtual int getValue(const std::string& sSection, const std::string& sEntry, int iDefaultValue) const = 0;
    virtual bool getValue(const std::string& sSection, const std::string& sEntry, bool bDefaultValue) const = 0;
    virtual double getValue(const std::string& sSection, const std::string& sEntry, double dDefaultValue) const = 0;
    virtual msepoch_t getValue(const std::string& sSection, const std::string& sEntry, const msepoch_t& dtDefaultValue) const = 0;
    virtual std::string getValue(const std::string& sSection, const std::string& sEntry, const std::string& sDefaultValue) const = 0;

    virtual bool hasSection(const std::string& sSection) const = 0;
    virtual bool hasEntry(const std::string& sSection, const std::string& sEntry) const = 0;
    virtual bool hasEntry(const std::string& sEntry) const = 0;

};


// CxISetSkv : special set value
class CxISetSkv
{

public:
    inline void setValue(const std::string& sEntry, uint iValue) { setValue(sEntry, (int)iValue); }
    inline void setValue(const std::string& sSection, const std::string& sEntry, uint iValue) { setValue(sSection, sEntry, (int)iValue); }

    virtual void setValue(const std::string& sEntry, int iValue) =0;
    virtual void setValue(const std::string& sEntry, bool bValue) =0;
    virtual void setValue(const std::string& sEntry, double dValue) =0;
    virtual void setValue(const std::string& sEntry, msepoch_t dtValue) =0;
    virtual void setValue(const std::string& sEntry, std::string sValue) =0;

    virtual void setValue(const std::string& sSection, const std::string& sEntry, int iValue) =0;
    virtual void setValue(const std::string& sSection, const std::string& sEntry, bool bValue) =0;
    virtual void setValue(const std::string& sSection, const std::string& sEntry, double dValue) =0;
    virtual void setValue(const std::string& sSection, const std::string& sEntry, msepoch_t dtValue) =0;
    virtual void setValue(const std::string& sSection, const std::string& sEntry, std::string sValue) =0;

};


class CxGetSkvMap : public CxIGetSkv
{
public:
    CxGetSkvMap(const std::map<std::string, std::string> & sEntryValues);

    int getValue(const std::string& sEntry, int iDefaultValue) const ;
    bool getValue(const std::string& sEntry, bool bDefaultValue) const ;
    double getValue(const std::string& sEntry, double dDefaultValue) const ;
    msepoch_t getValue(const std::string& sEntry, const msepoch_t& dtDefaultValue) const ;
    std::string getValue(const std::string& sEntry, const std::string& sDefaultValue) const ;

    int getValue(const std::string& sSection, const std::string& sEntry, int iDefaultValue) const { return iDefaultValue; }
    bool getValue(const std::string& sSection, const std::string& sEntry, bool bDefaultValue) const { return bDefaultValue; }
    double getValue(const std::string& sSection, const std::string& sEntry, double dDefaultValue) const { return dDefaultValue; }
    msepoch_t getValue(const std::string& sSection, const std::string& sEntry, const msepoch_t& dtDefaultValue) const { return dtDefaultValue; }
    std::string getValue(const std::string& sSection, const std::string& sEntry, const std::string& sDefaultValue) const { return sDefaultValue; }

    bool hasSection(const std::string& sSection) const { return false; }
    bool hasEntry(const std::string& sSection, const std::string& sEntry) const { return false; }
    bool hasEntry(const std::string& sEntry) const ;

private:
    std::map<std::string, std::string> _defaultEntryValues;

};


#endif // CXCONTEXT_H
