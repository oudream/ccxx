#include <unistd.h>

#include <stdio.h>

int main()
{
    int m = 0;

    printf("\nmain - [%lld]", sizeof(char *));
    if (fork() == 0)
    {
        printf("\nfork() == 0 - [%lld]", (long long)(&m));
        int n, k;
        while (scanf("%d%d", &n, &k) != EOF)
        {
            printf("\nfork() == 0 - [%d] - %d - %d", ++m, n, k);
            fflush(stdout);
            sleep(3);
        }
    }
    else
    {
        printf("\nfork() != 0 - [%lld]", (long long)(&m));
        int n, k;
        while (scanf("%d%d", &n, &k) != EOF)
        {
            printf("\nfork() != 0 - [%d] - %d - %d", ++m, n, k);
            fflush(stdout);
            sleep(3);
        }
    }
    return 0;
}