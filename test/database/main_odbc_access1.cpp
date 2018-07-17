#include <ccxx/ccxx.h>
#include <iostream>

using namespace std;

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define OTL_ODBC // Compile OTL 4.0/ODBC
//#define ODBCVER 0x0250 // ODBC Version # needs to be downgraded
// to 2.5 because the SQLite ODBC driver seems
// to run slower when ODBC 3.x functions
// used (performance is not as good as with
// ODBC 2.5 function calls)
// #define OTL_ODBC_UNIX // uncomment this line if UnixODBC is used
#include <ccxx/otlv4.h> // include the OTL 4.0 header file

otl_connect db; // connect object

void insert()
// insert rows into table
{
    otl_stream
            o(50, // stream buffer size
              "insert into test_tab values(:f1<int>,:f2<char[31]>)",
              // SQL statement
              db  // connect object
              );
    char tmp[32];
    for(int i=1;i<=100;++i){
#if defined(_MSC_VER)
#if (_MSC_VER >= 1400) // VC++ 8.0 or higher
        sprintf_s(tmp,sizeof(tmp),"Name%d",i);
#else
        sprintf(tmp,"Name%d",i);
#endif
#else
        sprintf(tmp,"Name%d",i);
#endif
        o<<i<<tmp;
    }
}

void update(const int af1)
// insert rows into table
{
    otl_stream
            o(1, // buffer size
              "UPDATE test_tab "
              "   SET f2=:f2<char[31]> "
              " WHERE f1=:f1<int>",
              // UPDATE statement
              db // connect object
              );

    o<<"Name changed"<<af1;
    o<<otl_null()<<af1+1; // set f2 to NULL
    // there is no need to flush the stream buffer here
    // because the buffer size is 1: the stream flushes
    // the buffer every time the buffer gets full
}

void select(const int af1)
{
    otl_stream i(50, // buffer size may be > 1
                 "select * from test_tab "
                 "where f1>=:f11<int> "
                 "  and f1<=:f12<int>*2",
                 // SELECT statement
                 db // connect object
                 );
    // create select stream

    int f1;
    char f2[31];

    i<<af1<<af1; // Writing input values into the stream
    while(!i.eof()){ // while not end-of-data
        i>>f1;
        cout<<"f1="<<f1<<", f2=";
        i>>f2;
        if(i.is_null())
            cout<<"NULL";
        else
            cout<<f2;
        cout<<endl;
    }

}

int fn_execSql(otl_connect & _db, char * sql)
{
    if(strlen(sql)<1) return -1;

    int ret = 0;

    if(db.connected>0)
    {
        try{
             //执行SQL 执行次数
            otl_stream o(1,sql,_db); // buffer size, SQL statement,connect object
        }

        catch(otl_exception& p)
        {                        // intercept OTL exceptions
            cout<<p.msg<<p.stm_text<<p.sqlstate<<p.var_info<<endl;
        }
    }
    return ret;
}

int interinfoIn_line( const std::string & sInfo, const msepoch_t & dtIn, int iSource, int eInType, int iTag)
{
    return 0;
}

bool f_bExit = false;

int interinfoIn_cmd( const std::string & sCommand, const std::map<std::string, std::string> & sParams, const msepoch_t & dtIn, int iSource, int eInType, int iTag)
{
    if (sCommand == "q")
    {
        f_bExit = true;
        return CxInterinfo::InDeal_Ed;
    }
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


int main(int argc, char * argv[])
{
    CxApplication::init(argc, argv);

    CxConsoleInterinfo::addObserver(interinfoIn_line);
    CxConsoleInterinfo::addObserver(interinfoIn_cmd);

    cxDebug() << "begin";

    otl_connect::otl_initialize(); // initialize ODBC environment

    try{
        cout << "db.rlogon begin" << endl;

        db.rlogon("DSN=odbc_t1",1);

        cout << "db connected=" << db.connected << endl;
     }

    catch(otl_exception& p){                        // intercept OTL exceptions
        cout << p.msg << p.stm_text << p.sqlstate<< p.var_info <<endl;
        return -1;
    }

    char sql[1024];

    while (! f_bExit) {

        sprintf(_connstr,"%s/%s@%s",uid,psw,dsn);
        fn_execSql();

        ::Sleep(10);
    }


    db.logoff(); // disconnect from the database

    return 0;

}
