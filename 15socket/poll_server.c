#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <arpa/inet.h>
#include  "warp_socket.h"
#include <poll.h>
#include <errno.h>

#define PORT 8080

/**
 * poll
 *
 * 缺点:相当于对select浅浅的封装,多提供了一个数据结构.且监听事件和返回事件集合分离
 * 优点:扩展监听上限.
 * @return
 */
#define OPEN_MAX 1024
int main() {
    int lfd, cfd;
    int n, nready, i;
    char buf[OPEN_MAX], str[INET_ADDRSTRLEN];
    struct pollfd client[OPEN_MAX];
    struct sockaddr_in srv_addr, cli_addr;
    socklen_t cli_addr_len = sizeof(cli_addr);
    create_ipv4_for_localhost(&srv_addr, PORT);
    lfd = default_my_socket_ipv4_tcp();
    my_bind(lfd, (struct sockaddr *) &srv_addr, sizeof(srv_addr));
    my_listen(lfd, 128);

    //设置待监听的文件描述符
    client[0].fd = lfd;
    //设置带监听的文件描述符的监听事件
    /**POLLIN:读
     * POLLOUT:写
     * POLLERR:错误
     */
    client[0].events = POLLIN;
    for (i = 1; i < FD_SETSIZE; i++) {
        client[i].fd = -1;
    }
    int maxi = 0;

    while (1) {

        /**  int poll(  struct pollfd *fds, nfds_t nfds, int timeout);
         *fds : 监听的文件描述符数组
         * nfds ：实际有效监听个数,也就是数组中具体监听多少个
         * timeout ：-1阻塞,0不阻塞,>0超时多少毫秒
         */
        nready = poll(client, maxi + 1, -1);
        check(nready, "select error");

        //说明lfd有读写事件来了
        if (client[0].revents & POLLIN) {
            cfd = my_accept(lfd, (struct sockaddr *) &cli_addr, &cli_addr_len);

            printf("received from %s at PORT %d \n", inet_ntop(AF_INET, &cli_addr.sin_addr, str, INET_ADDRSTRLEN),
                   ntohs(cli_addr.sin_port));

            for (i = 1; i < OPEN_MAX; i++) {
                if (client[i].fd == -1) {
                    client[i].fd = cfd;
                    break;
                }
            }
            if (i == OPEN_MAX) {
                fputs("too many clients \n", stderr);
                exit(1);
            }

            client[i].events = POLLIN;

            if (maxi < i) {
                maxi = i;
            }

            //若监听事件是lfd,且只有一个事件,就直接跳过当前循环
            if (0 == --nready)continue;
        }
        for (i = 1; i <= maxi; i++) {
            if (client[i].fd < 0)continue;
            //处理读事件
            if (client[i].revents & POLLIN) {
                //读取失败或读到末尾,则关闭文件描述符,以及清空select监听的事件.
                if ((n = my_read(client[i].fd, buf, OPEN_MAX)) < 0) {
                    //收到RST标志
                    if (errno == ECONNRESET) {
                        printf("client %d \n", i);
                        close(client[i].fd);
                        //POLL不监控该文件描述符,设置-1即可
                        client[i].fd = -1;
                    } else {
                        fputs("read error \n", stderr);
                    }
                } else if (n == 0) {
                    //读到文件末尾,说明客户端先关闭连接.
                    close(client[i].fd);
                    client[i].fd = -1;
                } else {
                    my_write(STDOUT_FILENO, buf, n);
                    for (int j = 0; j < n; j++) {
                        buf[j] = toupper(buf[j]);
                    }
                    my_write(client[i].fd, buf, n);
                }
                //匹配固定个数后,跳出当前循环
                if (--nready == 0)break;
            }
        }

    }
    close(lfd);

    return 0;
}