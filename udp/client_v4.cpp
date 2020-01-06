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


// const short port = 8000;
struct sockaddr_in server_addr;
int buf_size = 64;
int request_num = 100;



void* client(void* arg)
{
    int fd = *(int*)arg;
    char buf[buf_size];

    struct sockaddr_in recv_server_addr;
    socklen_t socklen = sizeof(struct sockaddr);

    for (int i = 0; i < request_num; i++)
    {
        if  (i == 0)
        {
            struct sockaddr_in saddr;
            struct sockaddr_in daddr;
            memset(&saddr, 0, sizeof(saddr));
            memset(&daddr, 0, sizeof(daddr));
            saddr.sin_family = daddr.sin_family = AF_INET;
            saddr.sin_port = htons(101);
            daddr.sin_port = htons(102);
            inet_pton(AF_INET, "172.0.0.1", &saddr.sin_addr);
            inet_pton(AF_INET, "172.0.0.2", &daddr.sin_addr);

            if  (set_ip_opt(fd, AF_INET, (struct sockaddr*)&saddr, (struct sockaddr*)&daddr) == 0)
                printf("set_ip_opt success\n");
            else
                printf("set_ip_opt failed\n");
        }

        int send_len = sendto(fd, buf, buf_size, 0, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));
        printf("client send %d bytes to %s:%d\n", 
                send_len, inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));
        
        if  (i == 0)
        {
            if  (del_ip_opt(fd, AF_INET) == 0)
                printf("del_ip_opt success\n");
            else
                printf("del_ip_opt failed\n");
        }

        int recv_len = recvfrom(fd, buf, buf_size, 0, (struct sockaddr*)&recv_server_addr, &socklen);
        printf("client recv %d bytes from %s:%d\n", 
                recv_len, inet_ntoa(recv_server_addr.sin_addr), ntohs(recv_server_addr.sin_port));
        
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

    

    // struct hostent* he;   
    // if  ((he = gethostbyname(argv[1])) == NULL)  perror("gethostbyname error");
    server_addr.sin_family = AF_INET;
    // server_addr.sin_addr = *((struct in_addr*)he->h_addr);
    if  (inet_pton(AF_INET, argv[1], &server_addr.sin_addr.s_addr) <= 0) perror("inet_pton error");
    
    int port_;
    if  (sscanf(argv[2], "%d", &port_) == -1)  perror("bad port");
    server_addr.sin_port = htons((short)port_);

    if  (argc > 3)  sscanf(argv[3], "%d", &request_num);

    if  (argc > 4)  sscanf(argv[4], "%d", &buf_size);

    
    int fd = -1;
    if  ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)  perror("socket error");



    
    client(&fd);


    return 0;
}