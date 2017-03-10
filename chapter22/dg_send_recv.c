#include    "unprtt.h"

static struct rtt_info rttinfo;
static int rttinit = 0;
static struct msghdr msgsend, msgrecv;
static struct hdr {
    uint32_t seq;
    uint32_t ts;
} sendhdr, recvhdr;

ssize_t dg_send_recv(int fd, const void *outbuff, size_t outbytes, 
        void *inbuff, size_t inbytes, const SA *destaddr, socklen_t destlen) {
    ssize_t n;
    fd_set rset;
    struct iovec iovsend[2], iovrecv[2];
    struct timeval tv;

    if (rttinit == 0) {
        rtt_init(&rttinfo);
        rttinit = 1;
        rtt_d_flag = 1;
    }

    sendhdr.seq++;
    msgsend.msg_name = destaddr;
    msgsend.msg_namelen = destlen;
    msgsend.msg_iov = iovsend;
    msgsend.msg_iovlen = 2;
    iovsend[0].iov_base = &sendhdr;
    iovsend[0].iov_len = sizeof(struct hdr);
    iovsend[1].iov_base = outbuff;
    iovsend[1].iov_len = outbytes;

    msgrecv.msg_name = NULL;
    msgrecv.msg_namelen = 0;
    msgrecv.msg_iov = iovrecv;
    msgrecv.msg_iovlen = 2;
    iovrecv[0].iov_base = &recvhdr;
    iovrecv[0].iov_len = sizeof(struct hdr);
    iovrecv[1].iov_base = inbuff;
    iovrecv[1].iov_len = inbytes;

    rtt_newpack(&rttinfo);

sendagain:
    sendhdr.ts = rtt_ts(&rttinfo);
    Sendmsg(fd, &msgsend, 0);

    tv.tv_usec = 0;
    tv.tv_sec = rtt_start(&rttinfo);

    while (1) {
        FD_ZERO(&rset);
        FD_SET(fd, &rset);

        if (select(fd, &rset, NULL, NULL, &tv) <= 0) {
            if (rtt_timeout(&rttinfo) < 0) {
                err_msg("dg_send_recv: no response from server, giving up");
                rttinit = 0;
                errno = ETIMEDOUT;
                return -1;
            }
            goto sendagain;
        } else {
            n = Recvmsg(fd, &msgrecv, 0);
            if (n < sizeof(struct hdr) || recvhdr.seq != sendhdr.seq) {
                continue;
            }
        }
    }

    rtt_stop(&rttinfo, rtt_ts(&rttinfo) - recvhdr.ts);

    return (n - sizeof(struct hdr));
}
