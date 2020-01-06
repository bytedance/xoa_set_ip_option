#include "util.h"
#include <cstdio>
#include <sys/socket.h>

ssize_t send_full(int fd, const char* msg, size_t len, int flags) 
{
    size_t remaining = len;
    const char* cur = msg;
    ssize_t sent;

    while (remaining > 0) {
        if  ((sent = send(fd, cur, remaining, flags)) == -1)  perror("send");
        cur += sent;
        remaining -= sent;
    }

    return (len - remaining);
}

ssize_t recv_full(int fd, char* msg, size_t len, int flags) {
    size_t remaining = len;
    char* cur = msg;
    ssize_t recvd;

    while (remaining > 0) {
        recvd = recv(fd, cur, remaining, flags);
        if ((recvd == -1) || (recvd == 0)) break;
        cur += recvd;
        remaining -= recvd;
    }

    return (len - remaining);
}