#include <ccxx/ccxx.h>

using namespace std;

int main(int argc, char * argv[])
{
    CxApplication::init(argc, argv);

    vector<string> ss;
    vector<string> sss;
    CxFile::load("D:\\data.ouspring\\002.csv", ss);
    for (int i = 0; i < ss.size(); ++i)
    {
        string s = ss.at(i);
        string sName = CxString::token(s, ',');
        string sTel = s;
        sss.push_back(CxString::format("BEGIN:VCARD\r\nN:%s\r\nTEL;CELL:%s\r\nPHOTO;ENCODING=B:\r\nEND:VCARD", sName.c_str(), sTel.c_str()));
    }
    string sFilePath = "D:\\data.ouspring\\003.vcf";
    CxFile::save(sFilePath, CxEncoding::gb2312ToUtf8(sss));

    cxPrompt() << "save to file : " << sFilePath;

    return 0;
}

