#ifndef CXSEMIPC_H
#define CXSEMIPC_H

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <unistd.h>
//#include <sys/types.h>
//#include <sys/ipc.h>
//#include <sys/shm.h>
//#include <errno.h>

class CxSemIpc
{
public:
    CxSemIpc();

    void init_sem(int sem_id, int sem_num);

    void sem_v(int sem_id);

    void sem_p(int sem_id);


    void delete_sem(int sem_id);
};

#endif // CXSEMIPC_H
