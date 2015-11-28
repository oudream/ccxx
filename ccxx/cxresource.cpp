#include "cxresource.h"


#include "cxapplication.h"
#include "cxfile.h"
#include "cxprofile.h"


using namespace std;


CxResource * fn_oResource()
{
    CxApplication::registInitFunction(CxResource::start);
    CxApplication::registStopFunction(CxResource::stop);

    static CxResource m;
    return & m;
}

static CxResource * f_oResource = fn_oResource();


map<string, string> f_resourceStrings;


void CxResource::start()
{
    loadResourceString();
}

void CxResource::stop()
{
}

const map<string, string> &CxResource::getResourceStrings()
{
    return f_resourceStrings;
}

string CxResource::findResourceString(const string &sStringName)
{
    map<string, string>::const_iterator it = f_resourceStrings.find(sStringName);
    if (it != f_resourceStrings.end())
    {
        return it->second;
    }
    else
    {
        return std::string();
    }
}

void CxResource::loadResourceString()
{
    string sFilePath = CxApplication::findConfig(CS_SectionProgramConfig, CS_EntryResourceConfigFile, std::string());
    if (sFilePath.empty()) sFilePath = "config/apps.resource.ini";
    {
        string sResourceFilePath = CxFileSystem::fullFilePath(CxApplication::applicationDeployPath(), sFilePath);
        CxSkverBase * oProfile = CxSkverManager::beginUse(sResourceFilePath);
        if (oProfile)
        {
            f_resourceStrings = oProfile->getEntryValues(CS_SectionResourceStrings);
            CxSkverManager::endUse(oProfile);
        }
    }
}

