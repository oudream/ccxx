#ifndef CXPROFILE_SKV_XML_H
#define CXPROFILE_SKV_XML_H

#include "cxprofile.h"

class GM_CCXX_CORE_API CxSkverXml : public CxSkverBase {
public:
    CxSkverXml();

    CxSkverXml(const std::map<std::string, std::map<std::string, std::string> > &sectionEntryValues);

    CxSkverXml(const std::string &sFilePath);

    ~CxSkverXml();

protected:
    bool
    loadImpl(std::map<std::string, std::map<std::string, std::string> > &sectionEntryValues, const std::string &sFilePath);

    bool
    saveImpl(const std::map<std::string, std::map<std::string, std::string> > &sectionEntryValues, const std::string &sFilePath, bool bRenew = true) const;

private:
    bool
    loadXmlNormal(std::map<std::string, std::map<std::string, std::string> > &sectionEntryValues, const std::string &sFilePath);

    bool
    saveXmlNormal(const std::map<std::string, std::map<std::string, std::string> > &sectionEntryValues, const std::string &sFilePath, bool bRenew = true) const;

private:
    std::map<std::string, std::map<std::string, std::string> > *oSectionEntryValues;
    int _dealState;
    int _dealType;
    char _end;
    char _textData[1024];
    char *_text;
    std::string _currentSection;
    std::string _currentEntry;
    std::string _currentValue;

};
//Cui 2017-02-21
class GM_CCXX_CORE_API CxSkverXmlFactory : public CxSkverFactoryBase {
public:
    std::string factoryName()
    {
        return ".xml";
    }

    CxSkverBase *createObject()
    {
        return new CxSkverXml();
    }

};

#endif // CXPROFILE_SKV_XML_H
