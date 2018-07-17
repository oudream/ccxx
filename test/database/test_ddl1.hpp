
#include <ccxx/ccxx.h>

using namespace std;

int testCreate1()
{
    std::string sTableName = "table1";
    std::map<std::string, int> columns;
    columns["int1"] = CxDatabase::ColumnTypeInt;
    columns["int2"] = CxDatabase::ColumnTypeInt;
    columns["int3"] = CxDatabase::ColumnTypeInt;
    columns["int4"] = CxDatabase::ColumnTypeInt;
    columns["string64"] = CxDatabase::column_type_varchar64;
    CxDatabase::defaultDb()->createTable(sTableName, columns);
    return 0;
}

void testDdl1()
{
    CxDatabase * pDb = CxDatabaseManager::getDefaultDb();

    string sSqlDrop = "DROP TABLE IF EXISTS `table1`;";

    string sSqlCreate = "\
						CREATE TABLE `table1` (\
						`column1` int(11) DEFAULT NULL,\
						`column2` bigint(20) DEFAULT NULL,\
						`column3` double DEFAULT NULL,\
						`column4` text\
						) ENGINE = InnoDB DEFAULT CHARSET = utf8";

    string sSqlInsert = "INSERT INTO db1.table1(column1, column2, column3, column4) VALUES(%d, 1234567890124, 1.246, '2abcde');";

    string sSqlDelete = "DELETE FROM table1 WHERE column1=%d";

    //check sql
    pDb->execSql(sSqlDrop);
    pDb->execSql(sSqlCreate);
    if (pDb->execSql(CxString::format(sSqlInsert.c_str(), 1001)) > 0)
    {
        if (pDb->execSql(CxString::format(sSqlDelete.c_str(), 1001)) <= 0)
        {
            cout << "exit, sSqlDelete execSql error!";
        }
    }
    else
    {
        cout << "exit, sSqlInsert execSql error!";
    }

    while (true)
    {
        for (size_t i = 0; i < 100; i++)
        {
            string sSql2 = CxString::format(sSqlInsert.c_str(), i);
            int iResult = pDb->execSql(sSql2);
            cout << sSql2 << " result=" << iResult;
            cxLogInfo() << sSql2 << " result=" << iResult;
            CxThread::sleep(10);
        }

        for (size_t i = 0; i < 100; i++)
        {
            string sSql2 = CxString::format(sSqlDelete.c_str(), i);
            int iResult = pDb->execSql(sSql2);
            cout << sSql2 << " result=" << iResult;
            cxLogInfo() << sSql2 << " result=" << iResult;
            CxThread::sleep(10);
        }
    }
}

int testInsert1(msepoch_t dtNow)
{
    std::string sTableName = "table1";
    std::vector<std::string> columnNames;
    columnNames.push_back("int1");
    columnNames.push_back("int2");
    columnNames.push_back("int3");
    columnNames.push_back("int4");
    columnNames.push_back("string64");
    std::vector<std::vector<std::string> > rows;
    for (size_t i = 0; i < 10000; ++i)
    {
        vector<string> row;
        for (size_t j = 0; j < columnNames.size(); ++j)
        {
            row.push_back(CxString::toString(i * 100 + j));
        }
        rows.push_back(row);
    }

    cout << "time 0031 : " << CxTime::currentSystemTime() - dtNow << endl;

    return CxDatabase::defaultDb()->saveTable(sTableName, columnNames, rows);
}

int testSelect1(msepoch_t dtNow)
{
    std::string sTableName = "table1";
    std::vector<std::string> columnNames;
//    columnNames.push_back("int1");
//    columnNames.push_back("string64");
    std::vector<std::vector<std::string> > rows;

    int iCount = CxDatabase::defaultDb()->loadTable(sTableName, rows, & columnNames);

    for (size_t i = 0; i < columnNames.size(); ++i)
    {
        cout << "column name : " << columnNames.at(i) << endl;
    }

    for (size_t i = 0; i < rows.size(); ++i)
    {
        const vector<string> & row = rows.at(i);
        string sLine;
        for (size_t j = 0; j < row.size(); ++j)
        {
            sLine += row.at(j) + "  -  ";
        }
        cout << "row " << i << ": " << sLine << endl;
    }

    return iCount;
}

