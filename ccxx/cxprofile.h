#ifndef CXPROFILE_H
#define CXPROFILE_H

#include "cxglobal.h"
#include "cxcontext.h"
#include "cxstring.h"
#include "cxfile.h"
#include "cxcontainer.h"

// skv == section key value
// loo == line object overload / override

class CxProfileLoo
{
public:
};


// skv == section key value
//attention : do not support multi thread share one CxSkver

class CxSkverBase;


// CxSkvBase : to set value to cache
class GM_CCXX_CORE_API CxSkv : public CxIGetSkv, public CxISetSkv
{
public:
    CxSkv();

    CxSkv(const std::map<std::string, std::map<std::string, std::string> > & sectionEntryValues);

    virtual ~CxSkv();

    ///*** *** *** get value by key *** *** ***

    void prepareGet(const std::string & sSection);

    int getValue(const std::string& sEntry, int iDefaultValue) const ;

    bool getValue(const std::string& sEntry, bool bDefaultValue) const ;

    double getValue(const std::string& sEntry, double dDefaultValue) const ;

    ///attention error with : msepoch_t l = getValue(sSection, l);
    msepoch_t getValue(const std::string& sEntry, const msepoch_t& dtDefaultValue) const ;

    ///attention error with : std::string s = getValue(sEntry, s);
    std::string getValue(const std::string& sEntry, const std::string& sDefaultValue) const ;

    ///--- --- --- end --- --- ---


    ///*** *** *** get value by section and key *** *** ***

    int getValue(const std::string& sSection, const std::string& sEntry, int iDefaultValue) const ;

    bool getValue(const std::string& sSection, const std::string& sEntry, bool bDefaultValue) const ;

    double getValue(const std::string& sSection, const std::string& sEntry, double dDefaultValue) const ;

    ///attention error with : std::string s = getValue(sSection, sEntry, s);
    std::string getValue(const std::string& sSection, const std::string& sEntry, const std::string& sDefaultValue) const ;

    ///attention error with : msepoch_t l = getValue(sSection, sEntry, l);
    msepoch_t getValue(const std::string& sSection, const std::string& sEntry, const msepoch_t& dtDefaultValue) const ;

    ///--- --- --- end --- --- ---


    ///*** *** *** set value by key *** *** ***

    void prepareSet(const std::string & sSection);

    void setValue(const std::string& sEntry, int iValue);

    void setValue(const std::string& sEntry, bool bValue);

    void setValue(const std::string& sEntry, double dValue);

    void setValue(const std::string& sEntry, msepoch_t dtValue);

    void setValue(const std::string& sEntry, std::string sValue);

    ///--- --- --- end --- --- ---


    ///*** *** *** set value by setion and key *** *** ***

    void setValue(const std::string& sSection, const std::string& sEntry, int iValue);

    void setValue(const std::string& sSection, const std::string& sEntry, bool bValue);

    void setValue(const std::string& sSection, const std::string& sEntry, double dValue);

    void setValue(const std::string& sSection, const std::string& sEntry, msepoch_t dtValue);

    void setValue(const std::string& sSection, const std::string& sEntry, std::string sValue);

    ///--- --- --- end --- --- ---

    inline size_t sectionCount() const { return _sectionEntryValues.size(); }

    inline bool empty() const { return _sectionEntryValues.size() == 0; }

    bool hasSection(const std::string& sSection) const;

    bool hasEntry(const std::string& sSection, const std::string& sEntry) const ;

    bool hasEntry(const std::string& sEntry) const ;

    void removeSection(const std::string& sSection);

    void removeEntry(const std::string& sSection, const std::string& sEntry);

    std::vector<std::string> sectionNames(int iSort = 0) const;

    std::vector<std::string> entryNames(const std::string& sSection) const;

    const std::map<std::string, std::string> & getEntryValues(const std::string& sSection, bool * ok = NULL) const;

    inline const std::map<std::string, std::map<std::string, std::string> > & getSectionEntryValues() const { return _sectionEntryValues; }

    void appendEntryValues(const std::string& sSection, const std::map<std::string, std::string>& entryValues);

    void updateSectionEntryValues(const std::map<std::string, std::map<std::string, std::string> > & sectionEntryValues);

    int upateEntryValues(const std::string & sSection, const std::map<std::string, std::string> & entryValues);

protected:
    std::map<std::string, std::string> * findEntryValues(const std::string& sSection) ;

    std::map<std::string, std::map<std::string, std::string> > _sectionEntryValues;

    std::map<std::string, std::string> * _defaultEntryValues;

};


class CxSkverFactoryBase : public CxFactoryTemplate<CxSkverBase>
{

};


class GM_CCXX_CORE_API CxSkverBase : public CxSkv
{
public:
    static std::vector<CxFactoryTemplate<CxSkverBase>*>* factoriesContainer();

    static void factoriesCreateAndRegister();

public:
    CxSkverBase();

    CxSkverBase(const std::map<std::string, std::map<std::string, std::string> > & sectionEntryValues);

    ~CxSkverBase();

    bool load(const std::string & sFilePath);

    inline bool hadLoaded() const { return _filePath.empty(); }

    bool save(const std::string & sFilePath = std::string());

    inline const std::string & skvType() const { return _skvType; }
    inline void setSkvType(const std::string & value) { _skvType = value; }

    inline const std::string & skvPath() const { return _skvPath; }
    inline void setSkvPath(const std::string & value) { _skvPath = value; }

    inline const std::string & skvName() const { return _skvName; }
    inline void setSkvName(const std::string & value) { _skvName = value; }

    inline const std::string & skvParam() const { return _skvParam; }
    inline void setSkvParam(const std::string & value) { _skvParam = value; }

    inline const std::string & filePath() const { return _filePath; }

    inline const std::string & fileName() const { return _fileName; }

    inline const std::string & path() const { return _path; }

    inline const std::string & fileSuffixName() const { return _fileSuffixName; }

    inline std::string defaultSaveFilePath() { return _defaultSaveFilePath; }

    inline void setDefaultSaveFilePath(const std::string & value) { _defaultSaveFilePath = value; }

protected:
    virtual bool loadImpl(std::map<std::string, std::map<std::string, std::string> > & sectionEntryValues, const std::string& sFilePath) = 0;

    virtual bool saveImpl(const std::map<std::string, std::map<std::string, std::string> > & sectionEntryValues, const std::string& sFilePath, bool bRenew = true) const = 0;

private:
    inline int useCount() const { return m_useCount; }

    inline int useSaveCount() const { return m_useSaveCount; }

    inline void useBegin() { m_useCount++; }

    inline void useEnd(bool bSave) { m_useCount--; if (bSave) m_useSaveCount++; }

    friend class CxSkverManager;

private:
    std::string _skvType;
    std::string _skvPath;
    std::string _skvName;
    std::string _skvParam;
    std::string _filePath;
    std::string _path;
    std::string _fileName;
    std::string _fileSuffixName;
    std::string _defaultSaveFilePath;
    int m_useCount;
    int m_useSaveCount;

};


class CxSkverFactoryManager : public CxFactoryManagerTemplate<CxSkverBase>
{
};


class GM_CCXX_CORE_API CxSkverManager
{

public:
    static const std::vector<CxSkverBase*>& skvers();

    static CxSkverBase* beginUse(const std::string& sFilePath, bool bLoad = true);

    static CxSkverBase* beginUse(const std::string& sFilePath, const std::string& sDataType, const std::string& sDataParam, bool bLoad = true);

    static int endUse(CxSkverBase * oProfile, bool bSave = false);

    static void closeDeleteAll();

    static CxSkverBase* skver(const std::string& sFilePath);

    static int load(std::map<std::string, std::map<std::string, std::string> > & sectionEntryValues, const std::string& sFilePath);

    static int save(const std::map<std::string, std::map<std::string, std::string> > & sectionEntryValues, const std::string& sFilePath, bool bRenew = true);

    //fromContext(const CxISetSkv &)
    template<class ObjectDataBase>
    static std::vector<ObjectDataBase*> loadObjects(const std::string& sFilePath)
    {
        std::vector<ObjectDataBase*> objectList;
        CxSkverBase * oProfile = CxSkverManager::beginUse(sFilePath, true);
        if (oProfile) {
            std::vector<std::string> oSections = oProfile->sectionNames();
            for (size_t i = 0; i < oSections.size(); ++i)
            {
                ObjectDataBase* oObject = new ObjectDataBase();
                oProfile->prepareGet(oSections.at(i));
                oObject->fromContext( *oProfile );
                objectList.push_back( oObject );
            }
            CxSkverManager::endUse(oProfile, false);
        }
        return objectList;
    }

    //toContext(CxISetSkv &)
    template<class ObjectDataBase>
    static void saveObjects(const std::string& sFilePath, std::vector<ObjectDataBase*>* oObjectList)
    {
        if ( ! CxFileSystem::isExist(sFilePath) ) {
            if ( ! CxFileSystem::createFile(sFilePath) )
                return;
        }
        CxSkverBase * oProfile = CxSkverManager::beginUse(sFilePath, false);
        if (oProfile){
            for (size_t i = 0; i < oObjectList->size(); ++i) {
                ObjectDataBase* oObject = oObjectList->at(i);
                oProfile->prepareSet(CxString::format("%06d",i));
                oObject->toContext( *oProfile );
            }
            CxSkverManager::endUse(oProfile, true);
        }
    }

    //fromContext(const CxISetSkv &)
    template<class StructDataBase>
    static std::vector<StructDataBase> loadStructs(const std::string& sFilePath)
    {
        std::vector<StructDataBase> mStructList;
        CxSkverBase * oProfile = CxSkverManager::beginUse(sFilePath, true);
        if (oProfile) {
            std::vector<std::string> oSections = oProfile->sectionNames();
            for (size_t i = 0; i < oSections.size(); ++i)
            {
                StructDataBase m;
                oProfile->prepareGet(oSections.at(i));
                m.fromContext( *oProfile );
                mStructList.push_back( m );
            }
            CxSkverManager::endUse(oProfile, false);
        }
        return mStructList;
    }

    //toContext(CxISetSkv &)
    template<class StructDataBase>
    static void saveStructs(const std::string& sFilePath, const std::vector<StructDataBase> & mStructList)
    {
        if ( ! CxFileSystem::isExist(sFilePath) )
        {
            if ( ! CxFileSystem::createFile(sFilePath) )
                return;
        }
        CxSkverBase * oProfile = CxSkverManager::beginUse(sFilePath, false);
        if (oProfile)
        {
            for (size_t i = 0; i < mStructList.size(); ++i)
            {
                const StructDataBase & m = mStructList.at(i);
                oProfile->prepareSet(CxString::format("%06d",i));
                m.toContext( *oProfile );
            }
            CxSkverManager::endUse(oProfile, true);
        }
    }

};




class GM_CCXX_CORE_API CxSkverTxt : public CxSkverBase
{
public:
    CxSkverTxt();

    CxSkverTxt(const std::map<std::string, std::map<std::string, std::string> > & sectionEntryValues);

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

class GM_CCXX_CORE_API CxSkverTxtFactory : public CxSkverFactoryBase
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




class GM_CCXX_CORE_API CxSkverIni : public CxSkverBase
{
public:
    static std::vector<std::vector<std::string> > split(const std::vector<std::string> & ss, char cHeadCharacter, char cIgnore);

public:
    CxSkverIni();

    CxSkverIni(const std::map<std::string, std::map<std::string, std::string> > & sectionEntryValues);

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

class GM_CCXX_CORE_API CxSkverIniFactory : public CxSkverFactoryBase
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

#endif // CXPROFILE_H
