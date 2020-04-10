#include "template1.hpp"

using namespace std;

int f_argc = 0;
const char * * f_argv = NULL;

int main(int argc,const char * argv[])
{
    f_argc = argc;
    f_argv = argv;

    cout << "begin test xxx : " << endl;

    helloTemplate1();

//    std::string sLine;
//    std::getline (std::cin, sLine);
//
//    cout << "this size :" << sLine.size() << endl;

    cout << "end test xxx !!!" << endl;

    return 0;
}

