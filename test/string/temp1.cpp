#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include <unistd.h>
#include <getopt.h>
#include <netdb.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define DICE_PORT 6173

int do_debug = 0;
int do_thread = 0;
int do_stdin = 0;
int do_sleep = 0;

struct sockets {
    int local;
    FILE *in, *out;
};

struct sockets *get_sockets(int);
int socket_setup(void);
int debug(char *, ...);
int fail(char *, ...);
int warn(char *, ...);
int roll_die(int);
void *roll_dice(void *);

int
debug(char *fmt, ...) {
    va_list ap;
    int r;

    va_start(ap, fmt);
    if (do_debug) {
        r = vfprintf(stderr, fmt, ap);
    } else {
        r = 0;
    }
    va_end(ap);
    return r;
}

int
warn(char *fmt, ...) {
    int r;
    va_list ap;
    va_start(ap, fmt);
    r = vfprintf(stderr, fmt, ap);
    va_end(ap);
    return r;
}
int
fail(char *fmt, ...) {
    int r;
    va_list ap;
    va_start(ap, fmt);
    r = vfprintf(stderr, fmt, ap);
    exit(1);
    /* notreached */
    va_end(ap);
    return r;
}

int
roll_die(int n) {
    return rand() % n + 1;
}

/* read dice on standard input, write results on standard output */
void *
roll_dice(void *v) {
    struct sockets * s = (struct sockets *)v;
    char inbuf[512];

    /* think globally, program defensively */
    if (!s || !s->out || !s->in)
        return NULL;
    fprintf(s->out, "enter die rolls, or q to quit\n");
    while (fgets(inbuf, sizeof(inbuf), s->in) != 0) {
        int dice;
        int size;
        if (inbuf[0] == 'q') {
            fprintf(s->out, "buh-bye!\n");
            if (s->local == 0) {
                shutdown(fileno(s->out), SHUT_RDWR);
            }
            fclose(s->out);
            fclose(s->in);
            if (s->local == 0) {
                free(s);
            }
            return 0;
        }
        if (sscanf(inbuf, "%dd%d", &dice, &size) != 2) {
            fprintf(s->out, "Sorry, but I couldn't understand that.\n");
        } else {
            int i;
            int total = 0;
            for (i = 0; i < dice; ++i) {
                int x = roll_die(size);
                total += x;
                fprintf(s->out, "%d ", x);
                fflush(s->out);
                if (do_sleep)
                {
                    timespec ts;
                    ts.tv_sec = 0;
                    ts.tv_nsec = 1000 * 10;
                    nanosleep(ts);
                }
            }
            fprintf(s->out, "= %d\n", total);
        }
    }
    return 0;
}

int
temp11(int argc, char *argv[]) {
    int o;
    int sock;
    while ((o = getopt(argc, argv, "dstS")) != -1) {
        switch (o) {
        case 'S':
            do_sleep = 1;
            break;
        case 'd':
            do_debug = 1;
            break;
        case 's':
            do_stdin = 1;
            break;
        case 't':
            do_thread = 1;
            break;
        }
    }

    if (do_stdin) {
        struct sockets s;
        s.local = 1;
        s.in = stdin;
        s.out = stdout;
        if (do_thread) {
            pthread_t p;
            pthread_create(&p, NULL, roll_dice, (void *) &s);
        } else {
            roll_dice(&s);
            exit(0);
        }
    }

    sock = socket_setup();
    while (1) {
        struct sockets *s = get_sockets(sock);
        if (s) {
            if (do_thread) {
                pthread_t p;
                pthread_create(&p, NULL, roll_dice, (void *) s);
            } else {
                roll_dice(s);
                exit(0);
            }
        }
    }
    return 0;
}

int
socket_setup(void) {
    struct protoent *tcp_proto;
    struct sockaddr_in local;
    int r, s, one;

    tcp_proto = getprotobyname("tcp");
    if (!tcp_proto) {
        fail("Can't find TCP/IP protocol: %s\n", strerror(errno));
    }
    s = socket(PF_INET, SOCK_STREAM, tcp_proto->p_proto);
    if (s == -1) {
        fail("socket: %s\n", strerror(errno));
    }
    one = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    memset(&local, 0, sizeof(struct sockaddr_in));
    local.sin_family = AF_INET;
    local.sin_port = htons(DICE_PORT);
    r = bind(s, (struct sockaddr *) &local, sizeof(struct sockaddr_in));
    if (r == -1) {
        fail("bind: %s\n", strerror(errno));
    }
    r = listen(s, 5);
    if (r == -1) {
        fail("listen: %s\n", strerror(errno));
    }
    return s;
}

struct sockets *
        get_sockets(int sock) {
    int conn;
    if ((conn = accept(sock, NULL, NULL)) < 0) {
        warn("accept: %s\n", strerror(errno));
        return 0;
    } else {
        struct sockets *s;
        s = (struct sockets *)malloc(sizeof(struct sockets));
        if (s == NULL) {
            warn("malloc failed.\n");
            return 0;
        }
        s->local = 0;
        s->in = fdopen(conn, "r");
        s->out = fdopen(conn, "w");
        setlinebuf(s->in);
        setlinebuf(s->out);
        return s;
    }
}

