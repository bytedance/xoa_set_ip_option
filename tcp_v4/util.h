#ifndef UTIL_H
#define UTIL_H
#include <sys/types.h>

typedef long long ll;


extern ssize_t send_full(int fd, const char* msg, size_t len, int flags);
extern ssize_t recv_full(int fd, char* msg, size_t len, int flags);

#endif