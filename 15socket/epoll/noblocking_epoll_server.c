#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <arpa/inet.h>
#include  "../warp_socket.h"
#include <errno.h>
#include <sys/epoll.h>
#include <fcntl.h>

#define PORT 8080

/**
 * epoll
 * 测试ET边沿触发+非阻塞
 * @return
 */
#define MAXLINE 10

int main() {
    int efd, lfd, cfd, handle_cfd;
    int len, nready, i;
    char buf[MAXLINE], str[INET_ADDRSTRLEN];
    struct sockaddr_in srv_addr, cli_addr;
    socklen_t cli_addr_len = sizeof(cli_addr);
    create_ipv4_for_localhost(&srv_addr, PORT);
    lfd = default_my_socket_ipv4_tcp();
    my_bind(lfd, (struct sockaddr *) &srv_addr, sizeof(srv_addr));
    my_listen(lfd, 128);

    //创建epoll红黑树节点(内核会自动调整,仅供参考)
    efd = epoll_create(10);
    tep.events = EPOLLIN | EPOLLET;//ET边沿触发
//    tep.events = EPOLLIN;
    printf("my_accept ...");
    cfd = my_accept(lfd, (struct sockaddr *) &cli_addr, &cli_addr_len);
    printf("received from %s at PORT %d \n", inet_ntop(AF_INET, &cli_addr.sin_addr, str, INET_ADDRSTRLEN),
           ntohs(cli_addr.sin_port));

    //修改文件非阻塞
    int flags = fcntl(cfd, F_GETFD);
    flags |= O_NONBLOCK;
    int ret = fcntl(cfd, F_SETFL, flags);

    //将lfd以及结构体设置树上
    struct epoll_event tep, ep[10];
    //新建立连接的fd,加入红黑树

    tep.data.fd = cfd;
    int res = epoll_ctl(efd, EPOLL_CTL_ADD, cfd, &tep);

    while (1) {
        res = epoll_wait(efd, ep, 10, -1);
        printf("res %d \n", res);
        if (ep[0].data.fd == cfd) {
            len = my_read(cfd, buf, MAXLINE / 2);
            my_write(STDOUT_FILENO, buf, len);
        }
    }
    return 0;
}