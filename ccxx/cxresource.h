#ifndef CXRESOURCE_H
#define CXRESOURCE_H


#include "cxglobal.h"
#include "cxstring.h"
#include "cxbitmap.h"


static const std::string CS_EntryResourceConfigFile = "ResourceConfigFile";
static const std::string CS_SectionResourceStrings = "ResourceStrings";


class GM_CCXX_CORE_API CxResource
{
public:
    static void start( );

    static void stop( );

    static const std::map<std::string, std::string>& getResourceStrings();

    static std::string findResourceString(const std::string& sStringName);

private:
    static void loadResourceString();

};

#endif // CXRESOURCE_H
