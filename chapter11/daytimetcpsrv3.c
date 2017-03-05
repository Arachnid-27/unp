#include "unp.h"
#include <time.h>

int main(int argc, char **argv)
{
    int             listenfd, connfd;
    socklen_t       addrlen, len;
    struct sockaddr *cliaddr;
    struct linger   ling;
    char            buff[MAXLINE];
    time_t          ticks;
    char            host[NI_MAXHOST], serv[NI_MAXSERV];

    if (argc == 2) {
        listenfd = Tcp_listen(NULL, argv[1], &addrlen);
    } else if (argc == 3) {
        listenfd = Tcp_listen(argv[1], argv[2], &addrlen);
    } else {
        err_quit("usage: daytimetcpsrv3 [ <host> ] <service or port>");
    }

    cliaddr = (struct sockaddr *) Malloc(addrlen);

    for ( ; ; ) {
        len = addrlen;
        connfd = Accept(listenfd, cliaddr, &len);

        if (getnameinfo(cliaddr, len, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV) == 0) {
            printf("connection from %s:%s\n", host, serv);
        }

        ling.l_onoff = 1;
        ling.l_linger = 0;
        Setsockopt(connfd, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling));

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        Write(connfd, buff, strlen(buff));

        sleep(2);
        Close(connfd);
    }
}
