
#include <ccxx/cxappenv.h>
#include <ccxx/cxinterinfo.h>

using namespace std;

int main(int argc, const char*argv[])
{
    //应用初始化
    CxAppEnv::init(argc, argv);

    cxPrompt() << CxAppEnv::applicationTargetName();

    return 0;
}
