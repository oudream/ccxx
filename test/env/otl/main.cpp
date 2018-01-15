#include <typeinfo>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <bitset>
#include <stack>
#include <queue>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <iso646.h>
#include <locale.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>
#include <wctype.h>

using namespace std;

int f_argc = 0;
const char * * f_argv = NULL;

#define OTL_ORA11G

#include <ccxx/otl/otlv4.h>



otl_connect db; // connect object

void insert()
// insert rows into table
{
    otl_stream o(50, // buffer size
                 "insert into test_tab values(:f1<int>,:f2<char[31]>)",
                 // SQL statement
                 db // connect object
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
#if defined(OTL_ANSI_CPP_11_VARIADIC_TEMPLATES)
        otl_write_row(o,i,tmp);
#else
        // when variadic template functions are not supported by the C++
        // compiler, OTL provides nonvariadic versions of the same template
        // functions in the range of [1..15] parameters
        otl_write_row(o,i,tmp);

        // the old way (operators >>() / <<()) is available as always:
        //  o<<i<<tmp;

#endif
    }
}

void select()
{
    otl_stream i(50, // buffer size
                 "select * from test_tab where f1>=:f<int> and f1<=:f*2",
                 // SELECT statement
                 db // connect object
                 );
    // create select stream

    float f1;
    char f2[31];

    i<<8; // assigning :f = 8
    // SELECT automatically executes when all input variables are
    // assigned. First portion of output rows is fetched to the buffer

    while(!i.eof()){ // while not end-of-data
#if defined(OTL_ANSI_CPP_11_VARIADIC_TEMPLATES)
        otl_read_row(i,f1,f2);
#else
        // when variadic template functions are not supported by the C++
        // compiler, OTL provides nonvariadic versions of the same template
        // functions in the range of [1..15] parameters
        otl_read_row(i,f1,f2);
        // the old way (operators >>() / <<()) is available as always:
        //   i>>f1>>f2;
#endif
        cout<<"f1="<<f1<<", f2="<<f2<<endl;
    }

    i<<4; // assigning :f = 4
    // SELECT automatically executes when all input variables are
    // assigned. First portion of output rows is fetched to the buffer

    while(!i.eof()){ // while not end-of-data
        i>>f1>>f2;
        cout<<"f1="<<f1<<", f2="<<f2<<endl;
    }

}

int main(int argc,const char * argv[])
{
    f_argc = argc;
    f_argv = argv;

    cout << "begin test xxx : " << endl;

    std::string sLine;
    std::getline (std::cin, sLine);

    cout << "this size :" << sLine.size() << endl;

    cout << "end test xxx !!!" << endl;
    otl_connect::otl_initialize(); // initialize OCI environment
    try{

        db.rlogon("ygct/ygct@XE"); // connect to Oracle

        otl_cursor::direct_exec
                (
                    db,
                    "drop table test_tab",
                    otl_exception::disabled // disable OTL exceptions
                    ); // drop table

        otl_cursor::direct_exec
                (
                    db,
                    "create table test_tab(f1 number, f2 varchar2(30))"
                    );  // create table

        insert(); // insert records into table
        select(); // select records from table

    }

    catch(otl_exception& p){ // intercept OTL exceptions
        cerr<<p.msg<<endl; // print out error message
        cerr<<p.stm_text<<endl; // print out SQL that caused the error
        cerr<<p.var_info<<endl; // print out the variable that caused the error
    }

    db.logoff(); // disconnect from Oracle

    return 0;

}












