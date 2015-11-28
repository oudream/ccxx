#include <ccxx/cxapplication.h>
#include <ccxx/cxstring.h>

using namespace std;

void testSplitn( bool bOutInfo = true )
{
    string ss[] = {
        "abds,asdf,sdaf,asd",
        ",abds,asdf,sdaf,asd",
        ",abds,asdf,sdaf,asd,",
        "abds,asdf,sdaf,asd,"};
    for (size_t i = 0; i < 4; ++i)
    {
        const string& s = ss[i];
        vector<string> sl = CxString::split(s, ',', true);
        if ( bOutInfo )
        {
            cout << i << " -> count : " << sl.size() << endl;
            for (size_t j = 0; j < sl.size(); ++j)
            {
                cout << sl[j] << endl;
            }
            cout << endl;
        }
    }
}

void testTrim( bool bOutInfo = true )
{
    string ss[] = {
        "abds,asdf,sdaf,asd",
        " abds,asdf,sdaf,asd",
        "  ",
        "  abds,asdf,sdaf,asd   ",
        "             abds,asdf,sdaf,asd",
        "abds,asdf,sdaf,asd         "};
    for (size_t i = 0; i < 6; ++i)
    {
        const string& s = ss[i];
        string str = CxString::trim(s);
        if ( bOutInfo )
        {
            cout << i << " -> size : " << str.size() << endl;
            cout << str << endl;
            cout << endl;
        }
    }
}

void testReplaceAll( bool bOutInfo = true )
{
    string ss[] = {
        "abds,asdf,sdaf,asd",
        " abds,asdf,sdaf,asd",
        "  ",
        "  abds,asdf,sdaf,asd   ",
        "             abds,asdf,sdaf,asd",
        "abds,asdf,sdaf,asd         "};
    for (size_t i = 0; i < 6; ++i)
    {
        const string& s = ss[i];
        string str = CxString::replace(s, ",", "");
        if ( bOutInfo )
        {
            cout << i << " -> : " << str << endl;
            cout << str << endl;
            cout << endl;
        }
    }
}

int testString2(int argc, char *argv[])
{
    testReplaceAll();

    return 0;

    clock_t ck;

    ck = clock();
    for (size_t i = 0; i < 1000 * 1000; ++i)
    {
        testReplaceAll(false);
    }
    cout << "spell time : " << clock() - ck << endl;

    return 0;
}

string formatString ( const char * format, ... )
{
  char buffer[512];
  va_list args;
  va_start (args, format);
  vsprintf (buffer, format, args);
  perror (buffer);
  va_end (args);
  return buffer;
}

size_t findString ()
{
    string s1 = "hello w world body every body";
//    string s1 = "hello";
    s1.push_back(0);
    s1.push_back(0);
    s1.push_back(1);
    s1.push_back(2);
    string s2 = "body";
    s2.push_back(0);
    s2.push_back(0);
    s2.push_back(1);
    s2.push_back(2);


    return s1.find(s2.data(), 0, 5);
}

size_t findString2 ()
{
    string s1 = "hello w world body every body";
    //    string s1 = "hello";
    s1.push_back(0);
    s1.push_back(0);
    s1.push_back(1);
    s1.push_back(2);
    string s2 = "body";
    s2.push_back(0);
    s2.push_back(0);
    s2.push_back(1);
    s2.push_back(2);


    return s1.find(s2);
}

size_t findString3 ()
{
    string s1 = "hello w world body every body";
    //    string s1 = "hello";
    s1.push_back(0);
    s1.push_back(0);
    s1.push_back(1);
    s1.push_back(2);
    string s2 = "body";
    s2.push_back(0);
    s2.push_back(0);
    s2.push_back(1);
    s2.push_back(2);

    cout << s1.size() << endl;

    return s1.rfind(s2);
}
