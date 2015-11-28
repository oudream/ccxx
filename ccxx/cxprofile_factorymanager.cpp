

#include "cxprofile_skv_ini.h"
#include "cxprofile_skv_txt.h"
#include "cxprofile_skv_xml.h"

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
        CxSkverFactoryManager::createAndRegister<CxSkverXmlFactory>();
        bNotInit = false;
    }
}


