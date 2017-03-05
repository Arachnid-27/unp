#include <unistd.h>
#include <netdb.h>
#include <signal.h>
#include <errno.h>
#include <arpa/inet.h>

void empty_handler(int) {}

int getnameinfo_timeo(const struct sockaddr *sa, socklen_t salen, 
                char *host, socklen_t hostlen, 
                char *serv, socklen_t servlen, 
                int flags, unsigned int seconds) {
    struct sigaction old_sa, new_sa;
    int ret;

    sigemptyset(&new_sa.sa_mask);
    new_sa.sa_flags = 0;
    new_sa.sa_handler = empty_handler;
    if (sigaction(SIGALRM, &new_sa, &old_sa) == -1) {
        return errno;
    }
    alarm(seconds);

    if ((ret = getnameinfo(sa, salen, host, hostlen, serv, servlen, flags)) == 0) {
        return 0;
    }

    if (flags & NI_NAMEREQD) {
        if (inet_ntop(sa->sa_family, sa, host, hostlen) != NULL) {
            return 0;
        }
    }

    alarm(0);
    if (sigaction(SIGALRM, &old_sa, NULL) == -1) {
        return errno;
    }

    return ret;
}
