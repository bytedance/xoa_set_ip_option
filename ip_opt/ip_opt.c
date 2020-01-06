#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/types.h>
#include "ip_opt.h"


union two_addr{
    struct{
        unsigned char saddr[4];
        unsigned char daddr[4];
    }ipv4;
    struct{
        unsigned char saddr[16];
        unsigned char daddr[16];
    }ipv6;
};

struct four_tuple{
    unsigned int type; // indicate this is ipv4 or ipv6 addresses;
    __be16 sport, dport;
    union two_addr addrs;
};

struct ip_option
{
    union {
        struct
        {
            __u8 type;
            __u8 length;
            __u8 operation;
            __u8 padding;
        } ipv4;
        struct
        {
            __u8 nexthdr;
            __u8 hdrlen;
            __u8 option;
            __u8 optlen;
        } ipv6;
    } header;

    __be16 sport, dport;

    union two_addr addrs;
};


#define IPV4_OPTION_TYPE 31
#define IPV6_HEADER_OPTION 31

#define IP_OPTION_IPV4_LEN 16
#define IP_OPTION_IPV6_LEN 40

#define IPV6_HEADER_IPV4_LEN ((IP_OPTION_IPV4_LEN) / 8 - 1)
#define IPV6_HEADER_IPV6_LEN ((IP_OPTION_IPV6_LEN) / 8 - 1)
#define IPV6_HEADER_OPTION_IPV4_LEN (IP_OPTION_IPV4_LEN - 4)
#define IPV6_HEADER_OPTION_IPV6_LEN (IP_OPTION_IPV6_LEN - 4)


int set_ip_opt__(int fd, int proto, struct four_tuple *param)
{
    if  (!param)  return -1;
    struct ip_option *opt = (struct ip_option*)param;
    int len = 0;
    
    if (proto == AF_INET)
    {
            int type = param->type;
            opt->header.ipv4.type = IPV4_OPTION_TYPE;
            opt->header.ipv4.padding = 0;

            if (type == 0)
            {   opt->header.ipv4.length = IP_OPTION_IPV4_LEN;
                opt->header.ipv4.operation = 0;
                len = IP_OPTION_IPV4_LEN;
            }
            else if (type == 1)
            {   opt->header.ipv4.length = IP_OPTION_IPV6_LEN;
                opt->header.ipv4.operation = 1;
                len = IP_OPTION_IPV6_LEN;
            }
            else
                return -1;

            return setsockopt(fd, IPPROTO_IP, IP_OPTIONS, opt, len);
    }
    else if (proto == AF_INET6)
    {
            int type = param->type;
            opt->header.ipv6.nexthdr = IPPROTO_DSTOPTS;
            opt->header.ipv6.option = IPV6_HEADER_OPTION;

            if (type == 0)
            {   opt->header.ipv6.hdrlen = IPV6_HEADER_IPV4_LEN;
                opt->header.ipv6.optlen = IPV6_HEADER_OPTION_IPV4_LEN;
                len = IP_OPTION_IPV4_LEN;
            }
            else if (type == 1)
            {   opt->header.ipv6.hdrlen = IPV6_HEADER_IPV6_LEN;
                opt->header.ipv6.optlen = IPV6_HEADER_OPTION_IPV6_LEN;
                len = IP_OPTION_IPV6_LEN;
            }
            else
                return -1;

            return setsockopt(fd, IPPROTO_IPV6, IPV6_DSTOPTS, opt, len);
    }
    else
        return -1;
}

int set_ip_opt(int fd, int fd_family, struct sockaddr* saddr, struct sockaddr* daddr)
{
    int addr_family = 0;
    struct four_tuple param;

    if  (saddr == NULL || daddr == NULL) return -1;
    if  (saddr->sa_family != daddr->sa_family)  return -1;
    addr_family = saddr->sa_family;

    if  (addr_family == AF_INET)
    {
        struct sockaddr_in* sa = (struct sockaddr_in*)saddr;
        struct sockaddr_in* da = (struct sockaddr_in*)daddr;
        param.type = 0;
        param.sport = sa->sin_port;
        param.dport = da->sin_port;
        memcpy(param.addrs.ipv4.saddr, &sa->sin_addr, 4);
        memcpy(param.addrs.ipv4.daddr, &da->sin_addr, 4);
    }
    else if  (addr_family == AF_INET6)
    {
        struct sockaddr_in6* sa = (struct sockaddr_in6*)saddr;
        struct sockaddr_in6* da = (struct sockaddr_in6*)daddr;
        param.type = 1;
        param.sport = sa->sin6_port;
        param.dport = da->sin6_port;
        memcpy(param.addrs.ipv6.saddr, &sa->sin6_addr, 16);
        memcpy(param.addrs.ipv6.daddr, &da->sin6_addr, 16);
    }
    else
        return -1;
    
    return set_ip_opt__(fd, fd_family, &param);
}

int del_ip_opt(int fd, int proto)
{
    int tmp;

    if  (proto == AF_INET)
    {
        return setsockopt(fd, IPPROTO_IP, IP_OPTIONS, &tmp, 0);
    }
    else if  (proto == AF_INET6)
    {
        return setsockopt(fd, IPPROTO_IPV6, IPV6_DSTOPTS, &tmp, 0);
    }
    else 
        return -1;
}