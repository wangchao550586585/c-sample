#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <errno.h>
#include <strings.h>

#define  SOCKET_ERROR "socket error"
#define  CONNECT_ERROR "connect error"
#define  LISTEN_ERROR "listen error"
#define  BIND_ERROR "bind error"
#define  ACCEPT_ERROR "accept error"
#define  PORT_ERROR "port error"

int check(int ret, char *str) {
    if (ret == -1) {
        perror(str);
        exit(EXIT_FAILURE);
    }
    return ret;
}

int my_socket(int domain, int type, int protocol) {
    return check(socket(domain, type, protocol), SOCKET_ERROR);
}

int default_my_socket_ipv4_tcp() {
    return my_socket(AF_INET, SOCK_STREAM, 0);
}

struct sockaddr_in *create_ipv4_for_localhost(struct sockaddr_in *srv_addr, uint16_t port) {
    //将地址结构清零
//    memset(&srv_addr,0, sizeof(srv_addr));
    bzero(srv_addr, sizeof(srv_addr));
    srv_addr->sin_family = AF_INET;
    srv_addr->sin_port = htons(port);
    srv_addr->sin_addr.s_addr = htonl(INADDR_ANY);
    return srv_addr;
}

int my_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    return check(bind(sockfd, addr, addrlen), BIND_ERROR);
}

int my_listen(int sockfd, int backlog) {
    return check(listen(sockfd, backlog), LISTEN_ERROR);
}

int my_accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
    int i;
    again :
    if ((i = accept(sockfd, addr, addrlen)) < 0) {
        //因为accept是个慢系统调用,发送信号后中断后,重试.
        /**      EINTR:如果进程在一个慢系统调用(slow system call)中阻塞时，
                当捕获到某个信号且相应信号处理函数返回时，
                这个系统调用被中断，调用返回错误，
                设置errno为EINTR
            */

        /**
         * ECONNABORTED
         *  该错误被描述为“software caused connection abort”，即“软件引起的连接中止”。
         *  原因在于当服务和客户进程在完成用于 TCP 连接的“三次握手”后，
         *  客户 TCP 却发送了一个 RST （复位）分节，在服务进程看来，就在该连接已由 TCP 排队，
         *  等着服务进程调用 accept 的时候 RST 却到达了。POSIX 规定此时的 errno 值必须 ECONNABORTED。
         *  源自 Berkeley 的实现完全在内核中处理中止的连接，
         *  服务进程将永远不知道该中止的发生。服务器进程一般可以忽略该错误，直接再次调用accept。
         */
        if ((errno == ECONNABORTED) || (errno == EINTR)) {
            //重试等待连接
            goto again;
        } else {
            return check(i, ACCEPT_ERROR);
        }
    }

}

int my_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    return check(connect(sockfd, addr, addrlen), CONNECT_ERROR);
}

ssize_t my_read(int fd, void *buf, size_t count) {
    ssize_t i;
    again :
    if ((i = read(fd, buf, count)) < 0) {
        if (errno == EINTR) {
            goto again;
        } else {
            return -1;
        }
    }
    return i;
}

/**
 * 重复读取count个数数据
 *
 * @param fd
 * @param buf
 * @param count 读取指定个数,应该<=buf空间
 * @return
 */
ssize_t my_readn(int fd, void *buf, size_t count) {
    ssize_t i;
    //记录剩余尚未读取个数
    ssize_t nleft = count;
    char *ptr = buf;
    while (nleft > 0) {
        if ((i = read(fd, buf, nleft)) < 0) {
            if (errno == EINTR) {
                i = 0;
            } else {
                return -1;
            }
        } else if (i == 0) {
            break;
        }
        //说明读取成功
        //重新计算应读个数,以及指针偏移
        nleft -= i;
        ptr += i;
    }
    //返回已读个数
    return count - nleft;
}

ssize_t my_writen(int fd, void *buf, size_t count) {
    ssize_t i;
    ssize_t nleft = count;
    char *ptr = buf;
    while (nleft > 0) {
        if ((i = write(fd, buf, nleft)) <= 0) {
            if (i < 0 && errno == EINTR) {
                i = 0;
            } else {
                return -1;
            }
        }
        nleft -= i;
        ptr += i;
    }
    //返回已写个数
    return count - nleft;
}

ssize_t my_write(int fd, const void *buf, size_t count) {
    ssize_t i;
    again :
    if ((i = write(fd, buf, count)) < 0) {
        if (errno == EINTR) {
            goto again;
        } else {
            return -1;
        }
    }
    return i;
}

/**
* 设置端口复用,也就是可以重复利用同一个端口运行服务器
*/
int my_setsockopt(int sockfd) {
    //是否启用,0不启用
    int opt = 1;
    return check(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (void *) &opt, sizeof(opt)), PORT_ERROR);
}