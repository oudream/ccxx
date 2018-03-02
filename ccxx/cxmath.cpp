#include"cxmath.h"


int CxMath::gcd(int a,int b)
{
    int temp;
    if(a<b)
    {
        temp=a;
        a=b;
        b=temp;
    }
    while(b!=0)
    {
        temp=a%b;
        a=b;
        b=temp;
    }
    return a;

    if(b) while((a %= b) && (b %= a));
    return a + b;
}

int CxMath::lcm(int a, int b)
{
    return a/gcd(a,b)*b;

    return a * b / gcd(a, b);
}

int CxMath::random(int iMax)
{
    int r = rand();
    if (iMax != 0)
        r = r % iMax;
    return r;
    /*
        srand((int)time(0));
        int num = rand()%100000;
    //    printf("rand num:%6d\n", num);

        struct tm *p;
        time_t second;
        time(&second);

        p = localtime(&second);

        char buf[64] = {0};

        sprintf(buf, "%d-%d-%d %d:%d:%d_%06d", 1900+p->tm_year, 1+p->tm_mon, p->tm_mday,
            p->tm_hour, p->tm_min, p->tm_sec, num);

        return std::string(buf);

    //    printf("guid:%s\n", buf);

    */
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

