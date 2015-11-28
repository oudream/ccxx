#ifndef CXPROFILE_SKV_TXT_H
#define CXPROFILE_SKV_TXT_H

#include "cxprofile_skv.h"

class CxSkverTxt : public CxSkverBase
{
public:
    CxSkverTxt();

    CxSkverTxt( const std::string & sFilePath );

    ~CxSkverTxt();


protected:
    bool loadImpl(std::map<std::string, std::map<std::string, std::string> > & sectionEntryValues, const std::string& sFilePath);

    bool saveImpl(const std::map<std::string, std::map<std::string, std::string> > & sectionEntryValues, const std::string& sFilePath, bool bRenew = true) const;

private:
    std::string _currentSection;
    std::string _currentEntry;
    std::string _currentValue;


};

class CxSkverTxtFactory : public CxSkverFactoryBase
{
public:
    std::string factoryName()
    {
        return ".txt";
    }

    CxSkverBase * createObject()
    {
        return new CxSkverTxt();
    }

//private:
//    static CxSkverTxtFactory * s_instance;

//    static CxSkverTxtFactory * createSingleton()
//    {
//        CxSkverFactoryManager::createAndRegister<CxSkverTxtFactory>();
//        static CxSkverTxtFactory mSingleton;
//        return & mSingleton;
//    }

};

#endif // CXPROFILE_SKV_TXT_H
