#ifndef CXSOCKET_H
#define CXSOCKET_H

#include "cxglobal.h"

#include "cxstring.h"
#include "cxthread.h"

extern "C" {
    struct addrinfo;
}

#ifdef  GM_OS_WIN
#define SHUT_RDWR   SD_BOTH
#define SHUT_WR     SD_SEND
#define SHUT_RD     SD_RECV
#else
#include <unistd.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netdb.h>
#endif

#include <errno.h>
#include <stdio.h>

#ifndef IPTOS_LOWDELAY
#define IPTOS_LOWDELAY      0x10
#define IPTOS_THROUGHPUT    0x08
#define IPTOS_RELIABILITY   0x04
#define IPTOS_MINCOST       0x02
#endif

#ifdef  AF_UNSPEC
#define DEFAULT_FAMILY  AF_UNSPEC
#else
#define DEFAULT_FAMILY  AF_INET
#endif

#if defined(AF_INET6) || defined(__CYGWIN__)
/**
 * An object that can hold a ipv4 or ipv6 socket address.  This would be
 * used for tcpip socket connections.  We do not use sockaddr_storage
 * because it is not present in pre ipv6 stacks, and because the storage
 * size also includes the size of the path of a unix domain socket on
 * posix systems.
 */
struct sockaddr_internet
{
    union {
#ifdef  AF_INET6
        struct sockaddr_in6 ipv6;
#endif
        struct sockaddr_in ipv4;
        struct sockaddr address;
    };
};
#else
struct sockaddr_internet
{
    union {
        struct sockaddr_in ipv4;
        struct sockaddr address;
    };
};

struct sockaddr_storage
{
#ifdef  AF_UNIX
    char sa_data[128];
#else
    char sa_data[sizeof(struct sockaddr_in)];
#endif
};
#endif

#ifndef SOCK_DCCP
#define SOCK_DCCP       6
#endif

#ifndef IPPROTO_DCCP
#define IPPROTO_DCCP    23
#endif

#ifndef SOL_DCCP
#define SOL_DCCP        269
#endif

#define DCCP_SOCKOPT_AVAILABLE_CCIDS    12
#define DCCP_SOCKOPT_CCID               13
#define DCCP_SOCKOPT_TX_CCID            14
#define DCCP_SOCKOPT_RX_CCID            15

#ifndef SOCKET_ERROR
#define SOCKET_ERROR	(-1)
#endif


/**
 * A generic socket base class.  This class can be used directly or as a
 * base class for building network protocol stacks.  This common base tries
 * to handle UDP and TCP sockets, as well as support multicast, IPV4/IPV6
 * addressing, and additional addressing domains (such as Unix domain sockets).
 * @author David Sugar <dyfet@gnutelephony.org>
 */
class GM_CCXX_CORE_API CxSocket
{
public:
    inline static bool isValidSocket(socket_t iSocket) {
        return iSocket > 0 && iSocket != INVALID_SOCKET;
    }

    /**
     * Initialize socket subsystem.
     */
    static void init(void);

    /**
     * Initialize with program name.  Used by socks, for example.
     * @param program name.
     */
    static void init(const char *program);

    /**
     * Set the default socket behavior for v6-v4 mapping.  This also
     * effects v6 address lookup as to whether v4 remapped addresses
     * can be used if no v6 address is found.
     * @param enable true to set mapping.  This is default.
     */
    static void v4mapping(bool enable);

    /**
     * Return error code of last socket operation,
     * @return errno style error code.
     */
    static int error(void);

    /**
     * Simple function to validate that a given IP address string is a "zero"
     * address.  Such address strings are used for example in SIP to indicate
     * "hold" by re-inviting peers to a null address.  Supports IPV4 and
     * IPV6 addresses.
     * @param string address to check.
     * @return true if zero/null address.
     */
    static bool is_null(const char *string);

    /**
     * Simple function to validate that a given IP address string is a numeric
     * address.  This can be used to verify an address is not a "host" name.
     * Supports IPV4 and IPV6 address strings.
     * @param string address to check.
     * @return true if zero/null address.
     */
    static bool is_numeric(const char *string);

    /**
     * Get local address to which the socket is bound.  This is defined here
     * because we may re-define the backend linkage for the socks proxy in
     * the future.
     * @param socket descriptor to examine.
     * @param address storage for local address.
     * @return 0 on success, -1 on failure.
     */
    static int local(socket_t socket, struct sockaddr_storage *addr);

    /**
     * Get remote address to which the socket is connected.  This is defined
     * here because we may re-define the backend linkage for the socks proxy in
     * the future.
     * @param socket descriptor to examine.
     * @param address storage for remote address.
     * @return 0 on success, -1 on failure.
     */
    static int remote(socket_t socket, struct sockaddr_storage *addr);

    /**
     * Get the type of a socket.
     * @param socket descriptor.
     * @return socket type.
     */
    static int type(socket_t socket);

    /**
     * Set segment size and get MTU.
     * @param socket to modify.
     * @param size of segment or zero to not set.
     * @return mtu size of socket.
     */
    static unsigned segsize(socket_t socket, unsigned size = 0);

    /**
     * Set congestion control id.
     * @param socket to modify.
     * @param ccid value to set.
     * @return true if success, false if not dccp or not supported ccid used.
     */
    static bool ccid(socket_t socket, uint8 id);

    /**
     * Get the number of bytes pending in the receive buffer of a socket
     * descriptor.
     * @param socket descriptor.
     * @return number of pending bytes.
     */
    static unsigned pending(socket_t socket);

    /**
     * receiveTimeout
     * @param so : socket descriptor.
     * @param to : millisecond
     * @return 0 on success, -1 on error.
     */
    static int receiveTimeout(socket_t so, msepoch_t to);

    /**
     * sendTimeout
     * @param so : socket descriptor.
     * @param to : millisecond
     * @return 0 on success, -1 on error.
     */
    static int sendTimeout(socket_t so, msepoch_t to);

    /**
     * Set the send size of a socket descriptor.
     * @param socket descriptor.
     * @param size of send buffer to set.
     * @return 0 on success, -1 on error.
     */
    static int sendsize(socket_t socket, unsigned size);

    /**
     * Set the size to wait before sending.
     * @param socket descriptor.
     * @param size of send wait buffer to set.
     * @return 0 on success, -1 on error.
     */
    static int sendwait(socket_t socket, unsigned size);

    /**
     * Set the receive size of a socket descriptor.
     * @param socket descriptor.
     * @param size of receive buffer to set.
     * @return 0 on success, -1 on error.
     */
    static int recvsize(socket_t socket, unsigned size);

    /**
     * Connect socket descriptor to a remote host from an address list.
     * For TCP (and DCCP) sockets, the entire list may be tried.  For UDP,
     * connect is only a state and the first valid entry in the list is used.
     * @param socket descriptor.
     * @param list of addresses to connect to.
     * @return 0 on success, -1 on error.
     */
    static int connectto(socket_t socket, struct addrinfo *list);

    /**
     * Disconnect a connected socket descriptor.
     * @param socket descriptor.
     * @return 0 on success, -1 on error.
     */
    static int disconnect(socket_t socket);

    /**
     * Drop socket descriptor from multicast group.
     * @param socket descriptor.
     * @param list of groups to drop.
     * @return 0 on success, -1 on error.
     */
    static int drop(socket_t socket, const struct addrinfo *list);

    /**
     * Join socket descriptor to multicast group.
     * @param socket descriptor.
     * @param list of groups to join.
     * @return 0 on success, -1 on error.
     */
    static int join(socket_t socket, const struct addrinfo *list);

    /**
     * Get socket error code of socket descriptor.
     * @param socket descriptor.
     * @return socket error code.
     */
    static int error(socket_t socket);

    /**
     * Set multicast mode and multicast broadcast range for socket descriptor.
     * @param socket descriptor.
     * @param ttl to set for multicast socket or 0 to disable multicast.
     * @return 0 if success, -1 if error.
     */
    static int multicast(socket_t socket, unsigned ttl = 1);

    /**
     * Set loopback to read multicast packets socket descriptor broadcasts.
     * @param socket descriptor.
     * @param enable true to loopback, false to ignore.
     * @return 0 if success, -1 if error.
     */
    static int loopback(socket_t socket, bool enable);

    /**
     * Set socket blocking I/O mode of socket descriptor.
     * @param socket descriptor.
     * @param enable true for blocking I/O.
     * @return 0 if success, -1 if error.
     */
    static int blocking(socket_t socket, bool enable);

    /**
     * Set socket for keepalive packets for socket descriptor.
     * @param socket descriptor.
     * @param enable keep-alive if true.
     * @return 0 if success, -1 if error.
     */
    static int keepalive(socket_t socket, bool enable);

    /**
     * Set socket for unicast mode broadcasts on socket descriptor.
     * @param socket descriptor.
     * @param enable broadcasting if true.
     * @return 0 if success, -1 if error.
     */
    static int broadcast(socket_t socket, bool enable);

    /**
     * Set tcp nodelay option on socket descriptor.
     * @param socket descriptor.
     * @return 0 if success, -1 if error.
     */
    static int nodelay(socket_t socket);

    /**
     * Set packet priority of socket descriptor.
     * @param socket descriptor.
     * @param scheduling priority for packet scheduling.
     * @return 0 on success, -1 on error.
     */
    static int priority(socket_t socket, int scheduling);

    /**
     * Set type of service of socket descriptor.
     * @param socket descriptor.
     * @param type of service.
     * @return 0 on success, -1 on error.
     */
    static int tos(socket_t socket, int type);

    /**
     * Set the time to live for the socket descriptor.
     * @param socket descriptor.
     * @param time to live to set.
     * @return 0 on success, -1 on error.
     */
    static int ttl(socket_t socket, unsigned char time);

    /**
     * Get the address family of the socket descriptor.
     * @return address family.
     */
    static int family(socket_t socket);

    /**
     * Get the address family of a socket address object.
     * @param address to examine.
     * @return address family.
     */
    inline static int family(const struct sockaddr_storage& address)
        {return ((const struct sockaddr *)&address)->sa_family;}

    /**
     * Get the address family of an internet socket address object.
     * @param address to examine.
     * @return address family.
     */
    inline static int family(const struct sockaddr_internet& address)
        {return address.address.sa_family;}

    /**
     * Get data waiting in receive queue.
     * @param socket to get from.
     * @param buffer to save.
     * @param size of data buffer to request.
     * @param flags for i/o operation (MSG_OOB, MSG_PEEK, etc).
     * @param address of source.
     * @return number of bytes received, -1 if error.
     */
    static ssize_t recvfrom(socket_t socket, void *buffer, size_t size, int flags = 0, struct sockaddr_storage *addr = NULL);

    /**
     * Send data on socket.
     * @param socket to send to.
     * @param buffer to send.
     * @param size of data buffer to send.
     * @param flags for i/o operation (MSG_OOB, MSG_PEEK, etc).
     * @param address of destination, NULL if connected.
     * @return number of bytes sent, -1 if error.
     */
    static ssize_t sendto(socket_t socket, const void *buffer, size_t size, int flags = 0, const struct sockaddr *addr = NULL);

    /**
     * Send reply on socket.  Used to reply to a recvfrom message.
     * @param socket to send to.
     * @param buffer to send.
     * @param size of data buffer to send.
     * @param flags for i/o operation (MSG_OOB, MSG_PEEK, etc).
     * @param address to reply to.
     * @return number of bytes sent, -1 if error.
     */
    inline static ssize_t replyto(socket_t socket, const void *buffer, size_t size, int flags, const struct sockaddr_storage *address)
        {return sendto(socket, buffer, size, flags, (const struct sockaddr *)address);}

    /**
     * Send to internet socket.
     * @param socket to send to.
     * @param buffer to send.
     * @param size of data buffer to send.
     * @param flags for i/o operation (MSG_OOB, MSG_PEEK, etc).
     * @param address to send to.
     * @return number of bytes sent, -1 if error.
     */
    inline static ssize_t sendinet(socket_t socket, const void *buffer, size_t size, int flags, const struct sockaddr_internet *address)
        {return sendto(socket, buffer, size, flags, (const struct sockaddr *)address);}

    /**
     * Get internet data waiting in receive queue.
     * @param socket to get from.
     * @param buffer to save.
     * @param size of data buffer to request.
     * @param flags for i/o operation (MSG_OOB, MSG_PEEK, etc).
     * @param address of source.
     * @return number of bytes received, -1 if error.
     */
    static ssize_t recvinet(socket_t socket, void *buffer, size_t size, int flags = 0, struct sockaddr_internet *addr = NULL);

    /**
     * Bind the socket descriptor to a known interface.
     * @param socket descriptor to bind.
     * @param address of interface to bind to.
     * @return 0 on success, -1 if error.
     */
    static int bindto(socket_t socket, const struct sockaddr *addr);

    /**
     * Bind the socket descriptor to a known interface listen on service port.
     * @param socket descriptor to bind.
     * @param address of interface to bind to.
     * @param backlog for service.
     * @return 0 on success, -1 if error.
     */
    static int listento(socket_t socket, int backlog = 5);

    /**
     * Connect the socket to remote.
     * @param socket descriptor to connect.
     * @param address of interface to connect to.
     * @return 0 on success, -1 if error.
     */
    static int connectto(socket_t socket, const struct sockaddr *addr);

    /**
     * Accept a socket connection from a remote host.
     * @param socket descriptor to accept from.
     * @param address of socket accepting.
     * @return new socket accepted.
     */
    static socket_t acceptfrom(socket_t socket, struct sockaddr_storage *addr = NULL);

    /**
     * Create a socket object unbound.
     * @param family of socket.
     * @param type of socket.
     * @param protocol of socket.
     * @return socket descriptor created or INVALID_SOCKET.
     */
    static socket_t create(int family, int type, int protocol);

    /**
     * Create a connected socket.
     * @param address list to connect to.
     * @param type of socket to create.
     * @param protocol of socket.
     * @return socket descriptor created or INVALID_SOCKET.
     */
    static socket_t create(const struct addrinfo *addr, int type, int protocol);

    /**
     * Release (close) a socket.
     * @param socket to close.
     */
    static void release(socket_t socket);

    /**
     * Create an address info lookup hint based on the family and type
     * properties of a socket descriptor.
     * @param socket descriptor.
     * @param hint buffer.
     * @return hint buffer.
     */
    static struct addrinfo *hinting(socket_t socket, struct addrinfo *hint);

    /**
     * Get the size of a socket address.
     * @param address of socket.
     * @return size to use for this socket address object.
     */
    static socklen_t len(const struct sockaddr *addr);

    /**
     * Compare socket addresses.  Test if the address and service matches
     * or if there is no service, then just the host address values.
     * @param address1 to compare.
     * @param address2 to compare.
     * @return true if same family and equal.
     */
    static bool equal(const struct sockaddr *address1, const struct sockaddr *address2);

    /**
     * Copy a socket address.
     * @param target address pointer to copy into.
     * @param origin address pointer to copy from.
     * @return number of bytes copied, 0 if invalid.
     */
    static unsigned copy(struct sockaddr *target, const struct sockaddr *origin);

    /**
     * Store an address into an address object.
     * @param storage for address.
     * @param address to store.
     * @return number of bytes stored.
     */
    inline static unsigned store(struct sockaddr_storage *storage, const struct sockaddr *address)
        {return copy((struct sockaddr*)storage, address);}

    /**
     * Store an address into an internet address object.
     * @param storage for address.
     * @param address to store.
     * @return number of bytes stored.
     */
    static unsigned store(struct sockaddr_internet *storage, const struct sockaddr *addr);

    /**
     * Compare socket host addresses.  Test if the host address matches
     * or if there is no service, then just the host address values.
     * @param address1 to compare.
     * @param address2 to compare.
     * @return true if same family and equal.
     */
    static bool eq_host(const struct sockaddr *address1, const struct sockaddr *address2);

    /**
     * Compare socket addresses.  Test if the stored addresses received match.
     * or if there is no service, then just the host address values.
     * @param address1 to compare.
     * @param address2 to compare.
     * @return true if same family and equal.
     */
    inline static bool eq_from(const struct sockaddr_storage *address1, const struct sockaddr_storage *address2)
        {return equal((const struct sockaddr *)address1, (const struct sockaddr *)address2);}

    /**
     * Compare socket addresses.  Test if the internet addresses received match.
     * or if there is no service, then just the host address values.
     * @param address1 to compare.
     * @param address2 to compare.
     * @return true if same family and equal.
     */
    inline static bool eq_inet(const struct sockaddr_internet *address1, const struct sockaddr_internet *address2)
        {return equal((const struct sockaddr *)address1, (const struct sockaddr *)address2);}

    /**
     * See if both addresses are in the same subnet.  This is only relevant
     * to IPV4 and class domain routing.
     * @param address1 to test.
     * @param address2 to test.
     * @return true if in same subnet.
     */
    static bool eq_subnet(const struct sockaddr *address1, const struct sockaddr *address2);

    /**
     * Get the socket address of the interface needed to reach a destination
     * address.
     * @param address of interface found.
     * @param destination address.
     * @return 0 on success, -1 on error.
     */
    static int via(struct sockaddr *addr, const struct sockaddr *destination);

    /**
     * Get the hostname of a socket address.
     * @param address to lookup.
     * @param buffer to save hostname in.
     * @param size of hostname buffer.
     * @return buffer if found or NULL if not.
     */
    static char *query(const struct sockaddr *addr, char *buffer, socklen_t size);

    /**
     * Get the service port of a socket.
     * @param address of socket to examine.
     * @return service port number.
     */
    static short service(const struct sockaddr *addr);

    /**
     * Get the service port of an inet socket.
     * @param address of internet socket to examine.
     * @return service port number.
     */
    inline static short service(const struct sockaddr_internet *address)
        {return service((const struct sockaddr *)address);}

    /**
     * Convert a socket address and service into a hash map index.
     * @param address to convert.
     * @param size of map index.
     * @return key index path.
     */
    static unsigned keyindex(const struct sockaddr *addr, unsigned size);

    /**
     * Convert a socket host address into a hash map index.
     * @param address to convert.
     * @param size of map index.
     * @return key index path.
     */
    static unsigned keyhost(const struct sockaddr *addr, unsigned size);

    /**
     * cancel pending i/o by shutting down the socket.
     * @param socket to shutdown.
     */
    static void cancel(socket_t socket);

    /**
     * Test for pending input data.  This function can wait up to a specified
     * timeout for data to appear.
     * @param socket to test.
     * @param timeout or 0 if none.
     * @return true if input data waiting.
     */
    static bool wait(socket_t socket, timems_t timeout = 0);

    /**
     * Read a newline of text data from the socket and save in NULL terminated
     * string.  This uses an optimized I/O method that takes advantage of
     * socket peeking.  As such, it has to be rewritten to be used in a ssl
     * layer socket.
     * @param socket to read from.
     * @param data to save input line.
     * @param size of input line buffer.
     * @param timeout to wait for a complete input line.
     * @return number of bytes read, 0 if none, -1 if error.
     */
    static ssize_t readline(socket_t socket, char *data, size_t size, timems_t timeout = CxGlobal::inf);

    /**
     * Print formatted string to socket.
     * @param socket to write to.
     * @param format string.
     * @return number of bytes sent, -1 if error.
     */
    static ssize_t printf(socket_t socket, const char *format, ...) __PRINTF(2,3);

protected:
    socket_t so;
    int ioerr;
    timems_t iowait;

public:
    /**
     * Create a socket object for use.
     */
    CxSocket();

    /**
     * Create socket from existing socket descriptor.
     * @param socket descriptor to use.
     */
    CxSocket(socket_t socket);

    /**
     * Create and connect a socket to an address from an address list.  The
     * type of socket created is based on the type we are connecting to.
     * @param address list to connect with.
     */
    CxSocket(struct addrinfo *addr);

    /**
     * Create an unbound socket of a specific type.
     * @param family of our new socket.
     * @param type (stream, udp, etc) of our new socket.
     * @param protocol number of our new socket.'
     */
    CxSocket(int family, int type, int protocol = 0);

    /**
     * Create socket as duped handle of existing socket.
     * @param existing socket to dup.
     */
    CxSocket(const CxSocket& existing);

    /**
     * Shutdown, close, and destroy socket.
     */
    virtual ~CxSocket();

    /**
     * cancel pending i/o by shutting down the socket.
     */
    void cancel(void);

    /**
     * Shutdown and close the socket.
     */
    void release(void);

    /**
     * Get error code.
     */
    inline int err(void) const
        {return ioerr;}

    /**
     * See the number of bytes in the receive queue.
     * @param value to test for.
     * @return true if at least that many bytes waiting in receive queue.
     */
    bool is_pending(unsigned value) const;

    /**
     * Test if socket is connected.
     * @return true if connected.
     */
    bool connected(void) const;

    /**
     * Test for pending input data.  This function can wait up to a specified
     * timeout for data to appear.
     * @param timeout or 0 if none.
     * @return true if input data waiting.
     */
    bool wait(timems_t timeout = 0) const;

    /**
     * Set nodelay option for tcp socket.
     * @return 0 if successful, -1 on error.
     */
    inline int nodelay(void) const
        {return nodelay(so);}

    /**
     * Test for output data sent.  This function can wait up to a specified
     * timeout for data to appear sent.
     * @param timeout or 0 if none.
     * @return false if cannot send more output/out of buffer space.
     */
    bool waitSending(timems_t timeout = 0) const;

    /**
     * Get the number of bytes of data in the socket receive buffer.
     * @return bytes pending.
     */
    inline unsigned pending(void) const
        {return pending(so);}

    /**
     * Set socket for unicast mode broadcasts.
     * @param enable broadcasting if true.
     * @return 0 on success, -1 if error.
     */
    inline int broadcast(bool enable)
        {return broadcast(so, enable);}

    /**
     * Set socket for keepalive packets.
     * @param enable keep-alive if true.
     * @return 0 on success, -1 if error.
     */
    inline int keepalive(bool enable)
        {return keepalive(so, enable);}

    /**
     * Set socket blocking I/O mode.
     * @param enable true for blocking I/O.
     * @return 0 on success, -1 if error.
     */
    inline int blocking(bool enable)
        {return blocking(so, enable);}

    /**
     * Set multicast mode and multicast broadcast range.
     * @param ttl to set for multicast socket or 0 to disable multicast.
     * @return 0 on success, -1 if error.
     */
    inline int multicast(unsigned ttl = 1)
        {return multicast(so, ttl);}

    /**
     * Set loopback to read multicast packets we broadcast.
     * @param enable true to loopback, false to ignore.
     * @return 0 on success, -1 if error.
     */
    inline int loopback(bool enable)
        {return loopback(so, enable);}

    /**
     * Get socket error code.
     * @return socket error code.
     */
    inline int getError(void)
        {return error(so);}

    /**
     * Set the time to live before packets expire.
     * @param time to live to set.
     * @return 0 on success, -1 on error.
     */
    inline int ttl(unsigned char time)
        {return ttl(so, time);}

    /**
     * Set the size of the socket send buffer.
     * @param size of send buffer to set.
     * @return 0 on success, -1 on error.
     */
    inline int sendsize(unsigned size)
        {return sendsize(so, size);}

    /**
     * Set the size to wait before sending.
     * @param size of send wait buffer to set.
     * @return 0 on success, -1 on error.
     */
    inline int sendwait(unsigned size)
        {return sendwait(so, size);}


    /**
     * Set the size of the socket receive buffer.
     * @param size of recv buffer to set.
     * @return 0 on success, -1 on error.
     */
    inline int recvsize(unsigned size)
        {return recvsize(so, size);}

    /**
     * Get the type of a socket.
     * @return socket type.
     */
    inline int type(void)
        {return type(so);}

    /**
     * Set segment size and get mtu of a socket.
     * @param size of segment or 0 to leave unchanged.
     * @return mtu size.
     */
    inline unsigned segsize(unsigned size)
        {return segsize(so, size);}

    /**
     * Set ccid of dccp socket.
     * @param ccid to set.
     * @return true if success, false if not dccp or not supported ccid used.
     */
    inline bool ccid(uint8 id)
        {return ccid(so, id);}

    /**
     * Set the type of service field of outgoing packets.  Some useful
     * values include IPTOS_LOWDELAY to minimize delay for interactive
     * traffic, IPTOS_THROUGHPUT to optimize throughput, OPTOS_RELIABILITY
     * to optimize for reliability, and IPTOS_MINCOST for low speed use.
     * @param type of service value.
     * @return 0 on success or -1 on error.
     */
    inline int tos(int type)
        {return tos(so, type);}

    /**
     * Set packet priority, 0 to 6 unless privileged.  Should be set before
     * type-of-service.
     * @param scheduling priority for packet scheduling.
     * @return 0 on success, -1 on error.
     */
    inline int priority(int scheduling)
        {return priority(so, scheduling);}

    /**
     * Shutdown the socket communication channel.
     */
    inline void shutdown(void)
        {::shutdown(so, SHUT_RDWR);}

    /**
     * Connect our socket to a remote host from an address list.
     * For TCP (and DCCP) sockets, the entire list may be tried.  For UDP,
     * connect is only a state and the first valid entry in the list is used.
     * @param list of addresses to connect to.
     * @return 0 on success or error.
     */
    int connectto(struct addrinfo *list);

    /**
     * Disconnect a connected socket.  Depending on the implementation, this
     * might be done by connecting to AF_UNSPEC, connecting to a 0 address,
     * or connecting to self.
     * @return 0 on success or error.
     */
    int disconnect(void);

    /**
     * Join socket to multicast group.
     * @param list of groups to join.
     * @return 0 on success, -1 on error.
     */
    int join(const struct addrinfo *list);

    /**
     * Drop socket from multicast group.
     * @param list of groups to drop.
     * @return 0 on success, -1 on error.
     */
    int drop(const struct addrinfo *list);

    /**
     * Socket i/o timer setting.
     * @param timeout to wait, inf for blocking, 0 pure non-blocking.
     * @return 0 on success or error code.
     */
    int wait(timems_t timeout = CxGlobal::inf);

    /**
     * Peek at data waiting in the socket receive buffer.
     * @param data pointer to save data in.
     * @param number of bytes to peek.
     * @return number of bytes actually read, or 0 if no data waiting.
     */
    size_t peek(void *data, size_t number) const;

    /**
     * Read data from the socket receive buffer.  This will be used in abi 4.
     * @param data pointer to save data in.
     * @param number of bytes to read.
     * @param address of peer data was received from.
     * @return number of bytes actually read, 0 if none, -1 if error.
     */
    size_t readfrom(void *data, size_t number, struct sockaddr_storage *addr = NULL);

    /**
     * Write data to the socket send buffer.  This will be used in abi 4.
     * @param data pointer to write data from.
     * @param number of bytes to write.
     * @param address of peer to send data to if not connected.
     * @return number of bytes actually sent, 0 if none, -1 if error.
     */
    size_t writeto(const void *data, size_t number, const struct sockaddr *addr = NULL);

    /**
     * Read a newline of text data from the socket and save in NULL terminated
     * string.  This uses an optimized I/O method that takes advantage of
     * socket peeking.  This presumes a connected socket on a streamble
     * protocol.  Because the trailing newline is dropped, the return size
     * may be greater than the string length.  If there was no data read
     * because of eof of data, an error has occured, or timeout without
     * input, then 0 will be returned.
     * @param data to save input line.
     * @param size of input line buffer.
     * @return number of bytes read, 0 if none, err() has error.
     */
    size_t readline(char *data, size_t size);

    /**
     * Print formatted string to socket.
     * @param format string.
     * @return number of bytes sent.
     */
    size_t printf(const char *format, ...) __PRINTF(2,3);

    /**
     * Write a null terminated string to the socket.  This exists because
     * we messed up consistency with the original puts() method.  In the
     * future there will be a single puts() that has a NULL default.
     * @param string to write.
     * @return number of bytes sent, 0 if none, -1 if error.
     */
    size_t writes(const char *string);

    /**
     * Test if socket is valid.
     * @return true if valid socket.
     */
    operator bool();

    /**
     * Test if socket is invalid.
     * @return true if socket is invalid.
     */
    bool operator!() const;

    /**
     * Assign socket from a socket descriptor.  Release existing socket if
     * one present.
     * @param socket descriptor to assign to object.
     */
    CxSocket& operator=(socket_t socket);

    /**
     * Get the socket descriptor by casting.
     * @return socket descriptor of object.
     */
    inline operator socket_t() const
        {return so;}

    /**
     * Get the socket descriptor by pointer reference.
     * @return socket descriptor of object.
     */
    inline socket_t operator*() const
        {return so;}

};

#define CxSockAddrMaxSize 64
class GM_CCXX_CORE_API CxIpAddress
{
public:
    static std::string getIp(const struct sockaddr * oSockAddr);

    static short getPort(const struct sockaddr * oSockAddr);

    static void extractIpPort(const struct sockaddr * oSockAddr, std::string & sIp, ushort & iPort);

    //return : invalid = INADDR_NONE
    static uint32 ipV4ToInt32(const std::string & sIp);

    static std::string ipV4ToString(uint32 iIp);

    static bool isValidIpV4(const std::string & sIp);

    static bool isIpV6(const std::string & sIp);

    static bool isIpV6(const struct sockaddr * oSockAddr);

    static bool isLocalIp(const std::string & sIp);

public:
    CxIpAddress() :
        _sockAddr(CxSockAddrMaxSize, 0), _ip(), _port(0),
        _family(0), _isBind(false), _isValid(false)
    {
    }

    CxIpAddress(const std::string & sIp, ushort iPort, bool isBind = false) :
        _sockAddr(CxSockAddrMaxSize, 0), _ip(sIp.c_str(), sIp.size()), _port(iPort),
        _family(0), _isBind(isBind), _isValid(false)
    {
        init(sIp, iPort, isBind);
    }

    CxIpAddress(const struct sockaddr * oSockAddr) :
        _sockAddr(CxSockAddrMaxSize, 0), _ip(), _port(0),
        _family(0), _isBind(false), _isValid(false)
    {
        init(oSockAddr);
    }

    inline CxIpAddress(const CxIpAddress &o) :
        _sockAddr(o._sockAddr), _ip(o._ip.c_str(), o._ip.size()), _port(o._port),
        _family(o._family), _isBind(o._isBind), _isValid(o._isValid)
    {
    }

    inline CxIpAddress &operator=(const CxIpAddress &other)
    {
        if (this != &other)
        {
            _sockAddr=other._sockAddr; _ip=std::string(other._ip.c_str(), other._ip.size()); _port=other._port;
            _family=other._family; _isBind=other._isBind; _isValid=other._isValid;
        }
        return *this;
    }

    void setIp(const std::string & value);

    void setPort(ushort iPort);

    void setIsBind(bool value);

    void init(const std::string & sIp, ushort iPort, bool isBind = false);

    void init(const struct sockaddr * oSockAddr);

    inline const std::string & ip() const { return _ip; }

    inline ushort port() const { return _port; }

    inline std::string getIpPort() const { return CxString::format("%s:%d",_ip.c_str(),_port); }

    inline ushort family() const { return _family; }

    inline bool isBind() const { return _isBind; }

    inline bool isV6() const {
#ifdef  AF_INET6
        return _family == AF_INET6;
#endif
        return false;
    }

    inline bool isValid() const { return _isValid;}

    inline const struct sockaddr * getSockAddr() const {
        return (struct sockaddr *)(&_sockAddr.front())/*_sockAddr.data()*/;
    }

    inline int getSockAddrSize() const {
        if (isV6())
        {
#ifdef  AF_INET6
            return sizeof(sockaddr_in6);
#endif
            return 0;
        }
        else
        {
            return sizeof(sockaddr_in);
        }
    }

    bool isSameIpAddress(const struct sockaddr * oSockAddr) const;

    bool isSameIpAddress(const std::string & sIp, ushort iPort) const;

    inline bool isLocalIpAddress() const { return isLocalIp(_ip); }

private:
    void updateSockAddr();

    void updateIpPort();

private:
    std::vector<char> _sockAddr;
    std::string _ip;
    ushort _port;
    ushort _family;
    bool _isBind;
    bool _isValid;

};





/**
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */


/**
 * A generic socket address class.  This class uses the addrinfo list
 * to store socket multiple addresses in a protocol and family
 * independent manner.  Hence, this address class can be used for ipv4
 * and ipv6 sockets, for assigning connections to multiple hosts, etc.
 * The address class will call the resolver when passed host names.
 * @author David Sugar <dyfet@gnutelephony.org>
 */
class GM_CCXX_CORE_API CxIpAddressExtend
{
protected:
    struct addrinfo *list;

public:
    /**
     * Construct a socket address.  This is used to get an address to
     * bind a socket interface to.  The address can be specified as the
     * ip address of the interface or as a "hostname".  If a hostname
     * is used, then family should be specified for clarity.
     * @param family of socket address.  Needed when host names are used.
     * @param address or hostname.
     * @param type of socket (stream, dgram, etc).
     * @param protocol number of socket.
     */
    CxIpAddressExtend(int family, const char *addr, int type = SOCK_STREAM, int protocol = 0);

    /**
     * Construct a socket address for an existing socket.  This can be the
     * name of a host or to perform a lookup in a domain for a service.
     * Family can be used to restrict the set of results returned, however
     * since things like connecto() already filter by family and create
     * will use family from the addrinfo, in most cases AF_UNSPEC can be
     * used.  This may be depreciated in favor of the constructor that
     * matches a set() method.
     * @param family of hosts to filter by or AF_UNSPEC.
     * @param hostname or ip address.  The socket family is used for hostnames.
     * @param service port or name we are referencing or NULL.
     */
    CxIpAddressExtend(int family, const char *hostname, const char *service = NULL);

    /**
     * Construct a socket address list for a service.
     * @param host address for service.
     * @param service name or port number.
     * @param type of service, stream, dgram, etc.
     */
    CxIpAddressExtend(const char *host, const char *service, int type = SOCK_STREAM);

    /**
     * Construct a socket address from host and service.  This is primarily
     * used to construct a list of potential service connections by pure
     * port numbers or for host lookup only.
     * @param hostname or address to use.
     * @param service port or 0.
     */
    CxIpAddressExtend(const char *hostname, unsigned service = 0);

    /**
     * Construct an empty address.
     */
    CxIpAddressExtend();

    /**
     * Copy constructor.
     * @param reference to object to copy from.
     */
    CxIpAddressExtend(const CxIpAddressExtend& reference);

    /**
     * Destroy address.  Deallocate addrinfo structure.
     */
    ~CxIpAddressExtend();

    /**
     * Get the first socket address in our address list.
     * @return first socket address or NULL if none.
     */
    struct sockaddr *get(void) const;

    inline struct sockaddr *getAddr(void) const
        {return get();}

    inline struct sockaddr *operator()(void) const
        {return get();}

    /**
     * Get the first socket address by casted reference.
     * @return first socket address we resolved or NULL if none.
     */
    inline operator struct sockaddr *() const
        {return get();}

    /**
     * Get the first socket address of specified family from our list.
     * @param family to seek.
     * @return first socket address of family or NULL if none.
     */
    struct sockaddr *get(int family) const;

    inline struct sockaddr *operator()(int family) const
        {return get(family);}

    inline operator struct sockaddr_in *() const
        {return (struct sockaddr_in *)get(AF_INET);}

#ifdef  AF_INET6
    inline operator struct sockaddr_in6 *() const
        {return (struct sockaddr_in6 *)get(AF_INET6);}
#endif

    /**
     * Get the family of the first member in a list of services.
     * @return family of first socket address or 0 if none.
     */
    int family(void) const;

    /**
     * Find a specific socket address in our address list.
     * @return matching address from list or NULL if not found.
     */
    struct sockaddr *find(const struct sockaddr *addr) const;

    /**
     * Get the full socket address list from the object.
     * @return addrinfo list we resolved or NULL if none.
     */
    inline struct addrinfo *getList(void) const
        {return list;}

    /**
     * Get the full socket address list by casted reference.
     * @return addrinfo list we resolved or NULL if none.
     */
    inline operator struct addrinfo *() const
        {return list;}

    /**
     * Return the full socket address list by pointer reference.
     * @return addrinfo list we resolved or NULL if none.
     */
    inline struct addrinfo *operator*() const
        {return list;}

    /**
     * Test if the address list is valid.
     * @return true if we have an address list.
     */
    inline operator bool() const
        {return list != NULL;}

    /**
     * Test if we have no address list.
     * @return true if we have no address list.
     */
    inline bool operator!() const
        {return list == NULL;}


    /**
     * Clear current object.
     */
    void clear(void);

    /**
     * Set the host addresses to form a new list.
     * @param hostname or address to resolve.
     * @param service name or port number, or NULL if not used.
     * @param type of socket (stream or dgram) to filter list by.
     */
    void set(const char *hostname, const char *service = NULL, int type = SOCK_STREAM);

    /**
     * Append additional host addresses to our list.
     * @param hostname or address to resolve.
     * @param service name or port number, or NULL if not used.
     * @param type of socket (stream or dgram).
     */
    void add(const char *hostname, const char *service = NULL, int type = SOCK_STREAM);

    /**
     * Set an entry for host binding.
     * @param family of socket address.  Needed when hostnames are used.
     * @param address or hostname.
     * @param type of socket (stream, dgram, etc).
     * @param protocol number of socket.
     */
    void set(int family, const char *addr, int type = SOCK_STREAM, int protocol = 0);

    /**
     * Add an individual socket address to our address list.
     * @param address to add.
     */
    void add(sockaddr *addr);

    /**
     * Insert unique members from another socket address list to ours.
     * @param address list to insert into list.
     * @return count of addresses added.
     */
    unsigned insert(struct addrinfo *addr);

    /**
     * Remove members from another socket address list from ours.
     * @param address list to remove from list.
     * @return count of addresses removed.
     */
    unsigned remove(struct addrinfo *addr);

    /**
     * Remove an individual socket address from our address list.
     * @param address to remove.
     * @return true if found and removed, false if not found.
     */
    bool remove(struct sockaddr *addr);

    /**
     * Insert an individual socket address to our address list only if
     * unique.
     * @param address to insert into list.
     * @return true if inserted, false if duplicate.
     */
    bool insert(struct sockaddr *addr);

    /**
     * Copy an existing addrinfo into our object.  This is also used
     * to support the copy constructor.
     * @param address list to copy from.
     */
    void copy(const struct addrinfo *addr);

    /**
     * Set an individual socket address for our address list.
     * @param address to add.
     */
    void set(struct sockaddr *addr);

    /**
     * Set a socket address from host and service.
     * @param hostname or address to use.
     * @param service port or 0.
     */
    void set(const char *hostname, unsigned service = 0);

    /**
     * Duplicate a socket address.
     * @param address to duplicate.
     * @return duplicate address object.
     */
    static struct sockaddr *dup(struct sockaddr *addr);

    /**
     * Convert address object into ipv4 address.
     * @param address to convert.
     * @return new ipv4 address or NULL if not ipv4.
     */
    static struct sockaddr_in *ipv4(struct sockaddr *addr);

#ifdef  AF_INET6
    /**
     * Convert address object into ipv6 address.
     * @param address to convert.
     * @return new ipv6 address or NULL if not ipv6.
     */
    static struct sockaddr_in6 *ipv6(struct sockaddr *addr);
#endif
};








class GM_CCXX_CORE_API CxSocketExtend : public CxSocket
{
public:
    CxSocketExtend();

    /**
     * Create a bound socket.  If one wishes to listen for connections on
     * a protocol, then ListenSocket should be used instead.
     * @param address to bind or "*" for all.
     * @param port number of service to bind.
     * @param family to bind as.
     * @param type of socket to bind (stream, udp, etc).
     * @param protocol of socket to bind.
     */
    CxSocketExtend(const char *addr, const char *port, int family = AF_UNSPEC, int type = 0, int protocol = 0);

    /**
     * Lookup and return the host name associated with a socket address.
     * @param address to lookup.
     * @param buffer to save hostname into.
     * @param size of buffer to save hostname into.
     * @return buffer or NULL if lookup fails.
     */
    static char *hostname(const struct sockaddr *addr, char *buffer, size_t size);

    /**
     * Get an address list directly.  This is used internally by some derived
     * socket types when generic address lists would be invalid.
     * @param host name in the form address or "address:port"
     * @param service id or port to use if not specified in host string.
     * @param type of service to get.
     * @param protocol of service to get.
     */
    static struct addrinfo *query(const char *host, const char *service, int type = SOCK_STREAM, int protocol = 0);

    /**
     * Lookup a host name and service address based on the addressing family
     * and socket type of a socket descriptor.  Store the result in a socket
     * address structure.
     * @param socket descriptor.
     * @param address that is resolved.
     * @param hostname to resolve.
     * @param service port.
     * @return socket address size.
     */
    static socklen_t query(socket_t socket, struct sockaddr_storage *addr, const char *hostname, const char *service);

    /**
     * Set default socket family preference for query options when the
     * socket type is otherwise not specified.
     * @param family to select.
     */
    static void query(int family);

    /**
     * Bind the socket descriptor to a known interface and service port.
     * @param socket descriptor to bind.
     * @param address to bind to or "*" for all.
     * @param service port to bind.
     * @param protocol to use or 0 if default.
     * @return 0 on success, -1 if error.
     */
    static int bindto(socket_t socket, const char *addr, const char *service, int protocol = 0);

    /**
     * Create a bound socket for a service.
     * @param iface to bind.
     * @param service port to bind.
     * @param family to select or AF_UNSPEC
     * @param type of socket to create.
     * @param protocol of socket to create.
     * @return socket descriptor created or INVALID_SOCKET.
     */
    static socket_t create(const char *iface, const char *service, int family = AF_UNSPEC, int type = 0, int protocol = 0);

    /**
     * Create a connected socket for a service.
     * @param address of service for connect.
     * @return socket descriptor.
     */
    static socket_t create(const CxIpAddressExtend &IpAddress);

    /**
     * Release an address list directly.  This is used internally by some
     * derived socket types which do not use generic address lists.
     * @param list of addresses.
     */
    static void release(struct addrinfo *list);

};










/**
 * A bound socket used to listen for inbound socket connections.  This class
 * is commonly used for TCP and DCCP listener sockets.
 * @author David Sugar <dyfet@gnutelephony.org>
 */
class GM_CCXX_CORE_API CxListenSocket : protected CxSocketExtend
{
public:
    /**
     * Create and bind a listener socket.
     * @param address to bind on or "*" for all.
     * @param service port to bind listener.
     * @param backlog size for buffering pending connections.
     * @param family of socket.
     * @param type of socket.
     * @param protocol for socket if not TCPIP.
     */
    CxListenSocket(const char *addr, const char *service, unsigned backlog = 5, int family = AF_UNSPEC, int type = 0, int protocol = 0);

    /**
     * Create a listen socket directly.
     * @param address to bind on or "*" for all.
     * @param service port to bind listener.
     * @param backlog size for buffering pending connections.
     * @param family of socket.
     * @param type of socket.
     * @param protocol for socket if not TCPIP.
     * @return bound and listened to socket.
     */
    static socket_t create(const char *addr, const char *service, unsigned backlog = 5, int family = AF_UNSPEC, int type = 0, int protocol = 0);

    /**
     * Accept a socket connection.
     * @param address to save peer connecting.
     * @return socket descriptor of connected socket.
     */
    socket_t accept(struct sockaddr_storage *addr = NULL) const;

    /**
     * Wait for a pending connection.
     * @param timeout to wait.
     * @return true when acceptable connection is pending.
     */
    inline bool wait(timems_t timeout = CxGlobal::inf) const
        {return CxSocket::wait(timeout);}

    /**
     * Get the socket descriptor of the listener.
     * @return socket descriptor.
     */
    inline operator socket_t() const
        {return so;}

    /**
     * Get the socket descriptor of the listener by pointer reference.
     * @return socket descriptor.
     */
    inline socket_t operator*() const
        {return so;}

    /**
     * Get the socket descriptor of the listener.
     * @return socket descriptor.
     */
    inline socket_t getsocket(void) const
        {return so;}

    inline socket_t handle(void) const
        {return so;}

};










/**
 * Helper function for linked_pointer<struct sockaddr>.
 */
struct addrinfo * _nextaddrinfo(struct addrinfo * addrinfo);

/**
 * Helper function for linked_pointer<struct sockaddr>.
 */
struct sockaddr * _getaddrinfo(struct addrinfo * addrinfo);

/**
 * Helper function for linked_pointer<struct sockaddr>.
 */
socket_t _getaddrsock(struct addrinfo * addrinfo);

/**
 * A convenience function to convert a socket address list into an addrinfo.
 * @param address list object.
 * @return addrinfo list or NULL if empty.
 */
inline struct addrinfo * addrinfo(CxIpAddressExtend & address)
    {return address.getList();}

/**
 * A convenience function to convert a socket address list into a socket
 * address.
 * @param address list object.
 * @return first socket address in list or NULL if empty.
 */
inline struct sockaddr * addr(CxIpAddressExtend & address)
    {return address.get();}






class GM_CCXX_CORE_API CxNetwork
{
public:
    static std::string getHostName();

    static std::vector<std::string> getLocalIps();

    static int ping(const std::string & sIpAddress);

};









/**
 * Compare two socket addresses to see if equal.  If the port is zero
 * then this is the same as comparing host address alone.
 * @param s1 socket address to compare.
 * @param s2 socket address to compare.
 * @return true if addresses same.
 */
inline bool eq(const struct sockaddr * s1, const struct sockaddr * s2)
    {return CxSocket::equal(s1, s2);}

/**
 * Compare two stored socket addresses to see if equal.  If the port is zero
 * then this is the same as comparing host address alone.
 * @param s1 stored socket address to compare.
 * @param s2 stored socket address to compare.
 * @return true if addresses same.
 */
inline bool eq(const struct sockaddr_storage * s1, const struct sockaddr_storage * s2)
    {return CxSocket::equal((const struct sockaddr *)s1, (const struct sockaddr *)s2);}

/**
 * Compare two host addresses to see if equal.  The port numbers are
 * ignored.
 * @param s1 socket address to compare.
 * @param s2 socket address to compare.
 * @return true if addresses same.
 */
inline bool eq_host(const struct sockaddr * s1, const struct sockaddr * s2)
    {return CxSocket::eq_host(s1, s2);}

inline bool eq_subnet(const struct sockaddr * s1, const struct sockaddr * s2)
    {return CxSocket::eq_subnet(s1, s2);}



/*
 IOCP , kqueue , epoll
 http://libevent.org/
*/

#endif // CXSOCKET_H
