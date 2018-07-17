#include <ccxx/ccxx.h>
#include <iostream>

using namespace std;

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define OTL_ODBC // Compile OTL 4.0/ODBC
#define ODBCVER 0x0250 // ODBC Version # needs to be downgraded
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

int main()
{
    otl_connect::otl_initialize(); // initialize ODBC environment
    try{

        db.rlogon("scott/tiger@sqlite");

        otl_cursor::direct_exec
                (
                    db,
                    "drop table test_tab",
                    otl_exception::disabled // disable OTL exceptions
                    ); // drop table

        otl_cursor::direct_exec
                (
                    db,
                    "create table test_tab(f1 int, f2 varchar(30))"
                    );  // create table

        insert(); // insert records into the table
        update(10); // update records in the table
        select(8); // select records from the table

    }

    catch(otl_exception& p){ // intercept OTL exceptions
        cerr<<p.msg<<endl; // print out error message
        cerr<<p.stm_text<<endl; // print out SQL that caused the error
        cerr<<p.sqlstate<<endl; // print out SQLSTATE message
        cerr<<p.var_info<<endl; // print out the variable that caused the error
    }

    db.logoff(); // disconnect from the database

    return 0;

}
