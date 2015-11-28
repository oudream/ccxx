#ifndef CXPROFILE_SKV_INI_H
#define CXPROFILE_SKV_INI_H

#include "cxprofile_skv.h"

class CxSkverIni : public CxSkverBase
{
public:
    CxSkverIni();

    CxSkverIni( const std::string & sFilePath );

    ~CxSkverIni();

    int dealLoad(int iLength, const char * pData);

protected:
    bool loadImpl(std::map<std::string, std::map<std::string, std::string> > & sectionEntryValues, const std::string& sFilePath);

    bool saveImpl(const std::map<std::string, std::map<std::string, std::string> > & sectionEntryValues, const std::string& sFilePath, bool bRenew = true) const ;

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

class CxSkverIniFactory : public CxSkverFactoryBase
{
public:
    std::string factoryName()
    {
        return ".ini";
    }

    CxSkverBase * createObject()
    {
        return new CxSkverIni();
    }

//private:
//    static CxSkverIniFactory* s_instance;

//    static CxSkverIniFactory * createSingleton()
//    {
//        CxSkverFactoryManager::createAndRegister<CxSkverIniFactory>();
//        static CxSkverIniFactory mSingleton;
//        return & mSingleton;
//    }

};

#endif // CXPROFILE_SKV_INI_H
