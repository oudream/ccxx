
#include <ccxx/cxappenv.h>

using namespace std;

int main(int argc, const char*argv[])
{
    //应用初始化
    CxAppEnv::init(argc, argv);

    std::cout << CxAppEnv::applicationTargetName() << std::endl;
    std::cout << CxAppEnv::applicationPath() << std::endl;
    std::cout << CxAppEnv::configPath() << std::endl;
    std::cout << CxAppEnv::applicationDeployPath() << std::endl;
    std::cout << CxAppEnv::logPath() << std::endl;
    std::cout << CxAppEnv::tempPath() << std::endl;

    return 0;
}
