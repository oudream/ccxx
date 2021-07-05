
#include <ccxx/ccxx.h>

using namespace std;

void helloRead1()
{
    string sKey="SOFTWARE\\Microsoft\\Cryptography";
    std::string sValue;
    CxWinRegEx::read(sKey,"MachineGuid", &sValue);
    cxDebug() << sKey << ":" << sValue;
}

void helloWrite1()
{
    string sKey="SOFTWARE\\SinoVoice\\jTTS4_Professional";

    CxWinRegEx::write(sKey,"LibPath4", CxApplication::applicationDeployPath()+"\\VoiceFile");
    CxWinRegEx::write(sKey,"SerialNo","56CCB31A7362F2DF");
    string v;
    CxWinRegEx::read(sKey, "LibPath4",&v);
    cxPromptOut(v);
}

int main(int argc,const char * argv[])
{
    CxApplication::init(argc, argv);

//    helloRead1();
    helloWrite1();

    return CxApplication::exec();
}

