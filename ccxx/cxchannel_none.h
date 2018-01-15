#ifndef CXCHANNEL_NONE_H
#define CXCHANNEL_NONE_H


#include "cxchannel.h"


class GM_CCXX_CORE_API CxChannelNone : public CxChannelBase
{
public:
    CxChannelNone();

    ~CxChannelNone();

    inline const std::string& typeSimpleName() const { return CS_TypeSimpleName_None; }

    inline int typeSimpleId() const { return CxInterinfo::Type_IO_None; }

    void toContext(CxISetSkv& context) const ;

    void fromContext(const CxIGetSkv& context);
protected:
    ///----------Abstruct Realization --- begin ---
    bool getConnectedImpl() const{return false;}

    void openChannelImpl(){}

    void closeChannelImpl(){}

    int writeDataImpl(const char* pData, int iLength, void * oTarget){return 0;}

    void processError( int iError , const std::string& sError , int iTag ){}

    bool isSameChannelImpl(const std::map<std::string, std::string> &params) const {return false;}

    void toReport(std::vector<std::string>& sReports)const{}

    void toDescribe( CxDescribeUnit & describeUnit) const{}


    ///----------Abstruct Realization --- end ---

};

class GM_CCXX_CORE_API CxChannelNoneFactory : public CxChannelFactoryBase
{
//    GM_SINGLETON_DECLARATION( CxChannelNoneFactory )
public:
    std::string factoryName() {
        return CS_TypeSimpleName_None;
    }

    CxChannelBase* createObject() {
        return new CxChannelNone;
    }

};

#endif // CXCHANNEL_NONE_H
