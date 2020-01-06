/*
    echo client
    one connection per thread
    synchronization method
*/ 
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>

#include "util.h"

const int max_conn = 1000;
int sock_fds[max_conn];
pthread_t sock_threads[max_conn];

int conn_num = 10;
int request_num = 100;
int buf_size = 64;


void* worker(void* arg)
{
    int fd = *(int*)arg;
    printf("worker on %d\n", fd);

    char send_buf[buf_size];
    char recv_buf[buf_size];

    for (int i = 0; i < request_num; i++)
    {
        printf("i: %d\n", i);
        for (int j = 0; j < buf_size; j++)
            send_buf[i] = fd;
        int send_len = send_full(fd, send_buf, buf_size, 0);

        memset(recv_buf, 0, buf_size);
        int recv_len = recv_full(fd, recv_buf, buf_size, 0);
        
        sleep(1);
    }

    close(fd);
    printf("fd %d closed\n", fd);

    pthread_exit(NULL);
}



int main(int argc, char** argv)
{
    if  (!(argc > 2))  
    {   printf("usage: %s <dst ip> <dst ip> [<connections> [<requests> [<buf_size>]]]\n", argv[0]);
        return 0;
    }

    struct sockaddr_in that_addr;
    that_addr.sin_family = AF_INET;

    if  (inet_pton(AF_INET, argv[1], &that_addr.sin_addr) == -1)  perror("bad addr");

    int port_;
    if  (sscanf(argv[2], "%d", &port_) == -1)  perror("bad port");
    that_addr.sin_port = htons((short)port_);

    
    if  (argc > 3)  sscanf(argv[3], "%d", &conn_num);
    if  (conn_num > max_conn)  perror("too many connections");

    if  (argc > 4)  sscanf(argv[4], "%d", &request_num);
    
    if  (argc > 5)  sscanf(argv[5], "%d", &buf_size);




    for (int i = 0; i < conn_num; i++)
    {
        if  ((sock_fds[i] = socket(AF_INET, SOCK_STREAM, 0)) == -1)  perror("socket error");
        

        if  (connect(sock_fds[i], (struct sockaddr*)&that_addr, sizeof(struct sockaddr)) == -1)  perror("connect error");
        printf("establish connection %d to %s:%d\n", sock_fds[i], 
                inet_ntoa(that_addr.sin_addr), ntohs(that_addr.sin_port));

        if  (pthread_create(&sock_threads[i], NULL, worker, &sock_fds[i]) == -1)  perror("pthread_create error");
    
    }

    for (int i = 0; i < conn_num; i++)
    {
        pthread_join(sock_threads[i], NULL);
    }


    return 0;
}

