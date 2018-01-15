#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

typedef void (*fn_void_int_t)(int);

void fn_app_SIGABRT (int param)
{
    cout        << "signal signal signal !!! CxApplication Received SIGABRT , param=" << param << endl;
    exit(1);
}
void fn_app_SIGFPE (int param)
{
    cout        << "signal signal signal !!! CxApplication Received SIGFPE , param=" << param << endl;
    exit(1);
}
void fn_app_SIGILL (int param)
{
    cout        << "signal signal signal !!! CxApplication Received SIGILL , param=" << param << endl;
    exit(1);
}
void fn_app_SIGINT (int param)
{
    cout        << "signal signal signal !!! CxApplication Received SIGINT , param=" << param << endl;
    exit(1);
}
void fn_app_SIGSEGV (int param)
{
    cout        << "signal signal signal !!! CxApplication Received SIGSEGV , param=" << param << endl;
    exit(1);
}
void fn_app_SIGTERM (int param)
{
    cout        << "signal signal signal !!! CxApplication Received SIGTERM , param=" << param << endl;
    exit(1);
}


void my_handler(int s){
           printf("Caught signal %d\n",s);
           exit(1);

}

std::string ginput()
{
    std::string msg;
    std::cin >> msg;
    return msg;
}

template<typename _Tp>
static void fn_remove(std::vector<_Tp> & vector1, const _Tp & value)
{
    vector1.erase(std::remove(vector1.begin(), vector1.end(), value), vector1.end());
}

int main(int argc,char** argv)
{
    fn_void_int_t fn_sig_handler;
//    fn_sig_handler = signal (SIGABRT, fn_app_SIGABRT);
//    if (fn_sig_handler == SIG_ERR)
//    {
//        std::cout << "error : signal (SIGINT , *)" << std::endl;
//    }
    fn_sig_handler = signal (SIGFPE, fn_app_SIGFPE);
    if (fn_sig_handler == SIG_ERR)
    {
        std::cout << "error : signal (SIGFPE , *)" << std::endl;
    }
    fn_sig_handler = signal (SIGILL, fn_app_SIGILL);
    if (fn_sig_handler == SIG_ERR)
    {
        std::cout << "error : signal (SIGILL , *)" << std::endl;
    }
    fn_sig_handler = signal (SIGINT, fn_app_SIGINT);
    if (fn_sig_handler == SIG_ERR)
    {
        std::cout << "error : signal (SIGINT , *)" << std::endl;
    }
    fn_sig_handler = signal (SIGSEGV, fn_app_SIGSEGV);
    if (fn_sig_handler == SIG_ERR)
    {
        std::cout << "error : signal (SIGSEGV , *)" << std::endl;
    }
    fn_sig_handler = signal (SIGTERM, fn_app_SIGTERM);
    if (fn_sig_handler == SIG_ERR)
    {
        std::cout << "error : signal (SIGTERM , *)" << std::endl;
    }

//   struct sigaction sigIntHandler;

//   sigIntHandler.sa_handler = my_handler;
//   sigemptyset(&sigIntHandler.sa_mask);
//   sigIntHandler.sa_flags = 0;

//   sigaction(SIGINT, &sigIntHandler, NULL);

    string s("sigIntHandler");
    std::cout << s.find_first_of("ai") << std::endl;
    std::cout << s.substr(0, s.find_first_of("ai")) << std::endl;

    std::vector<int> myvector;
     // set some values (from 1 to 10)
     for (int i=1; i<=10; i++) myvector.push_back(i);
    fn_remove(myvector, 100);
    for (int i=1; i<=myvector.size(); i++)
    {
        cout << i << endl;
    }
    fn_remove(myvector, 1);
    fn_remove(myvector, 3);
    fn_remove(myvector, 5);
    for (int i=1; i<myvector.size(); i++)
    {
        cout << myvector[i] << endl;
    }

   while (1) {
       string s1 = ginput();
       if (s1.find("exit") != string::npos) break;
       std::cout << s1 << std::endl;
   }

   return 0;
}
