
#include "global.h"

using namespace std;

//*******************************************************************//
//生成N个互不重复的随机数函数
//*******************************************************************//

int testRandom1(int argc,char*argv[])
{
    int randValue[10];
    srand(time(0));
    cout<<"本次所用种子为： "<<time(0)<<endl;
    for(int i=0;i<10;i++)
    {
        randValue[i]=rand();
        cout<<randValue[i]<<endl;
    }
    system("pause");
}


//*******************************************************************//
//生成N个互不重复的随机数函数
//*******************************************************************//

//定义需要生成的随机数数量
#define Number 15
int a1[Number];
int a2[Number];

void Randarrary( vector<int>& arrarInt) ;

int testRandom2(int argc,char*argv[])
{
    vector<int> arrarInt;
    for(int i=0;i<Number;i++)
    {
        arrarInt.push_back(i);
    }
    Randarrary(arrarInt);
    for(int i=0;i<Number;i++)
    {
        cout<<"第 "<<i<<" 个不重复的随机数是"<<arrarInt[i]<<endl;
    }
    system("pause");
}

void Randarrary( vector<int>& arrarInt)
{
    int i1 = int(time(0));
    srand(i1);
    for (int i = 0; i < arrarInt.size(); i++)
    {
        int i2 = rand();
        int a = i2 % (arrarInt.size() - i) + i;
        swap(arrarInt[i], arrarInt[a]);
        a2[i] = i2;
    }
}


//*******************************************************************//
//生成N个互不重复的随机数函数
//*******************************************************************//

#include <random>   // for default_random_engine & uniform_int_distribution<int>
#include <chrono>   // to provide seed to the default_random_engine

default_random_engine dre (chrono::steady_clock::now().time_since_epoch().count());     // provide seed

int random(int lim)
{
    uniform_int_distribution<int> uid {0,lim};   // help dre to generate nos from 0 to lim (lim included);
    return uid(dre);    // pass dre as an argument to uid to generate the random no
}

int testRandom3(int argc,char*argv[])
{
    for (int i=0;i<10;++i)
        cout<<random(10)<<" ";
    return 0;
}
