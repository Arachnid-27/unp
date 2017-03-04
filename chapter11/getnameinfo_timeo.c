#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

int getnameinfo_timeo(const struct sockaddr *sa, socklen_t salen, 
                char *host, socklen_t hostlen, 
                char *serv, socklen_t servlen, 
                int flags, unsigned int seconds) {
    int ret;

    alarm(seconds);

    if ((ret = getnameinfo(sa, salen, host, hostlen, serv, servlen, flags)) == 0) {
        return 0;
    }

    if (flags & NI_NAMEREQD) {
        if (inet_ntop(sa->sa_family, sa, host, hostlen) != NULL) {
            return 0;
        }
    }

    return ret;
}
