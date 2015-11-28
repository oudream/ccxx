#include <iostream>
using namespace std;
#include <stdio.h>

#define OTL_ODBC
#define OTL_STL
// #define OTL_ODBC_UNIX // uncomment this line if UnixODBC is used
#undef UNICODE
#undef _UNICODE
#include <ccxx/otlv4.h> // include the OTL 4 header file

otl_connect db; // connect object

void insert(void)
// insert rows into table
{

    otl_long_string f2(70000); // define long string variable
    db.set_max_long_size(70000); // set maximum long string size for connect object

    otl_stream o(1, // buffer size needs to be set to 1
                 "insert into test_tab values(:f1<int>,:f2<raw_long>)",
                 // INSERT statement
                 db // connect object
                 );


    for(int i=1;i<=10;++i){
        for(int j=0;j<50000;++j)
            f2[j]='*';
        f2[50000]='?';
        f2.set_len(50001);
        o<<i<<f2;
    }
}

void insert2(void)
// insert rows into table
{

    //    otl_long_string f2(70000); // define long string variable
    //    db.set_max_long_size(70000); // set maximum long string size for connect object

    otl_stream o(1, // buffer size needs to be set to 1
                 "insert into t1 (f1, f2) values(:f1<int>,:f2<int>)",
                 // INSERT statement
                 db // connect object
                 );


    for(int i=1;i<=10;++i)
    {
        string v1 = "2";
        string v2 = "12345678";
        //        string v3 = "123457890123";
        //        string v4 = "12345789.0123";
        //        string v5 = "a好b你";
        //        string v6 = "1地dsfasfasd地要1地dsfasfasd地要1地dsfasfasd地要";
        //        string v7 = "1";
        o<<v1<<v2;
    }
}

void select(void)
{
    otl_long_string f2(70000); // define long string variable
    db.set_max_long_size(70000); // set maximum long string size for connect object

    otl_stream i(1, // buffer size needs to be set to 1 in case of BLOB columns
                 "select * from test_tab",
                 // SELECT statement
                 db // connect object
                 );
    // create select stream

    int f1;


    while(!i.eof()){ // while not end-of-data
        i>>f1>>f2;
        cout<<"f1="<<f1<<", f2="<<f2[0]<<f2[f2.len()-1]<<", len="<<f2.len()<<endl;
    }

}

void select2()
{
    otl_stream i(10, // buffer size
                 "select * from t1",
                 // SELECT statement
                 db // connect object
                 );
    // create select stream

    otl_column_desc* desc;
    int desc_len;

    desc=i.describe_select(desc_len);

    for(int n=0;n<desc_len;++n){
        cout<<"========== COLUMN #"<<n+1<<" ==========="<<endl;
        cout<<"name="<<desc[n].name<<endl;
        cout<<"dbtype="<<desc[n].dbtype<<endl;
        cout<<"otl_var_dbtype="<<desc[n].otl_var_dbtype<<endl;
        cout<<"dbsize="<<desc[n].dbsize<<endl;
        cout<<"scale="<<desc[n].scale<<endl;
        cout<<"prec="<<desc[n].prec<<endl;
        cout<<"nullok="<<desc[n].nullok<<endl;
    }

}

void select3()
{
    otl_stream s;

    s.set_all_column_types(otl_all_num2str|otl_all_date2str);
    // map all numeric and date/time output columns to strings for simplicity.

    db.auto_commit_on(); // setting database connect auto-commit to ON

    db.get_connect_struct().get_henv();

    s.open(100, // buffer size
           "$SQLColumns $1:'t1'",
           //  get a list of all columns of table
           // "test_tab" that belongs to schema "dbo",
           // catalog "sybsql"
           db // connect object
           );

    otl_column_desc* desc;
    int desc_len;
    desc=s.describe_select(desc_len);
    // describe the structure of the output columns of the result set.
    for(int col_num=0;col_num<desc_len;++col_num){
        cout<<desc[col_num].name<<desc[col_num].dbtype<<"=NULL ";
    }
    int rpc=0;
    while(!s.eof()){
        ++rpc;
        char str[512];
        cout<<"ROW#"<<rpc<<" ";
        for(int col_num=0;col_num<desc_len;++col_num){
            s>>str;
            if(s.is_null())
                cout<<desc[col_num].name<<"=NULL ";
            else
                cout<<desc[col_num].name<<"="<<str<<" ";
        }
        cout<<endl;
    }

}

int main()
{
    otl_connect::otl_initialize(); // initialize ODBC environment
    try{

        db.rlogon("UID=ygct;PWD=ygct;DSN=dns_mysql_ansi"); // connect to ODBC

        otl_cursor::direct_exec
                (
                    db,
                    "drop table test_tab",
                    otl_exception::disabled // disable OTL exceptions
                    ); // drop table

        otl_cursor::direct_exec
                (
                    db,
                    "create table test_tab(f1 int, f2 longblob)"
                    );  // create table

        insert(); // insert records into table
//        insert2(); // insert records into table
        //        select(); // select records from table
//        select3(); // select records from table
        select2(); // select records from table

    }

    catch(otl_exception& p){ // intercept OTL exceptions
        cerr<<p.msg<<endl; // print out error message
        cerr<<p.stm_text<<endl; // print out SQL that caused the error
        cerr<<p.sqlstate<<endl; // print out SQLSTATE message
        cerr<<p.var_info<<endl; // print out the variable that caused the error
    }

    db.logoff(); // disconnect from ODBC

    return 0;

}

