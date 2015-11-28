#include"cxmath.h"


int CxMath::gcd(int a,int b)
{
    int temp;
    if(a<b)
    {
        /*交换两个数，使大数放在a上*/
        temp=a;
        a=b;
        b=temp;
    }
    while(b!=0)
    {
        /*利用辗除法，直到b为0为止*/
        temp=a%b;
        a=b;
        b=temp;
    }
    return a;
}

int CxMath::lcm(int a, int b)
{
    return a/gcd(a,b)*b;
}

int CxMath::random(int iMax)
{
    int r = rand();
    if (iMax != 0)
        r = r % iMax;
    return r;
}

int CxMath::divRound(int iDivisor, int iDivisored)
{
    div_t r = div(iDivisor, iDivisored);
    if (r.rem != 0)
    {
        if (r.quot > 0)
            return r.quot + 1;
        else
            return r.quot - 1;
    }
    else
    {
        return r.quot;
    }
}

