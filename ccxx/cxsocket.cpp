#include "cxsocket.h"

#ifndef GM_OS_WIN
#include <net/if.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <sys/utsname.h>
#endif

#include <fcntl.h>
#include <errno.h>

#if defined(HAVE_SOCKS)
#include <socks.h>
#endif


#ifndef MSG_DONTWAIT
#define MSG_DONTWAIT 0
#endif

#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL 0
#endif

#if defined(__linux__) && !defined(IP_MTU)
#define IP_MTU 14
#endif

#ifdef  __FreeBSD__
#ifdef  AI_V4MAPPED
#undef  AI_V4MAPPED
#endif
#endif

typedef struct multicast_internet
{
    union {
        struct ip_mreq  ipv4;
#ifdef  AF_INET6
        struct ipv6_mreq ipv6;
#endif
    };
} inetmulticast_t;

#if defined(HAVE_SOCKS)
#define _getsockname_(so, addr, alen) ::Rgetsockname(so, addr, alen)
#define _getpeername_(so, addr, alen) ::Rgetpeername(so, addr, alen)
#define _accept_(so, addr, alen) ::Raccept(so, addr, alen)
#define _connect_(so, addr, alen) ::Rconnect(so, addr, alen)
#define _bind_(so, addr, alen) ::Rbind(so, addr, alen)
#define _listen_(so, backlog) ::Rlisten(so, backlog)
#define _select_(cnt, rfd, wfd, efd, timeout) ::Rselect(cnt, rfd, wfd, efd, timeout)
#define _send_(so, buf, bytes, flag) ::Rsend(so, buf, bytes, flag)
#define _recv_(so, buf, bytes, flag) ::Rrecv(so, buf, bytes, flag)
#define _sendto_(so, buf, bytes, flag, to, tolen) ::Rsendto(so, buf, bytes, flag, to, tolen)
#define _recvfrom_(so, buf, bytes, flag, from, fromlen) ::Rrecvfrom(so, buf, bytes, flag, from, fromlen)
#undef  accept
#undef  sendto
#undef  recvfrom
#undef  select
#elif defined(__PTH__)
#define _send_(so, buf, bytes, flag) pth_send(so, buf, bytes, flag)
#define _recv_(so, buf, bytes, flag) pth_recv(so, buf, bytes, flag)
#define _sendto_(so, buf, bytes, flag, to, tolen) pth_sendto(so, buf, bytes, flag, to, tolen)
#define _recvfrom_(so, buf, bytes, flag, from, fromlen) pth_recvfrom(so, buf, bytes, flag, from, fromlen)
#define _connect_(so, addr, addrlen) pth_connect(so, addr, addrlen)
#define _accept_(so, addr, addrlen) pth_accept(so, addr, addrlen)
#define _select_(cnt, rfd, wfd, efd, timeout) pth_select(cnt, rfd, wfd, efd, timeout)
#define _getsockname_(so, addr, alen) ::getsockname(so, addr, alen)
#define _getpeername_(so, addr, alen) ::getpeername(so, addr, alen)
#define _bind_(so, addr, alen) ::bind(so, addr, alen)
#define _listen_(so, count) ::listen(so, count)
#else
#define _send_(so, buf, bytes, flag) ::send(so, buf, bytes, flag)
#define _recv_(so, buf, bytes, flag) ::recv(so, buf, bytes, flag)
#define _sendto_(so, buf, bytes, flag, to, tolen) ::sendto(so, buf, bytes, flag, to, tolen)
#define _recvfrom_(so, buf, bytes, flag, from, fromlen) ::recvfrom(so, buf, bytes, flag, from, fromlen)
#define _connect_(so, addr, addrlen) ::connect(so, addr, addrlen)
#define _accept_(so, addr, addrlen) ::accept(so, addr, addrlen)
#define _select_(cnt, rfd, wfd, efd, timeout) ::select(cnt, rfd, wfd, efd, timeout)
#define _getsockname_(so, addr, alen) ::getsockname(so, addr, alen)
#define _getpeername_(so, addr, alen) ::getpeername(so, addr, alen)
#define _bind_(so, addr, alen) ::bind(so, addr, alen)
#define _listen_(so, count) ::listen(so, count)
#endif

static int v6only = 0;

static void socket_mapping(int family, socket_t so)
{
    if(so == INVALID_SOCKET)
        return;

#if defined(IPV6_V6ONLY) && defined(IPPROTO_IPV6)
    if(family == AF_INET6)
        setsockopt (so, IPPROTO_IPV6, IPV6_V6ONLY, (char *) &v6only, sizeof (v6only));
#endif
}

#ifndef AF_UNSPEC
#define AF_UNSPEC   0
#endif

#ifdef  GM_OS_WIN

static volatile int f_iWinsockStartup = 0;

static void _socketcleanup(void)
{
    while (f_iWinsockStartup>0)
    {
        --f_iWinsockStartup;
        WSACleanup();
    }
}

/**
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

void CxSocket::init(void)
{
    if(f_iWinsockStartup > 0)
        return;

    unsigned short version = MAKEWORD(2,2);
    WSADATA status;
    status.wVersion = 0;
    WSAStartup(version, &status);

    GM_ASSERT_IFNOT( status.wVersion == version, "socket init failure" );
    atexit(_socketcleanup);
    ++f_iWinsockStartup;
}
#else
void CxSocket::init(void)
{
}
#endif

void CxSocket::init(const char *progname)
{
    assert(progname != NULL);

    CxSocket::init();
#ifdef  HAVE_SOCKS
    const char *cp;

#ifdef  GM_OS_WIN
    char path[65];

    cp = strrchr(progname, '/');
    if(!cp)
        cp = strrchr(progname, '\\');
    if(!cp)
        cp = strrchr(progname, ':');
    if(cp)
        progname = ++cp;

    CxStringC::set(path, sizeof(path), progname);
    const char *ext = strrchr(path, '.');
    if(ext && (case_eq(ext, ".exe") || case_eq(ext, ".com")))
        *ext = 0;
    progname = path;
#else
    cp = strrchr(progname, '/');
    if(cp)
        progname = ++cp;
#endif

    SOCKSinit((char *)progname);
#endif
}

void CxSocket::v4mapping(bool enable)
{
    if(enable)
        v6only = 0;
    else
        v6only = 1;
}

#ifdef  GM_OS_WIN
int CxSocket::error(void)
{
    switch(WSAGetLastError())
    {
    case WSANOTINITIALISED:
    case WSAENETDOWN:
    case WSASYSNOTREADY:
        return ENETDOWN;
    case WSAEFAULT:
        return EFAULT;
    case WSAEINTR:
    case WSAECANCELLED:
    case WSA_OPERATION_ABORTED:
    case WSA_IO_INCOMPLETE:
    case WSASYSCALLFAILURE:
    case WSA_E_CANCELLED:
        return EINTR;
    case WSA_IO_PENDING:
    case WSAEINPROGRESS:
        return EINPROGRESS;
    case WSAEINVAL:
        return EINVAL;
    case WSAEMFILE:
        return EMFILE;
    case WSAENETUNREACH:
        return ENETUNREACH;
    case WSAENOBUFS:
    case WSAETOOMANYREFS:
    case WSA_NOT_ENOUGH_MEMORY:
        return ENOMEM;
    case WSAEACCES:
        return EACCES;
    case WSAEBADF:
    case WSAENOTSOCK:
    case WSA_INVALID_HANDLE:
        return EBADF;
    case WSAEOPNOTSUPP:
        return ENOSYS;
    case WSAEWOULDBLOCK:
    case WSAEALREADY:
        return EAGAIN;
    case WSAENOPROTOOPT:
        return ENOPROTOOPT;
    case WSAEADDRINUSE:
        return EADDRINUSE;
    case WSAENETRESET:
        return ENETRESET;
    case WSAECONNABORTED:
        return ECONNABORTED;
    case WSAECONNRESET:
        return ECONNRESET;
    case WSAEISCONN:
        return EISCONN;
    case WSAENOTCONN:
        return ENOTCONN;
    case WSAESHUTDOWN:
        return ESHUTDOWN;
    case WSAETIMEDOUT:
        return ETIMEDOUT;
    case WSAECONNREFUSED:
        return ECONNREFUSED;
    case WSAEHOSTDOWN:
        return EHOSTDOWN;
    case WSAEHOSTUNREACH:
        return EHOSTUNREACH;
    }
    return EINVAL;
}
#else
int CxSocket::error(void)
{
    return errno;
}
#endif


bool CxSocket::is_null(const char *str)
{
    assert(str != NULL);

    while(*str && strchr("0:.*", *str) != NULL)
        ++str;

    // allow field separation...
    if(*str <= ' ')
        return true;

    if(*str)
        return false;

    return true;
}

bool CxSocket::is_numeric(const char *str)
{
    assert(str != NULL);

    // if raw ipv6, then we can just exit, no chance to confuse with names
    if(strchr(str, ':'))
        return true;

    while(*str && strchr("0123456789.", *str) != NULL)
        ++str;

    // allow field separators
    if(*str <= ' ')
        return true;

    if(*str)
        return false;

    return true;
}

int CxSocket::local(socket_t sock, struct sockaddr_storage *addr)
{
    socklen_t slen = sizeof(sockaddr_storage);
    return _getsockname_(sock, (struct sockaddr *)addr, &slen);
}

int CxSocket::remote(socket_t sock, struct sockaddr_storage *addr)
{
    socklen_t slen = sizeof(sockaddr_storage);
    return _getpeername_(sock, (struct sockaddr *)addr, &slen);
}

int CxSocket::type(socket_t so)
{
    int sotype;
    socklen_t slen = sizeof(sotype);
    if(getsockopt(so, SOL_SOCKET, SO_TYPE, (caddr_t)&sotype, &slen))
        return 0;
    return sotype;
}

unsigned CxSocket::segsize(socket_t so, unsigned size)
{
#ifdef  IP_MTU
    socklen_t alen = sizeof(size);
#endif

    switch(type(so)) {
    case SOCK_STREAM:
#ifdef  TCP_MAXSEG
        if(size)
            setsockopt(so, IPPROTO_TCP, TCP_MAXSEG, (char *)&size, sizeof(size));
#endif
        break;
    case SOCK_DCCP:
#ifdef  DCCP_MAXSEG
        if(size)
            setsockopt(so, IPPROTO_DCCP, DCCP_MAXSEG, (char *)&size, sizeof(size));
#endif
        break;
    }
#ifdef  IP_MTU
    getsockopt(so, IPPROTO_IP, IP_MTU, (char*)(&size), &alen);
#else
    size = 0;
#endif
    return size;
}

bool CxSocket::ccid(socket_t so, uint8 ccid)
{
    uint8 ccids[4];
    socklen_t len = sizeof(ccids);
    bool supported = false;

    // maybe also not dccp socket...
    if(getsockopt(so, SOL_DCCP, DCCP_SOCKOPT_AVAILABLE_CCIDS, (char *)&ccids, &len) < 0)
        return false;

    for(unsigned pos = 0; pos < sizeof(ccids); ++pos) {
        if(ccid == ccids[pos]) {
            supported = true;
            break;
        }
    }

    if(!supported)
        return false;

    if(setsockopt(so, SOL_DCCP, DCCP_SOCKOPT_CCID, (char *)&ccid, sizeof(ccid)) < 0)
        return false;

    return true;
}

ssize_t CxSocket::recvinet(socket_t so, void *data, size_t len, int flags, struct sockaddr_internet *addr)
{
    assert(data != NULL);
    assert(len > 0);

    socklen_t slen = sizeof(struct sockaddr_internet);
    return _recvfrom_(so, (caddr_t)data, len, flags, (struct sockaddr *)addr, &slen);
}

ssize_t CxSocket::recvfrom(socket_t so, void *data, size_t len, int flags, struct sockaddr_storage *addr)
{
    assert(data != NULL);
    assert(len > 0);

    socklen_t slen = sizeof(struct sockaddr_storage);
    return _recvfrom_(so, (caddr_t)data, len, flags, (struct sockaddr *)addr, &slen);
}

size_t CxSocket::readfrom(void *data, size_t len, struct sockaddr_storage *from)
{
    assert(data != NULL);
    assert(len > 0);

    // wait for input by timer if possible...
    if(iowait && iowait != CxGlobal::inf && !CxSocket::wait(so, iowait))
        return 0;

    socklen_t slen = sizeof(struct sockaddr_storage);
    ssize_t result = _recvfrom_(so, (caddr_t)data, len, 0, (struct sockaddr *)from, &slen);

    if(result < 0) {
        ioerr = CxSocket::error();
        return 0;
    }
    return (size_t)result;
}

size_t CxSocket::writeto(const void *data, size_t dlen, const struct sockaddr *dest)
{
    assert(data != NULL);
    assert(dlen > 0);

    socklen_t slen = 0;
    if(dest)
        slen = len(dest);

    ssize_t result = _sendto_(so, (caddr_t)data, dlen, MSG_NOSIGNAL, dest, slen);
    if(result < 0) {
        ioerr = CxSocket::error();
        return 0;
    }
    return (size_t)result;
}

ssize_t CxSocket::sendto(socket_t so, const void *data, size_t dlen, int flags, const struct sockaddr *dest)
{
    assert(data != NULL);
    assert(dlen > 0);

    socklen_t slen = 0;
    if(dest)
        slen = len(dest);

    return _sendto_(so, (caddr_t)data, dlen, MSG_NOSIGNAL | flags, dest, slen);
}

size_t CxSocket::writes(const char *str)
{
    if(!str)
        return 0;

    if(!*str)
        return 0;

    return writeto(str, strlen(str), NULL);
}

size_t CxSocket::readline(char *data, size_t max)
{
    assert(data != NULL);
    assert(max > 0);

    *data = 0;

    ssize_t result = CxSocket::readline(so, data, max, iowait);
    if(result < 0) {
        ioerr = CxSocket::error();
        return 0;
    }
    return (size_t)result;
}

ssize_t CxSocket::readline(socket_t so, char *data, size_t max, timems_t timeout)
{
    assert(data != NULL);
    assert(max > 0);

    bool crlf = false;
    bool nl = false;
    int nleft = max - 1;        // leave space for null byte
    int nstat, c;

    if(max < 1)
        return -1;

    data[0] = 0;
    while(nleft && !nl) {
        if(timeout) {
            if(!wait(so, timeout))
                return 0;
        }
        nstat = _recv_(so, data, nleft, MSG_PEEK);
        if(nstat < 0)
            return -1;

        if(nstat == 0)
            return max - nleft - 1;

        for(c = 0; c < nstat; ++c) {
            if(data[c] == '\n') {
                if(c > 0 && data[c - 1] == '\r')
                    crlf = true;
                ++c;
                nl = true;
                break;
            }
        }

        nstat = _recv_(so, (caddr_t)data, c, 0);
        if(nstat < 0)
            break;

        if(crlf) {
            --nstat;
            data[nstat - 1] = '\n';
        }

        data += nstat;
        nleft -= nstat;
    }

    if(nl)
        --data;

    *data = 0;
    return ssize_t(max - nleft - 1);
}

int CxSocket::loopback(socket_t so, bool enable)
{
    union {
        struct sockaddr_storage saddr;
        struct sockaddr_in inaddr;
    } us;

    struct sockaddr *addr = (struct sockaddr *)&us.saddr;
    int family;
    socklen_t len = sizeof(us.saddr);
    int opt = 0;

    if(enable)
        opt = 1;

    if(so == INVALID_SOCKET)
        return EBADF;

    _getsockname_(so, addr, &len);
    family = us.inaddr.sin_family;
    switch(family) {
    case AF_INET:
        if(!setsockopt(so, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&opt, sizeof(opt)))
            return 0;
        break;
#if defined(AF_INET6) && defined(IPROTO_IPV6)
    case AF_INET6:
        if(!setsockopt(so, IPPROTO_IPV6, IPV6_MULTICAST_LOOP, (char *)&opt, sizeof(opt)))
            return 0;
#endif
    }
    int err = CxSocket::error();
    if(!err)
        err = EIO;
    return err;
}

int CxSocket::ttl(socket_t so, unsigned char t)
{
    union {
        struct sockaddr_storage saddr;
        struct sockaddr_in inaddr;
    } us;

    struct sockaddr *addr = (struct sockaddr *)&us.saddr;
    int family;
    socklen_t len = sizeof(us.saddr);

    if(so == INVALID_SOCKET)
        return EBADF;

    _getsockname_(so, addr, &len);
    family = us.inaddr.sin_family;
    switch(family) {
    case AF_INET:
        if(!setsockopt(so, IPPROTO_IP, IP_TTL, (char *)&t, sizeof(t)))
            return 0;
        break;
#if defined(AF_INET6) && defined(IPPROTO_IPV6)
    case AF_INET6:
        if(!setsockopt(so, IPPROTO_IPV6, IPV6_UNICAST_HOPS, (char *)&t, sizeof(t)))
            return 0;
#endif
    }
    int err = CxSocket::error();
    if(!err)
        err = EIO;
    return err;
}

int CxSocket::priority(socket_t so, int pri)
{
    if(so == INVALID_SOCKET)
        return EBADF;
#ifdef  SO_PRIORITY
    if(!setsockopt(so, SOL_SOCKET, SO_PRIORITY, (char *)&pri, (socklen_t)sizeof(pri)))
        return 0;
    int err = CxSocket::error();
    if(!err)
        err = EIO;
    return err;
#else
    return ENOSYS;
#endif
}

int CxSocket::tos(socket_t so, int ts)
{
    if(so == INVALID_SOCKET)
        return EBADF;

#ifdef  SOL_IP
    if(!setsockopt(so, SOL_IP, IP_TOS,(char *)&ts, (socklen_t)sizeof(ts)))
        return 0;

    int err = CxSocket::error();
    if(!err)
        err = EIO;
    return err;
#else
    return ENOSYS;
#endif
}

int CxSocket::broadcast(socket_t so, bool enable)
{
    if(so == INVALID_SOCKET)
        return EBADF;
    int opt = (enable ? 1 : 0);
    if(!::setsockopt(so, SOL_SOCKET, SO_BROADCAST,
              (char *)&opt, (socklen_t)sizeof(opt)))
        return 0;

    int err = CxSocket::error();
    if(!err)
        err = EIO;
    return err;
}

int CxSocket::nodelay(socket_t so)
{
    if(so == INVALID_SOCKET)
        return EBADF;
#if defined(TCP_NODELAY)
    int opt = 1;
    if(!::setsockopt(so, IPPROTO_TCP, TCP_NODELAY,
            (char *)&opt, (socklen_t)sizeof(opt)))
        return 0;
#else
    return ENOSYS;
#endif
    int err = CxSocket::error();
    if(!err)
        err = EIO;
    return err;
}

int CxSocket::keepalive(socket_t so, bool enable)
{
    if(so == INVALID_SOCKET)
        return EBADF;
#if defined(SO_KEEPALIVE) || defined(GM_OS_WIN)
    int opt = (enable ? ~0 : 0);
    if(!::setsockopt(so, SOL_SOCKET, SO_KEEPALIVE, (char *)&opt, (socklen_t)sizeof(opt)))
        return 0;
    int err = CxSocket::error();
    if(!err)
        err = EIO;
    return err;
#else
    return ENOSYS;
#endif
}

int CxSocket::multicast(socket_t so, unsigned ttl)
{
    struct sockaddr_internet addr;
    socklen_t len = sizeof(addr);
    bool enable;
    int rtn;

    if(so == INVALID_SOCKET)
        return EBADF;

    if(ttl)
        enable = true;
    else
        enable = false;

    _getsockname_(so, (struct sockaddr *)&addr, &len);
    if(!enable)
        switch(addr.address.sa_family)
        {
        case AF_INET:
            memset(&addr.ipv4.sin_addr, 0, sizeof(addr.ipv4.sin_addr));
            break;
#ifdef  AF_INET6
        case AF_INET6:
            memset(&addr.ipv6.sin6_addr, 0, sizeof(addr.ipv6.sin6_addr));
            break;
#endif
        default:
            break;
        }
    switch(addr.address.sa_family) {
#if defined(AF_INET6) && defined(IPPROTO_IPV6)
    case AF_INET6:
        rtn = ::setsockopt(so, IPPROTO_IPV6, IPV6_MULTICAST_IF, (char *)&addr.ipv6.sin6_addr, sizeof(addr.ipv6.sin6_addr));
        if(!rtn)
            rtn = ::setsockopt(so, IPPROTO_IPV6, IPV6_MULTICAST_HOPS, (char *)&ttl, sizeof(ttl));
        if(rtn) {
            rtn = CxSocket::error();
            if(!rtn)
                rtn = EIO;
        }
        return rtn;
#endif
    case AF_INET:
#ifdef  IP_MULTICAST_IF
        rtn = ::setsockopt(so, IPPROTO_IP, IP_MULTICAST_IF, (char *)&addr.ipv4.sin_addr, sizeof(addr.ipv4.sin_addr));
        if(!rtn)
            rtn = ::setsockopt(so, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&ttl, sizeof(ttl));
        if(rtn) {
            rtn = CxSocket::error();
            if(!rtn)
                rtn = EIO;
        }
        return rtn;
#else
        return ENOSYS;
#endif
    default:
        return ENOSYS;
    }
}

int CxSocket::join(const struct addrinfo *addr)
{
    int rtn = CxSocket::join(so, addr);
    if(rtn)
        ioerr = rtn;
    return rtn;
}

int CxSocket::drop(const struct addrinfo *addr)
{
    int rtn = CxSocket::drop(so, addr);
    if(rtn)
        ioerr = rtn;
    return rtn;
}

int CxSocket::wait(timems_t timeout)
{
    bool mode = true;

    if(timeout < CxGlobal::inf)
        mode = false;

    int rtn = CxSocket::blocking(so, mode);
    if(!rtn)
        iowait = timeout;
    else
        ioerr = rtn;

    return rtn;
}

int CxSocket::blocking(socket_t so, bool enable)
{
    if(so == INVALID_SOCKET)
        return EBADF;

#if defined(GM_OS_WIN)
    unsigned long flag = (enable ? 0 : 1);
    if(!ioctlsocket(so, FIONBIO, &flag))
        return 0;

#else
    long flags = fcntl(so, F_GETFL);
    if(enable)
        flags &=~ O_NONBLOCK;
    else
        flags |= O_NONBLOCK;
    if(!fcntl(so, F_SETFL, flags))
        return 0;
#endif
    int err = CxSocket::error();
    if(!err)
        err = EIO;
    return err;
}

int CxSocket::disconnect(socket_t so)
{
    union {
        struct sockaddr_storage saddr;
        struct sockaddr_in inaddr;
    } us;

    struct sockaddr *addr = (struct sockaddr *)&us.saddr;
    socklen_t len = sizeof(us.saddr);

#if defined(GM_OS_WIN)
    _getsockname_(so, addr, &len);
    int family = us.inaddr.sin_family;
    memset(addr, 0, sizeof(us.saddr));
    us.inaddr.sin_family = family;
#else
    memset(addr, 0, sizeof(us.saddr));
    us.inaddr.sin_family = AF_UNSPEC;
#endif
    if((size_t)len > sizeof(us.saddr))
        len = sizeof(us.saddr);
    if(so == INVALID_SOCKET)
        return EBADF;
    if(!_connect_(so, addr, len))
        return 0;
    int err = CxSocket::error();
    if(!err)
        err = EIO;
    return err;
}

int CxSocket::join(socket_t so, const struct addrinfo *node)
{
    assert(node != NULL);

    struct multicast_internet mcast;
    struct sockaddr_internet addr;
    socklen_t len = sizeof(addr);
    const struct sockaddr_internet *target;
    int family;
    int rtn = 0;

    if(so == INVALID_SOCKET)
        return EBADF;

    _getsockname_(so, (struct sockaddr *)&addr, &len);
    while(!rtn && node && node->ai_addr) {
        target = (const struct sockaddr_internet *)node->ai_addr;
        family = node->ai_family;
        node = node->ai_next;
        if(family != addr.address.sa_family)
            continue;
        switch(addr.address.sa_family) {
#if defined(AF_INET6) && defined(IPV6_ADD_MEMBERSHIP) && defined(IPPROTO_IPV6)
        case AF_INET6:
            mcast.ipv6.ipv6mr_interface = 0;
            memcpy(&mcast.ipv6.ipv6mr_multiaddr, &target->ipv6.sin6_addr, sizeof(target->ipv6.sin6_addr));
            rtn = ::setsockopt(so, IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP, (char *)&mcast, sizeof(mcast.ipv6));
            break;
#endif
#if defined(IP_ADD_MEMBERSHIP)
        case AF_INET:
            mcast.ipv4.imr_interface.s_addr = INADDR_ANY;
            memcpy(&mcast.ipv4.imr_multiaddr, &target->ipv4.sin_addr, sizeof(target->ipv4.sin_addr));
            rtn = ::setsockopt(so, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mcast, sizeof(mcast.ipv4));
            break;
#endif
        default:
            return ENOSYS;
        }
    }
    if(rtn) {
        rtn = CxSocket::error();
        if(!rtn)
            rtn = EIO;
    }
    return rtn;
}

int CxSocket::drop(socket_t so, const struct addrinfo *node)
{
    assert(node != NULL);

    struct multicast_internet mcast;
    struct sockaddr_internet addr;
    socklen_t len = sizeof(addr);
    const struct sockaddr_internet *target;
    int family;
    int rtn = 0;

    if(so == INVALID_SOCKET)
        return EBADF;

    _getsockname_(so, (struct sockaddr *)&addr, &len);
    while(!rtn && node && node->ai_addr) {
        target = (const struct sockaddr_internet *)node->ai_addr;
        family = node->ai_family;
        node = node->ai_next;

        if(family != addr.address.sa_family)
            continue;

        switch(addr.address.sa_family) {
#if defined(AF_INET6) && defined(IPV6_DROP_MEMBERSHIP) && defined(IPPROTO_IPV6)
        case AF_INET6:
            mcast.ipv6.ipv6mr_interface = 0;
            memcpy(&mcast.ipv6.ipv6mr_multiaddr, &target->ipv6.sin6_addr, sizeof(target->ipv6.sin6_addr));
            rtn = ::setsockopt(so, IPPROTO_IPV6, IPV6_DROP_MEMBERSHIP, (char *)&mcast, sizeof(mcast.ipv6));
            break;
#endif
#if defined(IP_DROP_MEMBERSHIP)
        case AF_INET:
            mcast.ipv4.imr_interface.s_addr = INADDR_ANY;
            memcpy(&mcast.ipv4.imr_multiaddr, &target->ipv4.sin_addr, sizeof(target->ipv4.sin_addr));
            rtn = ::setsockopt(so, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char *)&mcast, sizeof(mcast.ipv4));
            break;
#endif
        default:
            return ENOSYS;
        }
    }
    if(rtn) {
        rtn = CxSocket::error();
        if(!rtn)
            rtn = EIO;
    }
    return rtn;
}

socket_t CxSocket::create(const struct addrinfo *node, int stype, int sprotocol)
{
    assert(node != NULL);

    socket_t so = INVALID_SOCKET;
    int sfamily = AF_UNSPEC;
    int cprotocol, ctype;

    while(node) {
        if(stype && node->ai_socktype && node->ai_socktype != stype)
            goto next;

        if(sprotocol && node->ai_protocol && node->ai_protocol != sprotocol)
            goto next;

        if(node->ai_family != sfamily) {
            if(so != INVALID_SOCKET)
                CxSocket::release(so);
            sfamily = node->ai_family;
            if(stype)
                ctype = stype;
            else
                ctype = node->ai_socktype;
            if(sprotocol)
                cprotocol = sprotocol;
            else
                cprotocol = node->ai_protocol;
            so = CxSocket::create(sfamily, ctype, cprotocol);
        }
        if(so != INVALID_SOCKET) {
            if(!_connect_(so, node->ai_addr, node->ai_addrlen))
                return so;
        }
next:
        node = node->ai_next;
    }
    if(so != INVALID_SOCKET)
        CxSocket::release(so);
    return INVALID_SOCKET;
}

int CxSocket::connectto(struct addrinfo *node)
{
    return (ioerr = connectto(so, node));
}

int CxSocket::disconnect(void)
{
    return (ioerr = disconnect(so));
}

int CxSocket::connectto(socket_t so, struct addrinfo *node)
{
    assert(node != NULL);

    int rtn = -1;
    int socket_family;

    if(so == INVALID_SOCKET)
        return EBADF;

    socket_family = family(so);

    while(node) {
        if(node->ai_family == socket_family) {
            if(!_connect_(so, node->ai_addr, node->ai_addrlen)) {
                rtn = 0;
                goto exit;
            }
        }
        node = node->ai_next;
    }

exit:
#ifndef GM_OS_WIN
    if(!rtn || errno == EINPROGRESS)
        return 0;
#endif
    if(rtn) {
        rtn = CxSocket::error();
        if(!rtn)
            rtn = EIO;
    }
    return rtn;
}

int CxSocket::error(socket_t so)
{
    assert(so != INVALID_SOCKET);

    int opt;
    socklen_t slen = sizeof(opt);

    if(getsockopt(so, SOL_SOCKET, SO_ERROR, (caddr_t)&opt, &slen))
        return ENOSYS;

    return opt;
}

#ifdef GM_OS_WIN
unsigned CxSocket::pending(socket_t so)
{
    u_long opt;
    if(so == INVALID_SOCKET)
        return 0;

    ioctlsocket(so, FIONREAD, &opt);
    return (unsigned)opt;
}

#else
unsigned CxSocket::pending(socket_t so)
{
    int opt;
    if(so == INVALID_SOCKET)
        return 0;

    if(::ioctl(so, FIONREAD, &opt))
        return 0;
    return (unsigned)opt;
}

#endif

int CxSocket::sendsize(socket_t so, unsigned size)
{
    assert(so != INVALID_SOCKET);

#ifdef  SO_SNDBUF
    if(!setsockopt(so, SOL_SOCKET, SO_SNDBUF, (caddr_t)&size, sizeof(size)))
        return 0;
    int err = CxSocket::error();
    if(!err)
        err = EIO;
    return err;
#else
    return ENOSYS;
#endif
}

int CxSocket::sendwait(socket_t so, unsigned size)
{
    assert(so != INVALID_SOCKET);

#ifdef  SO_SNDLOWAT
    if(!setsockopt(so, SOL_SOCKET, SO_SNDLOWAT, (caddr_t)&size, sizeof(size)))
        return 0;
    int err = CxSocket::error();
    if(!err)
        err = EIO;
    return err;
#else
    return ENOSYS;
#endif
}

int CxSocket::recvsize(socket_t so, unsigned size)
{
#ifdef  SO_RCVBUF
    if(!setsockopt(so, SOL_SOCKET, SO_RCVBUF, (caddr_t)&size, sizeof(size)))
        return 0;
    int err = CxSocket::error();
    if(!err)
        err = EIO;
    return err;
#else
    return ENOSYS;
#endif
}

bool CxSocket::connected(void) const
{
    char buf;

    if(so == INVALID_SOCKET)
        return false;

    if(!wait())
        return true;

    if(_recv_(so, &buf, 1, MSG_DONTWAIT | MSG_PEEK) < 1)
        return false;

    return true;
}

bool CxSocket::is_pending(unsigned qio) const
{
    if(pending() >= qio)
        return true;

    return false;
}

bool CxSocket::wait(timems_t timeout) const
{
    return wait(so, timeout);
}

//don't support poll
bool CxSocket::wait(socket_t so, timems_t timeout)
{
    int status;
    struct timeval tv;
    struct timeval *tvp = &tv;
    fd_set grp;

    if(so == INVALID_SOCKET)
        return false;

    if(timeout == CxGlobal::inf)
        tvp = NULL;
    else {
        tv.tv_usec = (timeout % 1000) * 1000;
        tv.tv_sec = timeout / 1000;
    }

    FD_ZERO(&grp);
    FD_SET(so, &grp);
    status = _select_((int)(so + 1), &grp, NULL, NULL, tvp);
    if(status < 1)
        return false;
    if(FD_ISSET(so, &grp))
        return true;
    return false;
}

//don't support poll
bool CxSocket::waitSending(timems_t timeout) const
{
    int status;
    struct timeval tv;
    struct timeval *tvp = &tv;
    fd_set grp;

    if(so == INVALID_SOCKET)
        return false;

    if(timeout == CxGlobal::inf)
        tvp = NULL;
    else {
        tv.tv_usec = (timeout % 1000) * 1000;
        tv.tv_sec = timeout / 1000;
    }

    FD_ZERO(&grp);
    FD_SET(so, &grp);
    status = _select_((int)(so + 1), NULL, &grp, NULL, tvp);
    if(status < 1)
        return false;
    if(FD_ISSET(so, &grp))
        return true;
    return false;
}


#ifdef  GM_OS_WIN
#undef  AF_UNIX
#endif

struct ::addrinfo *CxSocket::hinting(socket_t so, struct addrinfo *hint)
{
    assert(hint != NULL);

    union {
        struct sockaddr_storage st;
        struct sockaddr_in in;
    } us;
    struct sockaddr *sa = (struct sockaddr *)&us.st;
    socklen_t slen = sizeof(us.st);

    memset(hint, 0, sizeof(struct addrinfo));
    memset(sa, 0, sizeof(us.st));
    if(_getsockname_(so, sa, &slen))
        return NULL;
    hint->ai_family = us.in.sin_family;
    slen = sizeof(hint->ai_socktype);
    getsockopt(so, SOL_SOCKET, SO_TYPE, (caddr_t)&hint->ai_socktype, &slen);
    return hint;
}

int CxSocket::bindto(socket_t so, const struct sockaddr *iface)
{
    if(!_bind_(so, iface, len(iface)))
        return 0;
    return CxSocket::error();
}

int CxSocket::listento(socket_t so, int backlog)
{
    if(_listen_(so, backlog))
        return CxSocket::error();
    return 0;
}

int CxSocket::connectto(socket_t so, const struct sockaddr *iface)
{
    if(!_connect_(so, iface, len(iface)))
        return 0;
    return CxSocket::error();
}

socket_t CxSocket::acceptfrom(socket_t so, struct sockaddr_storage *addr)
{
    socklen_t len = sizeof(struct sockaddr_storage);
    if(addr)
        return _accept_(so, (struct sockaddr *)addr, &len);
    else
        return _accept_(so, NULL, NULL);
}

unsigned CxSocket::keyhost(const struct sockaddr *addr, unsigned keysize)
{
    assert(addr != NULL);
    assert(keysize > 0);

    unsigned key = 0;
    caddr_t cp = NULL;
    unsigned len;
switch(addr->sa_family) {
#ifdef  AF_INET6
    case AF_INET6:
        cp = (caddr_t)(&((const struct sockaddr_in6 *)(addr))->sin6_addr);
        len = 16;
        break;
#endif
    case AF_INET:
        cp = (caddr_t)(&((const struct sockaddr_in *)(addr))->sin_addr);
        len = 4;
        break;
    default:
        return 0;
    }
    while(len--) {
        key = key << 1;
        key ^= cp[len];
    }
    return key % keysize;
}

unsigned CxSocket::keyindex(const struct sockaddr *addr, unsigned keysize)
{
    assert(addr != NULL);
    assert(keysize > 0);

    unsigned key = 0;
    caddr_t cp = NULL;
    unsigned len;
switch(addr->sa_family) {
#ifdef  AF_INET6
    case AF_INET6:
        cp = (caddr_t)(&((const struct sockaddr_in6 *)(addr))->sin6_addr);
        len = 16;
        key = service(addr);
        break;
#endif
    case AF_INET:
        cp = (caddr_t)(&((const struct sockaddr_in *)(addr))->sin_addr);
        len = 4;
        key = service(addr);
        break;
    default:
        return 0;
    }
    while(len--) {
        key = key << 1;
        key ^= cp[len];
    }
    return key % keysize;
}

short CxSocket::service(const struct sockaddr *addr)
{
    assert(addr != NULL);

    switch(addr->sa_family) {
#ifdef  AF_INET6
    case AF_INET6:
        return ntohs(((const struct sockaddr_in6 *)(addr))->sin6_port);
#endif
    case AF_INET:
        return ntohs(((const struct sockaddr_in *)(addr))->sin_port);
    }
    return 0;
}

char *CxSocket::query(const struct sockaddr *addr, char *name, socklen_t size)
{
    assert(addr != NULL);
    assert(name != NULL);

#ifdef  GM_OS_WIN
    DWORD slen = size;
#endif

    *name = 0;
    if(!addr)
        return NULL;

    switch(addr->sa_family) {
#ifdef  AF_UNIX
    case AF_UNIX:
        CxStringC::set(name, size, ((const struct sockaddr_un *)(addr))->sun_path);
        return name;
#endif
#ifdef  GM_OS_WIN
#ifdef  AF_INET6
    case AF_INET6:
        struct sockaddr_in6 saddr6;
        memcpy(&saddr6, addr, sizeof(saddr6));
        saddr6.sin6_port = 0;
        WSAAddressToString((struct sockaddr *)&saddr6, sizeof(saddr6), NULL, name, &slen);
        return name;
#endif
    case AF_INET:
        struct sockaddr_in saddr;
        memcpy(&saddr, addr, sizeof(saddr));
        saddr.sin_port = 0;
        WSAAddressToString((struct sockaddr *)&saddr, sizeof(saddr), NULL, name, &slen);
        return name;
#else
#ifdef  HAVE_INET_NTOP
#ifdef  AF_INET6
    case AF_INET6:
        inet_ntop(addr->sa_family, &((const struct sockaddr_in6 *)(addr))->sin6_addr, name, size);
        return name;
#endif
    case AF_INET:
        inet_ntop(addr->sa_family, &((const struct sockaddr_in *)(addr))->sin_addr, name, size);
        return name;
#else
    case AF_INET:
        ENTER_EXCLUSIVE
        CxStringC::set(name, size, inet_ntoa(((const struct sockaddr_in *)(addr))->sin_addr));
        LEAVE_EXCLUSIVE
        return name;
#endif
#endif
    }
    return NULL;
}

int CxSocket::via(struct sockaddr *iface, const struct sockaddr *dest)
{
    assert(iface != NULL);
    assert(dest != NULL);

    int rtn = -1;
    socket_t so = INVALID_SOCKET;
    socklen_t slen = len(dest);

    if(slen)
        memset(iface, 0, slen);

    iface->sa_family = AF_UNSPEC;
    switch(dest->sa_family) {
#ifdef  AF_INET6
    case AF_INET6:
#endif
    case AF_INET:
        so = ::socket(dest->sa_family, SOCK_DGRAM, 0);
        if((socket_t)so == INVALID_SOCKET)
            return -1;
        socket_mapping(dest->sa_family, so);
        if(!_connect_(so, dest, slen))
            rtn = _getsockname_(so, iface, &slen);
        break;
    default:
        return ENOSYS;
    }
    switch(iface->sa_family) {
    case AF_INET:
        ((struct sockaddr_in*)(iface))->sin_port = 0;
        break;
#ifdef  AF_INET6
    case AF_INET6:
        ((struct sockaddr_in6*)(iface))->sin6_port = 0;
        break;
#endif
    }

    if((socket_t)so != INVALID_SOCKET) {
#ifdef  GM_OS_WIN
        ::closesocket(so);
#else
        ::shutdown(so, SHUT_RDWR);
        ::close(so);
#endif
        so = INVALID_SOCKET;
    }
    if(rtn)
        rtn = CxSocket::error();
    return rtn;
}

bool CxSocket::eq_subnet(const struct sockaddr *s1, const struct sockaddr *s2)
{
    assert(s1 != NULL && s2 != NULL);

    unsigned char *a1, *a2;
    if(s1->sa_family != s2->sa_family)
        return false;

    if(s1->sa_family != AF_INET)
        return true;

    a1 = (unsigned char *)&(((const struct sockaddr_in *)(s1))->sin_addr);
    a2 = (unsigned char *)&(((const struct sockaddr_in *)(s1))->sin_addr);

    if(*a1 == *a2 && *a1 < 128)
        return true;

    if(*a1 != *a2)
        return false;

    if(*a1 > 127 && *a1 < 192 && a1[1] == a2[1])
        return true;

    if(a1[1] != a2[1])
        return false;

    if(a1[2] != a2[2])
        return false;

    return true;
}

unsigned CxSocket::store(struct sockaddr_internet *storage, const struct sockaddr *address)
{
    if(storage == NULL || address == NULL)
        return 0;

    if(address->sa_family == AF_INET) {
        memcpy(&storage->ipv4, address, sizeof(storage->ipv4));
        return sizeof(storage->ipv4);
    }

#ifdef  AF_INET6
    if(address->sa_family == AF_INET6) {
        memcpy(&storage->ipv6, address, sizeof(storage->ipv6));
        return sizeof(storage->ipv6);
    }
#endif

    return 0;
}

unsigned CxSocket::copy(struct sockaddr *s1, const struct sockaddr *s2)
{
    if(s1 == NULL || s2 == NULL)
        return 0;

    socklen_t slen = len(s1);
    if(slen > 0) {
        memcpy(s1, s2, slen);
        return slen;
    }
    return 0;
}

bool CxSocket::eq_host(const struct sockaddr *s1, const struct sockaddr *s2)
{
    assert(s1 != NULL && s2 != NULL);

    if(s1->sa_family != s2->sa_family)
        return false;

    switch(s1->sa_family) {
    case AF_INET:
        if(memcmp(&(((const struct sockaddr_in *)s1)->sin_addr),
            &(((const struct sockaddr_in *)s2)->sin_addr), 4))
                return false;

        return true;
#ifdef  AF_INET6
    case AF_INET6:
        if(memcmp(&(((const struct sockaddr_in6 *)s1)->sin6_addr),
            &(((const struct sockaddr_in6 *)s2)->sin6_addr), 4))
                return false;

        return true;
#endif
    default:
        if(memcmp(s1, s2, len(s1)))
            return false;
        return true;
    }
    return false;
}


bool CxSocket::equal(const struct sockaddr *s1, const struct sockaddr *s2)
{
    assert(s1 != NULL && s2 != NULL);

    if(s1->sa_family != s2->sa_family)
        return false;

    switch(s1->sa_family) {
    case AF_INET:
        if(memcmp(&(((const struct sockaddr_in *)s1)->sin_addr),
            &(((const struct sockaddr_in *)s2)->sin_addr), 4))
                return false;

        if(!((const struct sockaddr_in *)s1)->sin_port || !((const struct sockaddr_in *)s2)->sin_port)
            return true;

        if(((const struct sockaddr_in *)s1)->sin_port != ((const struct sockaddr_in *)s2)->sin_port)
            return false;

        return true;
#ifdef  AF_INET6
    case AF_INET6:
        if(memcmp(&(((const struct sockaddr_in6 *)s1)->sin6_addr),
            &(((const struct sockaddr_in6 *)s2)->sin6_addr), 4))
                return false;

        if(!((const struct sockaddr_in6 *)s1)->sin6_port || !((const struct sockaddr_in6 *)s2)->sin6_port)
            return true;

        if(((const struct sockaddr_in6 *)s1)->sin6_port != ((const struct sockaddr_in6 *)s2)->sin6_port)
            return false;

        return true;
#endif
    default:
        if(memcmp(s1, s2, len(s1)))
            return false;
        return true;
    }
    return false;
}


/**
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */


CxSocket::CxSocket()
{
#ifdef  GM_OS_WIN
    init();
#endif
    so = INVALID_SOCKET;
    iowait = CxGlobal::inf;
    ioerr = 0;
}

CxSocket::CxSocket(socket_t s)
{
#ifdef  GM_OS_WIN
    init();
#endif
    so = s;
    iowait = CxGlobal::inf;
    ioerr = 0;
}

CxSocket::CxSocket(struct addrinfo *addr)
{
#ifdef  GM_OS_WIN
    init();
#endif
    assert(addr != NULL);

    while(addr) {
        so = ::socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
        socket_mapping(addr->ai_family, so);
        if(so != INVALID_SOCKET) {
            if(!_connect_(so, addr->ai_addr, addr->ai_addrlen))
                return;
        }
        addr = addr->ai_next;
    }
    so = INVALID_SOCKET;
    iowait = CxGlobal::inf;
    ioerr = 0;
}

CxSocket::CxSocket(int family, int type, int protocol)
{
#ifdef  GM_OS_WIN
    init();
#endif
    so = create(family, type, protocol);
    iowait = CxGlobal::inf;
    ioerr = 0;
}

CxSocket::CxSocket(const CxSocket &s)
{
#ifdef  GM_OS_WIN
    HANDLE pidH = GetCurrentProcess();
    HANDLE dupH;

    if(DuplicateHandle(pidH, reinterpret_cast<HANDLE>(s.so), pidH, &dupH, 0, FALSE, DUPLICATE_SAME_ACCESS))
        so = reinterpret_cast<SOCKET>(dupH);
    else
        so = INVALID_SOCKET;
#else
    so = ::dup(s.so);
#endif
    iowait = s.iowait;
    ioerr = 0;
}

CxSocket::~CxSocket()
{
    release();
}

socket_t CxSocket::create(int family, int type, int protocol)
{
    socket_t so;
#ifdef  GM_OS_WIN
    init();
#endif
    so = ::socket(family, type, protocol);
    socket_mapping(family, so);
    return so;
}

void CxSocket::cancel(void)
{
    if(so != INVALID_SOCKET)
        ::shutdown(so, SHUT_RDWR);
}

void CxSocket::cancel(socket_t so)
{
    if(so != INVALID_SOCKET)
        ::shutdown(so, SHUT_RDWR);
}

void CxSocket::release(socket_t so)
{
#ifdef  GM_OS_WIN
    ::closesocket(so);
#else
    if(!::shutdown(so, SHUT_RDWR))
        ::close(so);
#endif
}

void CxSocket::release(void)
{
    if(so != INVALID_SOCKET) {
#ifdef  GM_OS_WIN
        ::closesocket(so);
#else
        ::shutdown(so, SHUT_RDWR);
        ::close(so);
#endif
        so = INVALID_SOCKET;
    }
    iowait = CxGlobal::inf;
    ioerr = 0;
}

CxSocket::operator bool()
{
    if(so == INVALID_SOCKET)
        return false;
    return true;
}

bool CxSocket::operator!() const
{
    if(so == INVALID_SOCKET)
        return true;
    return false;
}

CxSocket &CxSocket::operator=(socket_t s)
{
    release();
    so = s;
    return *this;
}

size_t CxSocket::peek(void *data, size_t len) const
{
    assert(data != NULL);
    assert(len > 0);

    if(iowait && iowait != CxGlobal::inf && !CxSocket::wait(so, iowait))
        return 0;

    ssize_t rtn = _recv_(so, (caddr_t)data, 1, MSG_DONTWAIT | MSG_PEEK);
    if(rtn < 1)
        return 0;
    return (size_t)rtn;
}

size_t CxSocket::printf(const char *format, ...)
{
    assert(format != NULL);

    char buf[1024];
    va_list args;

    va_start(args, format);
    vsnprintf(buf, sizeof(buf), format, args);
    va_end(args);

    return writes(buf);
}

ssize_t CxSocket::printf(socket_t so, const char *format, ...)
{
    assert(format != NULL);

    char buf[536];
    va_list args;

    va_start(args, format);
    vsnprintf(buf, sizeof(buf), format, args);
    va_end(args);

    return sendto(so, buf, strlen(buf), 0, NULL);
}

socklen_t CxSocket::len(const struct sockaddr *sa)
{
    if(!sa)
        return 0;

    switch(sa->sa_family)
    {
    case AF_INET:
        return sizeof(sockaddr_in);
#ifdef  AF_INET6
    case AF_INET6:
        return sizeof(sockaddr_in6);
#endif
    default:
        return sizeof(sockaddr_storage);
    }
}

int CxSocket::family(socket_t so)
{
    assert(so != INVALID_SOCKET);

    union {
        struct sockaddr_storage saddr;
        struct sockaddr_in inaddr;
    } us;

    socklen_t len = sizeof(us.saddr);
    struct sockaddr *addr = (struct sockaddr *)(&us.saddr);

    if(_getsockname_(so, addr, &len))
        return AF_UNSPEC;

    return us.inaddr.sin_family;
}


/**
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */


std::string CxIpAddress::getIp(const sockaddr * oSockAddr)
{
    char sName[64];
    memset(sName, 0, sizeof(sName));
    CxSocket::query(oSockAddr, sName, sizeof(sName));
    return sName;
}

short CxIpAddress::getPort(const sockaddr *oSockAddr)
{
    return CxSocket::service(oSockAddr);
}

void CxIpAddress::extractIpPort(const sockaddr *oSockAddr, std::string &sIp, ushort &iPort)
{
    char sName[64];
    memset(sName, 0, sizeof(sName));
    CxSocket::query(oSockAddr, sName, sizeof(sName));
    sIp = sName;
    iPort = CxSocket::service(oSockAddr);
}

uint32 CxIpAddress::ipV4ToInt32(const std::string &sIp)
{
    return ::inet_addr(sIp.c_str());
}

std::string CxIpAddress::ipV4ToString(uint32 iIp)
{
    in_addr addr;
#ifdef GM_OS_WIN
    addr.S_un.S_addr = iIp;
#else
    addr.s_addr = iIp;
#endif
    return ::inet_ntoa(addr);
}

bool CxIpAddress::isValidIpV4(const std::string &sIp)
{
    return ::inet_addr(sIp.c_str()) != INADDR_NONE;
}

bool CxIpAddress::isIpV6(const std::string &sIp)
{
    return CxString::countOfString(sIp, '.') > 3;
}

bool CxIpAddress::isIpV6(const sockaddr *oSockAddr)
{
    assert(oSockAddr != NULL);
#ifdef AF_INET6
    return oSockAddr->sa_family == AF_INET6;
#endif
    return false;
}

//besttodo
bool CxIpAddress::isLocalIp(const std::string &sIp)
{
    return sIp == "0.0.0.0" || sIp == "127.0.0.1" || CxString::equalCase(sIp, "localhost");
}

void CxIpAddress::setIp(const std::string &value)
{
    std::string sIp = CxString::trim(value);
    if (sIp != _ip && CxString::isValidIp(sIp))
    {
        _ip = sIp;
        if (CxString::isValidPort(_port))
        {
            updateSockAddr();
        }
    }
}

void CxIpAddress::setPort(ushort iPort)
{
    if (_port != iPort && CxString::isValidPort(iPort))
    {
        _port = iPort;
        if (CxString::isValidIp(_ip))
        {
            updateSockAddr();
        }
    }
}

void CxIpAddress::setIsBind(bool value)
{
    if (_isBind != value)
    {
        _isBind = value;
        if (CxString::isValidIp(_ip) && CxString::isValidPort(_port))
        {
            updateSockAddr();
        }
    }
}

void CxIpAddress::init(const std::string &sIp, ushort iPort, bool isBind)
{
    if ( CxString::isValidPort(iPort) && CxString::isValidIp(sIp) )
    {
        _ip = CxString::trim(sIp);
        _port = iPort;
        _isBind = isBind;
        updateSockAddr();
    }
}

void CxIpAddress::init(const sockaddr *oSockAddr)
{
    assert(oSockAddr != NULL);

    if (oSockAddr->sa_family == AF_INET)
    {
        memmove(&_sockAddr.front()/*(char *)_sockAddr.data()*/, oSockAddr, sizeof(sockaddr_in));
    }
#ifdef AF_INET6
    else if (oSockAddr->sa_family == AF_INET6)
    {
        memmove(&_sockAddr.front()/*(char *)_sockAddr.data()*/, oSockAddr, sizeof(sockaddr_in6));
    }
#endif
    updateIpPort();
}

bool CxIpAddress::isSameIpAddress(const sockaddr *oSockAddr) const
{
    if (oSockAddr->sa_family == AF_INET && _family == AF_INET)
    {
        const sockaddr_in * oSelfSockAddr = (const sockaddr_in *)(&_sockAddr.front())/*_sockAddr.data()*/;
        const sockaddr_in * oOtherSockAddr = (const sockaddr_in *)oSockAddr;
#ifdef GM_OS_WIN
        return oSelfSockAddr->sin_port == oOtherSockAddr->sin_port && oSelfSockAddr->sin_addr.S_un.S_addr == oOtherSockAddr->sin_addr.S_un.S_addr;
#else
        return oSelfSockAddr->sin_port == oOtherSockAddr->sin_port && oSelfSockAddr->sin_addr.s_addr == oOtherSockAddr->sin_addr.s_addr;
#endif
    }
    else
    {
#ifdef AF_INET6
        if (oSockAddr->sa_family == AF_INET6 && _family == AF_INET6)
        {
            const sockaddr_in6 * oSelfSockAddr = (const sockaddr_in6 *)(&_sockAddr.front())/*_sockAddr.data()*/;
            const sockaddr_in6 * oOtherSockAddr = (const sockaddr_in6 *)oSockAddr;
            return oSelfSockAddr->sin6_port == oOtherSockAddr->sin6_port && memcmp(& oSelfSockAddr->sin6_addr, &oOtherSockAddr->sin6_addr, sizeof(in6_addr)) == 0;
        }
#endif
    }
    return false;
}

bool CxIpAddress::isSameIpAddress(const std::string &sIp, ushort iPort) const
{
    return _port == iPort && _ip == sIp;
}

void CxIpAddress::updateSockAddr()
{
    memset(&_sockAddr.front()/*(char*)_sockAddr.data()*/, 0, _sockAddr.size());
    if (isIpV6(_ip))
    {
        _family = 0;
#ifdef  AF_INET6
        _family = AF_INET6;
        if (_ip.empty() || _ip == "0.0.0.0" || CxString::equalCase(_ip, "localhost"))
            _ip = "127.0.0.1";

        struct in6_addr l_addr;
        memset(&l_addr, 0, sizeof(l_addr));
        struct in6_addr l_empty = l_addr;
    #ifdef  GM_OS_WIN
        struct sockaddr saddr;
        int slen = sizeof(saddr);
        struct sockaddr_in6 *paddr = (struct sockaddr_in6 *)&saddr;
        WSAStringToAddress((LPSTR)_ip.c_str(), AF_INET6, NULL, &saddr, &slen);
        l_addr = paddr->sin6_addr;
    #else
        int ok = inet_pton(AF_INET6, _ip.c_str(), &l_addr);
    #endif

        struct sockaddr_in6 * oIpv6 = (struct sockaddr_in6 *)(&_sockAddr.front())/*_sockAddr.data()*/;
        oIpv6->sin6_family = AF_INET6;
        oIpv6->sin6_port = htons(_port);
        if (_isBind)
        {
            if (_ip == "127.0.0.1")
                oIpv6->sin6_addr = l_empty; //INADDR_ANY
            else
                oIpv6->sin6_addr = l_addr;
        }
        else
        {
            oIpv6->sin6_addr = l_addr;
        }
#endif
    }
    else
    {
        _family = AF_INET;
        struct sockaddr_in * oIpv4 = (struct sockaddr_in *)(&_sockAddr.front())/*_sockAddr.data()*/;
        oIpv4->sin_family = AF_INET;
        oIpv4->sin_port = htons(_port);
        if (_isBind)
        {
            if (_ip.empty() || _ip == "0.0.0.0" || _ip == "127.0.0.1" || CxString::equalCase(_ip, "localhost"))
                oIpv4->sin_addr.s_addr = INADDR_ANY;
            else
                oIpv4->sin_addr.s_addr = ::inet_addr(_ip.c_str());
        }
        else
        {
            if (_ip.empty() || _ip == "0.0.0.0" || CxString::equalCase(_ip, "localhost"))
                _ip = "127.0.0.1";
            oIpv4->sin_addr.s_addr = ::inet_addr(_ip.c_str());
        }
    }
    _isValid = true;
}

void CxIpAddress::updateIpPort()
{
    const sockaddr * oSockAddr = (const sockaddr *)(&_sockAddr.front())/*_sockAddr.data()*/;
    _family = oSockAddr->sa_family;
    _ip = getIp(oSockAddr);
    _port = getPort(oSockAddr);
    _isValid = CxString::isValidPort(_port) && CxString::isValidIp(_ip);
}



/**
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

#undef GM_USER_DEFINE_GETADDRINFO
#ifndef GM_OS_WIN
//#define GM_USER_DEFINE_GETADDRINFO
#endif

#if defined(_WIN32_WINNT) && (_WIN32_WINNT < 0x0501)
void freeaddrinfo (struct addrinfo*) {}
int getaddrinfo (const char*,const char*,const struct addrinfo*,struct addrinfo**) { return FALSE; }
int getnameinfo(const struct sockaddr*,socklen_t,char*,DWORD,char*,DWORD,int) { return FALSE; }
#else
/* FIXME: Need WS protocol-independent API helpers.  */
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
        so = CxSocket::create(AF_UNIX, type, 0);
        if(so == INVALID_SOCKET)
            return INVALID_SOCKET;
        if(_bind_(so, (struct sockaddr *)&uaddr, len)) {
            CxSocket::release(so);
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


std::string CxNetwork::getHostName()
{
    std::string sHostName;
#if defined(GM_OS_WIN)
    char buf[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD len = MAX_COMPUTERNAME_LENGTH + 1;
    if (GetComputerNameA(buf, &len))
    {
        sHostName = buf;
    }
#else
    struct utsname buf;
    if (0 != uname(&buf)) {
        // ensure null termination on failure
        *buf.nodename = '\0';
    }
    sHostName = buf.nodename;
#endif

    return sHostName;
}

std::vector<std::string> CxNetwork::getLocalIps()
{
    std::string sHostName = getHostName();
    CxIpAddressExtend localIpAddress(sHostName.c_str());
    std::vector<std::string> rIps;
    for(struct addrinfo *ptr=localIpAddress.getList(); ptr != NULL ;ptr=_nextaddrinfo(ptr))
    {
        rIps.push_back(CxIpAddress::getIp(_getaddrinfo(ptr)));
    }
    return rIps;
}

#ifdef GM_OS_WIN

#define DEF_BUF_SIZE 1024
#define IP_HEADER_SIZE 20
#define ICMP_HEADER_SIZE 12

typedef struct _ICMP_HEADER
{
    BYTE bType;        //
    BYTE bCode;        //
    USHORT nCheckSum;  //
    USHORT nId;        // pid
    USHORT nSequence;  // order
    UINT nTimeStamp;   //
}ICMP_HEADER, *PICMP_HEADER;

USHORT GetCheckSum(LPBYTE lpBuff, DWORD dwSize)
{
    DWORD dwCheckSum = 0;
    USHORT* lpWord = (USHORT*)lpBuff;
    while (dwSize > 1)
    {
        dwCheckSum += *lpWord++;
        dwSize -= 2;
    }
    if (dwSize == 1)
        dwCheckSum += *((LPBYTE)lpBuff);
    dwCheckSum = (dwCheckSum >> 16) + (dwCheckSum & 0XFFFF);
    return (USHORT)(~dwCheckSum);
}

int CxNetwork::ping(const std::string &sIpAddress)
{
    const char* lpDestIP = sIpAddress.c_str();
    SOCKADDR_IN DestSockAddr;
    DestSockAddr.sin_family = AF_INET;
    DestSockAddr.sin_addr.S_un.S_addr = inet_addr(lpDestIP);
    DestSockAddr.sin_port = htons(0);
    char ICMPPack[ICMP_HEADER_SIZE] = { 0 };
    PICMP_HEADER pICMPHeader = (PICMP_HEADER)ICMPPack;
    pICMPHeader->bType = 8;
    pICMPHeader->bCode = 0;
    pICMPHeader->nId = (USHORT)::GetCurrentProcessId();
    pICMPHeader->nCheckSum = 0;
    pICMPHeader->nTimeStamp = 0;

    WORD version = MAKEWORD(2, 2);
    WSADATA wsaData;
    if (WSAStartup(version, &wsaData) != 0)
    {
        printf("WSAStartup error\n");
        return FALSE;
    }
    SOCKET s = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    int nTime = 1000;
    int ret = ::setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char*)&nTime, sizeof(nTime));
    char szRcvBuff[DEF_BUF_SIZE];
    SOCKADDR_IN SourceSockAddr;
    for (int i = 0; i<4; i++)
    {
        pICMPHeader->nCheckSum = 0;
        pICMPHeader->nSequence = i;
        pICMPHeader->nTimeStamp = ::GetTickCount();
        pICMPHeader->nCheckSum = GetCheckSum((LPBYTE)(ICMPPack), ICMP_HEADER_SIZE);
        int nRet = ::sendto(s, ICMPPack, ICMP_HEADER_SIZE, 0, (SOCKADDR*)&DestSockAddr, sizeof(DestSockAddr));
        if (nRet == SOCKET_ERROR)
        {
            printf("send error.\n");
            return FALSE;
        }
        int nLen = sizeof(SOCKADDR_IN);
        if (nRet == SOCKET_ERROR)
        {
            int nError = ::WSAGetLastError();
            printf("Recv Error:%d.\n", nError);
            return FALSE;
        }
        nRet = ::recvfrom(s, szRcvBuff, DEF_BUF_SIZE, 0, (SOCKADDR*)&SourceSockAddr, &nLen);
        if (nRet == SOCKET_ERROR)
        {
            return FALSE;
        }

        PICMP_HEADER pRcvHeader = (PICMP_HEADER)(szRcvBuff + IP_HEADER_SIZE);
        int nTime = ::GetTickCount() - pRcvHeader->nTimeStamp;
//        printf("from target: %s bytes=%d time=%dms\n", inet_ntoa(SourceSockAddr.sin_addr), nRet, nTime);
        printf("from target ip=%s receive-bytes=%d time=%dms\n", inet_ntoa(SourceSockAddr.sin_addr), nRet, nTime);
        ::Sleep(1000);
    }
    closesocket(s);
    WSACleanup();
    return TRUE;
}

#else

int CxNetwork::ping(const std::string &sIpAddress)
{

}

#endif
