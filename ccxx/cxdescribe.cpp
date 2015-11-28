#include "cxdescribe.h"


#include "cxprofile.h"
#include "cxfile.h"
#include "cxinterinfo.h"
#include "cxcontainer.h"
#include "cxapplication.h"


using namespace std;


std::map<std::string, CxDescribeUnit*> f_describeUnits;



CxDescribeManager * fn_oDescribeManager()
{
    CxApplication::registInitFunction(CxDescribeManager::start);
    CxApplication::registStopFunction(CxDescribeManager::stop);

    static CxDescribeManager f_describeManager;
    return & f_describeManager;
}

static CxDescribeManager * f_oDescribeManager = fn_oDescribeManager();









CxDescribeBase::CxDescribeBase(){
    _name = "";
    _columnIndex = -1;
    _title = "";
    _desc = "";
    _canNull = false;
    _isVisible = true;
    _isReadOnly = false;
    _maxLength = 255;
    _minLength = 0;
    _regularExpression = "";
}


CxDescribeBase::~CxDescribeBase(){
}


void CxDescribeBase::toContext(CxISetSkv& context) const
{
    context.setValue(CxDescribeStrings::name ()             , _name              );
    context.setValue(CxDescribeStrings::typeSimpleName()    , typeSimpleName()   );
    context.setValue(CxDescribeStrings::title ()            , _title             );
    context.setValue(CxDescribeStrings::desc ()             , _desc              );
    context.setValue(CxDescribeStrings::canNull ()        , _canNull         );
    context.setValue(CxDescribeStrings::isVisible ()        , _isVisible         );
    context.setValue(CxDescribeStrings::isReadOnly ()       , _isReadOnly        );
    context.setValue(CxDescribeStrings::minLength ()        , _maxLength         );
    context.setValue(CxDescribeStrings::maxLength ()        , _minLength         );
    context.setValue(CxDescribeStrings::regularExpression (), _regularExpression );
    context.setValue(CxDescribeStrings::valueScopes()       , CxString::join(_valueScopes, ";"));
    context.setValue(CxDescribeStrings::foreignKey (), _foreignKey );
}


void CxDescribeBase::fromContext(const CxIGetSkv& context)
{
    _name              = context.getValue(CxDescribeStrings::name ()             , _name              );
    _columnIndex       = context.getValue(CxDescribeStrings::columnIndex()       , _columnIndex       );
    _title             = context.getValue(CxDescribeStrings::title ()            , _title             );
    _desc              = context.getValue(CxDescribeStrings::desc ()             , _desc              );
    _canNull           = context.getValue(CxDescribeStrings::canNull()           , _canNull           );
    _isVisible         = context.getValue(CxDescribeStrings::isVisible ()        , _isVisible         );
    _isReadOnly        = context.getValue(CxDescribeStrings::isReadOnly ()       , _isReadOnly        );
    _maxLength         = context.getValue(CxDescribeStrings::maxLength ()        , _maxLength         );
    _minLength         = context.getValue(CxDescribeStrings::minLength ()        , _minLength         );
    _regularExpression = context.getValue(CxDescribeStrings::regularExpression (), _regularExpression );

    string sContentValues;
    sContentValues     = context.getValue(CxDescribeStrings::valueScopes()     , sContentValues     );
    _valueScopes       = CxString::split(sContentValues, ';');

    _foreignKey        = context.getValue(CxDescribeStrings::foreignKey (), _foreignKey );
}

void CxDescribeBase::assignedFrom(const CxDescribeBase *oDescribe)
{
    assert(oDescribe != NULL);

    _name                  = oDescribe->_name             ;
    _columnIndex           = oDescribe->_columnIndex      ;
    _title                 = oDescribe->_title            ;
    _desc                  = oDescribe->_desc             ;
    _canNull             = oDescribe->_canNull        ;
    _isVisible             = oDescribe->_isVisible        ;
    _isReadOnly            = oDescribe->_isReadOnly       ;
    _maxLength             = oDescribe->_maxLength        ;
    _minLength             = oDescribe->_minLength        ;
    _regularExpression     = oDescribe->_regularExpression;
    _valueScopes           = oDescribe->_valueScopes      ;
    _foreignKey            = oDescribe->_foreignKey       ;
}
















//****************************************************************************************************************
//***********  Unit Catalog Project ...... begin ************************************************************************
//****************************************************************************************************************















CxDescribeUnit * CxDescribeUnit::createDescribes(const string &sFilePath)
{
    if (! CxFileSystem::isExist(sFilePath))
        return NULL;
    CxSkverBase * oProfile = CxSkverManager::beginUse(sFilePath);
    if (oProfile){
        CxDescribeUnit * oDescribeUnit = new CxDescribeUnit();
        oProfile->prepareGet(CxDescribeStrings::self());
        oDescribeUnit->fromContext(*oProfile);
        string sDescribes = oProfile->getValue(CxDescribeStrings::describes(), std::string());
        vector<string> describeNames = CxString::split(sDescribes, ',');
        for (size_t i = 0; i < describeNames.size(); ++i)
        {
            string sDescribeName = describeNames.at(i);
            oProfile->prepareGet(sDescribeName);
            string sTypeSimpleName = oProfile->getValue(CxDescribeStrings::typeSimpleName(), sTypeSimpleName);
            CxDescribeBase* oDescribe = CxDescribeFactoryManager::createObject(sTypeSimpleName);
            if (oDescribe)
            {
                oDescribeUnit->_describes.push_back(oDescribe);
                oDescribe->fromContext(*oProfile);
            }
        }
        CxSkverManager::endUse(oProfile);
        return oDescribeUnit;
    }
    return NULL;
}

bool CxDescribeUnit::saveDescribes(const CxDescribeUnit& describeUnit, const string &sFilePath)
{
    CxSkverBase * oProfile = CxSkverManager::beginUse(sFilePath);
    if (oProfile)
    {
        oProfile->prepareSet(CxDescribeStrings::self());
        describeUnit.toContext(*oProfile);
        const vector<CxDescribeBase*> & describes = describeUnit.describes();
        vector<string> sDescribes;
        for (size_t i = 0; i < describes.size(); ++i)
        {
            CxDescribeBase* oDescribe = describes.at(i);
            sDescribes.push_back(oDescribe->name());
            oProfile->prepareSet(oDescribe->name());
            oDescribe->toContext(*oProfile);
        }
        oProfile->setValue(CxDescribeStrings::self(), CxDescribeStrings::describes(), CxString::join(sDescribes, ';'));
        CxSkverManager::endUse(oProfile, true);
    }
    return true;
}

CxDescribeUnit::CxDescribeUnit()
{
    _funcDescribeDefaultValue = NULL;
    _funcDescribeValuesCalc = NULL;
}


CxDescribeUnit::~CxDescribeUnit()
{
    deleteAndClear();
}

int fn_compare_describe(const CxDescribeBase * v1, const CxDescribeBase * v2)
{
    return v1->columnIndex() < v2->columnIndex();
}

void CxDescribeUnit::setDescribes(const std::vector<CxDescribeBase *> &value)
{
    deleteAndClear();
    _describes = value;
    std::sort(_describes.begin(), _describes.end(), fn_compare_describe);
}

void CxDescribeUnit::setUnitParam(const string &value)
{
    _unitParam = value;
    _unitParams = CxString::splitToMap(value, '=', ';');
}

void CxDescribeUnit::setUnitParams(const std::map<string, string> &value)
{
    _unitParams = value;
    _unitParam = CxString::join(value, "=", ";");
}


CxDescribeBase* CxDescribeUnit::describe(const string& sDescribeName) const {
    for (size_t i = 0; i < _describes.size(); ++i) {
        CxDescribeBase* oDescribe = _describes.at(i);
        if ( oDescribe->name() == sDescribeName ){
            return oDescribe;
        }
    }
    return NULL;
}


CxDescribeBase* CxDescribeUnit::describe(int iColumnIndex) const {
    for (size_t i = 0; i < _describes.size(); ++i) {
        CxDescribeBase* oDescribe = _describes.at(i);
        if ( oDescribe->columnIndex() == iColumnIndex ){
            return oDescribe;
        }
    }
    return NULL;
}

int CxDescribeUnit::loadValues(const string &sDataName, const string &sDataSection, const string &sDataPath, const string &sDataType) const
{
    CxSkverBase * oProfile = CxSkverManager::beginUse(sDataName);
    if (oProfile)
    {
        oProfile->prepareGet(sDataSection);
        for (size_t i = 0; i < _describes.size(); ++i)
        {
            CxDescribeBase * oDescribe = _describes.at(i);
            string sValue;
            sValue = oProfile->getValue(oDescribe->name(), sValue);
            oDescribe->valueFromString(sValue);
        }
        CxSkverManager::endUse(oProfile);
        return _describes.size();
    }
    return 0;
}

int CxDescribeUnit::saveValues(const string &sDataName, const string &sDataSection, bool bSave, const string &sDataPath, const string &sDataType) const
{
    CxSkverBase * oProfile = CxSkverManager::beginUse(sDataName);
    if (oProfile)
    {
        oProfile->prepareSet(sDataSection);
        for (size_t i = 0; i < _describes.size(); ++i)
        {
            CxDescribeBase* oDescribe = _describes.at(i);
            string sValue = oDescribe->valueToString();
            oProfile->setValue(oDescribe->name(), sValue);
        }
        CxSkverManager::endUse(oProfile, bSave);
        return _describes.size();
    }
    return 0;
}

void CxDescribeUnit::toContext(CxISetSkv &context) const
{
    context.setValue(CxDescribeStrings::unitName(), _unitName);
    context.setValue(CxDescribeStrings::unitTitle(), _unitTitle);
    context.setValue(CxDescribeStrings::unitParam(), _unitParam);

    string sForeignKeyValues = CxString::join(_foreignKeyValues, "=", ";");
    context.setValue(CxDescribeStrings::foreignKeyValues(), sForeignKeyValues);

    context.setValue(CxDescribeStrings::localKeyName(), _localKeyName);
    context.setValue(CxDescribeStrings::localCaptionName(), _localCaptionName);
    context.setValue(CxDescribeStrings::filterName(), _filterName);
    context.setValue(CxDescribeStrings::filterValue(), _filterValue);
    context.setValue(CxDescribeStrings::partitionName(), _partitionName);
}

void CxDescribeUnit::fromContext(const CxIGetSkv &context)
{
    _unitName = context.getValue(CxDescribeStrings::unitName(), _unitName);
    _unitTitle = context.getValue(CxDescribeStrings::unitTitle(), _unitTitle);
    setUnitParam( context.getValue(CxDescribeStrings::unitParam(), _unitParam) );

    string sForeignKeyValues;
    sForeignKeyValues = context.getValue(CxDescribeStrings::foreignKeyValues(), sForeignKeyValues);
    _foreignKeyValues = CxString::splitToMap(sForeignKeyValues, '=', ';');

    _localKeyName     = context.getValue(CxDescribeStrings::localKeyName(), _localKeyName);
    _localCaptionName = context.getValue(CxDescribeStrings::localCaptionName(), _localCaptionName);
    _filterName       = context.getValue(CxDescribeStrings::filterName(), _filterName);
    _filterValue      = context.getValue(CxDescribeStrings::filterValue(), _filterValue);
    _partitionName    = context.getValue(CxDescribeStrings::partitionName(), _partitionName);
}

void CxDescribeUnit::deleteAndClear()
{
    for (size_t i = 0; i < _describes.size(); ++i)
    {
        CxDescribeBase* o = _describes.at(i);
        delete o;
    }
    _describes.clear();
}

int CxDescribeUnit::describeValuesCalc(const CxDescribeUnit *oDescribeUnit) const
{
    if (_funcDescribeValuesCalc) {
        _funcDescribeValuesCalc(oDescribeUnit);
        return oDescribeUnit->describes().size();
    } else {
        return 0;
    }
}

void CxDescribeUnit::restoreDefaultValues() const
{
    for (size_t i = 0; i < _describes.size(); ++i) {
        CxDescribeBase* oDescribe = _describes.at(i);
        oDescribe->valueFromString(oDescribe->defaultValueToString());
    }
}

void CxDescribeUnit::assignedFrom(const CxDescribeUnit &oDescribeUnit)
{
    assignedFrom(& oDescribeUnit);
}

void CxDescribeUnit::assignedFrom(const CxDescribeUnit *oDescribeUnit)
{
    _describes.clear();
    const vector<CxDescribeBase*> & describes = oDescribeUnit->describes();
    for (size_t i = 0; i < describes.size(); ++i)
    {
        CxDescribeBase * oDescribe = describes.at(i);
        CxDescribeBase * oNew = CxDescribeFactoryManager::createObject(oDescribe->typeSimpleName());
        assert(oNew != NULL);
        oNew->assignedFrom(oDescribe);
        _describes.push_back(oNew);
    }
    _unitName = oDescribeUnit->_unitName;
    _foreignKeyValues = oDescribeUnit->_foreignKeyValues;
    _localKeyName = oDescribeUnit->_localKeyName;
    _localCaptionName = oDescribeUnit->_localCaptionName;
    _filterName = oDescribeUnit->_filterName;
    _filterValue = oDescribeUnit->_filterValue;
    _partitionName = oDescribeUnit->_partitionName;
    _funcDescribeDefaultValue = oDescribeUnit->_funcDescribeDefaultValue;
    _funcDescribeValuesCalc = oDescribeUnit->_funcDescribeValuesCalc;
}
















//const std::string cs_DecribeFileName = "DecribeFileName";
//const std::string cs_Title           = "Title";
//const std::string cs_DataType        = "DataType";
//const std::string cs_DataPath        = "DataPath";
//const std::string cs_DataSection     = "DataSection";
//const std::string cs_DataName     = "DataName";

CxDescribeCatalog::CxDescribeCatalog()
{

}

CxDescribeCatalog::~CxDescribeCatalog()
{
    deleteAndClear();
}

void CxDescribeCatalog::setUnitAttrs(const std::vector<CxDescribeCatalog::UnitAttr *> & value)
{
    deleteAndClear();
    _unitAttrs = value;
}

void CxDescribeCatalog::setCatalogParam(const string &value)
{
    _catalogParam = value;
    _catalogParams = CxString::splitToMap(value, '=', ';');
}

void CxDescribeCatalog::setCatalogParams(const std::map<string, string> &value)
{
    _catalogParams = value;
    _catalogParam = CxString::join(value, "=", ";");
}

CxDescribeCatalog::UnitAttr *CxDescribeCatalog::findByCation(const string &sUnitCaption) const
{
    for (size_t i = 0; i < _unitAttrs.size(); ++i)
    {
        CxUnitAttr * o = _unitAttrs.at(i);
        if (o->unitCaption == sUnitCaption)
            return o;
    }
    return NULL;
}

CxDescribeCatalog::UnitAttr *CxDescribeCatalog::findByData(const string &sDataName, const string &sDataSection) const
{
    for (size_t i = 0; i < _unitAttrs.size(); ++i)
    {
        CxUnitAttr * o = _unitAttrs.at(i);
        if ((o->dataName == sDataName) && (o->dataSection == sDataSection))
            return o;
    }
    return NULL;
}

CxDescribeCatalog::UnitAttr *CxDescribeCatalog::findByData(const string &sDataName, const string &sDataSection, const string &sDataPath) const
{
    for (size_t i = 0; i < _unitAttrs.size(); ++i)
    {
        CxUnitAttr * o = _unitAttrs.at(i);
        if (o->dataName == sDataName && o->dataSection == sDataSection && o->dataPath == sDataPath)
            return o;
    }
    return NULL;
}

std::vector<CxDescribeCatalog::UnitAttr *> CxDescribeCatalog::getByData(const string &sDataName) const
{
    std::vector<CxDescribeCatalog::UnitAttr *> r;
    for (size_t i = 0; i < _unitAttrs.size(); ++i)
    {
        CxUnitAttr * o = _unitAttrs.at(i);
        if (o->dataName == sDataName)
            r.push_back( o );
    }
    return r;
}

std::vector<CxDescribeCatalog::UnitAttr *> CxDescribeCatalog::getByData(const string &sDataName, const string &sDataSection) const
{
    std::vector<CxDescribeCatalog::UnitAttr *> r;
    for (size_t i = 0; i < _unitAttrs.size(); ++i)
    {
        CxUnitAttr * o = _unitAttrs.at(i);
        if (o->dataName == sDataName && o->dataSection == sDataSection)
            r.push_back( o );
    }
    return r;
}

CxDescribeCatalog::UnitAttr *CxDescribeCatalog::addUnitAttr(CxDescribeUnit * oUnit, const string &sDescribeFilePath)
{
    GM_INVALID_RETURE_(oUnit, NULL);

    UnitAttr * oUnitAttr = new UnitAttr();
    oUnitAttr->unit = oUnit;
    oUnitAttr->describeFilePath = sDescribeFilePath;
    _unitAttrs.push_back(oUnitAttr);
    return oUnitAttr;
}

void CxDescribeCatalog::setUnitAttrsDataName(const string &sDataName)
{
    for (size_t i = 0; i < _unitAttrs.size(); ++i)
    {
        UnitAttr * o = _unitAttrs.at(i);
        o->dataName = sDataName;
    }
}

void CxDescribeCatalog::toContext(CxISetSkv &context) const
{
    context.setValue(CxDescribeStrings::catalogName(), _catalogName);
    context.setValue(CxDescribeStrings::catalogTitle(), _catalogTitle);
    context.setValue(CxDescribeStrings::catalogParam(), _catalogParam);
}

void CxDescribeCatalog::fromContext(const CxIGetSkv &context)
{
    _catalogName = context.getValue(CxDescribeStrings::catalogName(), _catalogName);
    _catalogTitle = context.getValue(CxDescribeStrings::catalogTitle(), _catalogTitle);
    setCatalogParam( context.getValue(CxDescribeStrings::catalogParam(), _catalogParam) );
}

void CxDescribeCatalog::deleteAndClear()
{
    for (size_t i = 0; i < _unitAttrs.size(); ++i)
    {
        UnitAttr * o = _unitAttrs.at(i);
        delete o;
    }
    _unitAttrs.clear();
}











//*project const string
//const std::string cs_DecribeFileName = "DecribeFileName";
//const std::string cs_Title = "Title";




CxDescribeProject::CxDescribeProject()
{

}

CxDescribeProject::~CxDescribeProject()
{
    deleteAndClear();
}

void CxDescribeProject::setCatalogAttrs(const std::vector<CatalogAttr *> &value)
{
    deleteAndClear();
    _catalogAttrs = value;
}

CxDescribeProject::CatalogAttr *CxDescribeProject::findByName(const string &sName) const
{
    for (size_t i = 0; i < _catalogAttrs.size(); ++i)
    {
        CatalogAttr * o = _catalogAttrs.at(i);
        if (o->catalogName() == sName)
            return o;
    }
    return NULL;
}

CxDescribeProject::CatalogAttr *CxDescribeProject::findByTitle(const string &sTitle) const
{
    for (size_t i = 0; i < _catalogAttrs.size(); ++i)
    {
        CatalogAttr * o = _catalogAttrs.at(i);
        if (o->catalogTitle() == sTitle)
            return o;
    }
    return NULL;
}

CxDescribeProject::CatalogAttr *CxDescribeProject::findByFilePath(const string &sFilePath) const
{
    for (size_t i = 0; i < _catalogAttrs.size(); ++i)
    {
        CatalogAttr * o = _catalogAttrs.at(i);
        if (CxString::equalCase(o->catalogFilePath, sFilePath))
            return o;
    }
    return NULL;
}

CxDescribeProject::CatalogAttr *CxDescribeProject::addCatalogAttr(const string &sCatalogName, const string &sCatalogTitle, const string &sCatalogUse, const string &sCatalogFilePath)
{
    GM_VALID_RETURE_(sCatalogTitle.empty(), NULL);
    GM_VALID_RETURE_(findByTitle(sCatalogTitle), NULL);

    CatalogAttr * oCatalogAttr = new CatalogAttr();
    CxDescribeCatalog * oCatalog = new CxDescribeCatalog();
    oCatalogAttr->catalogFilePath = sCatalogFilePath;
    oCatalog->_catalogName = sCatalogName;
    oCatalog->_catalogTitle = sCatalogTitle;
    oCatalog->_catalogParam = sCatalogUse;
    oCatalogAttr->catalog = oCatalog;
    _catalogAttrs.push_back(oCatalogAttr);
    return oCatalogAttr;
}

void CxDescribeProject::deleteCatalogAttr(const string &sCatalogName, const string &sCatalogTitle)
{
    for (size_t i = 0; i < _catalogAttrs.size(); ++i)
    {
        CatalogAttr * o = _catalogAttrs.at(i);
        if (sCatalogName.size()>0)
        {
            if (o->catalogName() == sCatalogName)
            {
                CxContainer::remove(_catalogAttrs, o);
                delete o;
                return;
            }
        }
        else if (sCatalogTitle.size()>0)
        {
            if (o->catalogTitle() == sCatalogTitle)
            {
                CxContainer::remove(_catalogAttrs, o);
                delete o;
                return;
            }
        }
    }
}

void CxDescribeProject::deleteCatalogAttr(CxDescribeProject::CatalogAttr * oCatalogAttr)
{
    CxContainer::remove(_catalogAttrs, oCatalogAttr);
}

void CxDescribeProject::toContext(CxISetSkv &context) const
{
    context.setValue(CxDescribeStrings::projectName(), _projectName);
    context.setValue(CxDescribeStrings::projectTitle(), _projectTitle);
}

void CxDescribeProject::fromContext(const CxIGetSkv &context)
{
    _projectName = context.getValue(CxDescribeStrings::projectName(), _projectName);
    _projectTitle = context.getValue(CxDescribeStrings::projectTitle(), _projectTitle);
}

void CxDescribeProject::deleteAndClear()
{
    for (size_t i = 0; i < _catalogAttrs.size(); ++i)
    {
        CatalogAttr * o = _catalogAttrs.at(i);
        if (o->catalog) { delete o->catalog; o->catalog = NULL; }
        delete o;
    }
    _catalogAttrs.clear();
}








void CxDescribeManager::start()
{
    loadDescribeUnits();
    prepareDescribeUnits();
}

void CxDescribeManager::stop()
{
    //不可以删除，删除了UI无得调用
    return;
    for (std::map<std::string, CxDescribeUnit*> ::iterator it = f_describeUnits.begin();  it != f_describeUnits.end(); it++)
    {
        CxDescribeUnit * oUnit = it->second;
        delete oUnit;
    }
    f_describeUnits.clear();
}

const std::map<std::string, CxDescribeUnit*> &  CxDescribeManager::describeUnits()
{
    return f_describeUnits;
}

CxDescribeUnit * CxDescribeManager::addDescribeUnit(const string &sFilePath)
{
    CxDescribeUnit * rDescribeUnit = NULL;
    std::vector<CxDescribeBase *> describes;

    //*->tab unit
    CxSkverBase * oProfile = CxSkverManager::beginUse(sFilePath);
    if (oProfile)
    {
        const std::map<std::string, std::map<std::string, std::string> > & sRows = oProfile->getSectionEntryValues();
        std::string sUnitName =oProfile->getValue(CxDescribeStrings::self(), CxDescribeStrings::unitName(), string());
        CxDescribeUnit * oUnit = findDescribeUnit(sUnitName);
        if (oUnit) return NULL;

        for (map<string, map<string, string> >::const_iterator itRows = sRows.begin(); itRows != sRows.end(); ++itRows)
        {
            //*->tab unit
            const string & sSection = itRows->first;
            if (sSection == CxDescribeStrings::self())
                continue;

            const map<string, string> & sRow = itRows->second;

            string sTypeSimpleName = CxContainer::value(sRow , CxDescribeStrings::typeSimpleName());
            CxDescribeBase * oDescribe = CxDescribeFactoryManager::createObject(sTypeSimpleName);
            if (oDescribe)
            {
                oProfile->prepareGet(sSection);
                oDescribe->fromContext(*oProfile);
                describes.push_back(oDescribe);
            }
            else
            {
                cxPrompt() << "loadUnit warning : no " << CxDescribeStrings::typeSimpleName() << "=" << sTypeSimpleName;
            }

        }
        if (describes.size() > 0)
        {
            rDescribeUnit = new CxDescribeUnit();
            oProfile->prepareGet(CxDescribeStrings::self());
            rDescribeUnit->fromContext(*oProfile);
            rDescribeUnit->setDescribes(describes);
            CxDescribeUnit * oUnit = findDescribeUnit(rDescribeUnit->unitName());
            if (oUnit) delete oUnit;
            f_describeUnits[rDescribeUnit->unitName()] = rDescribeUnit;
        }
        else
        {
            cxPrompt() << "loadUnit fail : can not load describes from" << sFilePath;
        }
        CxSkverManager::endUse(oProfile);
    }
    return rDescribeUnit;
}

CxDescribeUnit *CxDescribeManager::findDescribeUnit(const string &sUnitName)
{
    return CxContainer::value(f_describeUnits, sUnitName, (CxDescribeUnit *)NULL);
}

//[Self]
//[Application1]
//[Application2]
int CxDescribeManager::loadProject(CxDescribeProject &describeProject, const string &sFilePath)
{
    string sSelfPath = CxFileSystem::extractPath(sFilePath);
    string sParentPath = CxFileSystem::parentPath(CxFileSystem::extractPath(sFilePath));
    std::vector<CxCatalogAttr *> describeCatalogs;

//    ProjectName=YGCT_CP
//    ProjectTitle=远光共创通信平台
//    ConfigDataPath=./../
//    DescribePath=./../describe

    //*->application catalog
    CxSkverBase * oProfile = CxSkverManager::beginUse(sFilePath);
    if (oProfile)
    {
        const std::map<std::string, std::map<std::string, std::string> > & sRows = oProfile->getSectionEntryValues();

        const std::map<std::string, std::string> & entryValus = oProfile->getEntryValues(CxDescribeStrings::self());

        string sDescribePath = CxContainer::value(entryValus, CxDescribeStrings::describePath());
        string sConfigDataPath = CxContainer::value(entryValus, CxDescribeStrings::configDataPath() );
        string sProjectName = CxContainer::value(entryValus, CxDescribeStrings::name() );
        string sProjectTitle = CxContainer::value(entryValus, CxDescribeStrings::title());
        if (! CxFileSystem::hasRootPath(sDescribePath) )
            sDescribePath = CxFileSystem::cd(sDescribePath, sSelfPath);
        if (! CxFileSystem::isExist(sDescribePath))
            sDescribePath = sSelfPath;
        if (! CxFileSystem::hasRootPath(sConfigDataPath) )
            sConfigDataPath = CxFileSystem::cd(sConfigDataPath, sSelfPath);
        if (! CxFileSystem::isExist(sConfigDataPath))
            sConfigDataPath = CxFileSystem::cd("config", sParentPath);

        for (map<string, map<string, string> >::const_iterator itRows = sRows.begin(); itRows != sRows.end(); ++itRows)
        {
            //*->CxCatalogAttr application catalog
            const string & sSection = itRows->first;
            if (sSection == CxDescribeStrings::self())
                continue;

            const map<string, string> & sRow = itRows->second;

            string sCatalogFilePath = CxContainer::value(sRow, CxDescribeStrings::catalogFilePath());
            string sCatalogTitle = CxContainer::value(sRow, CxDescribeStrings::title());
            string sFilePath2 = CxFileSystem::fullFilePath(sDescribePath, sCatalogFilePath);

            bool bHas = false;
            for (size_t i = 0; i < describeCatalogs.size(); ++i)
            {
                CxCatalogAttr * o = describeCatalogs.at(i);
                if (o->catalogTitle() == sCatalogTitle)
                {
                    bHas = true;
                    break;
                }
            }
            GM_VALID_BREAK(bHas);

            CxDescribeCatalog * oCatalog = new CxDescribeCatalog();
            if (loadCatalog(* oCatalog, sFilePath2, sDescribePath)>0)
            {
                CxCatalogAttr * oCatalogAttr = new CxCatalogAttr();
                oCatalogAttr->catalog = oCatalog;
                oCatalogAttr->catalogFilePath = sCatalogFilePath;
                describeCatalogs.push_back(oCatalogAttr);
            }
            else
            {
                delete oCatalog;
            }
        }

        if (describeCatalogs.size()>0)
        {
            describeProject._projectName = sProjectName;
            describeProject._projectTitle = sProjectTitle;
            describeProject.setCatalogAttrs(describeCatalogs);
        }
        else
        {
            cxPrompt() << "loadProject fail : can not load catalogs from" << sFilePath;
        }

        CxSkverManager::endUse(oProfile);
    }
    return describeCatalogs.size();
}

int CxDescribeManager::saveProject(CxDescribeProject &describeProject, const string &sFilePath)
{
    const std::vector<CxCatalogAttr*> & catalogAttrs = describeProject.catalogAttrs();
    CxSkverBase * oProfile = CxSkverManager::beginUse(sFilePath, false);
    if (oProfile)
    {
        for (size_t i = 0; i < catalogAttrs.size(); ++i)
        {
            const CxCatalogAttr * oCatalogAttr = catalogAttrs.at(i);
            string sSection = CxString::format("%06d", i);
            oProfile->prepareSet(sSection);
            oProfile->setValue(CxDescribeStrings::catalogFilePath(), oCatalogAttr->catalogFilePath);
        }

        oProfile->prepareSet(CxDescribeStrings::self());
        describeProject.toContext(*oProfile);

        return CxSkverManager::endUse(oProfile, true);
    }
    return 0;
}

//[Catalog1]
//[Catalog2]
int CxDescribeManager::loadCatalog(CxDescribeCatalog &describeCatalog, const string &sFilePath, const std::string & sDescribePath)
{
    std::vector<CxUnitAttr *> describeUnits;

    //*->catalog
    CxSkverBase * oProfile = CxSkverManager::beginUse(sFilePath);
    if (oProfile)
    {
        const std::map<std::string, std::map<std::string, std::string> > & sRows = oProfile->getSectionEntryValues();

        for (map<string, map<string, string> >::const_iterator itRows = sRows.begin(); itRows != sRows.end(); ++itRows)
        {
            //*->[Catalog1]
            const string & sSection = itRows->first;
            if (sSection == CxDescribeStrings::self())
                continue;

            const map<string, string> & sRow = itRows->second;

            std::string sUnitName = CxString::trim(CxContainer::value(sRow, CxDescribeStrings::unitName()));
            std::string sDescribeFilePath = CxContainer::value(sRow, CxDescribeStrings::describeFilePath()) ;
            std::string sUnitCaption = CxString::trim(CxContainer::value(sRow, CxDescribeStrings::unitCaption()));
            std::string sDataType        = CxContainer::value(sRow, CxDescribeStrings::dataType())        ;
            std::string sDataPath        = CxContainer::value(sRow, CxDescribeStrings::dataPath())        ;
            std::string sDataSection     = CxContainer::value(sRow, CxDescribeStrings::dataSection())       ;
            std::string sDataName        = CxContainer::value(sRow, CxDescribeStrings::dataName())       ;
            std::string sDataParam        = CxContainer::value(sRow, CxDescribeStrings::dataParam())       ;
            std::string sDataRewrite        = CxContainer::value(sRow, CxDescribeStrings::dataRewrite())       ;

            CxDescribeUnit * oDescribeUnit = findDescribeUnit(sUnitName);
            if (! oDescribeUnit)
            {
                oDescribeUnit = addDescribeUnit(CxFileSystem::fullFilePath(sDescribePath, sDescribeFilePath));
            }
            if (oDescribeUnit)
            {
                if (sUnitCaption.empty()) sUnitCaption = oDescribeUnit->unitTitle();
                GM_VALID_CONTINUE(describeCatalog.findByCation(sUnitCaption));
                CxUnitAttr * oUnitAttr = new CxUnitAttr();
                oUnitAttr->unit = oDescribeUnit;
                oUnitAttr->describeFilePath = sDescribeFilePath;
                oUnitAttr->unitCaption = sUnitCaption;
                oUnitAttr->dataType = sDataType;
                oUnitAttr->dataPath = sDataPath;
                oUnitAttr->dataSection = sDataSection;
                oUnitAttr->dataName = sDataName;
                oUnitAttr->dataParam = sDataParam;
                oUnitAttr->dataRewrite = CxString::toBoolean(sDataRewrite);
                oUnitAttr->userData = NULL;
                describeUnits.push_back(oUnitAttr);
            }
        }
        if (describeUnits.size()>0)
        {
            oProfile->prepareGet(CxDescribeStrings::self());
            describeCatalog.fromContext(*oProfile);
            describeCatalog.setUnitAttrs(describeUnits);
        }
        else
        {
            cxPrompt() << "loadCatalog fail : can not load units from" << sFilePath;
        }
        CxSkverManager::endUse(oProfile);
    }
    return describeUnits.size();
}

int CxDescribeManager::saveCatalog(CxDescribeCatalog &describeCatalog, const string &sFilePath)
{
    const std::vector<CxUnitAttr *> & describeUnits = describeCatalog.unitAttrs();

    CxSkverBase * oProfile = CxSkverManager::beginUse(sFilePath, false);
    if (oProfile)
    {
        for (size_t i = 0; i < describeUnits.size(); ++i)
        {
            const CxUnitAttr * oUnitAttr = describeUnits.at(i);
            string sSection = CxString::format("%06d", i);
            oProfile->prepareSet(sSection);
            oProfile->setValue(CxDescribeStrings::unitName(), oUnitAttr->unitName());
            oProfile->setValue(CxDescribeStrings::describeFilePath(), oUnitAttr->describeFilePath);
            oProfile->setValue(CxDescribeStrings::unitCaption(), oUnitAttr->unitCaption);
            oProfile->setValue(CxDescribeStrings::dataType(), oUnitAttr->dataType);
            oProfile->setValue(CxDescribeStrings::dataPath(), oUnitAttr->dataPath);
            oProfile->setValue(CxDescribeStrings::dataSection(), oUnitAttr->dataSection);
            oProfile->setValue(CxDescribeStrings::dataName(), oUnitAttr->dataName);
            oProfile->setValue(CxDescribeStrings::dataParam(), oUnitAttr->dataParam);
            oProfile->setValue(CxDescribeStrings::dataRewrite(), oUnitAttr->dataRewrite);
        }

        oProfile->prepareSet(CxDescribeStrings::self());
        describeCatalog.toContext(*oProfile);

        return CxSkverManager::endUse(oProfile, true);
    }
    return 0;
}

//[Self]
//[Describe1]
//[Describe2]
int CxDescribeManager::loadUnit(CxDescribeUnit &describeUnit, const string &sFilePath)
{
    std::vector<CxDescribeBase *> describes;

    //*->tab unit
    CxSkverBase * oProfile = CxSkverManager::beginUse(sFilePath);
    if (oProfile)
    {
        const std::map<std::string, std::map<std::string, std::string> > & sRows = oProfile->getSectionEntryValues();

        for (map<string, map<string, string> >::const_iterator itRows = sRows.begin(); itRows != sRows.end(); ++itRows)
        {
            //*->tab unit
            const string & sSection = itRows->first;
            if (sSection == CxDescribeStrings::self())
                continue;

            const map<string, string> & sRow = itRows->second;

            string sTypeSimpleName = CxContainer::value(sRow , CxDescribeStrings::typeSimpleName());
            CxDescribeBase * oDescribe = CxDescribeFactoryManager::createObject(sTypeSimpleName);
            if (oDescribe)
            {
                oProfile->prepareGet(sSection);
                oDescribe->fromContext(*oProfile);
                describes.push_back(oDescribe);
            }
            else
            {
                cxPrompt() << "loadUnit warning : no " << CxDescribeStrings::typeSimpleName() << "=" << sTypeSimpleName;
            }

        }
        if (describes.size() > 0)
        {
            oProfile->prepareGet(CxDescribeStrings::self());
            describeUnit.fromContext(*oProfile);
            describeUnit.setDescribes(describes);
        }
        else
        {
            cxPrompt() << "loadUnit fail : can not load describes from" << sFilePath;
        }
        CxSkverManager::endUse(oProfile);
    }
    return describes.size();
}

void CxDescribeManager::loadDescribeUnits()
{
    string sFilePath = CxApplication::findConfig(CS_SectionProgramConfig, string("DescribeListFile"), std::string());
    if (sFilePath.size()>0)
    {
        string sDescribeConfigFilePath = CxFileSystem::fullFilePath(CxApplication::applicationDeployPath(), sFilePath);
        string sDescribePath = CxFileSystem::extractPath(sDescribeConfigFilePath);
        CxSkverBase * oProfile = CxSkverManager::beginUse(sDescribeConfigFilePath);
        if (oProfile)
        {
            const std::map<std::string, std::map<std::string, std::string> > & sRows = oProfile->getSectionEntryValues();

            for (map<string, map<string, string> >::const_iterator itRows = sRows.begin(); itRows != sRows.end(); ++itRows)
            {
                //*->CxCatalogAttr application catalog
                const string & sSection = itRows->first;
                if (sSection == CxDescribeStrings::self())
                    continue;

                const map<string, string> & sRow = itRows->second;

                string sDescribeFilePath = CxContainer::value(sRow, CxDescribeStrings::describeFilePath());
                addDescribeUnit(CxFileSystem::fullFilePath(sDescribePath, sDescribeFilePath));
            }
            CxSkverManager::endUse(oProfile);
        }
    }
    if (f_describeUnits.empty())
    {
        string sDescribePath = CxFileSystem::mergeFilePath(CxApplication::applicationDeployPath(), "describe");
        vector<CxFilePathInfo> pathInfos;
        CxFileSystem::scanDir(sDescribePath, pathInfos);
        for (size_t i = 0; i < pathInfos.size(); ++i)
        {
            const CxFilePathInfo & filePathInfo = pathInfos.at(i);
            const string sFileName = filePathInfo.fileName;
            if (CxString::endWith(sFileName, "describe.ini"))
            {
                addDescribeUnit(CxFileSystem::fullFilePath(sDescribePath, filePathInfo.filePath()));
            }
        }
    }
}

void CxDescribeManager::prepareDescribeUnits()
{
    for (std::map<string, CxDescribeUnit* >::const_iterator it = f_describeUnits.begin(); it != f_describeUnits.end(); ++it)
    {
//        const string & sUnitName = it->first;
        CxDescribeUnit* oUnit = it->second;
        prepareDescribeUnit(oUnit);
    }
}

void CxDescribeManager::prepareDescribeUnit(CxDescribeUnit * oUnit)
{
    GM_ASSER_OBJECT(oUnit);

    if (oUnit->localKeyName() == CxDescribeStrings::unitName())
    {
        std::map<std::string, std::string> foreignKeyValues;
        for (std::map<string, CxDescribeUnit* >::const_iterator it = f_describeUnits.begin(); it != f_describeUnits.end(); ++it)
        {
            CxDescribeUnit* oUnit = it->second;
            foreignKeyValues[oUnit->unitName()] = oUnit->unitTitle();
        }
        oUnit->setForeignKeyValues(foreignKeyValues);
    }
}














//****************************************************************************************************************
//***********  Integer Boolean String ...... begin ************************************************************************
//****************************************************************************************************************






















//CxDescribeBooleanFactory* CxDescribeBooleanFactory::s_instance =
//CxDescribeFactoryManager::createAndRegister<CxDescribeBooleanFactory>();

CxDescribeBoolean::CxDescribeBoolean(){

}

CxDescribeBoolean::~CxDescribeBoolean(){

}

void CxDescribeBoolean::toContext(CxISetSkv& context) const
{
    CxDescribeTemplate<bool>::toContext(context);
//    context.setValue(CxDescribeStrings::Max(), _max);
}

void CxDescribeBoolean::fromContext(const CxIGetSkv& context)
{
    CxDescribeTemplate<bool>::fromContext(context);
//    _max = context.getValue(CxDescribeStrings::Max(), _max);
}















//CxDescribeDateTimeFactory* CxDescribeDateTimeFactory::s_instance =
//CxDescribeFactoryManager::createAndRegister<CxDescribeDateTimeFactory>();

CxDescribeDateTime::CxDescribeDateTime(){
    _minValue = CxValueType::minValue<msepoch_t>();
    _maxValue = CxValueType::maxValue<msepoch_t>();
}

CxDescribeDateTime::~CxDescribeDateTime(){

}

void CxDescribeDateTime::toContext(CxISetSkv& context) const
{
    CxDescribeTemplate<msepoch_t>::toContext(context);
    context.setValue(CxDescribeStrings::maxValue(), _maxValue);
    context.setValue(CxDescribeStrings::minValue(), _minValue);
}

void CxDescribeDateTime::fromContext(const CxIGetSkv& context)
{
    CxDescribeTemplate<msepoch_t>::fromContext(context);

    string sMinValue = context.getValue(CxDescribeStrings::minValue(), std::string());
    if (sMinValue.empty())
        _minValue = CxValueType::minValue<msepoch_t>();
    else
        CxString::setFromString(sMinValue, _minValue);

    string sMaxValue = context.getValue(CxDescribeStrings::maxValue(), std::string());
    if (sMaxValue.empty())
        _maxValue = CxValueType::maxValue<msepoch_t>();
    else
        CxString::setFromString(sMaxValue, _maxValue);
}

bool CxDescribeDateTime::checkValueScope(const msepoch_t &value) const
{
    return ( ( value >= _minValue ) && ( value <= _maxValue ) );
}














//CxDescribeDoubleFactory* CxDescribeDoubleFactory::s_instance =
//CxDescribeFactoryManager::createAndRegister<CxDescribeDoubleFactory>();


CxDescribeDouble::CxDescribeDouble()
{
    _minValue = CxValueType::minValue<double>();
    _maxValue = CxValueType::maxValue<double>();
}

CxDescribeDouble::~CxDescribeDouble()
{
}

bool CxDescribeDouble::checkValueScope(const double& value) const
{
    return ( ( value >= _minValue ) && ( value <= _maxValue ) );
}

void CxDescribeDouble::toContext(CxISetSkv& context) const
{
    CxDescribeTemplate<double>::toContext(context);
    context.setValue(CxDescribeStrings::maxValue(), _maxValue);
    context.setValue(CxDescribeStrings::minValue(), _minValue);
}

void CxDescribeDouble::fromContext(const CxIGetSkv& context)
{
    CxDescribeTemplate<double>::fromContext(context);

    string sMinValue = context.getValue(CxDescribeStrings::minValue(), std::string());
    if (sMinValue.empty())
        _minValue = CxValueType::minValue<double>();
    else
        CxString::setFromString(sMinValue, _minValue);

    string sMaxValue = context.getValue(CxDescribeStrings::maxValue(), std::string());
    if (sMaxValue.empty())
        _maxValue = CxValueType::maxValue<double>();
    else
        CxString::setFromString(sMaxValue, _maxValue);
}















//CxDescribeEnumFactory* CxDescribeEnumFactory::s_instance =
//CxDescribeFactoryManager::createAndRegister<CxDescribeEnumFactory>();

CxDescribeEnum::CxDescribeEnum()
{
    _minValue = 0;
    _maxValue = 999;
}

CxDescribeEnum::~CxDescribeEnum(){

}

void CxDescribeEnum::toContext(CxISetSkv& context) const
{
    CxDescribeTemplate<int>::toContext(context);
    context.setValue(CxDescribeStrings::maxValue(), _maxValue);
    context.setValue(CxDescribeStrings::minValue(), _minValue);
}

void CxDescribeEnum::fromContext(const CxIGetSkv& context)
{
    CxDescribeTemplate<int>::fromContext(context);

    string sMinValue = context.getValue(CxDescribeStrings::minValue(), std::string());
    if (sMinValue.empty())
        _minValue = 0;
    else
        CxString::setFromString(sMinValue, _minValue);

    string sMaxValue = context.getValue(CxDescribeStrings::maxValue(), std::string());
    if (sMaxValue.empty())
        _maxValue = 999;
    else
        CxString::setFromString(sMaxValue, _maxValue);
}

bool CxDescribeEnum::checkValueScope(const int& value) const
{
    return ( ( value >= _minValue ) && ( value <= _maxValue ) );
}

void CxDescribeEnum::setEnumCopeFromZero(int iScope)
{
    vector<string> sScopes;
    for (int i = 0; i <= iScope; ++i) {
        sScopes.push_back(CxString::toString(i));
    }
    setValueScopes(sScopes);
    setMaxValue(iScope);
    setMinValue(0);
}














//CxDescribeIntegerFactory* CxDescribeIntegerFactory::s_instance =
//        CxDescribeFactoryManager::createAndRegister<CxDescribeIntegerFactory>();


CxDescribeInteger::CxDescribeInteger()
{
    _minValue = CxValueType::minValue<int>();
    _maxValue = CxValueType::maxValue<int>();
    _isHexDisplay = false;
}

CxDescribeInteger::~CxDescribeInteger()
{
}

void CxDescribeInteger::toContext(CxISetSkv& context) const
{
    CxDescribeTemplate<int>::toContext(context);
    context.setValue(CxDescribeStrings::maxValue(), _maxValue);
    context.setValue(CxDescribeStrings::minValue(), _minValue);
    context.setValue(CxDescribeStrings::isHexDisplay(), _isHexDisplay);
}

void CxDescribeInteger::fromContext(const CxIGetSkv& context)
{
    CxDescribeTemplate<int>::fromContext(context);

    string sMinValue = context.getValue(CxDescribeStrings::minValue(), std::string());
    if (sMinValue.empty())
        _minValue = CxValueType::minValue<int>();
    else
        CxString::setFromString(sMinValue, _minValue);

    string sMaxValue = context.getValue(CxDescribeStrings::maxValue(), std::string());
    if (sMaxValue.empty())
        _maxValue = CxValueType::maxValue<int>();
    else
        CxString::setFromString(sMaxValue, _maxValue);

    _isHexDisplay = context.getValue(CxDescribeStrings::isHexDisplay(), _isHexDisplay);
}

bool CxDescribeInteger::checkValueScope(const int& value) const
{
    return ( ( value >= _minValue ) && ( value <= _maxValue ) );
}

















//CxDescribeStringFactory* CxDescribeStringFactory::s_instance =
//CxDescribeFactoryManager::createAndRegister<CxDescribeStringFactory>();


CxDescribeString::CxDescribeString()
{
    setMinLength(0);
    setMaxLength(255);
}

CxDescribeString::~CxDescribeString()
{
}

void CxDescribeString::toContext(CxISetSkv& context) const
{
    CxDescribeTemplate<string>::toContext(context);
//    context.setValue(CxDescribeStrings::Max(), _max);
}

void CxDescribeString::fromContext(const CxIGetSkv& context)
{
    CxDescribeTemplate<string>::fromContext(context);
    //    _max = context.getValue(CxDescribeStrings::Max(), _max);
}


