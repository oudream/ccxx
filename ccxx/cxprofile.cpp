#include "cxprofile.h"

using namespace std;

const map<string, string> C_MapStringStringNULL;

//*************************** ContextBase begin **************************************************************

CxSkv::CxSkv()
{
    _defaultEntryValues = NULL;
}

CxSkv::CxSkv(const std::map<string, std::map<string, string> > & sectionEntryValues)
{
    _defaultEntryValues = NULL;
    _sectionEntryValues = sectionEntryValues;
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
    } else {
        map<string, string> entrys;
        map<string, map<string, string> >::iterator it = _sectionEntryValues.insert(_sectionEntryValues.end(), std::pair<string, map<string, string> >(sSection, entrys));
        _defaultEntryValues = &(it->second);
    }
}

//--------------------------- ContextBase end --------------------------------------------------------------



//*************************** CxSkverBase begin **************************************************************

CxSkverBase::CxSkverBase()
    :CxSkv()
{
    m_useCount = 0;
    m_useSaveCount = 0;
}

CxSkverBase::CxSkverBase(const std::map<std::string, std::map<std::string, std::string> > & sectionEntryValues)
    :CxSkv(sectionEntryValues)
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





















//CxSkverTxtFactory * CxSkverTxtFactory::s_instance = CxSkverTxtFactory::createSingleton();

CxSkverTxt::CxSkverTxt() :
    CxSkverBase()
{
}

CxSkverTxt::CxSkverTxt(const std::map<std::string, std::map<std::string, std::string> > & sectionEntryValues) :
    CxSkverBase(sectionEntryValues)
{
}

CxSkverTxt::CxSkverTxt(const string &sFilePath) :
    CxSkverBase()
{
    load(sFilePath);
}

CxSkverTxt::~CxSkverTxt()
{
}

bool CxSkverTxt::loadImpl(std::map<std::string, std::map<std::string, std::string> > & sectionEntryValues, const std::string& sFilePath)
{
    vector<string> sLines;
    if (CxFile::load(sFilePath, sLines, '\n'))
    {
        sectionEntryValues.clear();
        map<string, string> entrys;
        const char * pchEnd;
        char * pchIndex;
        char * pchFirst;
        size_t iRemain;
        for (size_t i = 0; i < sLines.size(); ++i)
        {
            const string& sLine = sLines.at(i);
            //sLine.end - 1;
            pchEnd = sLine.data() + sLine.size() - 1;
            pchIndex = (char*)memchr(sLine.data(), ',', sLine.size());
            if (pchIndex)
            {
                if (entrys.size() > 0 && _currentSection.size() > 0)
                {
                    sectionEntryValues[_currentSection] = entrys;
                }
                _currentSection = string(sLine.data(), pchIndex-sLine.data());
                pchFirst = pchIndex + 1;
                iRemain = pchEnd - pchIndex;
            }
            while (pchIndex != NULL)
            {
                pchIndex = (char*)memchr(pchIndex, '=', iRemain);
                if (pchIndex)
                {
                    _currentEntry = string(pchFirst, pchIndex-pchFirst);
                    pchFirst = pchIndex + 1;
                    iRemain = pchEnd - pchIndex;
                }
                else
                {
                    break;
                }
                pchIndex = (char*)memchr(pchIndex, ',', iRemain);
                if (pchIndex)
                {
                    _currentValue = string(pchFirst, pchIndex-pchFirst);
                    pchFirst = pchIndex + 1;
                    iRemain = pchEnd - pchIndex;
                }
                else
                {
                    _currentValue = string(pchFirst, pchEnd-pchFirst+1);
                }
                if (_currentSection.size() > 0 && _currentEntry.size() > 0)
                {
                    entrys[_currentEntry] = _currentValue;
                }
            }
        }
        if (entrys.size() > 0 && _currentSection.size() > 0)
        {
            sectionEntryValues[_currentSection] = entrys;
        }
        return true;
    }
    return false;
}

bool CxSkverTxt::saveImpl(const std::map<std::string, std::map<std::string, std::string> > & sectionEntryValues, const std::string &sFilePath, bool bRenew) const
{
    vector<string> sLines;
    for (map<string, map<string, string> >::const_iterator itSection = sectionEntryValues.begin(); itSection != sectionEntryValues.end(); ++itSection)
    {
        string sLine = CxString::newString(itSection->first);
        sLine.push_back(',');
        const map<string, string> & sEntrys = itSection->second;
        for (map<string, string>::const_iterator itEntry = sEntrys.begin(); itEntry != sEntrys.end(); ++itEntry)
        {
            sLine += itEntry->first + "=" + itEntry->second + ',';
        }
        sLines.push_back(sLine);
    }
    std::sort(sLines.begin(), sLines.end());
    return CxFile::save(sFilePath, sLines);
}



























//CxSkverIniFactory * CxSkverIniFactory::s_instance = CxSkverIniFactory::createSingleton();

int fn_deal_file_stream(void * oObject, int, int iLength, const void * pData)
{
    if (! oObject) return 0;
    CxSkverIni * oSkverIni = (CxSkverIni *)oObject;
    return oSkverIni->dealLoad(iLength, (char *)pData);
}

vector<vector<string> > CxSkverIni::split(const vector<string> & ss, char cHeadCharacter, char cIgnore)
{
    vector<vector<string> > lists;
    vector<string> list;
    for (size_t i = 0; i < ss.size(); ++i)
    {
        const string & s = ss.at(i);
        if (s.size()>0 && s[0] == cIgnore)
        {
            continue;
        }
        if (s.size()>0 && s[0] == cHeadCharacter)
        {
            if (list.size()>0)
            {
                lists.push_back(list);
                list.clear();
            }
            continue;
        }
        if (s.size()>0)
        {
            list.push_back(s);
        }
    }
    if (list.size()>0)
    {
        lists.push_back(list);
        list.clear();
    }
    return lists;
}

CxSkverIni::CxSkverIni() :
    CxSkverBase(),
    _dealState(0),
    _dealType(0)
{
    _text = _textData;
}

CxSkverIni::CxSkverIni(const std::map<std::string, std::map<std::string, std::string> > & sectionEntryValues) :
    CxSkverBase(sectionEntryValues),
    _dealState(0),
    _dealType(0)
{
    _text = _textData;
}

CxSkverIni::CxSkverIni( const std::string & sFilePath ) :
    CxSkverBase(),
    _dealState(0),
    _dealType(0)
{
    _text = _textData;
    load(sFilePath);
}

CxSkverIni::~CxSkverIni()
{
}

int CxSkverIni::dealLoad(int iLength, const char * pData)
{
    /*
    if ( i <= 0) return 0;
    const char * pBegin = pData;
    const char * pEnd = pData + iLength;
    const char * p;
    const char * pRead;
    for (; pBegin < pEnd; ++pBegin)
    {
        switch (_dealType)
        {
        case 0:

            break;
        case 1:
            char * getSection();
        case 2:
            char * getEntry();
        case 3:
            char * getValue();
            break;
        case 4:
             pBegin getRubbish();
            break;
        case 10:
            if (*pBegin == _dealEnd)
            {
                _text
                _textEnd =
                pRend =
            }
            break;
        case 0:

            break;
        case 0:

            break;
        default:
            break;
        }
    }
    memcpy();
*/
    for (int i = 0; i < iLength; ++i)
    {
        switch ( _dealState )
        {
            case 0:
            {
                if ( pData [ i ] == '[' )
                {
                    _dealState = 1;
                    _dealType = 1;
                    _end = ']';
                }
            }
                break;
            case 1:
            {
                if ( pData [ i ] == ']' && _text > _textData )
                {
                    _currentSection = string(_textData, _text);
                    _text = _textData;
                    _dealState = 2;
                }
                else if ( pData [ i ] > 0x20 && _text - _textData < sizeof(_textData))
                {
                    * _text = pData [ i ];
                    ++ _text;
                    _dealState = 3;
                }
                else if ( pData [ i ] == '\n' || pData [ i ] == '\r')
                {
                    _text = _textData;
                    _dealState = 0;
                }
            }
                break;
            case 2:
            {
                if ( pData [ i ] == '=' && _text > _textData )
                {
                    _currentEntry = string(_textData, _text);
                    _text = _textData;
                    _dealState = 3;
                }
                else if ( pData [ i ] > 0x20 && _text - _textData < sizeof(_textData))
                {
                    * _text = pData [ i ];
                    ++ _text;
                }
                else if ( pData [ i ] == '\n' || pData [ i ] == '\r')
                {
                    _text = _textData;
                }
                else if ( pData [ i ] == '[' )
                {
                    _text = _textData;
                    _dealState = 1;
                }
                else if ( pData [ i ] == '#')
                {
                    _dealState = 4;
                }
            }
                break;
            case 3:
            {
                if ( pData [ i ] < 0x20 && _text > _textData )
                {
                    _currentValue = string(_textData, _text);
                    setValue(_currentSection, _currentEntry, _currentValue);
                    _text = _textData;
                    _dealState = 2;
                }
                else
                {
                    if (_text - _textData < sizeof(_textData))
                    {
                        * _text = pData [ i ];
                        ++ _text;
                    }
                    else
                    {
                        _text = _textData;
                        _dealState = 2;
                    }
                }
            }
                break;
            case 4:
            {
                if ( pData [ i ] == '\n' || pData [ i ] == '\r')
                {
                    _text = _textData;
                    _dealState = 2;
                }
            }
                break;
        }
        // end case
    }
    return 0;
}

bool CxSkverIni::loadImpl(std::map<std::string, std::map<std::string, std::string> > & sectionEntryValues, const std::string& sFilePath)
{
    vector<string> sLines;
    if (CxFile::load(sFilePath, sLines, '\n'))
    {
        sectionEntryValues.clear();
//        std::pair<std::map<std::string, std::map<std::string, std::string> >::iterator,bool> ret;

        map<string, string> entrys;
        for (size_t i = 0; i < sLines.size(); ++i)
        {
            string sLine =  CxString::trim(sLines.at(i));
            switch (sLine[0])
            {
                case '[':
                {
                    if (entrys.size() > 0 && _currentSection.size() > 0)
                    {
                        sectionEntryValues[_currentSection] = entrys;
                    }
                    _currentSection = CxString::unquote(sLine, '[', ']');
                    entrys = map<string, string>();
                }
                    break;
                case '#':
                case ';':
                {
                }
                    break;
                default:
                {
                    bool bHas;
                    _currentValue = CxString::tokenLeft(sLine, '=', &bHas);
                    if (! bHas)
                    {
                        break;
                    }
                    _currentValue = CxString::trim(_currentValue);
                    _currentEntry = CxString::trim(sLine);
                    if (_currentSection.size() > 0 && _currentEntry.size() > 0)
                    {
                        entrys[_currentEntry] = _currentValue;
                    }
                }
                    break;
            }
        }
        if (entrys.size() > 0 && _currentSection.size() > 0)
        {
            sectionEntryValues[_currentSection] = entrys;
        }
        return true;
    }
    return false;
//    oSectionEntryValues = & sectionEntryValues;
//    return CxFile::load(sFilePath, this, fn_deal_file_stream);
}

bool CxSkverIni::saveImpl(const std::map<std::string, std::map<std::string, std::string> > & sectionEntryValues, const std::string& sFilePath, bool bRenew) const
{
    FILE * pFile;
    pFile = fopen (sFilePath.data(), "wb");
    size_t iWroteTotal = 0;
    if (pFile==NULL)
    {
        return false;
    }
    vector<string> sSectionNames = sectionNames(1);
    for (size_t i = 0; i < sSectionNames.size(); ++i)
    {
        const string & sSectionName = sSectionNames.at(i);
        map<string, map<string, string> >::const_iterator it = sectionEntryValues.find(sSectionName);
        if (it != sectionEntryValues.end())
        {
            const map<string, string> & sEntrys = it->second;
            string sLine("\r\n[");
            sLine += sSectionName;
            sLine += "]\r\n";
            iWroteTotal += fwrite (const_cast<char *>(sLine.data()) , 1, sLine.size(), pFile);
            for (map<string, string>::const_iterator itEntry = sEntrys.begin(); itEntry != sEntrys.end(); ++itEntry)
            {
                sLine = itEntry->first + "=" + itEntry->second + "\r\n";
                size_t iWrote = fwrite (const_cast<char *>(sLine.data()) , 1, sLine.size(), pFile);
                if (iWrote != sLine.size())
                    return false;
                iWroteTotal += iWrote;
            }
        }
    }

//    for (map<string, map<string, string> >::const_iterator itSection = sectionEntryValues.begin(); itSection != sectionEntryValues.end(); ++itSection)
//    {
//        string sLine("\r\n[");
//        sLine += itSection->first;
//        sLine += "]\r\n";
//        iWroteTotal += fwrite (const_cast<char *>(sLine.data()) , 1, sLine.size(), pFile);
//        const map<string, string> & sEntrys = itSection->second;
//        for (map<string, string>::const_iterator itEntry = sEntrys.begin(); itEntry != sEntrys.end(); ++itEntry)
//        {
//            sLine = itEntry->first + "=" + itEntry->second + "\r\n";
//            size_t iWrote = fwrite (const_cast<char *>(sLine.data()) , 1, sLine.size(), pFile);
//            if (iWrote != sLine.size())
//                return false;
//            iWroteTotal += iWrote;
//        }
//    }
    fclose (pFile);
    return iWroteTotal;
}

