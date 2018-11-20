
#include <ccxx/ccxx.h>

using namespace std;

int main(int argc, const char* argv[])
{
    CxApplication::init(argc, argv);

    std::vector<CxSystem::DiskInfo> diskInfos = CxSystem::getDiskInfos();

    for (int i = 0; i < diskInfos.size(); ++i)
    {
        const CxSystem::DiskInfo& diskInfo = diskInfos[i];
        cxDebug() << diskInfo.name;;
        cxDebug() << diskInfo.totalSize;;
        cxDebug() << diskInfo.freeSize; ;
        cxDebug() << "---";
    }

    cxDebug() << "---\n";
    cxDebug() << "c:";
    cxDebug() << CxSystem::getDiskSpace("c:") ;
    cxDebug() << CxSystem::getDiskFreeSpace("c:") ;

    cxDebug() << "---\n";
    cxDebug() << "c:";
    cxDebug() << (CxSystem::getDiskSpace("D:\\") / 1024 / 1024) << "MB" ;
    cxDebug() << (CxSystem::getDiskFreeSpace("D:\\") / 1024 / 1024) << "MB" ;


    return 0;
}