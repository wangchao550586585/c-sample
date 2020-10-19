#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <signal.h>
#include <wait.h>
#include  "warp_socket.h"

/**
 * 多进程
 * @return
 */
#define PORT 8080

void killP() {
    while (waitpid(0, NULL, WNOHANG) > 0);
    return;
}

int main() {
    int lfd, cfd;
    int ret;
    char buf[BUFSIZ];
    pid_t pid;
    struct sockaddr_in srv_addr, cli_addr;
    create_ipv4_for_localhost(&srv_addr, PORT);

    lfd = default_my_socket_ipv4_tcp();
    my_bind(lfd, (struct sockaddr *) &srv_addr, sizeof(srv_addr));
    my_listen(lfd, 128);

    socklen_t cli_addr_len = sizeof(cli_addr);
    while (1) {
        cfd = my_accept(lfd, (struct sockaddr *) &cli_addr, &cli_addr_len);
        pid = fork();
        if (pid < 0) {
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            //关闭主进程accept描述符
            close(lfd);
            break;
        } else {
            //关闭子进程描述符
            close(cfd);

            /**
             * 半关闭
             * SHUT_RDWR 关闭读写端
             * SHUT_WR 关闭写端
             * SHUT_RD 关闭读端
             * 与close最大的不同是,可以精确确定关闭那端
             * 而且close每被调用一次,引用计数-1，当=0时,才释放.
             * shutdown不同的是,直接关闭所有描述符.且关闭所有引用的描述符
             * 比如用dup2,2个描述符指向同一个.shuwdown后,都关闭,而close只会精确处理一个.
             */
//            shutdown(cfd, SHUT_RDWR);

            //回收子进程
            struct sigaction act;
            act.sa_handler = killP;
            sigemptyset(&act.sa_mask);
            act.sa_flags = 0;
            ret = sigaction(SIGCHLD, &act, NULL);
            if (ret != 0) {
                perror("error");
                exit(EXIT_FAILURE);
            }
            continue;
        }
    }
    if (pid == 0) {
        while (1) {
            ret = my_read(cfd, buf, sizeof(buf));
            printf("%d \n", ret);
            //读到文件末尾或者客户端主动断开,则直接返回
            if (ret <= 0) {
                close(cfd);
                perror("error");
                exit(EXIT_FAILURE);
            }
            my_write(STDOUT_FILENO, buf, ret);
            for (int i = 0; i < ret; i++) {
                buf[i] = toupper(buf[i]);
            }
            my_write(cfd, buf, ret);
        }
    }

    return 0;
}