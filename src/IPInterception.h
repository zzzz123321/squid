/*
 * $Id: IPInterception.h,v 1.7 2007/12/14 23:11:45 amosjeffries Exp $
 *
 * DEBUG: section 89    NAT / IP Interception
 * AUTHOR: Robert Collins
 * AUTHOR: Amos Jeffries
 *
 */
#ifndef SQUID_IPINTERCEPTION_H
#define SQUID_IPINTERCEPTION_H

class IPAddress;

/**
 \defgroup IPInterceptAPI IP Interception and Transparent Proxy API
 \ingroup SquidComponent
 \par
 * There is no formal state-machine for transparency and interception
 * instead there is this neutral API which other connection state machines
 * and the comm layer use to co-ordinate their own state for transparency.
 */
class IPIntercept
{
public:
    IPIntercept() : transparent_active(0), intercept_active(0) {};
    ~IPIntercept() {};

    int NatLookup(int fd, const IPAddress &me, const IPAddress &peer, IPAddress &dst);

#if LINUX_TPROXY2
    // only relevant to TPROXY v2 connections.
    // which require the address be set specifically post-connect.
    int SetTproxy2OutgoingAddr(int fd, const IPAddress &src);
#endif

    /**
     \retval 0	Full transparency is disabled.
     \retval 1  Full transparency is enabled and active.
     */
    inline int TransparentActive() { return transparent_active; };

    /** \par
     * Turn on fully Transparent-Proxy activities.
     * This function should be called during parsing of the squid.conf
     * When any option requiring full-transparency is encountered.
     */
    inline void StartTransparency() { transparent_active=1; };

    /** \par
     * Turn off fully Transparent-Proxy activities on all new connections.
     * Existing transactions and connections are unaffected and will run
     * to their natural completion.
     \param str    Reason for stopping. Will be logged to cache.log
     */
    void StopTransparency(const char *str);

    /**
     \retval 0	IP Interception is disabled.
     \retval 1  IP Interception is enabled and active.
     */
    inline int InterceptActive() { return intercept_active; };

    /** \par
     * Turn on IP-Interception-Proxy activities.
     * This function should be called during parsing of the squid.conf
     * When any option requiring interception / NAT handling is encountered.
     */
    inline void StartInterception() { intercept_active=1; };

    /** \par
     * Turn off IP-Interception-Proxy activities on all new connections.
     * Existing transactions and connections are unaffected and will run
     * to their natural completion.
     \param str    Reason for stopping. Will be logged to cache.log
     */
    inline void StopInterception(const char *str);

private:
    int transparent_active;
    int intercept_active;
};

#if !defined(IP_TRANSPARENT)
/// \ingroup IPInterceptAPI
#define IP_TRANSPARENT 19
#endif

/**
 \ingroup IPInterceptAPI
 * Globally available instance of the IP Interception manager.
 */
extern IPIntercept IPInterceptor;

#endif /* SQUID_IPINTERCEPTION_H */
