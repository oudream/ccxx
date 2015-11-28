#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>

#include "cxsemipc.h"

CxSemIpc::CxSemIpc()
{
}
void CxSemIpc::init_sem(int sem_id,int sem_num)
{
    /*设置信号量的初始值，就是资源个数*/
    union semun{
        int val;
        struct semid_ds *buf;
        unsigned short *array;
        struct seminfo *__buf;
    } sem_u;

    sem_u.val=1;
    semctl(sem_id,sem_num,SETVAL,sem_u);
}

void CxSemIpc::sem_v(int sem_id)
{
    struct sembuf v;
    v.sem_num = 0;
    v.sem_op  = 1;
    v.sem_flg = 0;
    if(semop(sem_id,&v,1)==-1)
    printf("p operation is fail\n");
}

void CxSemIpc::sem_p(int sem_id)
{
    struct sembuf p;
    p.sem_num = 0;
    p.sem_op  = -1;
    p.sem_flg = 0;
    if(semop(sem_id,&p,1)==-1)
    printf("p operation is fail\n");
}

void CxSemIpc::delete_sem(int sem_id)
{
    if(semctl(sem_id,0,IPC_RMID,0)==-1)
        printf("semctl delete error\n");
}
