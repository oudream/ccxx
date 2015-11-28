#include "cxsocketextend.h"

#include "cxsocket.cpp"

#undef GM_USER_DEFINE_GETADDRINFO
#ifndef GM_OS_WIN
#define GM_USER_DEFINE_GETADDRINFO
#endif

#if (_WIN32_WINNT < 0x0501)
void freeaddrinfo (struct addrinfo*) {}
int getaddrinfo (const char*,const char*,const struct addrinfo*,struct addrinfo**) { return FALSE; }
int getnameinfo(const struct sockaddr*,socklen_t,char*,DWORD,char*,DWORD,int) { return FALSE; }
#else
/* FIXME: Need WS protocol-independent API helpers.  */
#endif

#ifdef  __FreeBSD__
#ifdef  AI_V4MAPPED
#undef  AI_V4MAPPED
#endif
#endif

typedef unsigned char   bit_t;

static int query_family = 0;

/*
static void bitmask(bit_t *bits, bit_t *mask, unsigned len)
{
    assert(bits != NULL);
    assert(mask != NULL);

    while(len--)
        *(bits++) &= *(mask++);
}

static void bitimask(bit_t *bits, bit_t *mask, unsigned len)
{
    assert(bits != NULL);
    assert(mask != NULL);

    while(len--)
        *(bits++) |= ~(*(mask++));
}

static void bitset(bit_t *bits, unsigned blen)
{
    assert(bits != NULL);

    bit_t mask;

    while(blen) {
        mask = (bit_t)(1 << 7);
        while(mask && blen) {
            *bits |= mask;
            mask >>= 1;
            --blen;
        }
        ++bits;
    }
}

static unsigned bitcount(bit_t *bits, unsigned len)
{
    assert(bits != NULL);

    unsigned count = 0;
    bit_t mask, test;

    while(len--) {
        mask = (bit_t)(1<<7);
        test = *bits++;
        while(mask) {
            if(!(mask & test))
                return count;
            ++count;
            mask >>= 1;
        }
    }
    return count;
}
*/

static int setfamily(int family, const char *host)
{
    const char *hc = host;
    if(!host)
        return  family;

    if(!family || family == AF_UNSPEC) {
#ifdef  AF_INET6
        if(strchr(host, ':'))
            family = AF_INET6;
#endif
#ifdef  AF_UNIX
        if(*host == '/')
            family = AF_UNIX;
#endif
        while((*hc >= '0' && *hc <= '9') || *hc == '.')
            ++hc;
        if(!*hc)
            family = AF_INET;
    }

    if(!family || family == AF_UNSPEC)
        family = query_family;

    return family;
}


#ifdef GM_USER_DEFINE_GETADDRINFO
struct addrinfo {
    int ai_flags;
    int ai_family;
    int ai_socktype;
    int ai_protocol;
    size_t ai_addrlen;
    char * ai_canonname;
    struct sockaddr * ai_addr;
    struct addrinfo * ai_next;
};

#define NI_NUMERICHOST  0x0001
#define NI_NUMERICSERV  0x0002
#define NI_NAMEREQD     0x0004
#define NI_NOFQDN       0x0008
#define NI_DGRAM        0x0010

#define AI_PASSIVE      0x0100
#define AI_CANONNAME    0x0200
#define AI_NUMERICHOST  0x0400
#define AI_NUMERICSERV  0x0800

static mutex servmutex, hostmutex;

static void freeaddrinfo(struct addrinfo *aip)
{
    while(aip != NULL) {
        struct addrinfo *next = aip->ai_next;
        if(aip->ai_canonname != NULL)
            free(aip->ai_canonname);
        if(aip->ai_addr != NULL)
            free(aip->ai_addr);
        free(aip);
        aip = next;
    }
}

static int getnameinfo(const struct sockaddr *addr, socklen_t len, char *host, size_t hostlen, char *service, size_t servlen, int flags)
{
    char *cp;
    struct hostent *hp;
    struct servent *sp;
    assert(addr != NULL);
    assert(host != NULL || hostlen == 0);
    assert(service != NULL || servlen == 0);

    short port = 0;

    switch(addr->sa_family) {
#ifdef  AF_UNIX
    case AF_UNIX:
        if(hostlen > 0)
            snprintf(host, hostlen, "%s", ((struct sockaddr_un *)addr)->sun_path);
        if(servlen > 0)
            snprintf(service, servlen, "%s", ((struct sockaddr_un *)addr)->sun_path);
        return 0;
#endif
#ifdef  AF_INET6
    case AF_INET6:
        port = ((struct sockaddr_in6 *)addr)->sin6_port;
        break;
#endif
    case AF_INET:
        port = ((struct sockaddr_in *)addr)->sin_port;
        break;
    default:
        return -1;
    }
    if(hostlen > 0) {
        if(flags & NI_NUMERICHOST) {
            if(inet_ntop(addr->sa_family, addr, host, hostlen) == NULL)
                return -1;
        }
        else {
            hostmutex.lock();
            hp = gethostbyaddr((caddr_t)addr, len, addr->sa_family);
            if(hp != NULL && hp->h_name != NULL) {
                if(flags & NI_NOFQDN) {
                    cp = strchr(hp->h_name, '.');
                    if(cp)
                        *cp = 0;
                }
                snprintf(host, hostlen, "%s", hp->h_name);
                hostmutex.unlock();
            }
            else {
                hostmutex.unlock();
                if(flags & NI_NAMEREQD)
                    return -1;
                if(inet_ntop(addr->sa_family, addr, host, hostlen) != NULL)
                    return -1;
            }
        }
    }
    if(servlen > 0) {
        if(flags & NI_NUMERICSERV)
            snprintf(service, servlen, "%d", ntohs(port));
        else {
            servmutex.lock();
            sp = getservbyport(port, (flags & NI_DGRAM) ? "udp" : NULL);
            if(sp && sp->s_name)
                snprintf(service, servlen, "%s", sp->s_name);
            else
                snprintf(service, servlen, "%d", ntohs(port));
            servmutex.unlock();
        }
    }
    return 0;
}

static int getaddrinfo(const char *hostname, const char *servname, const struct addrinfo *hintsp, struct addrinfo **res)
{
    int family;
    const char *servtype = "tcp";
    struct hostent *hp;
    struct servent *sp;
    char **np;
    struct addrinfo hints;
    struct addrinfo *aip = NULL, *prior = NULL;
    socklen_t len;
    short port = 0;
    struct sockaddr_in *ipv4;
#ifdef  AF_INET6
    struct sockaddr_in6 *ipv6;
#endif
    if(hintsp == NULL) {
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
    }
    else
        memcpy(&hints, hintsp, sizeof(hints));

    *res = NULL;

#ifdef  AF_UNIX
    if(hints.ai_family == AF_UNIX || (hints.ai_family == AF_UNSPEC && hostname && *hostname == '/')) {
        if(hints.ai_socktype == 0)
            hints.ai_socktype = SOCK_STREAM;

        aip = (struct addrinfo *)malloc(sizeof(struct addrinfo));
        memset(aip, 0, sizeof(struct addrinfo));
        aip->ai_next = NULL;
        aip->ai_canonname = NULL;
        aip->ai_protocol = hints.ai_protocol;
        struct sockaddr_un *unp;
        if(strlen(hostname) >= sizeof(unp->sun_path))
            return -1;
        unp = (struct sockaddr_un *)malloc(sizeof(struct sockaddr_un));
        memset(unp, 0, sizeof(struct sockaddr_un));
        unp->sun_family = AF_UNIX;
        CxStringC::set(unp->sun_path, sizeof(unp->sun_path), hostname);
#ifdef  __SUN_LEN
        len = sizeof(unp->sun_len) + strlen(unp->sun_path) +
            sizeof(unp->sun_family) + 1;
        unp->sun_len = len;
#else
        len = strlen(unp->sun_path) + sizeof(unp->sun_family) + 1;
#endif
        if(hints.ai_flags & AI_PASSIVE)
            unlink(unp->sun_path);
        aip->ai_addr = (struct sockaddr *)unp;
        aip->ai_addrlen = len;
        *res = aip;
        return 0;
    }
#endif

    if(servname && *servname) {
        if(servname[0] >= '0' && servname[0] <= '9') {
            port = htons(atoi(servname));
        }
        else {
            if(hints.ai_socktype == SOCK_DGRAM)
                servtype = "udp";
            servmutex.lock();
            sp = getservbyname(servname, servtype);
            if(!sp) {
                servmutex.unlock();
                return -1;
            }
            port = sp->s_port;
            servmutex.unlock();
        }
    }

    if((!hostname || !*hostname)) {
        aip = (struct addrinfo *)malloc(sizeof(struct addrinfo));
        memset(aip, 0, sizeof(struct addrinfo));
        aip->ai_canonname = NULL;
        aip->ai_socktype = hints.ai_socktype;
        aip->ai_protocol = hints.ai_protocol;
        aip->ai_next = NULL;

#ifdef  AF_INET6
        if(hints.ai_family == AF_INET6) {
            aip->ai_family = AF_INET6;
            ipv6 = (struct sockaddr_in6 *)malloc(sizeof(struct sockaddr_in6));
            memset(ipv6, 0, sizeof(struct sockaddr_in6));
            if(!(hints.ai_flags & AI_PASSIVE))
                inet_pton(AF_INET6, "::1", &ipv6->sin6_addr);
            ipv6->sin6_family = AF_INET6;
            ipv6->sin6_port = port;
            aip->ai_addr = (struct sockaddr *)ipv6;
            *res = aip;
            return 0;
        }
#endif
        aip->ai_family = AF_INET;
        ipv4 = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));
        memset(ipv4, 0, sizeof(struct sockaddr_in));
        ipv4->sin_family = AF_INET;
        ipv4->sin_port = port;
        if(!(hints.ai_flags & AI_PASSIVE))
            inet_pton(AF_INET, "127.0.0.1", &ipv4->sin_addr);
        aip->ai_addr = (struct sockaddr *)ipv4;
        *res = aip;
        return 0;
    }
    family = hints.ai_family;
#ifdef  AF_UNSPEC
    if(family == AF_UNSPEC)
        family = AF_INET;
#endif
    hostmutex.lock();
#ifdef  HAVE_GETHOSTBYNAME2
    hp = gethostbyname2(hostname, family);
#else
    hp = gethostbyname(hostname);
#endif
    if(!hp) {
        hostmutex.unlock();
        return -1;
    }

    for(np = hp->h_addr_list; *np != NULL; np++) {
        aip = (struct addrinfo *)malloc(sizeof(struct addrinfo));
        memset(aip, 0, sizeof(struct addrinfo));
        if(hints.ai_flags & AI_CANONNAME)
            aip->ai_canonname = cx_strdup(hp->h_name);
        else
            aip->ai_canonname = NULL;
        aip->ai_socktype = hints.ai_socktype;
        aip->ai_protocol = hints.ai_protocol;
        aip->ai_next = NULL;
        if(prior)
            prior->ai_next = aip;
        else
            *res = aip;
        prior = aip;

#ifdef  AF_INET6
        if(hints.ai_family == AF_INET6) {
            aip->ai_family = AF_INET6;
            ipv6 = (struct sockaddr_in6 *)malloc(sizeof(struct sockaddr_in6));
            memset(ipv6, 0, sizeof(struct sockaddr_in6));
            memcpy(&ipv6->sin6_addr, *np, sizeof(&ipv6->sin6_addr));
            ipv6->sin6_family = AF_INET6;
            ipv6->sin6_port = port;
            aip->ai_addr = (struct sockaddr *)ipv6;
            continue;
        }
#endif
        aip->ai_family = AF_INET;
        ipv4 = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));
        memset(ipv4, 0, sizeof(struct sockaddr_in));
        ipv4->sin_family = AF_INET;
        ipv4->sin_port = port;
        memcpy(&ipv4->sin_addr, *np, sizeof(&ipv4->sin_addr));
        aip->ai_addr = (struct sockaddr *)ipv4;
    }

    hostmutex.unlock();
    if(*res)
        return 0;
    else
        return -1;
}
#endif

#if defined(AF_UNIX) && !defined(GM_OS_WIN)

static socklen_t unixaddr(struct sockaddr_un *addr, const char *path)
{
    assert(addr != NULL);
    assert(path != NULL && *path != 0);

    socklen_t len;
    unsigned slen = strlen(path);

    if(slen > sizeof(struct sockaddr_storage) - 8)
        slen = sizeof(struct sockaddr_storage) - 8;

    memset(addr, 0, sizeof(struct sockaddr_storage));
    addr->sun_family = AF_UNIX;
    memcpy(addr->sun_path, path, slen);

#ifdef  __SUN_LEN
    len = sizeof(addr->sun_len) + strlen(addr->sun_path) +
        sizeof(addr->sun_family) + 1;
    addr->sun_len = len;
#else
    len = strlen(addr->sun_path) + sizeof(addr->sun_family) + 1;
#endif
    return len;
}

#endif



















CxIpAddressExtend::CxIpAddressExtend(int family, const char *a, int type, int protocol)
{
    assert(a != NULL && *a != 0);

    list = NULL;
#ifdef  GM_OS_WIN
    CxSocket::init();
#endif
    set(family, a, type, protocol);
}

CxIpAddressExtend::CxIpAddressExtend(int family, const char *host, const char *svc)
{
    assert(host != NULL && *host != 0);
    assert(svc != NULL && *svc != 0);

    struct addrinfo hint;

#ifdef  GM_OS_WIN
    CxSocket::init();
#endif

    list = NULL;
    memset(&hint, 0, sizeof(hint));
    hint.ai_family = family;
    getaddrinfo(host, svc, &hint, &list);
}

CxIpAddressExtend::CxIpAddressExtend(const char *host, const char *service, int type)
{
#ifdef  GM_OS_WIN
    CxSocket::init();
#endif

    list = NULL;
    set(host, service, type);
}

CxIpAddressExtend::CxIpAddressExtend(const char *host, unsigned port)
{
    assert(host != NULL && *host != 0);

#ifdef  GM_OS_WIN
    CxSocket::init();
#endif

    list = NULL;
    set(host, port);
}

CxIpAddressExtend::CxIpAddressExtend()
{
#ifdef  GM_OS_WIN
    CxSocket::init();
#endif

    list = NULL;
}

CxIpAddressExtend::CxIpAddressExtend(const CxIpAddressExtend& from)
{
    list = NULL;
    copy(from.list);
}

CxIpAddressExtend::~CxIpAddressExtend()
{
    clear();
}

void CxIpAddressExtend::clear(void)
{
    if(list) {
        freeaddrinfo(list);
        list = NULL;
    }
}

void CxIpAddressExtend::set(const char *host, unsigned port)
{
    assert(host != NULL && *host != 0);

    char buf[16];

    clear();

    if(port) {
        snprintf(buf, sizeof(buf), "%u", port);
        list = CxSocketExtend::query(host, buf);
    }
    else
        list = CxSocketExtend::query(host, NULL);
}

void CxIpAddressExtend::set(int family, const char *a, int type, int protocol)
{
    assert(a != NULL && *a != 0);

    char *addr = cx_strdup(a);
    char *host = strchr(addr, '@');
    char *ep;
    char *svc = NULL;
    struct addrinfo hint;

    clear();

    memset(&hint, 0, sizeof(hint));
#ifdef  PF_UNSPEC
    hint.ai_family = PF_UNSPEC;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_flags = AI_PASSIVE;
#endif

    if(!host)
        host = addr;
    else
        ++host;

    if(*host != '[') {
        ep = strchr(host, ':');
        if(ep) {
            *(ep++) = 0;
            svc = ep;
        }
        goto proc;
    }
#ifdef  AF_INET6
    if(*host == '[') {
        family = AF_INET6;
        ++host;
        ep = strchr(host, ']');
        if(ep) {
            *(ep++) = 0;
            if(*ep == ':')
                svc = ++ep;
        }
    }
#endif
proc:
    hint.ai_family = family;
    hint.ai_socktype = type;
    hint.ai_protocol = protocol;

#if defined(AF_INET6) && defined(AI_V4MAPPED)
    if(hint.ai_family == AF_INET6 && !v6only)
        hint.ai_flags |= AI_V4MAPPED;
#endif

    getaddrinfo(host, svc, &hint, &list);
}

struct sockaddr *CxIpAddressExtend::get(void) const
{
    if(!list)
        return NULL;

    return list->ai_addr;
}

int CxIpAddressExtend::family(void) const
{
    struct sockaddr *ap;
    if(!list)
        return 0;

    ap = list->ai_addr;
    if(!ap)
        return 0;

    return ap->sa_family;
}

struct sockaddr *CxIpAddressExtend::get(int family) const
{
    struct sockaddr *ap;
    struct addrinfo *lp;

    lp = list;

    while(lp) {
        ap = lp->ai_addr;
        if(ap && ap->sa_family == family)
            return ap;
        lp = lp->ai_next;
    }
    return NULL;
}

void CxIpAddressExtend::set(struct sockaddr *addr)
{
    clear();
    add(addr);
}

bool CxIpAddressExtend::remove(struct sockaddr *addr)
{
    assert(addr != NULL);
    struct addrinfo *node = list, *prior = NULL;

    while(node) {
        if(node->ai_addr && CxSocket::equal(addr, node->ai_addr))
            break;
        prior = node;
        node = node->ai_next;
    }

    if(!node)
        return false;

    if(!prior)
        list = node->ai_next;
    else
        prior->ai_next = node->ai_next;

    node->ai_next = NULL;
    freeaddrinfo(node);
    return true;
}

unsigned CxIpAddressExtend::insert(struct addrinfo *alist)
{
    unsigned count = 0;
    while(alist) {
        if(insert(alist->ai_addr))
            ++count;
        alist = alist->ai_next;
    }
    return count;
}

unsigned CxIpAddressExtend::remove(struct addrinfo *alist)
{
    unsigned count = 0;
    while(alist) {
        if(remove(alist->ai_addr))
            ++count;
        alist = alist->ai_next;
    }
    return count;
}

bool CxIpAddressExtend::insert(struct sockaddr *addr)
{
    assert(addr != NULL);

    struct addrinfo *node = list;

    while(node) {
        if(node->ai_addr && CxSocket::equal(addr, node->ai_addr))
            return false;
        node = node->ai_next;
    }

    node = (struct addrinfo *)malloc(sizeof(struct addrinfo));
    memset(node, 0, sizeof(struct addrinfo));
    node->ai_family = addr->sa_family;
    node->ai_addrlen = CxSocket::len(addr);
    node->ai_next = list;
    node->ai_addr = (struct sockaddr *)malloc(node->ai_addrlen);
    memcpy(node->ai_addr, addr, node->ai_addrlen);
    list = node;
    return true;
}

void CxIpAddressExtend::copy(const struct addrinfo *addr)
{
    struct addrinfo *last = NULL;
    struct addrinfo *node;

    clear();
    while(addr) {
        node = (struct addrinfo *)malloc(sizeof(struct addrinfo));
        memcpy(node, addr, sizeof(struct addrinfo));
        node->ai_next = NULL;
        node->ai_addr = dup(addr->ai_addr);
        if(last)
            last->ai_next = node;
        else
            list = node;
        last = node;
    }
}

struct sockaddr_in *CxIpAddressExtend::ipv4(struct sockaddr *addr)
{
    if(addr == NULL || addr->sa_family != AF_INET)
        return NULL;

    return (struct sockaddr_in*)addr;
}

#ifdef  AF_INET6
struct sockaddr_in6 *CxIpAddressExtend::ipv6(struct sockaddr *addr)
{
    if(addr == NULL || addr->sa_family != AF_INET6)
        return NULL;

    return (struct sockaddr_in6*)addr;
}
#endif

struct sockaddr *CxIpAddressExtend::dup(struct sockaddr *addr)
{
    struct sockaddr *node;

    if(!addr)
        return NULL;

    size_t slen = CxSocket::len(addr);
    if(!slen)
        return NULL;

    node = (struct sockaddr *)malloc(slen);
    memcpy(node, addr, slen);
    return node;
}

void CxIpAddressExtend::add(struct sockaddr *addr)
{
    assert(addr != NULL);

    char buffer[80];
    char svc[8];

    CxSocket::query(addr, buffer, sizeof(buffer));
    snprintf(svc, sizeof(svc), "%d", CxSocket::service(addr));
    add(buffer, svc, addr->sa_family);
}

void CxIpAddressExtend::set(const char *host, const char *svc, int socktype)
{
    clear();
    add(host, svc, socktype);
}

void CxIpAddressExtend::add(const char *host, const char *svc, int socktype)
{
    assert(host != NULL && *host != 0);
    assert(svc != NULL && *svc != 0);

    struct addrinfo *join = NULL, *last = NULL;

    join = CxSocketExtend::query(host, svc, socktype);
    if(!join)
        return;

    if(!list) {
        list = join;
        return;
    }
    last = list;
    while(last->ai_next)
        last = last->ai_next;
    last->ai_next = join;
}

struct sockaddr *CxIpAddressExtend::find(const struct sockaddr *addr) const
{
    assert(addr != NULL);

    struct addrinfo *node = list;

    while(node) {
        if(CxSocket::equal(addr, node->ai_addr))
            return node->ai_addr;
        node = node->ai_next;
    }
    return NULL;
}













CxSocketExtend::CxSocketExtend() :
    CxSocket()
{

}

CxSocketExtend::CxSocketExtend(const char *iface, const char *port, int family, int type, int protocol)
{
    assert(iface != NULL && *iface != 0);
    assert(port != NULL && *port != 0);

#ifdef  GM_OS_WIN
    init();
#endif
    family = setfamily(family, iface);
    so = create(iface, port, family, type, protocol);
    iowait = CxGlobal::inf;
    ioerr = 0;
}

socket_t CxSocketExtend::create(const CxIpAddressExtend &address)
{
    socket_t so;
    struct addrinfo *res = *address;
    if(!res)
        return INVALID_SOCKET;

    so = CxSocket::create(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(so == INVALID_SOCKET)
        return INVALID_SOCKET;

    if(connectto(so, res)) {
        CxSocket::release(so);
        return INVALID_SOCKET;
    }
    return so;
}



char *CxSocketExtend::hostname(const struct sockaddr *sa, char *buf, size_t max)
{
    assert(sa != NULL);
    assert(buf != NULL);
    assert(max > 0);

    socklen_t sl;

#ifdef  AF_UNIX
    const struct sockaddr_un *un = (const struct sockaddr_un *)sa;
#endif

    switch(sa->sa_family) {
#ifdef  AF_UNIX
    case AF_UNIX:
        if(max > sizeof(un->sun_path))
            max = sizeof(un->sun_path);
        else
            --max;
        strncpy(buf, un->sun_path, max);
        buf[max] = 0;
        return buf;
#endif
    case AF_INET:
        sl = sizeof(struct sockaddr_in);
        break;
#ifdef  AF_INET6
    case AF_INET6:
        sl = sizeof(struct sockaddr_in6);
        break;
#endif
    default:
        return NULL;
    }

    if(getnameinfo(sa, sl, buf, max, NULL, 0, NI_NOFQDN))
        return NULL;

    return buf;
}

socklen_t CxSocketExtend::query(socket_t so, struct sockaddr_storage *sa, const char *host, const char *svc)
{
    assert(sa != NULL);
    assert(host != NULL && *host != 0);
    assert(svc != NULL && *svc != 0);

    socklen_t len = 0;
    struct addrinfo hint, *res = NULL;

#ifdef  AF_UNIX
    if(strchr(host, '/'))
        return unixaddr((struct sockaddr_un *)sa, host);
#endif

    if(!hinting(so, &hint) || !svc)
        return 0;

    if(getaddrinfo(host, svc, &hint, &res) || !res)
        goto exit;

    memcpy(sa, res->ai_addr, res->ai_addrlen);
    len = res->ai_addrlen;

exit:
    if(res)
        freeaddrinfo(res);
    return len;
}

void CxSocketExtend::query(int querymode)
{
    query_family = querymode;
}

int CxSocketExtend::bindto(socket_t so, const char *host, const char *svc, int protocol)
{
    assert(so != INVALID_SOCKET);
    assert(host != NULL && *host != 0);
    assert(svc != NULL && *svc != 0);

    int rtn = -1;
    int reuse = 1;

    struct addrinfo hint, *res = NULL;

    setsockopt(so, SOL_SOCKET, SO_REUSEADDR, (caddr_t)&reuse, sizeof(reuse));

#ifdef AF_UNIX
    if(strchr(host, '/')) {
        struct sockaddr_un uaddr;
        socklen_t len = unixaddr(&uaddr, host);
        rtn = _bind_(so, (struct sockaddr *)&uaddr, len);
        goto exit;
    };
#endif

    if(!hinting(so, &hint) || !svc)
        return ENOSYS;

    hint.ai_protocol = protocol;
    if(host && !strcmp(host, "*"))
        host = NULL;

#if defined(SO_BINDTODEVICE) && !defined(__QNX__)
    if(host && !strchr(host, '.') && !strchr(host, ':')) {
        struct ifreq ifr;
        memset(&ifr, 0, sizeof(ifr));
        strncpy(ifr.ifr_ifrn.ifrn_name, host, IFNAMSIZ);
        setsockopt(so, SOL_SOCKET, SO_BINDTODEVICE, &ifr, sizeof(ifr));
        host = NULL;
    }
#endif

    hint.ai_flags = AI_PASSIVE | AI_NUMERICHOST;

#if defined(AF_INET6) && defined(AI_V4MAPPED)
    if(hint.ai_family == AF_INET6 && !v6only)
        hint.ai_flags |= AI_V4MAPPED;
#endif

    rtn = getaddrinfo(host, svc, &hint, &res);
    if(rtn)
        goto exit;

    rtn = _bind_(so, res->ai_addr, res->ai_addrlen);
exit:
    if(res)
        freeaddrinfo(res);
    if(rtn)
        rtn = CxSocket::error();
    return rtn;
}

socket_t CxSocketExtend::create(const char *iface, const char *port, int family, int type, int protocol)
{
    assert(iface != NULL && *iface != 0);
    assert(port != NULL && *port != 0);

    struct addrinfo hint, *res;
    socket_t so;
    int reuse = 1;

#ifdef  GM_OS_WIN
    CxSocket::init();
#endif

    memset(&hint, 0, sizeof(hint));
    hint.ai_flags = AI_PASSIVE | AI_NUMERICHOST;
    hint.ai_family = setfamily(family, iface);
    hint.ai_socktype = type;
    hint.ai_protocol = protocol;

#if defined(AF_INET6) && defined(AI_V4MAPPED)
    if(hint.ai_family == AF_INET6 && !v6only)
        hint.ai_flags |= AI_V4MAPPED;
#endif

#if defined(AF_UNIX) && !defined(GM_OS_WIN)
    if(strchr(iface, '/')) {
        struct sockaddr_un uaddr;
        socklen_t len = unixaddr(&uaddr, iface);
        if(!type)
            type = SOCK_STREAM;
        so = create(AF_UNIX, type, 0);
        if(so == INVALID_SOCKET)
            return INVALID_SOCKET;
        if(_bind_(so, (struct sockaddr *)&uaddr, len)) {
            release(so);
            return INVALID_SOCKET;
        }
        return so;
    };
#endif

    if(iface && !strcmp(iface, "*"))
        iface = NULL;

    getaddrinfo(iface, port, &hint, &res);
    if(res == NULL)
        return INVALID_SOCKET;

    so = CxSocket::create(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(so == INVALID_SOCKET) {
        freeaddrinfo(res);
        return INVALID_SOCKET;
    }
    setsockopt(so, SOL_SOCKET, SO_REUSEADDR, (caddr_t)&reuse, sizeof(reuse));
    if(res->ai_addr) {
        if(_bind_(so, res->ai_addr, res->ai_addrlen)) {
            CxSocket::release(so);
            so = INVALID_SOCKET;
        }
    }
    freeaddrinfo(res);
    return so;
}

struct ::addrinfo *CxSocketExtend::query(const char *hp, const char *svc, int type, int protocol)
{
    assert(hp != NULL && *hp != 0);

    int family = DEFAULT_FAMILY;
    char hostbuf[256];
    struct addrinfo hint;
    CxStringC::set(hostbuf, sizeof(hostbuf), hp);
    char *cp = strchr(hostbuf, '/');
    char *host = hostbuf;

    memset(&hint, 0, sizeof(hint));

    hint.ai_socktype = type;
    hint.ai_protocol = protocol;

#ifdef  PF_UNSPEC
    hint.ai_family = PF_UNSPEC;
//    hint.ai_flags = AI_PASSIVE;
#endif

    if(cp)
        *cp = 0;

    if(*host == '[') {
        cp = strchr(++host, ']');
        if(cp) {
            *(cp++) = 0;
            if(*cp == ':')
                svc = ++cp;
        }
#ifdef  AF_INET6
        family = AF_INET6;
#else
        return NULL;
#endif
    }
    else if(((cp = strrchr(host, ':')) != NULL) && (strchr(host, ':') == cp)) {
        *(cp++) = 0;
        svc = cp;
    }

    if(is_numeric(host)) {
        hint.ai_flags |= AI_NUMERICHOST;

        if(strchr(host, ':')) {
#ifdef  AF_INET6
            family = AF_INET6;
#else
            return NULL;
#endif
        }
        else
            family = AF_INET;
    }

    if(family && family != AF_UNSPEC)
        hint.ai_family = family;

#if defined(AF_INET6) && defined(AI_V4MAPPED)
    if(hint.ai_family == AF_INET6 && !v6only)
        hint.ai_flags |= AI_V4MAPPED;
#endif
#ifdef  AI_NUMERICSERV
    if(svc && atoi(svc) > 0)
        hint.ai_flags |= AI_NUMERICSERV;
#endif

    struct addrinfo *result = NULL;
    getaddrinfo(host, svc, &hint, &result);
    return result;
}

void CxSocketExtend::release(struct addrinfo *list)
{
    if(list)
        freeaddrinfo(list);
}












CxListenSocket::CxListenSocket(const char *iface, const char *svc, unsigned backlog, int family, int type, int protocol) :
    CxSocketExtend()
{
    if(!iface)
        iface = "*";

    assert(iface != NULL && *iface != 0);
    assert(svc != NULL && *svc != 0);
    assert(backlog > 0);

    so = create(iface, svc, backlog, family, type, protocol);
}

socket_t CxListenSocket::create(const char *iface, const char *svc, unsigned backlog, int family, int type, int protocol)
{
    if(!type)
        type = SOCK_STREAM;

    socket_t so = CxSocketExtend::create(iface, svc, family, type, protocol);

    if(so == INVALID_SOCKET)
        return so;

    if(_listen_(so, backlog)) {
        CxSocket::release(so);
        return INVALID_SOCKET;
    }
    return so;
}

socket_t CxListenSocket::accept(struct sockaddr_storage *addr) const
{
    socklen_t len = sizeof(struct sockaddr_storage);
    if(addr)
        return _accept_(so, (struct sockaddr *)addr, &len);
    else
        return _accept_(so, NULL, NULL);
}








struct sockaddr *_getaddrinfo(struct addrinfo *list)
{
    return list->ai_addr;
}

struct addrinfo *_nextaddrinfo(struct addrinfo *list)
{
    if(!list)
        return NULL;

    return list->ai_next;
}

socket_t _getaddrsock(struct addrinfo *list)
{
    if(!list)
        return INVALID_SOCKET;

    return ::socket(list->ai_family, list->ai_socktype, list->ai_protocol);
}
