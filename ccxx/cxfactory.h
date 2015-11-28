#ifndef CXFACTORY_H
#define CXFACTORY_H

#include "cxglobal.h"


//生成器
template<class ClassBase>
class CxFactoryTemplate
{
public:
    CxFactoryTemplate(){}

    virtual ~CxFactoryTemplate(){}

    virtual std::string factoryName() =0;

    virtual ClassBase* createObject() = 0;

};


//生成器管理者
template<class ClassBase>
class CxFactoryManagerTemplate
{
public:
    typedef CxFactoryTemplate<ClassBase> CxFactoryBase;

    template<class FactorySon>
    static FactorySon * createAndRegister()
    {
        FactorySon * oNewFactory = new FactorySon();
        CxFactoryBase * oFactory = factory(oNewFactory->factoryName());
        if (oFactory)
        {
            delete oNewFactory;
            return reinterpret_cast<FactorySon*>(oFactory);
        }
        else
        {
            ClassBase::factoriesContainer()->push_back(oNewFactory);
            return oNewFactory;
        }
    }

    static const std::vector<CxFactoryBase*>* factorys()
    {
        return ClassBase::factoriesContainer();
    }

    static std::vector<std::string> factoryNames()
    {
        std::vector<std::string> slResult;
        std::vector<CxFactoryBase*>* oFactorys = ClassBase::factoriesContainer();
        for (size_t i = 0; i < oFactorys->size(); ++i)
        {
            CxFactoryBase* o = oFactorys->at(i);
            slResult.push_back(o->factoryName());
        }
        return slResult;
    }

    static CxFactoryBase* factory(const std::string& sFactoryName)
    {
        std::vector<CxFactoryBase*>* oFactorys = ClassBase::factoriesContainer();
        for (size_t i = 0; i < oFactorys->size(); ++i) {
            CxFactoryBase* o = oFactorys->at(i);
            if (o->factoryName() == sFactoryName)
                return o;
        }
        return NULL;
    }

    template<class ClassSon>
    static ClassSon* createInstance(const std::string& sFactoryName)
    {
        ClassBase::factoriesCreateAndRegister();

        CxFactoryBase* oFactoryBase = factory( sFactoryName );
        if ( oFactoryBase )
        {
            return reinterpret_cast<ClassSon*>(oFactoryBase->createObject());
        }
        return NULL;
    }

    static ClassBase* createObject(const std::string& sFactoryName)
    {
        ClassBase::factoriesCreateAndRegister();

        CxFactoryBase* oFactoryBase = factory( sFactoryName );
        if ( oFactoryBase )
        {
            return oFactoryBase->createObject();
        }
        return NULL;
    }

protected:
//    static std::vector<CxFactoryBase*>* factoriesContainer()
//    {
//        static std::vector<CxFactoryBase*> s_factories;
//        return & s_factories;
//        std::vector<CxFactoryBase*> * oFactories = (std::vector<CxFactoryBase*> *)s_factorys;
//        if (! s_factorys )
//        {
//            oFactories = new std::vector<CxFactoryBase*>();
//            s_factorys = oFactories;
//        }
//        return oFactories;
//    }

//    static void factoriesCreateAndRegister()
//    {
//        static int s_noRegister = true;
//        if (s_noRegister)
//        {
//            ClassBase::factoriesCreateAndRegister();
//            s_noRegister = false;
//        }
//    }

//    static void* s_factorys;

};

//template<class ClassBase>
//void* CxFactoryManagerTemplate<ClassBase>::s_factorys = NULL;


#endif // CXFACTORY_H
