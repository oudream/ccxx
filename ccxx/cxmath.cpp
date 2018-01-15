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
        srand((int)time(0));   //设置时间种子
        int num = rand()%100000;   //生成100000内的随机数
    //    printf("rand num:%6d\n", num);

        struct tm *p;
        time_t second;
        time(&second);

        p = localtime(&second);//获取当前时间

        char buf[64] = {0};

        sprintf(buf, "%d-%d-%d %d:%d:%d_%06d", 1900+p->tm_year, 1+p->tm_mon, p->tm_mday,
            p->tm_hour, p->tm_min, p->tm_sec, num);  //格式化时间和随机数字符串，存放在buf内

        return std::string(buf);

    //    printf("guid:%s\n", buf);  //输出guid到控制台

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

