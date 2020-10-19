#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <arpa/inet.h>
#include  "../warp_socket.h"
#include <errno.h>
#include <sys/epoll.h>

#define PORT 8080

/**
 * epoll
 *
 * 事件模型
 *      边沿触发：缓冲区剩余未读尽的数据不会导致epoll_wait返回,新的事件满足监听条件,才会触发
 *      水平出发(默认)：缓冲区剩余未读尽的数据会导致epoll_wait返回
 *
 *  ET边沿触发,高效,只支持文件非阻塞
 *
 *  epoll还可以适用管道之内用文件描述符的
 * @return
 */
#define OPEN_MAX 1024

int main() {
    int efd, lfd, cfd, handle_cfd;
    int n, nready, i;
    char buf[OPEN_MAX], str[INET_ADDRSTRLEN];
    struct sockaddr_in srv_addr, cli_addr;
    socklen_t cli_addr_len = sizeof(cli_addr);
    create_ipv4_for_localhost(&srv_addr, PORT);
    lfd = default_my_socket_ipv4_tcp();
    my_bind(lfd, (struct sockaddr *) &srv_addr, sizeof(srv_addr));
    my_listen(lfd, 128);

    //创建epoll红黑树节点(内核会自动调整,仅供参考)
//    int epoll_create(int size);
    efd = epoll_create(OPEN_MAX);
    check(efd, "epoll_create error");

    //将lfd以及结构体设置树上
    struct epoll_event tep, ep[OPEN_MAX];
    //监听读事件
    tep.events = EPOLLIN;//LT水平出发(默认)
//    tep.events = EPOLLIN|EPOLLET;//ET边沿触发
    tep.data.fd = lfd;
/**
 *  int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
 *  epfd: 红黑树根节点
 *  op：设置加入红黑树节点的监听事件
 *          EPOLL_CTL_ADD 添加节点到红黑树上
 *          EPOLL_CTL_DEL 删除节点,从红黑树上
 *          EPOLL_CTL_MOD 修改节点,从红黑树上
 *
 *  fd：加入红黑树的待监听节点
 *  event：
 *
            struct epoll_event {
                     uint32_t     events; 感兴趣的事件  EPOLLIN/EPOLLOUT/EPOLLERR
                     epoll_data_t data;
                };
            typedef union epoll_data {
                    void        *ptr;
                    int          fd;    添加的带监听fd
                    uint32_t     u32;
                    uint64_t     u64;
                } epoll_data_t;
 *
 *
 */
    int res = epoll_ctl(efd, EPOLL_CTL_ADD, lfd, &tep);
    check(res, "epoll_create error");

    while (1) {
        /**
         *  int epoll_wait(int epfd, struct epoll_event *events,int maxevents, int timeout);
         *  epfd：红黑树根节点
         *  events：传出参数,主要记录那些满足了监听条件的fd
         *  maxevents：传出参数数组总个数
         *  timeout：-1阻塞,0不阻塞,>0超时多少毫秒
         *
         *  返回:满足监听的个数
         */
        nready = epoll_wait(efd, ep, OPEN_MAX, -1);
        check(nready, "select error");

        for (i = 0; i < nready; i++) {
            if (!ep[i].events & EPOLLIN) {
                continue;
            }
            //说明是建立连接的事件
            if (ep[i].data.fd == lfd) {
                cfd = my_accept(lfd, (struct sockaddr *) &cli_addr, &cli_addr_len);
                printf("received from %s at PORT %d \n", inet_ntop(AF_INET, &cli_addr.sin_addr, str, INET_ADDRSTRLEN),
                       ntohs(cli_addr.sin_port));

                //新建立连接的fd,加入红黑树
                tep.events = EPOLLIN;
                tep.data.fd = cfd;
                res = epoll_ctl(efd, EPOLL_CTL_ADD, cfd, &tep);
                check(res, "epoll_create error");
            } else {
                //说明是读事件
                handle_cfd = ep[i].data.fd;
                if ((n = my_read(handle_cfd, buf, OPEN_MAX)) <= 0) {
                    //从红黑树上移除该监听fd
                    res = epoll_ctl(efd, EPOLL_CTL_DEL, handle_cfd, NULL);
                    check(res, "epoll_create error");
                    perror("close con \n");
                    close(handle_cfd);
                } else {
                    my_write(STDOUT_FILENO, buf, n);
                    for (int j = 0; j < n; j++) {
                        buf[j] = toupper(buf[j]);
                    }
                    my_write(handle_cfd, buf, n);
                }
            }
        }

    }
    close(lfd);
    close(efd);

    return 0;
}