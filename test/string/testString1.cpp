#include <ccxx/cxstring.h>

using namespace std;

//CxString::token
int testString11(int argc, char *argv[])
{
    char buff[33];
    char *tokens = NULL;
    unsigned count = 0;
    const char *tp;
    const char *array[5];

    memset(buff, ' ', 32);
    CxStringC::set(buff, sizeof(buff), "this is \"a test\"");
    while(NULL != (tp = CxStringC::token(buff, &tokens, " ", "\"\"")) && count < 4)
        array[count++] = tp;
    return 0;
}

int testString12(int argc, char *argv[])
{
    char str[] = "fcba73";
    char keys[] = "5689";
    int i;
//    i = strcspn (str,keys);
    i = CxStringC::strcspnRight(str,keys);

    printf ("The first number in str is at position %d.\n",i+1);

    return 0;
}

int testString13(int argc, char *argv[])
{
    clock_t ck;

    ck = clock();
    for (size_t i = 0; i < 1000 * 1000; ++i)
    {
        string s = " 3243214FSASDFASDF ASDFASDFASF ajhbcxvvzxcfjjgmmkljjhjkloirwewe l<mcncb ";
        CxString::toUpper(s);
    }
    cout << "spell time : " << clock() - ck << endl;

    ck = clock();
    for (size_t i = 0; i < 1000 * 1000; ++i)
    {
        string s = " 3243214fsasdfasdf asdfasdfasf AJHBCXVVZXCFJJGMMKLJJHJKLOIRWEWE L<MCNCB ";
        CxString::toLower(s);
    }
    cout << "spell time : " << clock() - ck << endl;

    return 0;
}

int testString14(int argc, char *argv[])
{
    clock_t ck;

    ck = clock();
    for (size_t i = 0; i < 1000 * 1000; ++i)
    {
        string s = "     ds adsf   ";
        CxString::trim(s);
    }
    cout << "spell time : " << clock() - ck << endl;

    ck = clock();
    for (size_t i = 0; i < 1000 * 1000; ++i)
    {
        char s[] = "     ds adsf   ";
        CxStringC::trimRight(s, " ");
    }
    cout << "spell time : " << clock() - ck << endl;

    return 0;
}
