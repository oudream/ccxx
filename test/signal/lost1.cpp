#include<stdio.h>
#include<unistd.h>
#include<signal.h>

//typedef void (*fn_void_int_t)(int);

void deal(int)
{
    printf("signal happened!\n");
}

int main(int argc, char *agrv[])
{
    printf("begin test signal-lost1:");

    signal(SIGINT, deal);
//    signal(35, deal);

    while (1)
    {
//        printf("begin test signal-lost1:");
//        sleep(1);
    }

    printf("end test signal-lost1.");

    return 0;
}