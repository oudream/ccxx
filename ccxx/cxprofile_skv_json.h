#ifndef CXPROFILE_SKV_JSON_H
#define CXPROFILE_SKV_JSON_H

#include "cxprofile.h"

class GM_CCXX_CORE_API CxSkverJson : public CxSkverBase
{
public:
    CxSkverJson();

    CxSkverJson(const std::map<std::string, std::map<std::string, std::string> > & sectionEntryValues);

    CxSkverJson( const std::string & sFilePath );

    ~CxSkverJson();

protected:
    bool loadImpl(std::map<std::string, std::map<std::string, std::string> > & sectionEntryValues, const std::string& sFilePath);

    bool saveImpl(const std::map<std::string, std::map<std::string, std::string> > & sectionEntryValues, const std::string& sFilePath, bool bRenew = true) const ;

};

class GM_CCXX_CORE_API CxSkverJsonFactory : public CxSkverFactoryBase
{
public:
    std::string factoryName()
    {
        return ".json";
    }

    CxSkverBase * createObject()
    {
        return new CxSkverJson();
    }

};

#endif // CXPROFILE_SKV_JSON_H
