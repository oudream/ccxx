
#include <ccxx.h>

using namespace std;

int f_argc = 0;
char * * f_argv = NULL;

int main(int argc, char * argv[])
{
    f_argc = argc;
    f_argv = argv;

    cout << "begin test encoding : " << endl;

    CxApplication::init(argc, argv);


    string src = "你好！";

    string dest = CxEncoding::utf8ToGb2312(src);

    cout << "utf : " << src << endl;

    cout << "gbk : " << dest << endl;

    FILE * pFile;
    pFile = fopen ("c:/0.txt", "wb");
    if (pFile==NULL) return false;
    rewind (pFile);
    size_t iWrote = fwrite (dest.data() , 1, dest.size(), pFile);
    fclose (pFile);



    cout << "end test encoding !!!" << endl;

    return CxApplication::exec();
}
