#include "cxprofile.h"

#ifndef GM_PROJECT_JSON_NULL
#include "cxprofile_skv_json.h"
#endif

#ifndef GM_PROJECT_XML_NULL
#include "cxprofile_skv_xml.h"
#endif

std::vector<CxFactoryTemplate<CxSkverBase> *> *CxSkverBase::factoriesContainer()
{
    static std::vector<CxFactoryTemplate<CxSkverBase> *> m;
    return &m;
}

void CxSkverBase::factoriesCreateAndRegister()
{
    static bool bNotInit = true;

    if (bNotInit)
    {
        CxSkverFactoryManager::createAndRegister<CxSkverIniFactory>();
        CxSkverFactoryManager::createAndRegister<CxSkverTxtFactory>();
#ifndef GM_PROJECT_JSON_NULL
        CxSkverFactoryManager::createAndRegister<CxSkverJsonFactory>();
#endif
#ifndef GM_PROJECT_XML_NULL
		CxSkverFactoryManager::createAndRegister<CxSkverXmlFactory>();
#endif
        bNotInit = false;
    }
}
