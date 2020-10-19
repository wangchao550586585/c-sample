#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <arpa/inet.h>
#include  "warp_socket.h"

#define PORT 8080

/**
 * select
 * 缺点:受操作系统文件描述符影响,最多只能1024连接
 *      需要遍历所有监听的fd(lfd~maxfd ,但是中间有很多失效的fd),虽然可以用数组解决,但是增加了复杂度.
 * @return
 */
int main() {
    int lfd, cfd, maxfd;
    int n, nready, client[FD_SETSIZE], i;   //client防止遍历所有的文件描述符,FD_SETSIZE=1024
    char buf[BUFSIZ], str[INET_ADDRSTRLEN];  //INET_ADDRSTRLEN=16
    struct sockaddr_in srv_addr, cli_addr;
    socklen_t cli_addr_len = sizeof(cli_addr);
    create_ipv4_for_localhost(&srv_addr, PORT);
    lfd = default_my_socket_ipv4_tcp();
    my_bind(lfd, (struct sockaddr *) &srv_addr, sizeof(srv_addr));
    my_listen(lfd, 128);

    maxfd = lfd;
    int maxi = -1;
    for (i = 0; i < FD_SETSIZE; i++) {
        client[i] = -1;
    }
    //读集合,和备份集合(用于注册所有读事件)
    fd_set rset, allset;
    //置0
    FD_ZERO(&allset);
    //监听lfd
    FD_SET(lfd, &allset);

    while (1) {
        rset = allset;
        /**
       int select(int nfds, fd_set *readfds, fd_set *writefds,fd_set *exceptfds, struct timeval *timeout);
         nfds：监听的所有文件描述符中,最大文件描述符+1
         readfds：读文件描述符监听集合
         writefds：写文件描述符监听集合
         exceptfds: 异常文件描述符监听集合
         timeout:
                >0:设置监听时长
                NULL:阻塞监听
                0:非阻塞监听,需要轮询
         返回值:
              0，尚未有读写事件来
               -1:error
               >0:所有监听集合有满足的.
         */
        nready = select(maxfd + 1, &rset, NULL, NULL, NULL);
        check(nready, "select error");

        //说明有读写事件来了,因为是lfd,说明有新的客户端连接请求
        if (FD_ISSET(lfd, &rset)) {
            //这里accept不会阻塞
            cfd = my_accept(lfd, (struct sockaddr *) &cli_addr, &cli_addr_len);

            printf("received from %s at PORT %d \n", inet_ntop(AF_INET, &cli_addr.sin_addr, str, INET_ADDRSTRLEN),
                   ntohs(cli_addr.sin_port));
            for (i = 0; i < FD_SETSIZE; i++) {
                if (client[i] == -1) {
                    client[i] = cfd;
                    break;
                }
            }
            if (i == FD_SETSIZE) {
                fputs("too many clients \n", stderr);
                exit(1);
            }

            //将新建立的客户端连接添加到select监听
            FD_SET(cfd, &allset);
            //记录最大的fd,主要用于遍历事件
            if (maxfd < cfd) {
                maxfd = cfd;
            }
            //记录最大索引
            if (maxi < i) {
                maxi = i;
            }


            //若监听事件是lfd,且只有一个事件,就直接跳过当前循环
            if (0 == --nready)continue;
        }
        //从4开始，因为0~3,分别被in,out,err以及lfd占用了
        for (i = 0; i <= maxi; i++) {
            if (client[i] < 0)continue;

            //处理读事件
            if (FD_ISSET(client[i], &rset)) {
                //读取失败或读到末尾,则关闭文件描述符,以及清空select监听的事件.
                if ((n = my_read(client[i], buf, BUFSIZ)) <= 0) {
                    close(client[i]);
                    FD_CLR(client[i], &allset);
                    client[i] = -1;
                } else {
                    my_write(STDOUT_FILENO, buf, n);
                    for (int j = 0; j < n; j++) {
                        buf[j] = toupper(buf[j]);
                    }
                    my_write(client[i], buf, n);
                }
                //匹配固定个数后,跳出当前循环
                if (--nready == 0)break;
            }
        }

/*     for (int i = lfd + 1; i <= maxfd; i++) {
            //处理读事件
            if (FD_ISSET(i, &rset)) {
                //读取失败或读到末尾,则关闭文件描述符,以及清空select监听的事件.
                if ((n = my_read(i, buf, BUFSIZ)) <= 0) {
                    close(i);
                    FD_CLR(i, &allset);
                } else {
                    my_write(STDOUT_FILENO, buf, n);
                    for (int i = 0; i < n; i++) {
                        buf[i] = toupper(buf[i]);
                    }
                    my_write(i, buf, n);
                }
            }
        }*/
    }
    close(lfd);

    return 0;
}