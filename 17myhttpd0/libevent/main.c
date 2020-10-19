#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include "libevent_http.h"

int main(int argc, char **argv) {

    // 端口
    int port = atoi(argv[1]);
    // 修改进程的工作目录, 方便后续操作
    int ret = chdir(argv[2]);
    if (ret == -1) {
        perror("chdir error");
        exit(1);
    }

    struct event_base *base = NULL;
    base = event_base_new();
    if (base == NULL) {
        perror("event_base_new");
        exit(1);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //创建监听的套接字,绑定,监听,以及接受客户端连接请求
    struct evconnlistener *lis =
            evconnlistener_new_bind(base, listencb, base, LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, -1,
                                    (struct sockaddr *) &addr, sizeof(addr));

    if (!lis) {
        perror("evconnlistener_new_bind");
        exit(1);
    }

    struct event *signal_event = evsignal_new(base, SIGINT, signal_cb, (void *) base);
    if (!signal_event || event_add(signal_event, NULL) < 0) {
        fprintf(stderr, "不能创建一个信号事件");
        exit(EXIT_FAILURE);
    }

    //调度
    event_base_dispatch(base);

    //释放
    event_free(signal_event);
    event_base_free(base);

    printf("done \n");
    return 0;

}