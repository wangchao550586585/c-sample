#include <string.h>
#include<event2/bufferevent.h>
#include <event2/event.h>
#include <arpa/inet.h>
#include <unistd.h>

#define  PORT 8080

void read_cb(struct bufferevent *bev, void *ctx) {
    char buf[1024];
    bufferevent_read(bev, buf, sizeof(buf));
    printf("server say ： %s \n", buf);
    bufferevent_write(bev, buf, strlen(buf) + 1);
    sleep(1);
}

void write_cb(struct bufferevent *bev, void *ctx) {
    printf("发送成功 \n");
}

void event_cb(struct bufferevent *bev, short what, void *ctx) {
    if (what & BEV_EVENT_EOF) {
        printf("connection closed \n");
    } else if (what & BEV_EVENT_ERROR) {
        printf("error \n");
    } else if (what & BEV_EVENT_CONNECTED) {
        printf("已经连接服务器 \n");
        return;
    }
    bufferevent_free(bev);
    printf("资源释放 \n");
}

void read_terminal(evutil_socket_t fd, short what, void *arg) {
    char buf[1024] = {0};
    int len = read(fd, buf, sizeof(buf));
    struct bufferevent *bev = (struct bufferevent *) arg;
    printf("terminal rev ： %s \n", buf);
    bufferevent_write(bev, buf, len + 1);
}

int main() {
    struct sockaddr_in serv_addr;

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    int fd = socket(AF_INET, SOCK_STREAM, 0);

    struct event_base *base = event_base_new();
    struct bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);

    /**
     * int bufferevent_socket_connect(struct bufferevent *bufferevent, struct sockaddr * addr, int len);
     * bufferevent        事件对象(封装了fd)
     * addr / len         等同于connect的addr和len参数
     */
    bufferevent_socket_connect(bev, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

    bufferevent_setcb(bev, read_cb, write_cb, event_cb, NULL);
    bufferevent_enable(bev, EV_READ);

    struct event *ev = event_new(base, STDIN_FILENO, EV_READ | EV_PERSIST, read_terminal, bev);
    event_add(ev, NULL);

    event_base_dispatch(base);

    event_free(ev);
    event_base_free(base);
    return 0;
}