#ifndef CXBUSINESS_H
#define CXBUSINESS_H


#include "cxglobal.h"


const std::string CS_SectionMsObjectTypes = "MsObjectTypes";


//表描述
class CxMsObjectType
{
public:
    enum StructType_Enum {StructType_None, StructType_Simple, StructType_Main};

    inline CxMsObjectType() {
        _howLongStore = 0;
    }
    inline CxMsObjectType(const std::string & typeName, StructType_Enum structType, const std::vector<std::string> & majorKeies, const std::string & detailKey, timems_t msHowLongStore) {
        _typeName = typeName;
        _structType = structType;
        _majorKeies = majorKeies;
        _detailKey = detailKey;
        _howLongStore = msHowLongStore;
    }
    inline CxMsObjectType(const CxMsObjectType & other) {
        assignFrom(other);
    }
    inline CxMsObjectType &operator=(const CxMsObjectType &other)
    {
        if (this != &other)
        {
            assignFrom(other);
        }
        return *this;
    }
    inline void assignFrom(const CxMsObjectType & other) {
        _typeName = other._typeName;
        _structType = other._structType;
        _majorKeies = other._majorKeies;
        _detailKey = other._detailKey;
        _howLongStore = other._howLongStore;
    }

    const std::string & typeName() const { return _typeName; }

    StructType_Enum structType() const { return _structType; }

    const std::vector<std::string> & majorKeies() const { return _majorKeies; }

    const std::string & detailKey() const { return _detailKey; }

private:
    std::string _typeName;
    StructType_Enum _structType; //表示这种内存对象的结构类型 main表示有从表
    std::vector<std::string> _majorKeies; //主表的字段名称
    std::string _detailKey; //从表的关键字段名称
    msepoch_t _howLongStore; //表示这种类型是否长久住内存

    friend class CxMsObjectTypeManager;

};

class GM_CCXX_CORE_API CxMsObjectTypeManager
{
public:
    static void start();

    static int load();

    static const CxMsObjectType * find(const std::string & sTypeName);

    static std::string name(const std::string & sTypeName, const std::map<std::string, std::string> & major);

    static bool isTempObject(const std::string & sTypeName);

};


//通用的业务对象
class GM_CCXX_CORE_API CxMsObjectData
{
public:
    inline CxMsObjectData() : _majorModify(false), _objectType(NULL) {}
    inline CxMsObjectData(const std::string & sTypeName, const std::string & sName, const std::map<std::string, std::string> & major) : _majorModify(false), _objectType(NULL) {
        _typeName = sTypeName;
        _name = sName;
        _major = major;
    }
    inline CxMsObjectData(const std::string & sTypeName, const std::string & sName, const std::map<std::string, std::string> & major, const std::map<std::string, std::map<std::string, std::string> > & details) : _majorModify(false), _objectType(NULL) {
        _typeName = sTypeName;
        _name = sName;
        _major = major;
        _details = details;
    }
    inline CxMsObjectData(const std::string & sText) : _majorModify(false), _objectType(NULL) {
        fromString(sText);
    }
    inline CxMsObjectData(const CxMsObjectData & other) : _majorModify(false), _objectType(NULL) {
        assignFrom(other);
    }
    inline CxMsObjectData &operator=(const CxMsObjectData &other)
    {
        if (this != &other)
        {
            assignFrom(other);
        }
        return *this;
    }
    inline void assignFrom(const CxMsObjectData & other) {
        _typeName = other._typeName;
        _name = other._name;
        _major = other._major;
        _details = other._details;
    }

    inline const std::string & typeName() const { return _typeName; }

    const CxMsObjectType * getMsObjectType() const;

    inline const std::string & name() const { return _name; }

    inline const std::map<std::string, std::string> & major() const { return _major; }

    inline const std::map<std::string, std::map<std::string, std::string> > & details() const { return _details; }

    inline bool majorModify() const { return _majorModify; }

    inline const std::map<std::string, int> & detailModifies() const { return _detailModifies; }

    std::string toString() const;

    int fromString(const std::string & sText);

    int updateMajor(const std::map<std::string, std::string> & major);

    int updateDetails(const std::map<std::string, std::map<std::string, std::string> > & details);

private:
    std::string _typeName; //类型名称
    std::string _name; //主键值
    std::map<std::string, std::string> _major; //主表内容
    std::map<std::string, std::map<std::string, std::string> > _details; //从表内容
    bool _majorModify;
    std::map<std::string, int> _detailModifies;
    int _source;
    CxMsObjectType * _objectType;

private:
    inline void setTypeName(const std::string & value) { _typeName = value; }
    inline void setName(const std::string & value) { _name = value; }

    friend class CxMsObjectDataManager;

};

class CxIMsObjectDataSubject
{
protected:
    virtual void msObject_changed(const CxMsObjectData & oMsObject, int iChangeType) = 0;

    virtual void msObjects_changed(const std::string & sTypeName, const std::vector<std::string> & sNames) = 0;

    friend class CxMsObjectDataManager;

};


class GM_CCXX_CORE_API CxMsObjectDataManager
{
public:
    static void addObserver(CxIMsObjectDataSubject * oSubject);

    static void removeObserver(CxIMsObjectDataSubject* oSubject);

    static void beginReceived(const std::string & sTypeName, int iSource = 0);

    static int receivedObject(const std::string & sTypeName, const std::string & sName, const std::map<std::string, std::string> & major);

    static int receivedObject(const std::string & sTypeName, const std::string & sName, const std::map<std::string, std::string> & major, const std::map<std::string, std::map<std::string, std::string> > & details);

    static int receivedObject(const CxMsObjectData * oMsObject, bool bReplace = false);

    static void endReceived();

    static const std::vector<CxMsObjectData> & msObjects();

    static std::vector<const CxMsObjectData *> findObjects(const std::string & sTypeName);

    static std::vector<const CxMsObjectData *> findObjects(const std::string & sTypeName, const std::vector<std::string> & sNames);

    static const CxMsObjectData * find(const std::string & sTypeName, const std::string & majorKey, const std::string & majorValue);

    static int saveMsObjects(const std::string & sFilePath);

private:
    static CxMsObjectData * find(const std::string & sTypeName, const std::string & sName);

};


#endif // CXBUSINESS_H
