#include "cxbusiness.h"


#include "cxstring.h"
#include "cxcontainer.h"
#include "cxfile.h"
#include "cxapplication.h"


using namespace std;


CxMsObjectTypeManager * fn_msObjectTypeManagerStart()
{
    CxApplication::registStartFunction(CxMsObjectTypeManager::start);
    static CxMsObjectTypeManager s_msObjectTypeManager;
    return & s_msObjectTypeManager;
}


static CxMsObjectTypeManager * f_oMsObjectTypeManager = fn_msObjectTypeManagerStart();


const CxMsObjectType * CxMsObjectData::getMsObjectType() const
{
    return CxMsObjectTypeManager::find(_typeName);
}

std::string CxMsObjectData::toString() const
{
    string r;
    r.append(_typeName);
    r.push_back('\n');
    r.append(_name);
    r.push_back('\n');
    string sMajor = CxString::join(_major, "=", ",");
    r.append(sMajor);
    r.push_back('\n');
    for (std::map<std::string, std::map<std::string, std::string> >::const_iterator it = _details.begin(); it != _details.end(); ++it)
    {
        const std::string & sDetailName = it->first;
        const std::map<std::string, std::string> & detail = it->second;
        string sDetail = CxString::join(detail, "=", ",");
        r.append(sDetailName);
        r.push_back(':');
        r.append(sDetail);
        r.push_back('\n');
    }
    return r;
}

int CxMsObjectData::fromString(const string &sText)
{
    vector<string> sLines = CxString::split(sText, '\n', true);
    if (sLines.size() > 3)
    {
        _typeName = sLines.at(0);
        _name = sLines.at(1);
        string sMajor = sLines.at(0);
        _major = CxString::splitToMap(sMajor, '=', ',');
        _details.clear();
        for (size_t i = 3; i < sLines.size(); ++i)
        {
            string sLine = sLines.at(i);
            bool bOk;
            string sDetailName = CxString::token(sLine, ':', &bOk);
            if (bOk)
            {
                map<string, string> detail = CxString::splitToMap(sLine, '=', ',');
                _details[sDetailName] = detail;
            }
        }
        return TRUE;
    }
    return FALSE;
}

int CxMsObjectData::updateMajor(const std::map<string, string> &major)
{
    for (map<string, string>::const_iterator it = major.begin(); it != major.end(); ++it)
    {
        map<string, string>::iterator itOld = _major.find(it->first);
        if (itOld != _major.end())
        {
            if (itOld->second != it->second)
            {
                //modify
                itOld->second = it->second;
            }
        }
        else
        {
            //add
            _major[it->first] = it->second;
        }
    }
    _majorModify = true;
    return true;
}

int CxMsObjectData::updateDetails(const std::map<string, std::map<string, string> > &details)
{
    for (std::map<std::string, std::map<std::string, std::string> >::const_iterator it = details.begin(); it != details.end(); ++it)
    {
        const std::string & sDetailName = it->first;

        std::map<std::string, std::map<std::string, std::string> >::iterator itOld = _details.find(sDetailName);
        if (itOld != _details.end())
        {
            //modify
            itOld->second = it->second;
            _detailModifies[sDetailName] = 2;
        }
        else
        {
            //add
            _details[it->first] = it->second;
            _detailModifies[sDetailName] = 1;
        }
    }
    return true;
}


static vector<CxMsObjectData> f_msObjects;
static bool f_msObjectChangeBegin = false;
static string f_msObjectChangingTypeName;
static vector<string> f_msObjectChangedNames;
static vector<CxIMsObjectDataSubject*> f_msObjectSubjects;
static volatile int f_receivedSource = 0;


void CxMsObjectDataManager::addObserver(CxIMsObjectDataSubject *oSubject)
{
    if ( ! CxContainer::contain(f_msObjectSubjects, oSubject) )
    {
        f_msObjectSubjects.push_back(oSubject);
    }
}

void CxMsObjectDataManager::removeObserver(CxIMsObjectDataSubject *oSubject)
{
    CxContainer::remove(f_msObjectSubjects, oSubject);
}

void CxMsObjectDataManager::beginReceived(const std::string & sTypeName, int iSource)
{
    f_receivedSource = iSource;
    f_msObjectChangeBegin = true;
    f_msObjectChangingTypeName = sTypeName;
    f_msObjectChangedNames.clear();
}

int CxMsObjectDataManager::receivedObject(const string &sTypeName, const string &sName, const std::map<string, string> &major)
{
    if (sTypeName.empty())
        return FALSE;
    int iChangeType = 0;
    CxMsObjectData * oMsObjectOld = find(sTypeName, sName);
    if (oMsObjectOld)
    {
        // update
        iChangeType = 3;
        oMsObjectOld->updateMajor(major);
        oMsObjectOld->_source = f_receivedSource;
        for (size_t i = 0; i < f_msObjectSubjects.size(); ++i)
        {
            CxIMsObjectDataSubject * oSubject = f_msObjectSubjects.at(i);
            oSubject->msObject_changed(* oMsObjectOld, iChangeType);
        }
    }
    else
    {
        iChangeType = 1;
        CxMsObjectData msObjectNew(sTypeName, sName, major);
        msObjectNew._source = f_receivedSource;
        f_msObjects.push_back(msObjectNew);
        for (size_t i = 0; i < f_msObjectSubjects.size(); ++i)
        {
            CxIMsObjectDataSubject * oSubject = f_msObjectSubjects.at(i);
            oSubject->msObject_changed(msObjectNew, iChangeType);
        }
    }
    if (f_msObjectChangeBegin)
    {
        f_msObjectChangedNames.push_back(sName);
    }
    return TRUE;
}

int CxMsObjectDataManager::receivedObject(const string &sTypeName, const string &sName, const std::map<string, string> &major, const std::map<string, std::map<string, string> > &details)
{
    if (sTypeName.empty())
        return FALSE;
    int iChangeType = 0;
    CxMsObjectData * oMsObjectOld = find(sTypeName, sName);
    if (oMsObjectOld)
    {
        // update
        iChangeType = 3;
        oMsObjectOld->updateMajor(major);
        oMsObjectOld->updateDetails(details);
        oMsObjectOld->_source = f_receivedSource;
        for (size_t i = 0; i < f_msObjectSubjects.size(); ++i)
        {
            CxIMsObjectDataSubject * oSubject = f_msObjectSubjects.at(i);
            oSubject->msObject_changed(* oMsObjectOld, iChangeType);
        }
    }
    else
    {
        iChangeType = 1;
        CxMsObjectData msObjectNew(sTypeName, sName, major, details);
        msObjectNew._source = f_receivedSource;
        f_msObjects.push_back(msObjectNew);
        for (size_t i = 0; i < f_msObjectSubjects.size(); ++i)
        {
            CxIMsObjectDataSubject * oSubject = f_msObjectSubjects.at(i);
            oSubject->msObject_changed(msObjectNew, iChangeType);
        }
    }
    if (f_msObjectChangeBegin)
    {
        f_msObjectChangedNames.push_back(sName);
    }
    return TRUE;
}

int CxMsObjectDataManager::receivedObject(const CxMsObjectData * oMsObject, bool bReplace)
{
    assert(oMsObject != NULL);

    int iChangeType = 0;
    CxMsObjectData * oMsObjectOld = find(oMsObject->_typeName, oMsObject->_name);
    if (oMsObjectOld)
    {
        if (bReplace)
        {
            // replace
            iChangeType = 2;
            oMsObjectOld->assignFrom(* oMsObject);
        }
        else
        {
            // update
            iChangeType = 3;
            oMsObjectOld->updateMajor(oMsObject->_major);
            oMsObjectOld->updateDetails(oMsObject->_details);
        }
        oMsObjectOld->_source = f_receivedSource;
    }
    else
    {
        iChangeType = 1;
        CxMsObjectData msObjectNew = * oMsObject;
        msObjectNew._source = f_receivedSource;
        f_msObjects.push_back(msObjectNew);
    }

    if (f_msObjectChangeBegin)
    {
        f_msObjectChangedNames.push_back(oMsObject->name());
    }

    for (size_t i = 0; i < f_msObjectSubjects.size(); ++i)
    {
        CxIMsObjectDataSubject * oSubject = f_msObjectSubjects.at(i);
        oSubject->msObject_changed(* oMsObject, iChangeType);
    }
    return TRUE;
}

void CxMsObjectDataManager::endReceived()
{
    if (f_msObjectChangeBegin)
    {
        f_msObjectChangeBegin = false;
        if (f_msObjectChangedNames.size() > 0)
        {
            for (size_t i = 0; i < f_msObjectSubjects.size(); ++i)
            {
                CxIMsObjectDataSubject * oSubject = f_msObjectSubjects.at(i);
                oSubject->msObjects_changed(f_msObjectChangingTypeName, f_msObjectChangedNames);
            }
            for(vector<CxMsObjectData>::iterator it = f_msObjects.begin(); it != f_msObjects.end();)
            {
                const CxMsObjectData & msObject = * it;
                if (CxMsObjectTypeManager::isTempObject(msObject.typeName()))
                {
                    f_msObjects.erase(it);
                    continue;
                }
                ++it;
            }
            for (size_t i = 0; i < f_msObjects.size(); ++i)
            {
                CxMsObjectData & msObject = f_msObjects.at(i);
                if (msObject.typeName() == f_msObjectChangingTypeName && CxContainer::contain(f_msObjectChangedNames, msObject.name()))
                {
                    msObject._majorModify = false;
                    msObject._detailModifies.clear();
                }
            }
        }
    }
}

const std::vector<CxMsObjectData> &CxMsObjectDataManager::msObjects()
{
    return f_msObjects;
}

std::vector<const CxMsObjectData *> CxMsObjectDataManager::findObjects(const string &sTypeName)
{
    vector<const CxMsObjectData *> r;
    for (size_t i = 0; i < f_msObjects.size(); ++i)
    {
        CxMsObjectData & msObject = f_msObjects.at(i);
        if (msObject._typeName == sTypeName)
        {
            r.push_back( & msObject );
        }
    }
    return r;
}

std::vector<const CxMsObjectData *> CxMsObjectDataManager::findObjects(const string &sTypeName, const std::vector<string> &sNames)
{
    vector<const CxMsObjectData *> r;
    for (size_t i = 0; i < f_msObjects.size(); ++i)
    {
        CxMsObjectData & msObject = f_msObjects.at(i);
        if (msObject._typeName == sTypeName && CxContainer::contain(sNames, msObject._name))
        {
            r.push_back( & msObject );
        }
    }
    return r;
}

const CxMsObjectData *CxMsObjectDataManager::find(const string &sTypeName, const string &majorKey, const string &majorValue)
{
    for (size_t i = 0; i < f_msObjects.size(); ++i)
    {
        CxMsObjectData & msObject = f_msObjects.at(i);
        if (msObject._typeName == sTypeName)
        {
            const std::map<std::string, std::string> & major = msObject.major();
            std::map<std::string, std::string>::const_iterator it = major.find(majorKey);
            if (it != major.end() && it->second == majorValue)
            {
                return & msObject;
            }
        }
    }
    return NULL;
}

int CxMsObjectDataManager::saveMsObjects(const string &sFilePath)
{
    vector<string> sLines;
    for (size_t i = 0; i < f_msObjects.size(); ++i)
    {
        CxMsObjectData & msObject = f_msObjects.at(i);
        sLines.push_back(msObject.toString());
        sLines.push_back("--- --- ---");
    }
    if (CxFile::save(sFilePath, sLines, std::string()))
    {
        return sLines.size() / 2;
    }
    else
    {
        return 0;
    }
}

CxMsObjectData *CxMsObjectDataManager::find(const string &sTypeName, const string &sName)
{
    for (size_t i = 0; i < f_msObjects.size(); ++i)
    {
        CxMsObjectData & msObject = f_msObjects.at(i);
        if (msObject._typeName == sTypeName && msObject._name == sName)
        {
            return & msObject;
        }
    }
    return NULL;
}


static vector<CxMsObjectType> f_msObjectTypes;

void CxMsObjectTypeManager::start()
{
    load();
}

int CxMsObjectTypeManager::load()
{
    f_msObjectTypes.clear();
    const map<string, string> & sSectionMsObjectTypes = CxApplication::findConfigs(CS_SectionMsObjectTypes);
    for (map<string, string>::const_iterator it = sSectionMsObjectTypes.begin(); it != sSectionMsObjectTypes.end(); ++it)
    {
        const string & sTypeName = it->first;
        const string & sSectionMsObjectType = it->second;
        vector<string> sLines = CxString::split(sSectionMsObjectType, ';');
        while (sLines.size() < 4)
        {
            sLines.push_back("");
        }
        int iStructType = CxString::fromString(sLines[0], ci_int_zero);
        std::vector<std::string> sMajorKeies = CxString::split(sLines[1], ',');
        std::string sDetailKey = sLines[2];
        int iHowLongStore = CxString::fromString(sLines[3], ci_int_zero);
        f_msObjectTypes.push_back(CxMsObjectType(sTypeName, (CxMsObjectType::StructType_Enum)iStructType, sMajorKeies, sDetailKey, iHowLongStore));
    }
    return f_msObjectTypes.size();
}

const CxMsObjectType * CxMsObjectTypeManager::find(const string & sTypeName)
{
    for (size_t i = 0; i < f_msObjectTypes.size(); ++i)
    {
        CxMsObjectType & msObject = f_msObjectTypes.at(i);
        if (msObject._typeName == sTypeName)
        {
            return & msObject;
        }
    }
    return NULL;
}

string CxMsObjectTypeManager::name(const string & stypeName, const std::map<string, string> &major)
{
    const CxMsObjectType * oObjectType = find(stypeName);
    if (oObjectType)
    {
        string r;
        for (size_t i = 0; i < oObjectType->_majorKeies.size(); ++i)
        {
            r += CxContainer::value(major, oObjectType->_majorKeies[i]);
        }
        return r;
    }
    return string();
}

bool CxMsObjectTypeManager::isTempObject(const string &sTypeName)
{
    const CxMsObjectType * oObjectType = find(sTypeName);
    if (oObjectType)
    {
        return oObjectType->_howLongStore == 0;
    }
    return false;
}
