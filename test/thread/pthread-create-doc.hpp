#ifndef CCXX_PTHREAD_CREATE_DOC_HPP
#define CCXX_PTHREAD_CREATE_DOC_HPP


//原文：https://blog.csdn.net/DLUTBruceZhang/article/details/8824779

int __pthread_create_2_1(pthread_t *thread, const pthread_attr_t *attr,
                         void * (*start_routine)(void *), void *arg)
// thread 为pthread_t，实则是pthread指针。
// typedef struct pthread *pthread_t; ->unsigned long int
// attr 创建线程时的附加属性
// start_routine 执行的线程函数
// arg参数首地址指针
{
    pthread_descr self = thread_self();
// 向管理线程的请求
    struct pthread_request request;
// 在初始化时 全局 int __pthread_manager_request = -1;
    if (__pthread_manager_request < 0) {
// 首先Linuxthreads需要有__pthread_manager_thread，这个线程是个很特殊的线程
// 初始化Linuxthreads系统线程
        if (__pthread_initialize_manager() < 0) return EAGAIN;
    }
    request.req_thread = self;
// 请求类型 CREATE ，并将新线程（轻量进程）的参数都打包到request结构中。
    request.req_kind = REQ_CREATE;
    request.req_args.create.attr = attr;
    request.req_args.create.fn = start_routine;
    request.req_args.create.arg = arg;
//sigprocmask用于改变进程的当前阻塞信号集
    sigprocmask(SIG_SETMASK, (const sigset_t *) NULL,
                &request.req_args.create.mask);
// 向__pthread_manager_request写入request请求。 request请求将被_pthread_manager_thread读取并处理。
    __libc_write(__pthread_manager_request, (char *) &request, sizeof(request));
// 挂起一下
    suspend(self);
    if (THREAD_GETMEM(self, p_retcode) == 0)
        *thread = (pthread_t) THREAD_GETMEM(self, p_retval);
    return THREAD_GETMEM(self, p_retcode);
}

#if defined HAVE_ELF && defined PIC && defined DO_VERSIONING
default_symbol_version (__pthread_create_2_1, pthread_create, GLIBC_2.1);

int __pthread_create_2_0(pthread_t *thread, const pthread_attr_t *attr,
void * (*start_routine)(void *), void *arg)
{
/* The ATTR attribute is not really of type `pthread_attr_t *'. It has
the old size and access to the new members might crash the program.
We convert the struct now. */
pthread_attr_t new_attr;

if (attr != NULL)
{
size_t ps = __getpagesize ();

memcpy (&new_attr, attr,
(size_t) &(((pthread_attr_t*)NULL)->__guardsize));
new_attr.__guardsize = ps;
new_attr.__stackaddr_set = 0;
new_attr.__stackaddr = NULL;
new_attr.__stacksize = STACK_SIZE - ps;
attr = &new_attr;
}
return __pthread_create_2_1 (thread, attr, start_routine, arg);
}
symbol_version (__pthread_create_2_0, pthread_create, GLIBC_2.0); // 由此可知pthread_create入口函数为__pthread_create_2_1
#else
strong_alias (__pthread_create_2_1, pthread_create)
#endif
//由上面的函数可知，pthread_create对线程的创建实际上是向pthread_manager_thread发送个request请求。
//下面我们就看pthread_manager_thread是怎样初始化的：


// 首先要进行基本系统的初始化工作：
// 即填充__pthread_initial_thread（算是模板吧？）其他属性：
/* Descriptor of the initial thread */
struct _pthread_descr_struct __pthread_initial_thread = {
    &__pthread_initial_thread, /* pthread_descr p_nextlive */ // 将进程中所有用户线程串在了一起
    &__pthread_initial_thread, /* pthread_descr p_prevlive */
    NULL, /* pthread_descr p_nextwaiting */
    NULL, /* pthread_descr p_nextlock */
    PTHREAD_THREADS_MAX, /* pthread_t p_tid */
    0, /* int p_pid */
    0, /* int p_priority */
    &__pthread_handles[0].h_lock, /* struct _pthread_fastlock * p_lock */
    0, /* int p_signal */
    NULL, /* sigjmp_buf * p_signal_buf */
    NULL, /* sigjmp_buf * p_cancel_buf */
    0, /* char p_terminated */
    0, /* char p_detached */
    0, /* char p_exited */
    NULL, /* void * p_retval */
    0, /* int p_retval */
    NULL, /* pthread_descr p_joining */
    NULL, /* struct _pthread_cleanup_buffer * p_cleanup */
    0, /* char p_cancelstate */
    0, /* char p_canceltype */
    0, /* char p_canceled */
    NULL, /* int *p_errnop */
    0, /* int p_errno */
    NULL, /* int *p_h_errnop */
    0, /* int p_h_errno */
    NULL, /* char * p_in_sighandler */
    0, /* char p_sigwaiting */
    PTHREAD_START_ARGS_INITIALIZER, /* struct pthread_start_args p_start_args */
    {NULL}, /* void ** p_specific[PTHREAD_KEY_1STLEVEL_SIZE] */
    {NULL}, /* void * p_libc_specific[_LIBC_TSD_KEY_N] */
    0, /* int p_userstack */
    NULL, /* void * p_guardaddr */
    0, /* size_t p_guardsize */
    &__pthread_initial_thread, /* pthread_descr p_self */
    0, /* Always index 0 */
    0, /* int p_report_events */
    {{{0, }}, 0, NULL}, /* td_eventbuf_t p_eventbuf */
    ATOMIC_INITIALIZER, /* struct pthread_atomic p_resume_count */
    0, /* char p_woken_by_cancel */
    NULL /* struct pthread_extricate_if *p_extricate */
};

static void pthread_initialize(void)
{
    struct sigaction sa;
    sigset_t mask;
    struct rlimit limit;
    int max_stack;

//__pthread_initial_thread_bos: Limit between the stack of the initial thread (above) and the
//stacks of other threads (below). Aligned on a STACK_SIZE boundary.
/* 如果初始化工作已完成，则退出 */
    if (__pthread_initial_thread_bos != NULL) return;
#ifdef TEST_FOR_COMPARE_AND_SWAP
    /* Test if compare-and-swap is available */
__pthread_has_cas = compare_and_swap_is_available();
#endif
/* For the initial stack, reserve at least STACK_SIZE bytes of stack
below the current stack address, and align that on a
STACK_SIZE boundary. */
    __pthread_initial_thread_bos =
        (char *)(((long)CURRENT_STACK_FRAME. - 2 * STACK_SIZE) & ~(STACK_SIZE - 1));
/* 对initial_thread.pid赋值：主进程pid */
    __pthread_initial_thread.p_pid = __getpid();
/* If we have special thread_self processing, initialize that for the
main thread now. */
#ifdef INIT_THREAD_SELF
    INIT_THREAD_SELF(&__pthread_initial_thread, 0);
#endif
/* 共享主进程errno/h_errno.*/
    __pthread_initial_thread.p_errnop = &_errno;
    __pthread_initial_thread.p_h_errnop = &_h_errno;
/* Play with the stack size limit to make sure that no stack ever grows
beyond STACK_SIZE minus two pages (one page for the thread descriptor
immediately beyond, and one page to act as a guard page). */
    getrlimit(RLIMIT_STACK, &limit); // 获取STACK limit
    max_stack = STACK_SIZE - 2 * __getpagesize();
    if (limit.rlim_cur > max_stack) {
        limit.rlim_cur = max_stack;
        setrlimit(RLIMIT_STACK, &limit);
    }
#ifdef __SIGRTMIN
    /* Initialize real-time signals. */
init_rtsigs ();
#endif
/* Setup signal handlers for the initial thread.
Since signal handlers are shared between threads, these settings
will be inherited by all other threads. */
// 设置initial thread信号处理函数 其他线程都将继承
#ifndef __i386__
    sa.sa_handler = pthread_handle_sigrestart;
#else
    sa.sa_handler = (__sighandler_t) pthread_handle_sigrestart;
#endif
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    __sigaction(__pthread_sig_restart, &sa, NULL);
#ifndef __i386__
    sa.sa_handler = pthread_handle_sigcancel;
#else
    sa.sa_handler = (__sighandler_t) pthread_handle_sigcancel;
#endif
    sa.sa_flags = 0;
    __sigaction(__pthread_sig_cancel, &sa, NULL);
    if (__pthread_sig_debug > 0) {
        sa.sa_handler = pthread_handle_sigdebug;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        __sigaction(__pthread_sig_debug, &sa, NULL);
    }
/* Initially, block __pthread_sig_restart. Will be unblocked on demand. */
    sigemptyset(&mask);
    sigaddset(&mask, __pthread_sig_restart);
    sigprocmask(SIG_BLOCK, &mask, NULL);
/* Register an exit function to kill all other threads. */
/* Do it early so that user-registered atexit functions are called
before pthread_exit_process. */
    __on_exit(pthread_exit_process, NULL);
}


// 下面是__pthread_manager_thread 的初始化
/* Descriptor of the manager thread; none of this is used but the error
variables, the p_pid and p_priority fields,
and the address for identification. */

struct _pthread_descr_struct __pthread_manager_thread = {
    NULL, /* pthread_descr p_nextlive */ // 这两个值为空！
    NULL, /* pthread_descr p_prevlive */
    NULL, /* pthread_descr p_nextwaiting */
    NULL, /* pthread_descr p_nextlock */
    0, /* int p_tid */
    0, /* int p_pid */
    0, /* int p_priority */
    &__pthread_handles[1].h_lock, /* struct _pthread_fastlock * p_lock */
    0, /* int p_signal */
    NULL, /* sigjmp_buf * p_signal_buf */
    NULL, /* sigjmp_buf * p_cancel_buf */
    0, /* char p_terminated */
    0, /* char p_detached */
    0, /* char p_exited */
    NULL, /* void * p_retval */
    0, /* int p_retval */
    NULL, /* pthread_descr p_joining */
    NULL, /* struct _pthread_cleanup_buffer * p_cleanup */
    0, /* char p_cancelstate */
    0, /* char p_canceltype */
    0, /* char p_canceled */
    &__pthread_manager_thread.p_errno, /* int *p_errnop */
    0, /* int p_errno */
    NULL, /* int *p_h_errnop */
    0, /* int p_h_errno */
    NULL, /* char * p_in_sighandler */
    0, /* char p_sigwaiting */
    PTHREAD_START_ARGS_INITIALIZER, /* struct pthread_start_args p_start_args */
    {NULL}, /* void ** p_specific[PTHREAD_KEY_1STLEVEL_SIZE] */
    {NULL}, /* void * p_libc_specific[_LIBC_TSD_KEY_N] */
    0, /* int p_userstack */
    NULL, /* void * p_guardaddr */
    0, /* size_t p_guardsize */
    &__pthread_manager_thread, /* pthread_descr p_self */
    1, /* Always index 1 */
    0, /* int p_report_events */
    {{{0, }}, 0, NULL}, /* td_eventbuf_t p_eventbuf */
    ATOMIC_INITIALIZER, /* struct pthread_atomic p_resume_count */
    0, /* char p_woken_by_cancel */
    NULL /* struct pthread_extricate_if *p_extricate */
};
int __pthread_initialize_manager(void)
{
// 管理线程与其他线程之间的通信是通过管道完成的。
/*在一个进程空间内，管理线程与其他线程之间通过一对"管理管道（manager_pipe[2]）"来通讯，
该管道在创建管理线程之前创建，在成功启动了管理线程之后，管理管道的读端和写端分别赋给两个全局变量
__pthread_manager_reader和__pthread_manager_request，之后，每个用户线程都通过
__pthread_manager_request向管理线程发请求，但管理线程本身并没有直接使用__pthread_manager_reader，
管道的读端（manager_pipe[0]）是作为__clone()的参数之一传给管理线程的，
管理线程的工作主要就是监听管道读端，并对从中取出的请求作出反应。*/
    int manager_pipe[2];
    int pid;
    struct pthread_request request;

/* 如果基本的初始化未进行，那么进行初始化*/
    if (__pthread_initial_thread_bos == NULL) pthread_initialize();
/* 设置stack */
    __pthread_manager_thread_bos = malloc(THREAD_MANAGER_STACK_SIZE);
    if (__pthread_manager_thread_bos == NULL) return -1;
// top值
    __pthread_manager_thread_tos =
        __pthread_manager_thread_bos + THREAD_MANAGER_STACK_SIZE;
/* 设置通信管道 */
    if (pipe(manager_pipe) == -1) {
        free(__pthread_manager_thread_bos);
        return -1;
    }
/* 在创建线程时 pid一定不会为0 */
    pid = 0;
    if (__pthread_initial_thread.p_report_events)
    {
/* It's a bit more complicated. We have to report the creation of
the manager thread. */
        int idx = __td_eventword (TD_CREATE);
        uint32_t mask = __td_eventmask (TD_CREATE);

        if ((mask & (__pthread_threads_events.event_bits[idx]
                     | __pthread_initial_thread.p_eventbuf.eventmask.event_bits[idx]))
            != 0)
        {
// sys_fork：创建 pthread_manager线程 ：共享VM FS FILES等
            pid = __clone(__pthread_manager_event,
                          (void **) __pthread_manager_thread_tos,
                          CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND,
                          (void *)(long)manager_pipe[0]); // 读管道

            if (pid != -1)
            {
/* Now fill in the information about the new thread in
the newly created thread's data structure. We cannot let
the new thread do this since we don't know whether it was
already scheduled when we send the event. */
// 对新线程添加属性
                __pthread_manager_thread.p_eventbuf.eventdata =
                    &__pthread_manager_thread;
                __pthread_manager_thread.p_eventbuf.eventnum = TD_CREATE;
                __pthread_last_event = &__pthread_manager_thread;
// _pthread_manager_thread的是2*PTHREAD_THREADS_MAX+1
                __pthread_manager_thread.p_tid = 2* PTHREAD_THREADS_MAX + 1;
                __pthread_manager_thread.p_pid = pid; // clone进程pid

/* Now call the function which signals the event. */
                __linuxthreads_create_event ();

/* Now restart the thread. */
                __pthread_unlock(__pthread_manager_thread.p_lock);
            }
        }
    }
// 如果__pthread_initial_thread.p_report_events标记了，自然不再创建新线程了
    if (pid == 0)
        pid = __clone(__pthread_manager, (void **) __pthread_manager_thread_tos,
                      CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND,
                      (void *)(long)manager_pipe[0]);
    if (pid == -1) {
        free(__pthread_manager_thread_bos);
        __libc_close(manager_pipe[0]);
        __libc_close(manager_pipe[1]);
        return -1;
    }
// __pthread_manager_request fd = manager_pipe[1]; 交给其他线程写请求
    __pthread_manager_request = manager_pipe[1]; /* 全局变量writing end */
    __pthread_manager_reader = manager_pipe[0]; /* 全局变量reading end */
    __pthread_manager_thread.p_tid = 2* PTHREAD_THREADS_MAX + 1;
    __pthread_manager_thread.p_pid = pid;
/* Make gdb aware of new thread manager */
    if (__pthread_threads_debug && __pthread_sig_debug > 0)
    {
        raise(__pthread_sig_debug);
/* We suspend ourself and gdb will wake us up when it is
ready to handle us. */
        __pthread_wait_for_restart_signal(thread_self());
    }
/* Synchronize debugging of the thread manager */
    request.req_kind = REQ_DEBUG;
    __libc_write(__pthread_manager_request, (char *) &request, sizeof(request));
    return 0;
}

//至此，__pthread_manager_thread创建成功。
//下面看看它是怎样管理的：__pthread_manager函数
/* The server thread managing requests for thread creation and termination */

int __pthread_manager(void *arg)
{
    int reqfd = (int) (long int) arg; // 管道的读口fd
    struct pollfd ufd;
    sigset_t mask;
    int n;
    struct pthread_request request;

/* If we have special thread_self processing, initialize it. */
#ifdef INIT_THREAD_SELF
    INIT_THREAD_SELF(&__pthread_manager_thread, 1);
#endif
/* Set the error variable. */
    __pthread_manager_thread.p_errnop = &__pthread_manager_thread.p_errno;
    __pthread_manager_thread.p_h_errnop = &__pthread_manager_thread.p_h_errno;
/* Block all signals except __pthread_sig_cancel and SIGTRAP */
    sigfillset(&mask);
    sigdelset(&mask, __pthread_sig_cancel); /* for thread termination */
    sigdelset(&mask, SIGTRAP); /* for debugging purposes */
    sigprocmask(SIG_SETMASK, &mask, NULL);
/* 提高优先级 等于主线程的优先级 */
    __pthread_manager_adjust_prio(__pthread_main_thread->p_priority);
/* Synchronize debugging of the thread manager */
// 从管道中读取请求
    n = __libc_read(reqfd, (char *)&request, sizeof(request));
    ASSERT(n == sizeof(request) && request.req_kind == REQ_DEBUG);
    ufd.fd = reqfd;
    ufd.events = POLLIN;
/* Enter server loop */
    while(1) {
        n = __poll(&ufd, 1, 2000); // 2秒超时

/* main thread的终止 将导致所有thread终止 */
        if (getppid() == 1) {
            pthread_kill_all_threads(SIGKILL, 0);
            _exit(0);
        }
/* Check for dead children */
        if (terminated_children) {
            terminated_children = 0;
            pthread_reap_children();
        }
/* 读取请求 */
        if (n == 1 && (ufd.revents & POLLIN)) {
            n = __libc_read(reqfd, (char *)&request, sizeof(request));
            ASSERT(n == sizeof(request));
            switch(request.req_kind) {
                case REQ_CREATE: //创建
                    request.req_thread->p_retcode =
                        pthread_handle_create((pthread_t *) &request.req_thread->p_retval,
                                              request.req_args.create.attr,
                                              request.req_args.create.fn,
                                              request.req_args.create.arg,
                                              &request.req_args.create.mask,
                                              request.req_thread->p_pid,
                                              request.req_thread->p_report_events,
                                              &request.req_thread->p_eventbuf.eventmask);
                    restart(request.req_thread);
                    break;
                case REQ_FREE:
                    pthread_handle_free(request.req_args.free.thread_id);
                    break;
                case REQ_PROCESS_EXIT:
                    pthread_handle_exit(request.req_thread,
                                        request.req_args.exit.code);
                    break;
                case REQ_MAIN_THREAD_EXIT:
                    main_thread_exiting = 1;
                    if (__pthread_main_thread->p_nextlive == __pthread_main_thread) {
                        restart(__pthread_main_thread);
                        return 0;
                    }
                    break;
                case REQ_POST:
                    __new_sem_post(request.req_args.post);
                    break;
                case REQ_DEBUG:
/* Make gdb aware of new thread and gdb will restart the
new thread when it is ready to handle the new thread. */
                    if (__pthread_threads_debug && __pthread_sig_debug > 0)
                        raise(__pthread_sig_debug);
                    break;
            }
        }
    }
}

//下面我们看看新的线程是怎样创建的：
static int pthread_handle_create(pthread_t *thread, const pthread_attr_t *attr,
                                 void * (*start_routine)(void *), void *arg,
                                 sigset_t * mask, int father_pid,
                                 int report_events,
                                 td_thr_events_t *event_maskp)
{
    size_t sseg;
    int pid;
    pthread_descr new_thread;
    char * new_thread_bottom;
    pthread_t new_thread_id;
    char *guardaddr = NULL;
    size_t guardsize = 0;
    int pagesize = __getpagesize();

/* First check whether we have to change the policy and if yes, whether
we can do this. Normally this should be done by examining the
return value of the __sched_setscheduler call in pthread_start_thread
but this is hard to implement. FIXME */
    if (attr != NULL && attr->__schedpolicy != SCHED_OTHER && geteuid () != 0)
        return EPERM;
/* Find a free segment for the thread, and allocate a stack if needed */
    for (sseg = 2; ; sseg++)
    {
        if (sseg >= PTHREAD_THREADS_MAX)
            return EAGAIN;
        if (__pthread_handles[sseg].h_descr != NULL)
            continue;
        if (pthread_allocate_stack(attr, thread_segment(sseg), pagesize,
                                   &new_thread, &new_thread_bottom,
                                   &guardaddr, &guardsize) == 0)
            break;
    }
    __pthread_handles_num++;
/* Allocate new thread identifier */
    pthread_threads_counter += PTHREAD_THREADS_MAX;
    new_thread_id = sseg + pthread_threads_counter;
/* Initialize the thread descriptor. Elements which have to be
initialized to zero already have this value. */
// 对新的线程进行属性赋值
    new_thread->p_tid = new_thread_id; //tid=n*PTHREAD_THREADS_MAX+n+1
    new_thread->p_lock = &(__pthread_handles[sseg].h_lock);
    new_thread->p_cancelstate = PTHREAD_CANCEL_ENABLE;
    new_thread->p_canceltype = PTHREAD_CANCEL_DEFERRED;
    new_thread->p_errnop = &new_thread->p_errno;
    new_thread->p_h_errnop = &new_thread->p_h_errno;
    new_thread->p_guardaddr = guardaddr;
    new_thread->p_guardsize = guardsize;
    new_thread->p_self = new_thread;
    new_thread->p_nr = sseg;
/* Initialize the thread handle */
    __pthread_init_lock(&__pthread_handles[sseg].h_lock);
    __pthread_handles[sseg].h_descr = new_thread;
    __pthread_handles[sseg].h_bottom = new_thread_bottom;
/* Determine scheduling parameters for the thread */
    new_thread->p_start_args.schedpolicy = -1;
    if (attr != NULL) {
        new_thread->p_detached = attr->__detachstate;
        new_thread->p_userstack = attr->__stackaddr_set;

        switch(attr->__inheritsched) {
            case PTHREAD_EXPLICIT_SCHED:
                new_thread->p_start_args.schedpolicy = attr->__schedpolicy;
                memcpy (&new_thread->p_start_args.schedparam, &attr->__schedparam,
                        sizeof (struct sched_param));
                break;
            case PTHREAD_INHERIT_SCHED:
                new_thread->p_start_args.schedpolicy = __sched_getscheduler(father_pid);
                __sched_getparam(father_pid, &new_thread->p_start_args.schedparam);
                break;
        }
        new_thread->p_priority =
            new_thread->p_start_args.schedparam.sched_priority;
    }
/* Finish setting up arguments to pthread_start_thread */
// 新线程 函数 参数赋值
    new_thread->p_start_args.start_routine = start_routine;
    new_thread->p_start_args.arg = arg;
    new_thread->p_start_args.mask = *mask;
/* Raise priority of thread manager if needed */
    __pthread_manager_adjust_prio(new_thread->p_priority);




/* Do the cloning. We have to use two different functions depending
on whether we are debugging or not. */
    pid = 0; /* Note that the thread never can have PID zero. */
    if (report_events)
    {
/* See whether the TD_CREATE event bit is set in any of the
masks. */
        int idx = __td_eventword (TD_CREATE);
        uint32_t mask = __td_eventmask (TD_CREATE);

        if ((mask & (__pthread_threads_events.event_bits[idx]
                     | event_maskp->event_bits[idx])) != 0)
        {
/* Lock the mutex the child will use now so that it will stop. */
            __pthread_lock(new_thread->p_lock, NULL);

/* We have to report this event. */
            pid = __clone(pthread_start_thread_event, (void **) new_thread,
                          CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND |
                          __pthread_sig_cancel, new_thread);
            if (pid != -1)
            {
/* Now fill in the information about the new thread in
the newly created thread's data structure. We cannot let
the new thread do this since we don't know whether it was
already scheduled when we send the event. */
                new_thread->p_eventbuf.eventdata = new_thread;
                new_thread->p_eventbuf.eventnum = TD_CREATE;
                __pthread_last_event = new_thread;

/* We have to set the PID here since the callback function
in the debug library will need it and we cannot guarantee
the child got scheduled before the debugger. */
                new_thread->p_pid = pid;

/* Now call the function which signals the event. */
                __linuxthreads_create_event ();

/* Now restart the thread. */
                __pthread_unlock(new_thread->p_lock);
            }
        }
    }
    if (pid == 0)
        pid = __clone(pthread_start_thread, (void **) new_thread,
                      CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND |
                      __pthread_sig_cancel, new_thread);
/* Check if cloning succeeded */
    if (pid == -1) {
/* Free the stack if we allocated it */
        if (attr == NULL || !attr->__stackaddr_set)
        {
            if (new_thread->p_guardsize != 0)
                munmap(new_thread->p_guardaddr, new_thread->p_guardsize);
            munmap((caddr_t)((char *)(new_thread+1) - INITIAL_STACK_SIZE),
                   INITIAL_STACK_SIZE);
        }
        __pthread_handles[sseg].h_descr = NULL;
        __pthread_handles[sseg].h_bottom = NULL;
        __pthread_handles_num--;
        return errno;
    }
/* 将线程添加到__pthread_main_thread 双向链表中 由管理线程管理*/
    new_thread->p_prevlive = __pthread_main_thread;
    new_thread->p_nextlive = __pthread_main_thread->p_nextlive;
    __pthread_main_thread->p_nextlive->p_prevlive = new_thread;
    __pthread_main_thread->p_nextlive = new_thread;
/* Set pid field of the new thread, in case we get there before the
child starts. */
    new_thread->p_pid = pid; // 记录了clone 返回的pid
/* We're all set */
    *thread = new_thread_id;
    return 0;
}

#endif //CCXX_PTHREAD_CREATE_DOC_HPP
