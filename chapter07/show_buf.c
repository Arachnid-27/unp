#include "unp.h"

int main(int argc, char *argv[])
{
    int sockfd, sz;
    socklen_t len;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        err_sys("socket");
    }

    len = sizeof(int);

    if (getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &sz, &len) == -1) {
        err_sys("getsockopt");
    }

    printf("tcp recv: %d\n", sz);

    if (getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &sz, &len) == -1) {
        err_sys("getsockopt");
    }

    printf("tcp send: %d\n", sz);

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        err_sys("socket");
    }

    len = sizeof(int);

    if (getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &sz, &len) == -1) {
        err_sys("getsockopt");
    }

    printf("udp recv: %d\n", sz);

    if (getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &sz, &len) == -1) {
        err_sys("getsockopt");
    }

    printf("udp send: %d\n", sz);

    exit(0);
}
