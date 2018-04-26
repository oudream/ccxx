
#include <ccxx/cxappenv.h>
#include <ccxx/cxinterinfo.h>
#include <ccxx/cxtinyxml.h>
#include <ccxx/cxsocket.h>

using namespace std;

std::string ginput()
{
	std::string msg;
	std::cin >> msg;
	return msg;
}

int main(int argc, const char*argv[])
{
    //应用初始化
    CxAppEnv::init(argc, argv);

    cxPrompt() << CxAppEnv::applicationTargetName();

    cxPrompt() << CxNetwork::getHostName();

    std::string sFilePath = CxFileSystem::mergeFilePath(CxAppEnv::tempPath(), CxTime::currentMsepochString(0, 0, 0)+".xml");
    std::vector<std::map<std::string, std::string> > rows;
    for (size_t i = 0; i < 100; ++i)
    {
        std::map<std::string, std::string> row;
        for (size_t j = 0; j < 10; ++j)
        {
            row[CxString::toString(j)] = CxString::toString(i << 8 | j);
        }
    }
    CxXml::saveTable4Level(sFilePath, rows);

	while (1) {
		string s1 = ginput();
		if (s1.find("exit") != string::npos) exit(0);
		std::cout << s1 << std::endl;
	}

    return 0;
}
