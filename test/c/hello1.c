#include <stdio.h>

typedef enum {eone,etwo} enum_a_t;

char * show1()
{
}

int show2()
{

}

int f_i1;

int show3()
{
    int a;
    int b;
    printf("%d - %d\n", a, b);
    printf("%d\n", f_i1);
    return 0;
}

int main()
{
    /* 我的第一个 C 程序 */
    printf("Hello, World! \n");

    printf("{%d, %d}\n", eone, etwo);

    char * p = show1();

    printf("{%d}\n", (long long)p);

    printf("{%d}\n", show2());

    printf("---\n");

    show3();

    return 0;
}
