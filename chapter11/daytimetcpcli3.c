#include "unp.h"

int main(int argc, char **argv)
{
    int                 sockfd, n;
    char                recvline[MAXLINE + 1];
    struct addrinfo     *pptr;
    struct addrinfo     *result;
    struct addrinfo     hints;

    if (argc != 3) {
        err_quit("usage: daytimetcpcli2 <hostname> <service>");
    }

    bzero(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(argv[1], argv[2], &hints, &result) != 0) {
        err_quit("getaddrinfo error for %s", hstrerror(h_errno));
    }

    for (pptr = result; pptr != NULL; pptr = pptr->ai_next) {
        sockfd = Socket(pptr->ai_family, pptr->ai_socktype, pptr->ai_protocol);

        printf("trying %s\n", Sock_ntop((SA *) pptr->ai_addr, pptr->ai_addrlen));
        if (connect(sockfd, (SA *) pptr->ai_addr, pptr->ai_addrlen) == 0) {
            break;
        }

        err_ret("connect error");
        close(sockfd);
    }

    if (pptr == NULL) {
        err_quit("unable to connect");
    }

    while ((n = Read(sockfd, recvline, MAXLINE)) > 0) {
        recvline[n] = 0;
        Fputs(recvline, stdout);
    }

    freeaddrinfo(result);

    exit(0);
}
