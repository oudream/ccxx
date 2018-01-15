//http://dev.mysql.com/doc/refman/5.7/en/mysql-stmt-execute.html

//stl
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

//clib
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


#include <my_global.h>
#include <mysql.h>

#define STRING_SIZE 50

#define DROP_SAMPLE_TABLE "DROP TABLE IF EXISTS test_table"
#define CREATE_SAMPLE_TABLE "CREATE TABLE test_table(col1 INT,\
col2 VARCHAR(40),\
col3 SMALLINT,\
col4 TIMESTAMP)"
#define INSERT_SAMPLE "INSERT INTO \
test_table(col1,col2,col3) \
VALUES(?,?,?)"

int main(int argc, char **argv)
{
    MYSQL_STMT    *stmt;
    MYSQL_BIND    bind[3];
    my_ulonglong  affected_rows;
    int           param_count;
    short         small_data;
    int           int_data;
    char          str_data[STRING_SIZE];
    unsigned long str_length;
    my_bool       is_null;

    MYSQL *mysql = mysql_init(NULL);

    if (mysql == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(mysql));
        exit(1);
    }

    if (mysql_real_connect(mysql, "127.0.0.1", "ygct", "ygct",
//    if (mysql_real_connect(con, "localhost", "root", "root_pswd",
                           NULL, 3306, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(mysql));
        mysql_close(mysql);
        exit(1);
    }

    if (mysql_query(mysql, DROP_SAMPLE_TABLE))
    {
        fprintf(stderr, " DROP TABLE failed\n");
        fprintf(stderr, " %s\n", mysql_error(mysql));
        goto lable_exit0;
    }

    if (mysql_query(mysql, CREATE_SAMPLE_TABLE))
    {
        fprintf(stderr, " CREATE TABLE failed\n");
        fprintf(stderr, " %s\n", mysql_error(mysql));
        goto lable_exit0;
    }

    /* Prepare an INSERT query with 3 parameters */
    /* (the TIMESTAMP column is not named; the server */
    /*  sets it to the current date and time) */
    stmt = mysql_stmt_init(mysql);
    if (!stmt)
    {
        fprintf(stderr, " mysql_stmt_init(), out of memory\n");
        goto lable_exit0;
    }
    if (mysql_stmt_prepare(stmt, INSERT_SAMPLE, strlen(INSERT_SAMPLE)))
    {
        fprintf(stderr, " mysql_stmt_prepare(), INSERT failed\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
        goto lable_exit0;
    }
    fprintf(stdout, " prepare, INSERT successful\n");

    /* Get the parameter count from the statement */
    param_count= mysql_stmt_param_count(stmt);
    fprintf(stdout, " total parameters in INSERT: %d\n", param_count);

    if (param_count != 3) /* validate parameter count */
    {
        fprintf(stderr, " invalid parameter count returned by MySQL\n");
        goto lable_exit0;
    }

    /* Bind the data for all 3 parameters */

    memset(bind, 0, sizeof(bind));

    /* INTEGER PARAM */
    /* This is a number type, so there is no need
   to specify buffer_length */
    bind[0].buffer_type= MYSQL_TYPE_LONG;
    bind[0].buffer= (char *)&int_data;
    bind[0].is_null= 0;
    bind[0].length= 0;

    /* STRING PARAM */
    bind[1].buffer_type= MYSQL_TYPE_STRING;
    bind[1].buffer= (char *)str_data;
    bind[1].buffer_length= STRING_SIZE;
    bind[1].is_null= 0;
    bind[1].length= &str_length;

    /* SMALLINT PARAM */
    bind[2].buffer_type= MYSQL_TYPE_SHORT;
    bind[2].buffer= (char *)&small_data;
    bind[2].is_null= &is_null;
    bind[2].length= 0;

    /* Bind the buffers */
    if (mysql_stmt_bind_param(stmt, bind))
    {
        fprintf(stderr, " mysql_stmt_bind_param() failed\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
        goto lable_exit0;
    }

    /* Specify the data values for the first row */
    int_data= 10;             /* integer */
    strncpy(str_data, "MySQL", STRING_SIZE); /* string  */
    str_length= strlen(str_data);

    /* INSERT SMALLINT data as NULL */
    is_null= 1;

    /* Execute the INSERT statement - 1*/
    if (mysql_stmt_execute(stmt))
    {
        fprintf(stderr, " mysql_stmt_execute(), 1 failed\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
        goto lable_exit0;
    }

    /* Get the number of affected rows */
    affected_rows= mysql_stmt_affected_rows(stmt);
    fprintf(stdout, " total affected rows(insert 1): %lu\n",
            (unsigned long) affected_rows);

    if (affected_rows != 1) /* validate affected rows */
    {
        fprintf(stderr, " invalid affected rows by MySQL\n");
        goto lable_exit0;
    }

    /* Specify data values for second row,
   then re-execute the statement */
    int_data= 1000;
    strncpy(str_data, "The most popular Open Source database",
            STRING_SIZE);
    str_length= strlen(str_data);
    small_data= 1000;         /* smallint */
    is_null= 0;               /* reset */

    /* Execute the INSERT statement - 2*/
    if (mysql_stmt_execute(stmt))
    {
        fprintf(stderr, " mysql_stmt_execute, 2 failed\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
        goto lable_exit0;
    }

    /* Get the total rows affected */
    affected_rows= mysql_stmt_affected_rows(stmt);
    fprintf(stdout, " total affected rows(insert 2): %lu\n",
            (unsigned long) affected_rows);

    if (affected_rows != 1) /* validate affected rows */
    {
        fprintf(stderr, " invalid affected rows by MySQL\n");
        goto lable_exit0;
    }

    /* Close the statement */
    if (mysql_stmt_close(stmt))
    {
        fprintf(stderr, " failed while closing the statement\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
        goto lable_exit0;
    }


lable_exit0:
    mysql_close(mysql);
    exit(0);

}
