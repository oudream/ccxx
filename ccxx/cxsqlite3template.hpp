#ifndef SQLITE3TEMPLATE_HPP
#define SQLITE3TEMPLATE_HPP

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

#include "sqlite3/sqlite3.h"

#ifdef _WIN32
#include <windows.h>
#endif

namespace SQL
{
    using std::string;
    using std::vector;
    using std::tuple;

    typedef std::runtime_error SQLRTerror;

    //*** get
    template<typename T>
    static T get_single(sqlite3_stmt *ppsm, int iCol);

    template<>
    inline int get_single(sqlite3_stmt *ppsm, int iCol)
    {
        return sqlite3_column_int(ppsm, iCol);
    }

    template<>
    inline sqlite3_int64 get_single(sqlite3_stmt *ppsm, int iCol)
    {
        return sqlite3_column_int64(ppsm, iCol);
    }

    template<>
    inline double get_single(sqlite3_stmt *ppsm, int iCol)
    {
        return sqlite3_column_double(ppsm, iCol);
    }

    template<>
    inline std::string get_single(sqlite3_stmt *ppsm, int iCol)
    {
        const char *foo = reinterpret_cast<const char *>(sqlite3_column_text(ppsm, iCol));
        return (foo ? string(foo) : "");
    }

    template<>
    inline std::vector<char> get_single(sqlite3_stmt *ppsm, int iCol)
    {
        size_t len = sqlite3_column_bytes(ppsm, iCol);
        const char *foo = reinterpret_cast<const char *>(sqlite3_column_blob(ppsm, iCol));
        if (foo)
        {
            std::vector<char> r(len);
            memcpy(r.data(), foo, len);
            return r;
        }
        return std::vector<char>();
    }

    //*** set
    template<typename T>
    static int set_single(sqlite3_stmt *ppsm, int iCol, const T & v);

    template<>
    inline int set_single(sqlite3_stmt *ppsm, int iCol, const int & v)
    {
        return sqlite3_bind_int(ppsm, iCol, v);
    }

    template<>
    inline int set_single(sqlite3_stmt *ppsm, int iCol, const sqlite3_int64 & v)
    {
        return sqlite3_bind_int64(ppsm, iCol, v);
    }

    template<>
    inline int set_single(sqlite3_stmt *ppsm, int iCol, const double & v)
    {
        return sqlite3_bind_double(ppsm, iCol, v);
    }

    template<>
    inline int set_single(sqlite3_stmt *ppsm, int iCol, const std::string & v)
    {
        return sqlite3_bind_text(ppsm, iCol, v.c_str(), v.size(), 0);
    }

    template<>
    inline int set_single(sqlite3_stmt *ppsm, int iCol, const std::vector<char> & v)
    {
        return sqlite3_bind_blob(ppsm, iCol, v.data(), v.size(), 0);
    }

    template<std::size_t I = 0, typename... Tp>
    inline typename std::enable_if<I == sizeof...(Tp), void>::type
    set_singles(sqlite3_stmt *ppsm, std::tuple<Tp...>& t)
    { }

    template<std::size_t I = 0, typename... Tp>
    inline typename std::enable_if<I < sizeof...(Tp), void>::type
    set_singles(sqlite3_stmt *ppsm, std::tuple<Tp...>& t)
    {
        set_single(ppsm, I+1, std::get<I>(t));
        set_singles<I + 1, Tp...>(ppsm, t);
    }


    class Con {
    protected:
        sqlite3 *_db;

    private:
        string _dbfilename;
        bool _autoClose;

    public:
        Con(const string &dbfile) :
            _dbfilename(dbfile),
            _autoClose(false)
        {
            int erg;
            erg = sqlite3_open(_dbfilename.c_str(), &_db);
            if (erg != SQLITE_OK)
            {
                throw (SQLRTerror(sqlite3_errmsg(_db)));
            }
            _autoClose = true;
        }

        Con(sqlite3 *db1) :
            _db(db1),
            _autoClose(false)
        {
            _dbfilename = string(sqlite3_db_filename(db1, NULL));
        }

        ~Con()
        {
            if (_autoClose)
            {
                sqlite3_close(_db);
            }
        }

    private:
        struct PPSM {
            sqlite3_stmt *me;

            PPSM(sqlite3 *db, const string &query) :
                me(nullptr)
            {
                if (sqlite3_prepare_v2(db, query.c_str(), query.size(),
                                       &me, nullptr)
                    != SQLITE_OK)
                {
                    throw (SQLRTerror(sqlite3_errmsg(db)));
                }
            }

            void bindvals(vector<string> &&vals)
            {
                int pos = 0;
                for (auto &v : vals)
                {
                    sqlite3_bind_text(me, ++pos, v.c_str(), v.size(), SQLITE_TRANSIENT);
                }
            }

            ~PPSM()
            {
                sqlite3_finalize(me);
            }

        };

    public:

//> Within the initializer-list of a braced-init-list, the initializer-clauses, including
// any that result from pack expansions, are evaluated in the order in which they appear.
// That is, every value computation and side effect associated with a given initializer-clause
// is sequenced before every value computation and side effect associated with any
// initializer-clause that follows it in the comma-separated list of the initializer-list.

        template<typename ...ARGS>
        vector<tuple<ARGS...>> bindnquery(const string &query,
                                          vector<string> &&bindvals = {})
        {
            PPSM ppsm(_db, query);
            ppsm.bindvals(std::forward<decltype(bindvals)>(bindvals));
            vector<tuple<ARGS...> > answer{};

            while (sqlite3_step(ppsm.me) == SQLITE_ROW)
            {
                int col = 0;
#pragma GCC diagnostic ignored "-Wsequence-point"
                tuple<ARGS...> r{get_single<ARGS>(ppsm.me, (col++))...};
                answer.push_back(r);
            }
            return answer;
        }

        template<typename ...ARGS>
        tuple<ARGS...> bind1query(const string &query,
                                          vector<string> &&bindvals = {})
        {
            PPSM ppsm(_db, query);
            ppsm.bindvals(std::forward<decltype(bindvals)>(bindvals));

            while (sqlite3_step(ppsm.me) == SQLITE_ROW)
            {
                int col = 0;
#pragma GCC diagnostic ignored "-Wsequence-point"
                tuple<ARGS...> r{get_single<ARGS>(ppsm.me, (col++))...};
                return r;
            }
            tuple<ARGS...> r{};
            return r;
        }

        template<typename ...ARGS>
        int bindnexec(const string &sql, const vector<tuple<ARGS...>> & rows, bool bTran = false, int * errid = NULL, string * err = NULL)
        {
            if (!_db)
            {
                return -1;
            }
            int result = sqlite3_total_changes(_db);
            if (bTran)
            {
                if (! begin_transaction())
                {
                    return -2;
                }
            }
            sqlite3_stmt* stmt;
            const char* tail;
            int rc = sqlite3_prepare(_db, sql.c_str(), sql.size(), &stmt, &tail);
            if (rc != SQLITE_OK)
            {
                // set LastError
                if (errid) * errid = rc;
                // rollback
                if (bTran)
                    rollback_transaction();
                return -2;
            }

            static const int size = sizeof...(ARGS);
            for (size_t i = 0; i < rows.size(); ++i)
            {
                rc = sqlite3_reset(stmt);
				if (rc != SQLITE_OK)
				{
					// set LastError
					if (errid) * errid = rc;
					if (err)
					{
						* err = "sql exec error: " + string(sqlite3_errmsg(_db));
					}
					// rollback
					if (bTran)
						rollback_transaction();
					return -3;
				}
                tuple<ARGS...> row = rows[i];
                int col = 0;
                set_singles(stmt, row);
//                for (size_t j = 0; j < sizeof...(ARGS); ++j)
//                {
//                    set_single(stmt, (col++), std::get<1>(row));
//                }
//                std::forward<T>(t).get()...
                rc = sqlite3_step(stmt);
                if (rc != SQLITE_OK && rc != SQLITE_DONE)
                {
                    // set LastError
                    if (errid) * errid = rc;
                    if (err)
                    {
                        * err = "sql exec error: " + string(sqlite3_errmsg(_db));
                    }
                    // roll back
                    // rollback
                    if (bTran)
                        rollback_transaction();
                    return -4;
                }
            }
			rc = sqlite3_finalize(stmt);
			if (rc != SQLITE_OK)
			{
				// set LastError
				if (errid) * errid = rc;
				if (err)
				{
					* err = "sql exec error: " + string(sqlite3_errmsg(_db));
				}
				// rollback
				if (bTran)
					rollback_transaction();
				return -5;
			}
            // commit
			if (bTran)
			{
				commit_transaction();
			}
            return sqlite3_total_changes(_db) - result;
        }

        template<typename ...ARGS>
        int bindnexec(const string &sql, tuple<ARGS...> & row, bool bTran = false, int * errid = NULL, string * err = NULL)
        {
            if (!_db)
            {
                return -1;
            }
            int result = sqlite3_total_changes(_db);
            if (bTran)
            {
                if (! begin_transaction())
                {
					if (err)
					{
						* err = "sql exec error: Can Not begin_transaction!";
					}
					return -2;
                }
            }
            sqlite3_stmt* stmt;
            const char* tail;
            int rc = sqlite3_prepare(_db, sql.c_str(), sql.size(), &stmt, &tail);
            if (rc != SQLITE_OK)
            {
                // set LastError
                if (errid) * errid = rc;
				if (err)
				{
					* err = "sql exec error: " + string(sqlite3_errmsg(_db));
				}
				// rollback
                if (bTran)
                    rollback_transaction();
                return -2;
            }

            static const int size = sizeof...(ARGS);
			rc = sqlite3_reset(stmt);
			if (rc != SQLITE_OK)
			{
				// set LastError
				if (errid) * errid = rc;
				if (err)
				{
					* err = "sql exec error: " + string(sqlite3_errmsg(_db));
				}
				// rollback
				if (bTran)
					rollback_transaction();
				return -3;
			}
			set_singles(stmt, row);
			rc = sqlite3_step(stmt);
			if (rc != SQLITE_OK && rc != SQLITE_DONE)
			{
				// set LastError
				if (errid) * errid = rc;
				if (err)
				{
					* err = "sql exec error: " + string(sqlite3_errmsg(_db));
				}
				// rollback
				if (bTran)
					rollback_transaction();
				return -4;
			}
            rc = sqlite3_finalize(stmt);
			if (rc != SQLITE_OK)
			{
				// set LastError
				if (errid) * errid = rc;
				if (err)
				{
					* err = "sql exec error: " + string(sqlite3_errmsg(_db));
				}
				// rollback
				if (bTran)
					rollback_transaction();
				return -5;
			}
			// commit
			if (bTran)
			{
				commit_transaction();
			}
            return sqlite3_total_changes(_db) - result;
        }

        //for  update and such....
        void query_nothing(const string &query, vector<string> &&bindvals = {})
        {
            PPSM ppsm(_db, query);
            ppsm.bindvals(std::forward<decltype(bindvals)>(bindvals));
            auto result = sqlite3_step(ppsm.me);
            if (result == SQLITE_BUSY)
            {
                do
                {
#ifdef _WIN32
                    Sleep(0);
#else
                    sched_yield();
#endif
                    result = sqlite3_step(ppsm.me);;
                }
                while (result == SQLITE_BUSY);
            }
        }

        template<typename QType>
        QType query_one(const string &query, vector<string> &&bindvals = {})
        {
            PPSM ppsm(_db, query);
            ppsm.bindvals(std::forward<decltype(bindvals)>(bindvals));
            if (sqlite3_step(ppsm.me) != SQLITE_ROW)
            {
                throw (std::runtime_error("Query did not yield answer:" + query));
            }
            return get_single<QType>(ppsm.me, 0);
        }

        bool exec_sql(const string& sql, int * errid = NULL, string * err = NULL)
        {
            if (!_db)
            {
                return false;
            }
            char* zErr;
            int rc = sqlite3_exec(_db, sql.c_str(), NULL, NULL, &zErr);
            if (rc != SQLITE_OK)
            {
                if (errid) * errid = rc;
                if (zErr != NULL)
                {
                    if (err)
                    {
                        * err = "sql exec error: " + string(zErr);
                    }
                    sqlite3_free(zErr);
                }
                return false;
            }
            return true;
        }

        bool begin_transaction()
        {
            return exec_sql("BEGIN");
        }

        bool commit_transaction()
        {
            return exec_sql("COMMIT");
        }

        bool rollback_transaction()
        {
            return exec_sql("ROLLBACK");
        }

    };

}

#endif