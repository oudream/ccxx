#ifndef CXDESCRIBE_H
#define CXDESCRIBE_H

#include "cxglobal.h"
#include "cxcontext.h"
#include "cxvalue.h"
#include "cxfactory.h"
#include "cxstring.h"


//DescribeCatalog -> CxDescribeUnit[n]
//CxDescribeUnit -> CxDescribeBase[n]
//CxDescribeBase <- CxDescribeBoolean, CxDescribeInteger, CxDescribeEnum, CxDescribeDateTime, CxDescribeDouble, CxDescribeString ......


class CxDescribeBase;


#define GM_DESCRIBETYPE(BaseClass, eValueType, sTypeFirstName) \
    GM_TYPEFULLNAME(BaseClass, sTypeFirstName) \
    static CxValueType::ValueTypeEnum VALUETYPE() { return eValueType; } \
    CxValueType::ValueTypeEnum valueType() const { return eValueType; }

#define GM_DESCRIBETYPE_BASE(sTypeBaseName) \
    GM_TYPEFULLNAME_BASE(sTypeBaseName) \
    virtual CxValueType::ValueTypeEnum valueType() const = 0;


class CxDescribeStrings
{
public:
    static std::string describe ()              {return"Describe";}
    static std::string describes()              {return"Describes";}
    static std::string name ()                  {return"Name";}
    static std::string columnIndex ()           {return"ColumnIndex";}
    static std::string title ()                 {return"Title";}
    static std::string use ()                   {return"Use";}
    static std::string desc ()                  {return"Desc";}
    static std::string canNull ()               {return"CanNull";}
    static std::string isVisible ()             {return"IsVisible";}
    static std::string isReadOnly ()            {return"IsReadOnly";}
    static std::string minLength ()             {return"MinLength";}
    static std::string maxLength ()             {return"MaxLength";}
    static std::string regularExpression ()     {return"RegularExpression";}
    static std::string valueScopes()            {return"ValueScopes";}
    static std::string foreignKey()             {return"ForeignKey";}
    static std::string defaultValue ()          {return"DefaultValue";}
    static std::string minValue ()              {return"MinValue";}
    static std::string maxValue ()              {return"MaxValue";}
    static std::string isHexDisplay ()          {return"IsHexDisplay";}
    static std::string enumString ()            {return"Enum";}
    static std::string self()                   {return"Self";}
    static std::string typeSimpleName()         {return"TypeSimpleName";}
    static std::string dataFilePath()           {return"DataFilePath";}
    static std::string unitName()               {return"UnitName";}
    static std::string unitTitle()              {return"UnitTitle";}
    static std::string unitParam()              {return"UnitParam";}
    static std::string unitCaption()              {return"UnitCaption";}
    static std::string describeFilePath()       {return"DescribeFilePath";}
    static std::string foreignKeyValues()       {return"ForeignKeyValues";}

    static std::string localKeyName()           {return"LocalKeyName";}
    static std::string localCaptionName()       {return"LocalCaptionName";}
    static std::string filterName()             {return"FilterName";}
    static std::string filterValue()            {return"FilterValue";}
    static std::string partitionName()          {return"PartitionName";}

    static std::string configDataPath()         {return"ConfigDataPath";}
    static std::string describePath()           {return"DescribePath";}

    static std::string catalogFilePath()        {return"CatalogFilePath";}
    static std::string catalogName()            {return"CatalogName";}
    static std::string catalogTitle()           {return"CatalogTitle";}
    static std::string catalogParam()           {return"CatalogParam";}

    static std::string projectName()            {return"ProjectName";}
    static std::string projectTitle()           {return"ProjectTitle";}

//    static std::string title()                {return"Title";}
    static std::string dataType()               {return"DataType";}
    static std::string dataPath()               {return"DataPath";}
    static std::string dataSection()            {return"DataSection";}
    static std::string dataName()               {return"DataName";}
    static std::string dataParam()              {return"DataParam";}
    static std::string dataRewrite()            {return"DataRewrite";}

};



class CxDescribeFactoryBase : public CxFactoryTemplate<CxDescribeBase>
{

};


class CxDescribeBase
{
public:
    static std::vector<CxFactoryTemplate<CxDescribeBase>*>* factoriesContainer();

    static void factoriesCreateAndRegister();

    GM_DESCRIBETYPE_BASE( CxDescribeStrings::describe() )

public:

    CxDescribeBase();

    virtual ~CxDescribeBase();

    virtual void toContext(CxISetSkv& context) const ;

    virtual void fromContext(const CxIGetSkv& context);

    /// Name 就是 Section
    inline std::string name() const { return _name; }
    inline void setName(const std::string& value) { _name = value; }

    virtual std::string valueToString() const  = 0;
    virtual void valueFromString(const std::string& value) = 0;

    virtual std::string defaultValueToString() const = 0;

    inline int columnIndex() const { return _columnIndex; }
    inline void setColumnIndex(int value) { _columnIndex = value; }

    inline std::string title() const { return _title; }
    inline void setTitle(const std::string& value) { _title = value; }

    inline std::string desc() const { return _desc; }
    inline void setDesc(const std::string& value) { _desc = value; }

    inline bool canNull() const { return _canNull; }
    inline void setCanNull(const bool& value) { _canNull = value; }

    inline bool isVisible() const { return _isVisible; }
    inline void setIsVisible(const bool& value) { _isVisible = value; }

    inline bool isReadOnly() const { return _isReadOnly; }
    inline void setIsReadOnly(const bool& value) { _isReadOnly = value; }

    inline int maxLength() const { return _maxLength; }
    inline void setMaxLength(int value) { _maxLength = value; }

    inline int minLength() const { return _minLength; }
    inline void setMinLength(int value) { _minLength = value; }

    inline std::string regularExpression() const { return _regularExpression; }
    inline void setRegularExpression(const std::string& value) { _regularExpression = value; }

    inline const std::vector<std::string>& valueScopes() const { return _valueScopes; }
    inline void setValueScopes(const std::vector<std::string>& value) { _valueScopes = value; }

    inline const std::string & foreignKey() const { return _foreignKey; }
    inline void setForeignKey(const std::string& value) { _foreignKey = value; }

    virtual void assignedFrom(const CxDescribeBase * oDescribe);

    bool isValidValue(const std::string& value) const
    {
        if (value.empty())
        {
            return _canNull;
        }
        else
        {
            if ((value.size() >= _minLength) && (value.size() <= _maxLength))
            {
                return checkValueImple(value);
            }
            else
            {
                return false;
            }
        }
    }

    bool isValidValue() const { return checkValueImple(); }

protected:
    virtual bool checkValueImple(const std::string& value) const = 0;

    virtual bool checkValueImple() const = 0;

private:
    std::string                 _name;
    int                         _columnIndex;
    std::string                 _title;
    std::string                 _desc;
    bool                        _canNull;
    bool                        _isVisible;
    bool                        _isReadOnly;
    size_t                      _maxLength;
    size_t                      _minLength;
    std::string                 _regularExpression;
    std::vector<std::string>    _valueScopes;
    std::string                 _foreignKey;

};




class CxDescribeFactoryManager : public CxFactoryManagerTemplate<CxDescribeBase>
{
};








class CxDescribeUnit;


typedef std::string (*GFUNC_DescribeDefaultValue)(CxDescribeUnit* oDescribeUnit, const CxDescribeBase* oDescribe, int iRowIndex);

typedef void (*GFUNC_DescribeValuesCalc)(const CxDescribeUnit* oDescribeUnit);


class CxDescribeUnit
{
public:
    static CxDescribeUnit* createDescribes(const std::string& sFilePath);

    static bool saveDescribes(const CxDescribeUnit& describeUnit, const std::string& sFilePath);

public:
    CxDescribeUnit();

    ~CxDescribeUnit();

    inline const std::vector<CxDescribeBase*> & describes() const { return  _describes; }
    inline void setDescribes(const std::vector<CxDescribeBase*> & value);

    inline const std::string & unitName() const { return _unitName; }
    inline void setUnitName(const std::string& value) { _unitName = value; }

    inline const std::string & unitTitle() const { return _unitTitle; }
    inline void setUnitTitle(const std::string& value) { _unitTitle = value; }

    inline const std::string & unitParam() const { return _unitParam; }
    void setUnitParam(const std::string& value);

    inline const std::map<std::string, std::string>& unitParams() const { return _unitParams; }
    void setUnitParams(const std::map<std::string, std::string>& value);

    //foreign Key
    inline const std::map<std::string, std::string>& foreignKeyValues() const { return _foreignKeyValues; }
    inline void setForeignKeyValues(const std::map<std::string, std::string>& value) { _foreignKeyValues = value; }

    //local key describe name
    inline const std::string& localKeyName() const { return _localKeyName; }
    inline void setLocalKeyName(const std::string& value) { _localKeyName = value; }

    //local key's title ( caption ) describe name
    inline const std::string& localCaptionName() const { return _localCaptionName; }
    inline void setLocalCaptionName(const std::string& value) { _localCaptionName = value; }

    //filter
    inline const std::string& filterName() const { return _filterName; }
    inline void setFilterName(const std::string& value) { _filterName = value; }

    inline const std::string& filterValue() const { return _filterValue; }
    inline void setFilterValue(const std::string& value) { _filterValue = value; }

    //group partition
    inline const std::string& partitionName() const { return _partitionName; }
    inline void setPartitionName(const std::string& value) { _partitionName = value; }

    inline void setFuncDescribeDefaultValue(GFUNC_DescribeDefaultValue value) { _funcDescribeDefaultValue = value; }

    inline std::string describeDefaultValue(CxDescribeUnit* oDescribeUnit, const CxDescribeBase* oDescribe, int iRowIndex) {
        if (_funcDescribeDefaultValue)
            return _funcDescribeDefaultValue(oDescribeUnit, oDescribe, iRowIndex);
        return std::string(); }

    inline void setFuncDescribeValuesCalc(GFUNC_DescribeValuesCalc value) { _funcDescribeValuesCalc = value; }

    int describeValuesCalc(const CxDescribeUnit* oDescribeUnit) const;

    void restoreDefaultValues() const ;

    void assignedFrom(const CxDescribeUnit & oDescribeUnit);

    void assignedFrom(const CxDescribeUnit * oDescribeUnit);

    CxDescribeBase * describe(const std::string& sDescribeName) const ;

    CxDescribeBase * describe(int iColumnIndex) const ;

    template<class DescribeSonClass>
    inline DescribeSonClass * describeSon(const std::string& sDescribeName) const
    {
        for (size_t i = 0; i < _describes.size(); ++i)
        {
            CxDescribeBase* oDescribe = _describes.at(i);
            if (oDescribe->name() == sDescribeName)
            {
                if ( oDescribe->typeSimpleName() == DescribeSonClass::TYPESIMPLENAME() )
                {
                    return reinterpret_cast<DescribeSonClass*>(oDescribe);
                }
            }
        }
        return NULL;
    }

    template<class DescribeSonClass>
    inline DescribeSonClass * describeSon(int iColumnIndex) const
    {
        for (size_t i = 0; i < _describes.size(); ++i)
        {
            CxDescribeBase* oDescribe = _describes.at(i);
            if (oDescribe->columnIndex() == iColumnIndex)
            {
                if ( oDescribe->typeSimpleName() == DescribeSonClass::TYPESIMPLENAME() )
                {
                    return reinterpret_cast<DescribeSonClass*>(oDescribe);
                }
            }
        }
        return NULL;
    }

    template<class DescribeSonClass>
    DescribeSonClass* createAndRegister(const std::string& sDescribeName, size_t iColumnIndex = CxGlobal::npos)
    {
        DescribeSonClass* oDescribe = new DescribeSonClass();
        oDescribe->setName(sDescribeName);
        oDescribe->setTitle(sDescribeName);
        oDescribe->setDesc(sDescribeName);
        if (iColumnIndex != CxGlobal::npos && iColumnIndex < _describes.size()) {
            _describes.insert(_describes.begin() + iColumnIndex, oDescribe);
        } else {
            _describes.push_back(oDescribe);
        }
        return oDescribe;
    }

    /**
     * @brief loadValues
     * @param sDataName    == fileName tableName
     * @param sDataSection == section  id
     * @param sDataPath    == path     database
     * @param sDataType    == xml sqlite mysql ini
     * @return
     */
    int loadValues(const std::string &sDataName, const std::string &sDataSection = std::string(), const std::string &sDataPath = std::string(), const std::string &sDataType = std::string()) const;

    int saveValues(const std::string &sDataName, const std::string &sDataSection, bool bSave = true, const std::string &sDataPath = std::string(), const std::string &sDataType = std::string()) const;

protected:
    void toContext(CxISetSkv& context) const ;

    void fromContext(const CxIGetSkv& context);

private:
    void deleteAndClear();

private:
    std::vector<CxDescribeBase*> _describes;
    std::string _unitName;
    std::string _unitTitle;
    std::string _unitParam;
    std::map<std::string, std::string> _unitParams;
    std::map<std::string, std::string> _foreignKeyValues;
    std::string _localKeyName;
    std::string _localCaptionName;
    std::string _filterName;
    std::string _filterValue;
    std::string _partitionName;
    GFUNC_DescribeDefaultValue _funcDescribeDefaultValue;
    GFUNC_DescribeValuesCalc _funcDescribeValuesCalc;

    friend class CxDescribeCatalog;
    friend class CxDescribeManager;

};










class CxDescribeCatalog
{
public:
    struct UnitAttr
    {
        std::string describeFilePath;
        CxDescribeUnit * unit;
        inline std::string unitTitle() const { return unit ? unit->unitTitle() : std::string(); }
        inline std::string unitName() const { return unit ? unit->unitName() : std::string(); }
        std::string unitCaption;
        std::string dataType;
        std::string dataPath;            //path     database
        std::string dataSection;         //section  id
        std::string dataName;            //filename tablename
        std::string dataParam;            //
        bool        dataRewrite;            //
        void * userData;
    };

    CxDescribeCatalog();

    ~CxDescribeCatalog();

    inline const std::vector<UnitAttr*> & unitAttrs() const { return _unitAttrs; }
    void setUnitAttrs(const std::vector<UnitAttr*> & value);

    inline const std::string & catalogName() const { return _catalogName; }
    inline void catalogName(const std::string & value) { _catalogName = value; }

    inline const std::string & catalogTitle() const { return _catalogTitle; }
    inline void catalogTitle(const std::string & value) { _catalogTitle = value; }

    inline const std::string & catalogParam() const { return _catalogParam; }
    void setCatalogParam(const std::string& value);

    inline const std::map<std::string, std::string>& catalogParams() const { return _catalogParams; }
    void setCatalogParams(const std::map<std::string, std::string>& value);

    UnitAttr * findByCation(const std::string & sUnitCaption) const;

    UnitAttr * findByData(const std::string & sDataName, const std::string & sDataSection) const;

    UnitAttr * findByData(const std::string &sDataName, const std::string &sDataSection, const std::string &sDataPath) const;

    std::vector<UnitAttr*> getByData(const std::string & sDataName) const;

    std::vector<UnitAttr*> getByData(const std::string & sDataName, const std::string & sDataSection) const;

    UnitAttr * addUnitAttr(CxDescribeUnit * oUnit, const std::string & sDescribeFilePath = std::string());

    void setUnitAttrsDataName(const std::string & sDataName);

protected:
    void toContext(CxISetSkv& context) const ;

    void fromContext(const CxIGetSkv& context);

private:
    void deleteAndClear();

private:
    std::vector<UnitAttr*> _unitAttrs;
    std::string _catalogName;
    std::string _catalogTitle;
    std::string _catalogParam;
    std::map<std::string, std::string> _catalogParams;

    friend class CxDescribeProject;
    friend class CxDescribeManager;

};
typedef CxDescribeCatalog::UnitAttr CxUnitAttr;








class CxDescribeProject
{
public:
    struct CatalogAttr {
        CxDescribeCatalog * catalog;
        inline std::string catalogName() const { return catalog ? catalog->catalogName() : std::string(); }
        inline std::string catalogTitle() const { return catalog ? catalog->catalogTitle() : std::string(); }
        inline std::string catalogParam() const { return catalog ? catalog->catalogParam() : std::string(); }
        inline const std::map<std::string, std::string> & catalogParams() const { return catalog ? catalog->catalogParams() : CxGlobal::emptyMapString; }
        std::string catalogFilePath;
    };

    CxDescribeProject();

    ~CxDescribeProject();

    const std::vector<CatalogAttr*> & catalogAttrs() const { return _catalogAttrs; }
    void setCatalogAttrs(const std::vector<CatalogAttr*> & value);

    CatalogAttr * findByName(const std::string & sName) const;

    CatalogAttr * findByTitle(const std::string & sTitle) const;

    CatalogAttr * findByFilePath(const std::string & sFilePath) const;

    const std::string & projectName() const { return _projectName; }

    const std::string & projectTitle() const { return _projectTitle; }

    CatalogAttr * addCatalogAttr(const std::string & sCatalogName, const std::string & sCatalogTitle, const std::string & sCatalogUse, const std::string & sCatalogFilePath);

    void deleteCatalogAttr(const std::string & sCatalogName, const std::string & sCatalogTitle);

    void deleteCatalogAttr(CatalogAttr * oCatalogAttr);

protected:
    void toContext(CxISetSkv& context) const ;

    void fromContext(const CxIGetSkv& context);

private:
    void deleteAndClear();

private:
    std::vector<CatalogAttr*> _catalogAttrs;
    std::string _projectName;
    std::string _projectTitle;

    friend class CxDescribeManager;

};
typedef CxDescribeProject::CatalogAttr CxCatalogAttr;





class CxDescribeManager
{
public:
    static void start( );

    static void stop( );


    static const std::map<std::string, CxDescribeUnit*> & describeUnits();

    static CxDescribeUnit * addDescribeUnit(const std::string & sFilePath);

    static CxDescribeUnit * findDescribeUnit(const std::string & sUnitName);


    //*file load
    static int loadProject(CxDescribeProject & describeProject, const std::string & sFilePath);

    static int saveProject(CxDescribeProject & describeProject, const std::string & sFilePath);

    static int loadCatalog(CxDescribeCatalog & describeCatalog, const std::string & sFilePath, const std::string & sDescribePath);

    static int saveCatalog(CxDescribeCatalog & describeCatalog, const std::string & sFilePath);

    static int loadUnit(CxDescribeUnit & describeUnit, const std::string & sFilePath);


private:
    static void loadDescribeUnits();

    static void prepareDescribeUnits();

    static void prepareDescribeUnit(CxDescribeUnit *oUnit);
};









template<typename T>
class CxDescribeTemplate : public CxDescribeBase
{

    GM_DESCRIBETYPE(CxDescribeBase, CxValueType::id<T>(), CxValueType::simpleName<T>())

    public:
        CxDescribeTemplate(){
        _tDefault = CxValueType::defaultValue<T>();
        _t = _tDefault;
    }

    void toContext(CxISetSkv& context) const {
        CxDescribeBase::toContext(context);
        context.setValue(CxDescribeStrings::defaultValue(), _tDefault);
    }

    void fromContext(const CxIGetSkv& context){
        CxDescribeBase::fromContext(context);
        _tDefault = context.getValue(CxDescribeStrings::defaultValue(), _tDefault);
    }

    inline const T& value() const { return _t; }
    inline void setValue(const T& t) { _t = t; }

    std::string valueToString() const {
        return CxString::toString(_t);
    }

    void valueFromString(const std::string &value)
    {
        if (CxString::isvalid(value, _t))
            CxString::setFromString(value, _t);
    }

    std::string defaultValueToString() const {
        return CxString::toString(_tDefault);
    }

    inline const T& defaultValue() const { return _tDefault; }
    inline void setDefaultValue(const T& t) { _tDefault = t; }

    inline void initValue(const T& t) { _t = t; _tDefault = t; }

    void assignedFrom(const CxDescribeBase * oDescribe) {
        CxDescribeBase::assignedFrom(oDescribe);
        const CxDescribeTemplate<T> * oDescribeTemplate = reinterpret_cast<const CxDescribeTemplate<T> *>(oDescribe);
        _t = oDescribeTemplate->_t;
        _tDefault = oDescribeTemplate->_tDefault;
        doAssignedFrom(oDescribe);
    }

protected:
    virtual void doAssignedFrom(const CxDescribeBase * oDescribe) {}

    bool checkValueImple(const std::string& value) const {
        T t;
        if (CxString::isvalid(value, t))
        {
            bool bOk;
            t = CxString::fromString(value, t, &bOk);
            if (bOk)
                return checkValueScope(t);
            else
                return false;
        }
        else
        {
            return false;
        }
    }

    bool checkValueImple() const { return checkValueScope(_t); }

protected:
    virtual bool checkValueScope(const T& t) const { return true; }

private:
    T _t;
    T _tDefault;

};

template<typename T>
class CxDescribeTemplateFactory : public CxDescribeFactoryBase
{
public:
    std::string factoryName(){
        return CxValueType::simpleName<T>();
    }
};















class CxDescribeBoolean : public CxDescribeTemplate<bool>
{
public:
    CxDescribeBoolean();

    ~CxDescribeBoolean();

    void toContext(CxISetSkv& context) const ;

    void fromContext(const CxIGetSkv& context);

};

class CxDescribeBooleanFactory : public CxDescribeTemplateFactory<bool>
{
//    GM_SINGLETON_DECLARATION(CxDescribeBooleanFactory)
public:
    CxDescribeBase* createObject() {
        return new CxDescribeBoolean();
    }
};

















class CxDescribeDateTime : public CxDescribeTemplate<msepoch_t>
{
public:
    CxDescribeDateTime();

    ~CxDescribeDateTime();

    void toContext(CxISetSkv& context) const ;

    void fromContext(const CxIGetSkv& context);

    inline const msepoch_t& maxValue() const { return _maxValue; }
    inline void setMaxValue(msepoch_t value) { _maxValue = value; }

    inline const msepoch_t& minValue() const { return _minValue; }
    inline void setMinValue(msepoch_t value) { _minValue = value; }

protected:
    void doAssignedFrom(const CxDescribeBase * oDescribe) {
        const CxDescribeDateTime * oDescribe2 = reinterpret_cast<const CxDescribeDateTime *>(oDescribe);
        _maxValue = oDescribe2->_maxValue;
        _minValue = oDescribe2->_minValue;
    }

    bool checkValueScope(const msepoch_t& value) const ;

private:
    msepoch_t _maxValue;
    msepoch_t _minValue;

};

class CxDescribeDateTimeFactory : public CxDescribeTemplateFactory<msepoch_t>
{
//    GM_SINGLETON_DECLARATION(CxDescribeDateTimeFactory)
public:
    virtual CxDescribeBase* createObject(){
        return new CxDescribeDateTime();
    }
};














class CxDescribeDouble : public CxDescribeTemplate<double>
{

public:

    CxDescribeDouble();
    ~CxDescribeDouble();

    void toContext(CxISetSkv& context) const ;

    void fromContext(const CxIGetSkv& context);

    inline double maxValue() const { return _maxValue; }
    inline void setMaxValue(double value) { _maxValue = value; }

    inline double minValue() const { return _minValue; }
    inline void setMinValue(double value) { _minValue = value; }

protected:
    void doAssignedFrom(const CxDescribeBase * oDescribe) {
        const CxDescribeDouble * oDescribe2 = reinterpret_cast<const CxDescribeDouble *>(oDescribe);
        _maxValue = oDescribe2->_maxValue;
        _minValue = oDescribe2->_minValue;
    }

    bool checkValueScope(const double& value) const;

private:
    double _maxValue;
    double _minValue;

};

class CxDescribeDoubleFactory : public CxDescribeTemplateFactory<double>
{
//    GM_SINGLETON_DECLARATION(CxDescribeDoubleFactory)
public:
    virtual CxDescribeBase* createObject(){
        return new CxDescribeDouble();
    }
};



















class CxDescribeEnum : public CxDescribeTemplate<int>
{

    GM_DESCRIBETYPE(CxDescribeBase, CxValueType::ValueType_Enum, CxDescribeStrings::enumString())

public:
    CxDescribeEnum();

    ~CxDescribeEnum();

    void toContext(CxISetSkv& context) const ;

    void fromContext(const CxIGetSkv& context);

    inline int maxValue() const { return _maxValue; }
    inline void setMaxValue(int value) { _maxValue = value; }

    inline int minValue() const { return _minValue; }
    inline void setMinValue(int value) { _minValue = value; }

    void setEnumCopeFromZero(int iScope);

protected:
    void doAssignedFrom(const CxDescribeBase * oDescribe) {
        const CxDescribeEnum * oDescribe2 = reinterpret_cast<const CxDescribeEnum *>(oDescribe);
        _maxValue = oDescribe2->_maxValue;
        _minValue = oDescribe2->_minValue;
    }

    bool checkValueScope(const int& value) const;

private:
    int _maxValue;
    int _minValue;

};

class CxDescribeEnumFactory : public CxDescribeFactoryBase
{
//    GM_SINGLETON_DECLARATION(CxDescribeEnumFactory)
public:
    std::string factoryName(){
        return CxDescribeStrings::enumString();
    }
    virtual CxDescribeBase* createObject(){
        return new CxDescribeEnum();
    }
};














class CxDescribeInteger : public CxDescribeTemplate<int>
{

public:

    CxDescribeInteger();
    ~CxDescribeInteger();

    void toContext(CxISetSkv& context) const ;

    void fromContext(const CxIGetSkv& context);

    inline int maxValue() const { return _maxValue; }
    inline void setMaxValue(int value) { _maxValue = value; }

    inline int minValue() const { return _minValue; }
    inline void setMinValue(int value) { _minValue = value; }

    inline void SetRang(int iMin, int iMax) { _minValue = iMin, _maxValue = iMax; }

    inline bool IsHexDisplay() const { return _isHexDisplay; }
    inline void SetIsHexDisplay(bool value) { _isHexDisplay = value; }

protected:
    void doAssignedFrom(const CxDescribeBase * oDescribe) {
        const CxDescribeInteger * oDescribe2 = reinterpret_cast<const CxDescribeInteger *>(oDescribe);
        _maxValue = oDescribe2->_maxValue;
        _minValue = oDescribe2->_minValue;
        _isHexDisplay = oDescribe2->_isHexDisplay;
    }

    bool checkValueScope(const int& value) const;

private:
    int _maxValue;
    int _minValue;
    bool _isHexDisplay;

};

class CxDescribeIntegerFactory : public CxDescribeTemplateFactory<int>
{
//    GM_SINGLETON_DECLARATION(CxDescribeIntegerFactory)
public:
    virtual CxDescribeBase* createObject(){
        return new CxDescribeInteger();
    }
};















class CxDescribeString : public CxDescribeTemplate<std::string>
{

public:

    CxDescribeString();

    ~CxDescribeString();

    void toContext(CxISetSkv& context) const ;

    void fromContext(const CxIGetSkv& context);

};

class CxDescribeStringFactory : public CxDescribeTemplateFactory<std::string>
{
//    GM_SINGLETON_DECLARATION(CxDescribeStringFactory)
public:
    virtual CxDescribeBase* createObject(){
        return new CxDescribeString();
    }
};

















#endif // CXDESCRIBE_H
