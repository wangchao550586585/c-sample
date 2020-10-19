#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <signal.h>
#include <wait.h>
#include <pthread.h>
#include  "warp_socket.h"

/**
 * 多线程
 * @return
 */
#define PORT 8080
struct s_info {
    struct sockaddr_in cliaddr;
    int fd;
};

void *work_fun(void *s_info) {
    struct s_info *s = (struct s_info *) s_info;
    int ret;
    char buf[BUFSIZ];
    char str[INET_ADDRSTRLEN];
    while (1) {
        ret = my_read(s->fd, buf, BUFSIZ);
        printf("%d \n", ret);
        //读到文件末尾或者客户端主动断开,则直接返回
        if (ret <= 0) {
            break;
        }

        printf("received from %s at PORT %d \n", inet_ntop(AF_INET, &(*s).cliaddr.sin_addr, str, INET_ADDRSTRLEN),
               ntohs((*s).cliaddr.sin_port));
        printf("received from %s at PORT %d \n", inet_ntop(AF_INET, &(s->cliaddr.sin_addr), str, INET_ADDRSTRLEN),
               ntohs(s->cliaddr.sin_port));
        my_write(STDOUT_FILENO, buf, ret);
        for (int i = 0; i < ret; i++) {
            buf[i] = toupper(buf[i]);
        }
        my_write(s->fd, buf, ret);
    }
    close(s->fd);
    return (void *) 0;
}

int main() {
    int lfd, cfd;
    int i = 0;

    struct sockaddr_in srv_addr, cli_addr;
    socklen_t cli_addr_len = sizeof(cli_addr);
    struct s_info t[256];

    create_ipv4_for_localhost(&srv_addr, PORT);

    lfd = default_my_socket_ipv4_tcp();
    my_bind(lfd, (struct sockaddr *) &srv_addr, sizeof(srv_addr));
    my_listen(lfd, 128);
    pthread_t tid;
    printf("wait for Accept... ");
    while (1) {
        cfd = my_accept(lfd, (struct sockaddr *) &cli_addr, &cli_addr_len);
        t[i].cliaddr = cli_addr;
        t[i].fd = cfd;
        pthread_create(&tid, NULL, work_fun, (void *) &t[i]);
        //线程分离,防止僵尸线程产生.
        pthread_detach(tid);
        i = (++i) % 256;
    }
    return 0;
}