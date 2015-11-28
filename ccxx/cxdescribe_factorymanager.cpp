
#include "cxdescribe.h"

std::vector<CxFactoryTemplate<CxDescribeBase> *> *CxDescribeBase::factoriesContainer()
{
    static std::vector<CxFactoryTemplate<CxDescribeBase> *> m;
    return &m;
}

void CxDescribeBase::factoriesCreateAndRegister()
{
    static bool bNotInit = true;

    if (bNotInit)
    {
        CxDescribeFactoryManager::createAndRegister<CxDescribeBooleanFactory>();
        CxDescribeFactoryManager::createAndRegister<CxDescribeDateTimeFactory>();
        CxDescribeFactoryManager::createAndRegister<CxDescribeDoubleFactory>();
        CxDescribeFactoryManager::createAndRegister<CxDescribeEnumFactory>();
        CxDescribeFactoryManager::createAndRegister<CxDescribeIntegerFactory>();
        CxDescribeFactoryManager::createAndRegister<CxDescribeStringFactory>();
        bNotInit = false;
    }
}


