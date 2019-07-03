#include<stdio.h>
#include<unistd.h>
#include<signal.h>

int main(int argc, char *agrv[])
{
    int i = 0;

    int pid = 13666; // config
    for(;i<5;i++)
    {
        kill(pid,SIGINT);
    }

    return 0;
}
