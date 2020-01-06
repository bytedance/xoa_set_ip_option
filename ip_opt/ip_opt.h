#ifndef IP_OPT_H
#define IP_OPT_H


int set_ip_opt(int fd, int fd_family, struct sockaddr* saddr, struct sockaddr* daddr);
int del_ip_opt(int fd, int proto);



#endif