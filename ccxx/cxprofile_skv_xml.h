#ifndef CXPROFILE_SKV_XML_H
#define CXPROFILE_SKV_XML_H


#include "cxprofile_skv.h"


class CxSkverXml : public CxSkverBase
{
public:
    CxSkverXml();

    CxSkverXml( const std::string & sFilePath );

    ~CxSkverXml();

    int dealLoad(int iLength, const char * pData);

protected:
    bool loadImpl(std::map<std::string, std::map<std::string, std::string> > & sectionEntryValues, const std::string& sFilePath);

    bool saveImpl(const std::map<std::string, std::map<std::string, std::string> > & sectionEntryValues, const std::string& sFilePath, bool bRenew = true) const ;

private:
    bool loadXmlNormal(std::map<std::string, std::map<std::string, std::string> > & sectionEntryValues, const std::string& sFilePath);

    bool saveXmlNormal(const std::map<std::string, std::map<std::string, std::string> > & sectionEntryValues, const std::string& sFilePath, bool bRenew = true) const ;

private:
    std::map<std::string, std::map<std::string, std::string> > * oSectionEntryValues;
    int _dealState;
    int _dealType;
    char _end;
    char _textData[1024];
    char * _text;
    std::string _currentSection;
    std::string _currentEntry;
    std::string _currentValue;

};

class CxSkverXmlFactory : public CxSkverFactoryBase
{
public:
    std::string factoryName()
    {
        return ".xml";
    }

    CxSkverBase * createObject()
    {
        return new CxSkverXml();
    }

};


#endif // CXPROFILE_SKV_XML_H
