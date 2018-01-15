
#include <ccxx/ccxx.h>

using namespace std;

int main(int argc,const char * argv[])
{
    CxApplication::init(argc, argv);

//    CxWinRegEx::write("SOFTWARE\\a","a1","abc");

    string sKey="SOFTWARE\\SinoVoice\\jTTS4_Professional";

    CxWinRegEx::write(sKey,"LibPath4", CxApplication::applicationDeployPath()+"\\VoiceFile");
    CxWinRegEx::write(sKey,"SerialNo","56CCB31A7362F2DF");
    string v;
    CxWinRegEx::read(sKey, "LibPath4",&v);
    cxPromptOut(v);

    return CxApplication::exec();
}

