#ifndef TCP_OPT_TEST_H
#define TCP_OPT_TEST_H

#include <netinet/in.h>

extern int generate_tcp_opt_v4(struct sockaddr* saddr, struct sockaddr* daddr);

extern int generate_tcp_opt_v6(struct sockaddr* saddr, struct sockaddr* daddr);


#endif