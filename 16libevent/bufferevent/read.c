#include <sys/stat.h>
#include <string.h>
#include<event2/bufferevent.h>
#include <event2/event.h>
#include <event2/listener.h>

#define  PORT 8080

/**
 * 缓冲事件
 *      有2个缓冲区,队列实现.
 */
void read_cb(struct bufferevent *bev, void *ctx) {
    char buf[1024];
    //从读缓存中读取数据
    bufferevent_read(bev, buf, sizeof(buf));
    printf("client say ： %s \n", buf);
    char *p = "world";
    //向写缓存中写数据,一旦有数据就会自动刷新,发送给对端,发送成功后,会被通知写回调方法.
    bufferevent_write(bev, p, strlen(p) + 1);
}

void write_cb(struct bufferevent *bev, void *ctx) {
    printf("发送成功 \n");
}

/**
 * 事件回调
 * @param bev
 *
 * @param what
 *          不同标志位,表示不同事件
           BEV_EVENT_READING    读缓冲区满足,则产生事件
           BEV_EVENT_WRITING	写缓冲区满足,则产生事件
           BEV_EVENT_EOF		读到文件结尾
           BEV_EVENT_ERROR		出错,则产生事件
           BEV_EVENT_TIMEOUT	超时
           BEV_EVENT_CONNECTED  请求的连接过程完成,客户端时可用
 *
 * @param ctx   回调函数参数
 */
void event_cb(struct bufferevent *bev, short what, void *ctx) {
    if (what & BEV_EVENT_EOF) {
        printf("connection closed \n");
    } else if (what & BEV_EVENT_ERROR) {
        printf("error \n");
    }
    bufferevent_free(bev);
    printf("资源释放 \n");
}

/**
 * 客户端连接后,回调
 * @param listener    当调用evconnlistener_new_bind返回的监听器
 * @param fd          客户端的套接字
 * @param addr        客户端地址
 * @param len         客户端地址长度
 * @param ptr         当调用evconnlistener_new_bind设置的参数
 */
void cb_listener(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *addr, int len, void *ptr) {
    struct event_base *base = (struct event_base *) ptr;
    /**
     * 创建bufferevent
     * struct bufferevent *bufferevent_socket_new(struct event_base *base, evutil_socket_t fd, int options);
     * base     event_base_new()返回值
     * fd       跟bufferevent绑定的fd
     * options
     *      BEV_OPT_CLOSE_ON_FREE:释放bufferevent时关闭底层传输端口,也就是关闭底层套接字,释放底层bufferevent等
     */
    struct bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
//    bufferevent_free() 释放缓冲事件

/**
 * 给bufferevent设置回调
void bufferevent_setcb(struct bufferevent *bufev,
    bufferevent_data_cb readcb, bufferevent_data_cb writecb,
    bufferevent_event_cb eventcb, void *cbarg);

    bufev    ： bufferevent_socket_new返回值
    readcb    :读回调,不用传NULL即可
    writecb     :写事件满足时回调
    eventcb     :事件回调
    cbarg       :回调函数所有参数
 *
 */
    bufferevent_setcb(bev, read_cb, write_cb, event_cb, NULL);


    /**
     * 启用缓冲区
     * 默认写缓冲是启用的,读缓冲没启用
     */
    bufferevent_enable(bev, EV_READ);
    //禁用缓冲区
//    bufferevent_disable(bev, EV_READ);
    //获取缓冲区禁用状态,可通过&判断
//    bufferevent_get_enabled(bev);

}

int main() {
    struct sockaddr_in serv_addr;

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    struct event_base *base = event_base_new();

    /**
     * 相当于socket,bind,listen，accept
     * 创建套接字,绑定,接收连接请求
     * struct evconnlistener *evconnlistener_new_bind(struct event_base *base,
        evconnlistener_cb cb, void *ptr, unsigned flags, int backlog,
        const struct sockaddr *sa, int socklen);

     * base         ：   event_base_new()返回值
     * cb           ：   接受连接之后,回调函数
     * ptr          ：   回调函数参数
     * flags        ：
     *                  LEV_OPT_CLOSE_ON_FREE  释放bufferevent时关闭底层传输端口,也就是关闭底层套接字,释放底层bufferevent等
     *                  LEV_OPT_REUSEABLE     端口复用
     * backlog      ：   listen的第二个参数, -1表示默认最大值
     * sa / socklen     ：   传给bind的参数   服务器的ip+port 以及长度
     */
    struct evconnlistener *listener = evconnlistener_new_bind(base, cb_listener, base,
                                                              LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, 36,
                                                              (struct sockaddr *) &serv_addr, sizeof(serv_addr));

    event_base_dispatch(base);

    evconnlistener_free(listener);
    event_base_free(base);
    return 0;
}