#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#include "ip_opt.h"


struct sockaddr_in6 server_addr;
int buf_size = 8;
int request_num = 100;


//const char remote_ip[] ="::1";



void* client(void* arg)
{
    int fd = *(int*)arg;
    char buf[buf_size];

    struct sockaddr_in6 recv_server_addr;
    socklen_t socklen = sizeof(struct sockaddr_in6);
    const int addr_buf_len = 100;
    char addr_buf[addr_buf_len];

    for (int i = 0; i < request_num; i++)
    {
        if  (i == 0)
        {
            struct sockaddr_in6 saddr;
            struct sockaddr_in6 daddr;
            memset(&saddr, 0, sizeof(saddr));
            memset(&daddr, 0, sizeof(daddr));
            saddr.sin6_family = daddr.sin6_family = AF_INET6;
            saddr.sin6_port = htons(101);
            daddr.sin6_port = htons(102);
            inet_pton(AF_INET6, "fdbd::1", &saddr.sin6_addr);
            inet_pton(AF_INET6, "fdbd::2", &daddr.sin6_addr);

            if  (set_ip_opt(fd, AF_INET6, (struct sockaddr*)&saddr, (struct sockaddr*)&daddr) == 0)
                printf("set_ip_opt success\n");
            else
                printf("set_ip_opt failed\n");
        }

        int send_len = sendto(fd, buf, buf_size, 0, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in6));
        printf("client send %d bytes to %s:%d\n", send_len, 
                inet_ntop(AF_INET6, &server_addr.sin6_addr, addr_buf, addr_buf_len), ntohs(server_addr.sin6_port));

        if  (i == 0)
        {
            if  (del_ip_opt(fd, AF_INET6) == 0)
                printf("del_ip_opt success\n");
            else
                printf("del_ip_opt failed\n");
        }

        int recv_len = recvfrom(fd, buf, buf_size, 0, (struct sockaddr*)&recv_server_addr, &socklen);
        printf("client recv %d bytes from %s:%d\n", recv_len, 
                inet_ntop(AF_INET6, &recv_server_addr.sin6_addr, addr_buf, addr_buf_len), ntohs(recv_server_addr.sin6_port));    
        
        printf("\n");
        sleep(1);
    }

    close(fd);
    return NULL;
}


int main(int argc, char** argv)
{
    if  (!(argc > 2))  
    {   printf("usage: ./client <dst ip> <dst port> [<requests> [<buf_size>]]\n");
        return 0;
    }

    server_addr.sin6_family = AF_INET6;
    inet_pton(AF_INET6, argv[1], &server_addr.sin6_addr);

    int port_;
    if  (sscanf(argv[2], "%d", &port_) == -1)  perror("bad port");
    // port = (short)port_;
    server_addr.sin6_port = htons((short)port_);

    if  (argc > 3)  sscanf(argv[3], "%d", &request_num);

    if  (argc > 4)  sscanf(argv[4], "%d", &buf_size);

    
    int fd = -1;
    if  ((fd = socket(AF_INET6, SOCK_DGRAM, 0)) == -1)  perror("socket error");



    
    client(&fd);


    return 0;
}