#include <ccxx/ccxx.h>


using namespace std;


void testSave31()
{
    cout << "begin :" <<  0 << endl;
    clock_t tNow = clock();

    CxSkverTxt ftxt;

    for (size_t i = 0; i < 30000; ++i)
    {
        string sSection = CxString::format("section_%06d", i);
        for (size_t j = 0; j < 20; ++j)
        {
            string sEntry = CxString::format("entry_%02d", j);
            string sValue = CxString::format("value_%09d", i * 0x10000 + j);
            ftxt.setValue(sSection, sEntry, sValue);
        }
    }

    cout << "ftxt setvalue :" <<  (clock()-tNow) << endl;
    tNow = clock();

    ftxt.save("c:/0.txt");

    cout << "ftxt save :" <<  (clock()-tNow) << endl;
    tNow = clock();

    system("pause");
}

void testLoad31()
{
    clock_t tNow = clock();

    cout << "begin :" <<  0 << endl;

    CxSkverTxt ftxt("c:/0.txt");

    cout << "ftxt load :" <<  (clock()-tNow) << endl;
    cout << "ftxt load count:" <<  ftxt.getSectionEntryValues().size() << endl;

    system("pause");

    const std::map<std::string, std::map<std::string, std::string> > & sectionEntryValues = ftxt.getSectionEntryValues();
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
}

int main3(int argc, char *argv[])
{
//    testSave31();
    testLoad31();
    return 0;
}


