#include <ccxx/ccxx.h>


using namespace std;


int main2(int argc, char *argv[])
{
    clock_t tNow = clock();

    cout << "begin :" <<  0 << endl;

    CxSkverIni fini(("c:/0.ini"));

    cout << "fini :" <<  (clock()-tNow) << endl;

    system("pause");

    const std::map<std::string, std::map<std::string, std::string> > & sectionEntryValues = fini.getSectionEntryValues();
    for (map<string, map<string, string> >::const_iterator itSection = sectionEntryValues.begin(); itSection != sectionEntryValues.end(); ++itSection)
    {
        string sLine("[");
        sLine += itSection->first;
        sLine += "]\r\n";

        cout << sLine;

        const map<string, string> & sEntrys = itSection->second;
        for (map<string, string>::const_iterator itEntry = sEntrys.begin(); itEntry != sEntrys.end(); ++itEntry)
        {
            sLine = itEntry->first + "=" + itEntry->second + "\r\n";
            cout << sLine;
        }

        cout << endl;
    }

    return 0;
}

