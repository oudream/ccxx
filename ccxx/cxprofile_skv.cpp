#include "cxprofile_skv.h"


using namespace std;

const map<string, string> C_MapStringStringNULL;


//*************************** ContextBase begin **************************************************************

CxSkv::CxSkv()
{
    _defaultEntryValues = NULL;
}

CxSkv::~CxSkv()
{
}

///*** *** *** GetValue2 *** *** ***

int CxSkv::getValue(const string& sEntry, int iDefaultValue) const
{
    if (_defaultEntryValues)
    {
        map<string, string>::const_iterator it = _defaultEntryValues->find(sEntry);
        if (it != _defaultEntryValues->end())
        {
            return CxString::fromString(it->second, iDefaultValue);
        }
    }
    return iDefaultValue;
}

bool CxSkv::getValue(const string& sEntry, bool bDefaultValue) const
{
    if (_defaultEntryValues)
    {
        map<string, string>::const_iterator it = _defaultEntryValues->find(sEntry);
        if (it != _defaultEntryValues->end())
        {
            return CxString::fromString(it->second, bDefaultValue);
        }
    }
    return bDefaultValue;
}

double CxSkv::getValue(const string& sEntry, double dDefaultValue) const
{
    if (_defaultEntryValues)
    {
        map<string, string>::const_iterator it = _defaultEntryValues->find(sEntry);
        if (it != _defaultEntryValues->end())
        {
            return CxString::fromString(it->second, dDefaultValue);
        }
    }
    return dDefaultValue;
}

msepoch_t CxSkv::getValue(const string& sEntry, const msepoch_t& dtDefaultValue) const
{
    if (_defaultEntryValues)
    {
        map<string, string>::const_iterator it = _defaultEntryValues->find(sEntry);
        if (it != _defaultEntryValues->end())
        {
            return CxString::fromString(it->second, dtDefaultValue);
        }
    }
    return dtDefaultValue;
}

string CxSkv::getValue(const string& sEntry, const string& sDefaultValue) const
{
    if (_defaultEntryValues)
    {
        map<string, string>::const_iterator it = _defaultEntryValues->find(sEntry);
        if (it != _defaultEntryValues->end())
        {
            return it->second;
        }
    }
    return sDefaultValue;
}


///--- --- --- GetValue2 --- --- ---


///*** *** *** GetValue3 *** *** ***

int CxSkv::getValue(const string& sSection, const string& sEntry, int iDefaultValue) const
{
    bool ok;
    const map<string, string> & kvs = getEntryValues(sSection, &ok);
    if (ok)
    {
        map<string, string>::const_iterator it = kvs.find(sEntry);
        if (it != kvs.end())
        {
            return CxString::fromString(it->second, iDefaultValue);
        }
    }
    return iDefaultValue;
}

bool CxSkv::getValue(const string& sSection, const string& sEntry, bool bDefaultValue) const
{
    bool ok;
    const map<string, string> & kvs = getEntryValues(sSection, &ok);
    if (ok)
    {
        map<string, string>::const_iterator it = kvs.find(sEntry);
        if (it != kvs.end())
        {
            return CxString::fromString(it->second, bDefaultValue);
        }
    }
    return bDefaultValue;
}

double CxSkv::getValue(const string& sSection, const string& sEntry, double dDefaultValue) const
{
    bool ok;
    const map<string, string> & kvs = getEntryValues(sSection, &ok);
    if (ok)
    {
        map<string, string>::const_iterator it = kvs.find(sEntry);
        if (it != kvs.end())
        {
            return CxString::fromString(it->second, dDefaultValue);
        }
    }
    return dDefaultValue;
}

msepoch_t CxSkv::getValue(const string& sSection, const string& sEntry, const msepoch_t& dtDefaultValue) const
{
    bool ok;
    const map<string, string> & kvs = getEntryValues(sSection, &ok);
    if (ok)
    {
        map<string, string>::const_iterator it = kvs.find(sEntry);
        if (it != kvs.end())
        {
            return CxString::fromString(it->second, dtDefaultValue);
        }
    }
    return dtDefaultValue;
}

string CxSkv::getValue(const string& sSection, const string& sEntry, const string& sDefaultValue) const
{
    bool ok;
    const map<string, string> & kvs = getEntryValues(sSection, &ok);
    if (ok)
    {
        map<string, string>::const_iterator it = kvs.find(sEntry);
        if (it != kvs.end())
        {
            return it->second;
        }
    }
    return sDefaultValue;
}

///--- --- --- GetValue3 --- --- ---


///*** *** *** SetValue2 *** *** ***

void CxSkv::setValue(const string& sEntry, int iValue)
{
    if (_defaultEntryValues)
    {
        (*_defaultEntryValues)[sEntry] = CxString::toString(iValue);
    }
}

void CxSkv::setValue(const string& sEntry, bool bValue){
    if (_defaultEntryValues)
    {
        (*_defaultEntryValues)[sEntry] = CxString::toString(bValue);
    }
}

void CxSkv::setValue(const string& sEntry, double dValue){
    if (_defaultEntryValues)
    {
        (*_defaultEntryValues)[sEntry] = CxString::toString(dValue);
    }
}

void CxSkv::setValue(const string& sEntry, msepoch_t dtValue){
    if (_defaultEntryValues)
    {
        (*_defaultEntryValues)[sEntry] = CxString::toString(dtValue);
    }
}

void CxSkv::setValue(const string& sEntry, string sValue){
    if (_defaultEntryValues)
    {
        (*_defaultEntryValues)[sEntry] = sValue;
    }
}

///--- --- --- SetValue2 --- --- ---


///*** *** *** SetValue3 *** *** ***

void CxSkv::setValue(const string& sSection, const string& sEntry, int iValue)
{
    setValue(sSection, sEntry, CxString::toString(iValue));
}

void CxSkv::setValue(const string& sSection, const string& sEntry, bool bValue){
    setValue(sSection, sEntry, CxString::toString(bValue));
}

void CxSkv::setValue(const string& sSection, const string& sEntry, double dValue){
    setValue(sSection, sEntry, CxString::toString(dValue));
}

void CxSkv::setValue(const string& sSection, const string& sEntry, msepoch_t dtValue){
    setValue(sSection, sEntry, CxString::toString(dtValue));
}

void CxSkv::setValue(const string& sSection, const string& sEntry, string sValue){
    map<string, string>* entryValues = findEntryValues(sSection);
    if (entryValues)
    {
        (*entryValues)[sEntry] = sValue;
    }
    else
    {
        map<string, string> entrys;
        entrys[sEntry] = sValue;
        _sectionEntryValues[sSection] = entrys;
    }
}

///--- --- --- SetValue3 --- --- ---


bool CxSkv::hasEntry(const string& sSection, const string& sEntry) const
{
    bool ok;
    const map<string, string>& kvs = getEntryValues(sSection, &ok);
    if (ok)
    {
        return kvs.find(sEntry) != kvs.end();
    }
    else
    {
        return false;
    }
}

bool CxSkv::hasEntry(const string &sEntry) const
{
    if (_defaultEntryValues)
    {
        return _defaultEntryValues->find(sEntry) != _defaultEntryValues->end();
    }
    return false;
}

bool CxSkv::hasSection(const string& sSection) const
{
    map<string, map<string, string> >::const_iterator it = _sectionEntryValues.find(sSection);
    return it != _sectionEntryValues.end();
}

void CxSkv::removeEntry(const string& sSection, const string& sEntry)
{
    map<string, string>* entryValues = findEntryValues(sSection);
    if (entryValues)
    {
        entryValues->erase(sEntry);
    }
}

void CxSkv::removeSection(const string& sSection)
{
    _sectionEntryValues.erase(sSection);
}

int _compara_(const string & s1, const string & s2)
{
    return s1 < s2;
}

vector<string> CxSkv::sectionNames(int iSort) const
{
    vector<string> sectionNames;
    for (map<string, map<string, string> >::const_iterator it = _sectionEntryValues.begin(); it != _sectionEntryValues.end(); ++it)
    {
        sectionNames.push_back(it->first);
    }
    if (iSort > 0)
        CxContainer::sort(sectionNames, true);
    else if (iSort < 0)
        CxContainer::sort(sectionNames, false);
    return sectionNames;
}

vector<string> CxSkv::entryNames(const string& sSection) const
{
    vector<string> entryNames;
    bool ok;
    const map<string, string>& kvs = getEntryValues(sSection, &ok);
    if (ok)
    {
        for (map<string, string>::const_iterator it = kvs.begin(); it != kvs.end(); ++it)
        {
            entryNames.push_back(it->first);
        }
    }
    return entryNames;
}

const map<string, string> & CxSkv::getEntryValues(const string &sSection, bool *ok) const
{
    map<string, map<string, string> >::const_iterator it = _sectionEntryValues.find(sSection);
    if ( it != _sectionEntryValues.end() )
    {
        if (ok) *ok = true;
        return it->second;
    } else {
        if (ok) *ok = false;
        return C_MapStringStringNULL;
    }
}

map<string, string> * CxSkv::findEntryValues(const string &sSection)
{
    map<string, map<string, string> >::iterator it = _sectionEntryValues.find(sSection);
    if ( it != _sectionEntryValues.end() )
    {
        return &(it->second);
    }
    return NULL;
}

void CxSkv::appendEntryValues(const string &sSection, const map<string, string> &entryValues)
{
    map<string, string> * oKvs = findEntryValues(sSection);
    if (oKvs)
    {
        oKvs->insert(entryValues.begin(), entryValues.end());
    }
    else
    {
        _sectionEntryValues[sSection] = entryValues;
    }
}

void CxSkv::updateSectionEntryValues(const std::map<string, std::map<string, string> > &sectionEntryValues)
{
    for (map<string, map<string, string> >::const_iterator itSkv = sectionEntryValues.begin(); itSkv != sectionEntryValues.end(); ++itSkv)
    {
        const string & sSection = itSkv->first;
        const map<string, string> & sKvs = itSkv->second;
        upateEntryValues(sSection, sKvs);
    }
}

int CxSkv::upateEntryValues(const string &sSection, const std::map<string, string> &entryValues)
{
    map<string, string> * oKvs = findEntryValues(sSection);
    if (oKvs)
    {
        CxContainer::update(*oKvs, entryValues);
        return TRUE;
    }
    else
    {
        _sectionEntryValues[sSection] = entryValues;
        return FALSE;
    }
}

void CxSkv::prepareGet(const string & sSection)
{
    _defaultEntryValues = findEntryValues(sSection);
}

void CxSkv::prepareSet(const string & sSection)
{
    map<string, string>* entryValues = findEntryValues(sSection);
    if (entryValues) {
        _defaultEntryValues = entryValues;
        _defaultSectionIsNew = false;
    } else {
        map<string, string> entrys;
        map<string, map<string, string> >::iterator it = _sectionEntryValues.insert(_sectionEntryValues.end(), std::pair<string, map<string, string> >(sSection, entrys));
        _defaultEntryValues = &(it->second);
        _defaultSectionIsNew = true;
    }
}

//--------------------------- ContextBase end --------------------------------------------------------------



//*************************** CxSkverBase begin **************************************************************

CxSkverBase::CxSkverBase()
{
    m_useCount = 0;
    m_useSaveCount = 0;
}

CxSkverBase::~CxSkverBase()
{
}

bool CxSkverBase::load(const std::string & sFilePath)
{
    if (! CxFileSystem::isExist(sFilePath))
        return false;
    if ( loadImpl( _sectionEntryValues, sFilePath ) )
    {
        _filePath = sFilePath;
        _fileName = CxFileSystem::extractFileName(sFilePath);
        _fileSuffixName = CxFileSystem::extractFileSuffixName(sFilePath);
        _path = CxFileSystem::extractPath(sFilePath);
        return true;
    }
    else
    {
        return false;
    }
}

bool CxSkverBase::save(const string &sFilePath)
{
    string sSaveFilePath = sFilePath;
    if (sSaveFilePath.empty())
    {
        if (_filePath.empty())
        {
            sSaveFilePath = _defaultSaveFilePath;
        }
        else
        {
            sSaveFilePath = _filePath;
        }
    }
    string sSavePath = CxFileSystem::extractPath(sSaveFilePath);
    if (! CxFileSystem::isExist(sSavePath))
    {
        CxFileSystem::createDirMultiLevel(sSavePath);
    }
    return saveImpl( _sectionEntryValues, sSaveFilePath );
}

//--------------------------- CxSkverBase end --------------------------------------------------------------



//*************************** CxSkverManager begin **************************************************************

vector<CxSkverBase*> f_skvers;

const vector<CxSkverBase*>& CxSkverManager::skvers()
{
    return f_skvers;
}

CxSkverBase* CxSkverManager::beginUse(const string& sFilePath, bool bLoad)
{
    CxSkverBase * oProfile = skver(sFilePath);
    if (! oProfile)
    {
        string sSuffixName = CxFileSystem::extractFileSuffixName(sFilePath);
        oProfile = CxSkverFactoryManager::createObject(sSuffixName);
        if (! oProfile)
        {
            return NULL;
        }
        oProfile->setDefaultSaveFilePath(sFilePath);
        if (bLoad)
        {
            oProfile->load(sFilePath);
        }
        f_skvers.push_back(oProfile);
    }
    oProfile->useBegin();
    return oProfile;
}

CxSkverBase *CxSkverManager::beginUse(const string &sFilePath, const string &sDataType, const string &sDataParam, bool bLoad)
{
    CxSkverBase * oProfile = skver(sFilePath);
    if (! oProfile)
    {
        string sSuffixName = CxFileSystem::extractFileSuffixName(sFilePath);
        oProfile = CxSkverFactoryManager::createObject(sSuffixName);
        if (! oProfile)
        {
            return NULL;
        }
        oProfile->setSkvType(sDataType);
        oProfile->setSkvParam(sDataParam);
        oProfile->setDefaultSaveFilePath(sFilePath);
        if (bLoad)
        {
            oProfile->load(sFilePath);
        }
        f_skvers.push_back(oProfile);
    }
    oProfile->useBegin();
    return oProfile;
}

int CxSkverManager::endUse(CxSkverBase * oProfile, bool bSave)
{
    int r = 0;
    oProfile->useEnd(bSave);
    if (oProfile->useCount() <= 0)
    {
        if (oProfile->useSaveCount()>0)
        {
            r = oProfile->save();
        }
        CxContainer::remove(f_skvers, oProfile);
        delete oProfile;
    }
    return r;
}

void CxSkverManager::closeDeleteAll(){
    for(vector<CxSkverBase*>::iterator it = f_skvers.begin(); it != f_skvers.end(); ++it)
    {
        CxSkverBase * o = * it;
        if (o->useSaveCount() > 0)
            o->save();
    }
    CxContainer::deleteAndclear(f_skvers);
}

CxSkverBase* CxSkverManager::skver(const string &sFilePath)
{
    for (size_t i = 0; i < f_skvers.size(); ++i)
    {
        CxSkverBase * oSkver = f_skvers.at(i);
        if (oSkver->filePath() == sFilePath || oSkver->defaultSaveFilePath() == sFilePath)
            return oSkver;
    }
    return NULL;
//    GM_LISTO_FIND(f_skvers, CxSkverBase, filePath(), sFilePath);
}

int CxSkverManager::load(std::map<string, std::map<string, string> > &sectionEntryValues, const string &sFilePath)
{
    string sSuffixName = CxFileSystem::extractFileSuffixName(sFilePath);
    CxSkverBase * oProfile = CxSkverFactoryManager::createObject(sSuffixName);
    if (oProfile)
    {
        oProfile->loadImpl(sectionEntryValues, sFilePath);
        delete oProfile;
        return sectionEntryValues.size();
    }
    return 0;
}

int CxSkverManager::save(const std::map<string, std::map<string, string> > &sectionEntryValues, const string &sFilePath, bool bRenew)
{
    string sSuffixName = CxFileSystem::extractFileSuffixName(sFilePath);
    CxSkverBase * oProfile = CxSkverFactoryManager::createObject(sSuffixName);
    if (oProfile)
    {
        bool bSuccess = oProfile->saveImpl(sectionEntryValues, sFilePath, bRenew);
        delete oProfile;
        if (bSuccess)
        {
            return sectionEntryValues.size();
        }
    }
    return 0;
}

//--------------------------- CxSkverManager end --------------------------------------------------------------


