#include "cxchannel_none.h"

#include "cxapplication.h"

CxChannelNone::CxChannelNone()
{
}

CxChannelNone::~CxChannelNone()
{
//    close();
}


void CxChannelNone::toContext(CxISetSkv& context) const
{
    CxChannelBase::toContext(context);
}


void CxChannelNone::fromContext(const CxIGetSkv& context)
{
    CxChannelBase::fromContext(context);
}


