
#include <ccxx/ccxx.h>
#include <ccxx/sqlite3.h>

using namespace std;

//#pragma comment(lib, "sqlite3.lib")

int f_argc;
char ** f_argv;

int testQuery1(int argc,char **argv)
{
    int rc,i,ncols;
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char *sql;
    const char*tail;
    // 打开数据
    rc=sqlite3_open("foods.db",&db);
    if(rc){
        fprintf(stderr,"Can'topendatabase:%sn",sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    sql="select * from episodes";
    // 预处理
    rc=sqlite3_prepare(db,sql,(int)strlen(sql),&stmt,&tail);
    if(rc!=SQLITE_OK){
        fprintf(stderr,"SQLerror:%sn",sqlite3_errmsg(db));
    }
    rc=sqlite3_step(stmt);
    ncols=sqlite3_column_count(stmt);
    while(rc==SQLITE_ROW){
        for(i=0;i<ncols;i++){
            fprintf(stderr,"'%s'",sqlite3_column_text(stmt,i));
        }
        fprintf(stderr,"\n");
        rc=sqlite3_step(stmt);
    }
    // 释放 statement
    sqlite3_finalize(stmt);
    // 关闭数据库
    sqlite3_close(db);

    printf("\n");

    return(0);
}



int callback(void* data, int ncols, char** values, char** headers);
int testExec1(int argc, char **argv)
{
    sqlite3 *db;
    int rc;
    char *sql;
    char *zErr;
    char* data;
    rc = sqlite3_open("test.db", &db);
    if(rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    data = "Callback function called";
    sql = "insert into episodes (name, cid) values ('Mackinaw Peaches', 1);"
        "select * from episodes;";
    rc = sqlite3_exec(db, sql, callback, data, &zErr);
    if(rc != SQLITE_OK) {
        if (zErr != NULL) {
            fprintf(stderr, "SQL error: %s\n", zErr);
            sqlite3_free(zErr);
        }
    }
    sqlite3_close(db);
    return 0;
}
int callback(void* data, int ncols, char** values, char** headers)
{
    int i;
    fprintf(stderr, "%s: ", (const char*)data);
    for(i=0; i < ncols; i++) {
        fprintf(stderr, "%s=%s ", headers[i], values[i]);
    }
    fprintf(stderr, "\n");
    return 0;
}


int testCreateTable1()
{
    sqlite3 *db;
    int rc;
    char *sql;
    char *zErr;
    char* data;
    rc = sqlite3_open("test.db", &db);
    if(rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    sql =   "CREATE TABLE [t3] ("
        "[i] BIGINT ,"
        "[dt] UNSIGNED BIGINT,"
        ")"
        ;
    rc = sqlite3_exec(db, sql, NULL, NULL, &zErr);
    if(rc != SQLITE_OK) {
        if (zErr != NULL) {
            fprintf(stderr, "SQL error: %s\n", zErr);
            sqlite3_free(zErr);
        }
    }
    sqlite3_close(db);
    return 0;
}

int testInsert1()
{
    sqlite3 *db;
    int rc;
    char *sql;
    char *zErr;
    char* data;
    rc = sqlite3_open("test.db", &db);
    if(rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    data = "Callback function called";
    sql = "insert into episodes (name, cid) values ('Mackinaw Peaches', 1);"
        "select * from episodes;";
    rc = sqlite3_exec(db, sql, callback, data, &zErr);
    if(rc != SQLITE_OK) {
        if (zErr != NULL) {
            fprintf(stderr, "SQL error: %s\n", zErr);
            sqlite3_free(zErr);
        }
    }
    sqlite3_close(db);
    return 0;
}

const std::string cs_sql_insert_monsb_changed =
    " INSERT INTO [T_MONSB_CHANGED_LOG] ( F_MeasureId, F_Value, F_Quality, F_ChangedTime, F_ChangedSourceId, F_ChangedReasonId )"
        " VALUES ( ? ,? ,? ,? ,? ,? )";

const std::string cs_sql_insert_ycadd_changed =
    " INSERT INTO [T_YCADD_CHANGED_LOG] ( F_MeasureId, F_Value, F_Quality, F_ChangedTime, F_ChangedSourceId, F_ChangedReasonId )"
        " VALUES ( ? ,? ,? ,? ,? ,? )";

const std::string cs_sql_insert_straw_changed =
    " INSERT INTO [T_STRAW_CHANGED_LOG] ( F_MeasureId, F_Value, F_Quality, F_ChangedTime, F_ChangedSourceId, F_ChangedReasonId )"
        " VALUES ( ? ,? ,? ,? ,? ,? )";

char f_sLastError[1024];

bool fn_beginTransaction(sqlite3 *db)
{
    if (! db)
        return false;

    char* err;
    int res = sqlite3_exec(db, "BEGIN", 0, 0, &err);

    if (res == SQLITE_OK)
        return true;

    CxStringC::set(f_sLastError, sizeof(f_sLastError), err);

//    setLastError(QSqlError(tr("Unable to begin transaction"),
//                           QString::fromAscii(err), QSqlError::TransactionError, res));
//    sqlite_freemem(err);
    return false;
}

bool fn_commitTransaction(sqlite3 *db)
{
    if (! db)
        return false;

    char* err;
    int res = sqlite3_exec(db, "COMMIT", 0, 0, &err);

    if (res == SQLITE_OK)
        return true;

    CxStringC::set(f_sLastError, sizeof(f_sLastError), err);

//    setLastError(QSqlError(tr("Unable to commit transaction"),
//                QString::fromAscii(err), QSqlError::TransactionError, res));
//    sqlite_freemem(err);
    return false;
}

bool fn_rollbackTransaction(sqlite3 *db)
{
    if (! db)
        return false;

    char* err;
    int res = sqlite3_exec(db, "ROLLBACK", 0, 0, &err);

    if (res == SQLITE_OK)
        return true;

    CxStringC::set(f_sLastError, sizeof(f_sLastError), err);

//    setLastError(QSqlError(tr("Unable to rollback transaction"),
//                           QString::fromAscii(err), QSqlError::TransactionError, res));
//    sqlite_freemem(err);
    return false;
}

void testInsert2(const string & sSql)
{
    sqlite3 *db;
    int rc;
    char *sql;
    char *zErr;
    char* data;
    rc = sqlite3_open("rtdb.db", &db);
    if(rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    fn_beginTransaction(db);

    sqlite3_stmt *stmt;
    const char*tail;
    rc = sqlite3_prepare(db, sSql.c_str(), sSql.size(), &stmt, &tail);
    if(rc != SQLITE_OK)
    {
        CxStringC::set(f_sLastError, sizeof(f_sLastError), sqlite3_errmsg(db));
        return;
    }
    msepoch_t dtNow = CxTime::currentSystemTime();
    for (size_t i = 0; i < 100*1000; ++i)
    {
        rc = sqlite3_reset(stmt);

        if(rc != SQLITE_OK)
        {
            continue;
        }
        int F_MeasureId = i + 0x010000;
        string F_Value = CxString::format("%dabcdsdfasdfj/.,mnbvcxzasdfghjjkl;'][poiuytreqw12345567788900-=\aZXCVBNM<>?\":LKJHGFDSAQWERTYUIOP{}\\ZXCVBNNM<><>?!@@#$^^&**(()__+%d", i+0x02000000, i+0x03000000);
        int F_Quality = 1;
        long long F_ChangedTime = dtNow;
        int F_ChangedSourceId = 0;
        int F_ChangedReasonId = 0;

        sqlite3_bind_int(stmt, 1, F_MeasureId);
//        sqlite3_bind_int(stmt, 2, F_Value);
        sqlite3_bind_text(stmt, 2, F_Value.c_str(), F_Value.size(), 0);
        sqlite3_bind_int(stmt, 3, F_Quality);
        sqlite3_bind_int64(stmt, 4, F_ChangedTime);
        sqlite3_bind_int(stmt, 5, F_ChangedSourceId);
        sqlite3_bind_int(stmt, 6, F_ChangedReasonId);
//        sqlite3_bind_text(stmt, j + 1, sValue.c_str(), sValue.size(), 0);

        rc = sqlite3_step(stmt);
        if(rc != SQLITE_OK && rc != SQLITE_DONE)
        {
            CxStringC::set(f_sLastError, sizeof(f_sLastError), sqlite3_errmsg(db));
            return;
        }
    }
    sqlite3_finalize(stmt);

    fn_commitTransaction(db);


    sqlite3_close(db);
    return;
}

int main(int argc,char **argv)
{
    cout << "Hello World!" << endl;

    f_argc = argc;
    f_argv = argv;

    msepoch_t dtNow = CxTime::currentSystemTime();

    testInsert2(cs_sql_insert_straw_changed);

    cout << "cost time = " << CxTime::currentSystemTime() - dtNow << " ms" << endl;

    string s;
    cin >> s;
    return 0;
}

