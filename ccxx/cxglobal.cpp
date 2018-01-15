#include "cxglobal.h"

const timems_t CxGlobal::inf = ((timems_t)(-1));

const msepoch_t CxGlobal::invalidMsepoch = ((msepoch_t)(-1));

const msepoch_t CxGlobal::invalidUsepoch = ((usepoch_t)(-1));

const double CxGlobal::deadZone = 0.0001f;

//const std::string CxGlobal::emptyString = "";

//const std::string CxGlobal::spaceString = " ";
//空格
const char CxGlobal::spaceChar = ' ';
//等号
const char CxGlobal::equalChar = '=';
//分号
const char CxGlobal::colonChar = ';';
//逗号
const char CxGlobal::commaChar = ',';

const char  CxGlobal::middleChar = '=';

const char  CxGlobal::splitChar = ';';

const std::map<std::string, std::string> CxGlobal::emptyMapString = std::map<std::string, std::string>();

#ifdef GM_OS_WIN
const char CxGlobal::pathCharacter = '\\';
const char f_sLineString[] = "\r\n";
#else
const char CxGlobal::pathCharacter = '/';
const char f_sLineString[] = "\n";
#endif
const char * CxGlobal::lineString = f_sLineString;

const char CxGlobal::lineCharacter = '\n';

const char CxGlobal::argumentCharacter = '-';

const size_t CxGlobal::npos = std::string::npos;

const char f_sWarningString[] = "!! ";
const char * CxGlobal::warningString = f_sWarningString;

const char f_sPromptString[] = "## ";
const char * CxGlobal::promptString = f_sPromptString;

const char f_sDebugString[] = "** ";
const char * CxGlobal::debugString = f_sDebugString;

const char f_sTrackString[] = "@@ ";
const char * CxGlobal::trackString = f_sTrackString;

const char f_sMiddleString[] = "=";
const char * CxGlobal::middleString = f_sMiddleString;

const char f_sSplitString[] = ";";
const char * CxGlobal::splitString = f_sSplitString;

const char f_sSpaceString[] = " ";
const char * CxGlobal::spaceString = f_sSpaceString;

fn_void_msg_tlv_t CxGlobal::assertCallBack = 0;

