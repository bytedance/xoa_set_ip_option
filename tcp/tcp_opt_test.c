#include "tcp_opt_test.h"
#include "ip_opt.h"
#include <string.h>
#include <arpa/inet.h>


int generate_tcp_opt_v4(struct sockaddr* saddr, struct sockaddr* daddr)
{
    struct sockaddr_in* sa = (struct sockaddr_in*)saddr;
    struct sockaddr_in* da = (struct sockaddr_in*)daddr;

    memset(sa, 0, sizeof(sa));
    memset(da, 0, sizeof(da));
    sa->sin_family = da->sin_family = AF_INET;
    sa->sin_port = htons(101);
    da->sin_port = htons(102);
    inet_pton(AF_INET, "172.0.0.1", &sa->sin_addr);
    inet_pton(AF_INET, "172.0.0.2", &da->sin_addr);

    return 0;
}

int generate_tcp_opt_v6(struct sockaddr* saddr, struct sockaddr* daddr)
{
    struct sockaddr_in6* sa = (struct sockaddr_in6*)saddr;
    struct sockaddr_in6* da = (struct sockaddr_in6*)daddr;

    memset(sa, 0, sizeof(sa));
    memset(da, 0, sizeof(da));
    sa->sin6_family = da->sin6_family = AF_INET6;
    sa->sin6_port = htons(101);
    da->sin6_port = htons(102);
    inet_pton(AF_INET6, "fdbd::1", &sa->sin6_addr);
    inet_pton(AF_INET6, "fdbd::2", &da->sin6_addr);

    return 0;
}