
#include "global.h"

using namespace std;

//比较标准的做法，是借助第三个变量实现的。
int testFibonacciPolynomial11(){
    int f1=0,f2=1,t,n=1;
//    cout<<"数列第1个："<<f1<<endl;
    cout<<"f1: "<<f1<<endl;
//    cout<<"数列第2个："<<f2<<endl;
    cout<<"f2: "<<f2<<endl;
    for(n=3;n<=20;n++){
        t=f2;
        f2=f1+f2;
        f1=t;
        cout<<"->f"<<n<<": "<<f2<<endl;
    }
    cout<<endl;
    return 0;
}

//这是楼主学习的时候自己想到的方法，可以通过两次加分，一次循环输出两个项。
int testFibonacciPolynomial12(){
    int f1=0,f2=1,t,n=1;
	//cout << "数列第一项：" << f1 << endl;
	cout << "first:" << f1 << endl;
	cout << "second:" << f2 << endl;
    for(n=2;n<10;n++){
        f1=f1+f2;
        cout<<"column["<<(2*n-1)<<"]:"<<f1<<endl;
        f2=f1+f2;
        cout<<"column["<<(2*n)<<"]"<<f2<<endl;
    }
    cout<<endl;
    return 0;
}