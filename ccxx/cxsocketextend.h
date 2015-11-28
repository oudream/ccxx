/*
 * socket addressInfo
*/

#ifndef CXSOCKETEXTEND_H
#define CXSOCKETEXTEND_H



#include "cxsocket.h"



/**
 * A generic socket address class.  This class uses the addrinfo list
 * to store socket multiple addresses in a protocol and family
 * independent manner.  Hence, this address class can be used for ipv4
 * and ipv6 sockets, for assigning connections to multiple hosts, etc.
 * The address class will call the resolver when passed host names.
 * @author David Sugar <dyfet@gnutelephony.org>
 */
class CxIpAddressExtend
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








class CxSocketExtend : public CxSocket
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
class CxListenSocket : protected CxSocketExtend
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








#endif // CXSOCKETEXTEND_H
