
#include <ccxx/ccxx.h>

using namespace std;

int main(int argc,const char * argv[])
{
    cout << "begin test xml : " << endl;

    CxApplication::init(argc, argv);

    string sFilePath = "c:/0.xml";
    if (! CxFileSystem::isExist(sFilePath))
    {
        cout << sFilePath << " is do not exist!!!" << endl;
        return 0;
    }

    std::vector<std::map<std::string, std::string> > rows;
    CxXml::loadTable1Level(sFilePath, rows);
    CxXml::loadTable2Level(sFilePath, rows);
    CxXml::loadTable4Level(sFilePath, rows);
    for (size_t i = 0; i < rows.size(); ++i)
    {
        const map<string, string> & row = rows.at(i);
        string sLine = CxString::join(row, "=", ";");
        cout << "line " << i << " : " << sLine << endl;
    }

    return 0;
}



