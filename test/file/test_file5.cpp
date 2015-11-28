
#include <ccxx/ccxx.h>

using namespace std;

void testSave5()
{
    string sFilePath = "c:/0.txt";
    FILE * pFile;
    pFile = fopen (sFilePath.data(), "a+");
    if (pFile==NULL) return;
    rewind (pFile);

    size_t iWrote = 0;
    for (size_t i = 0; i < 1000 * 1000; ++i)
    {
        string sText = string("bbbbbb - ") + CxString::toString(i) + " - " + CxString::toString(iWrote) + " - " + CxTime::currentSystemTimeString() + "\n\n";
        iWrote += fwrite (const_cast<char *>(sText.c_str()) , 1, sText.size(), pFile);
        fflush(pFile);
        ::Sleep(1);
        if (i % 50 == 0)
        {
            cout << sText << endl;
        }
    }

    fclose (pFile);
}

void testLoad5()
{
    std::vector<std::string> _sendFileTexts;
    string sFilePath = "c:/bootmgr";
    for (size_t i = 0; i < 1000; ++i)
    {
        if (CxFile::load(sFilePath, _sendFileTexts, 1024 * 2) > 0)
        {
            cout << i << " load file : " << sFilePath << " - size : " << CxString::sizeOf(_sendFileTexts) << endl;
            CxThread::sleep(10);
        }
    }
}

int main(int argc, char *argv[])
{
    testLoad5();

    return 0;
}

