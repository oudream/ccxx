
//for Ubuntu: -D"OTL_ODBC_UNIX" -lodbc
//sudo apt-get install unixodbc unixodbc-dev
//
//for CentOS: -D"OTL_ODBC_UNIX" -lodbc
//sudo yum install unixODBC-devel



template<typename T1>
class CxTableTemple1
{
public:

};

template<typename T1, typename T2>
class CxTableTemple2
{
public:
    struct DataStruct
    {
        T1 f1;
        T2 f2;
    };

    std::vector<DataStruct>
    load();

    std::vector<DataStruct>
    load(const std::string& sTableName);

    int
    save(const std::vector<DataStruct>& datas);

};

template<typename T1, typename T2, typename T3>
class CxTableTemple3
{
public:
    struct DataStruct
    {
        T1 f1;
        T2 f2;
        T3 f3;
    };

    std::vector<DataStruct>
    load();

    std::vector<DataStruct>
    load(const std::string& sTableName);

    int
    save(const std::vector<DataStruct>& datas);

};

class CxDbDriverBase
{

};

class CxDbDriverSqlite : public CxDbDriverBase
{

};

class CxDbDriverOtl : public CxDbDriverBase
{

};

class CxField
{
public:
    CxField() {}

    const std::string&
    fieldName() const;

    int
    valueType();

    int
    toInt();

    bool
    toBoolean();

};

class CxRow
{
public:
};

class CxTable
{
public:
    const std::vector<CxField>&
    fields() const;

    void* s;
};

class CxDb
{

};
