#include <unistd.h>

int fd[2];
char * argv[ ]={"ls","-l","/ddd",(char *)0};
char * argv2[ ]={"wc","-l",(char *)0};
char * envp[ ]={"PATH=/bin:/usr/bin",(char *)0};

void run_ls()
{
    dup2(fd[1],1);
    close(fd[0]);
    close(fd[1]);
    execve("/bin/ls",argv,envp);
}
void run_wc()
{
    dup2(fd[0],0);
    close(fd[0]);
    close(fd[1]);
    execve("/usr/bin/wc",argv2,envp);
}

int main()
{
    pipe(fd);
    if(fork()==0)
        run_ls();
    else
        run_wc();
    return 0;
}