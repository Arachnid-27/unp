#include "unp.h"

void str_echo(int sockfd)
{
    char    line[MAXLINE];
    FILE    *fpin, *fpout;

    fpin = Fdopen(sockfd, "r");
    fpout = Fdopen(sockfd, "w");

    if (setvbuf(fpout, NULL, _IOLBF, 0) != 0) {
        err_quit("setvbuf error");
    }

    while (Fgets(line, MAXLINE, fpin) != NULL) {
        Fputs(line, fpout);
    }

    exit(0);
}

void str_echo2(int sockfd)
{
    char    line[MAXLINE];
    FILE    *fpin, *fpout;

    fpin = Fdopen(sockfd, "r");
    fpout = Fdopen(sockfd, "w");

    while (Fgets(line, MAXLINE, fpin) != NULL) {
        Fputs(line, fpout);
        fflush(fpout);
    }

    exit(0);
}
