
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <event2/event.h>

void read_cb(evutil_socket_t fd, short what, void *arg) {
    char buf[1024];
    int len = read(fd, buf, sizeof(buf));
    printf("%s \n", what & EV_READ ? "yes" : "no");
    printf("%s %d \n", buf, len);
    sleep(1);
}

int main() {
    unlink("myfifo");
    mkfifo("myfifo", 0664);
    /**
     * 获取读权限,设置非阻塞.
     * 这里之所以非阻塞是因为libevent是基于事件异步的通行模型,主要异步执行回调函数.
     */
    int fd = open("myfifo", O_RDONLY | O_NONBLOCK);
    //创建event_base,类似于epoll红黑树根节点
    struct event_base *base = event_base_new();

    /**
     * 创建事件
     * fd表示绑定到 创建的ev 上的文件描述符
     * EV_READ 一次读事件
     * EV_WRITE 一次写事件
     * EV_PERSIST 持续触发,需要结合event_base_dispatch使用,才生效.不指定,则只执行一次
     * read_cb满足事件监听的回调函数
     *
     * void read_cb(evutil_socket_t fd, short what, void *arg)
     *  fd 对应这里的fd
     *  what 对应这里的EV_READ | EV_PERSIST
     *  arg 对应最后一个参数NULL
     */
    struct event *ev = event_new(base, fd, EV_READ | EV_PERSIST, read_cb, NULL);

    /**
     * 添加一个[常规事件]到event_base
     * timeout:设置超时时间
     *      NULL:不会超时,等到事件被触发,回调函数立刻执行
     *      非0:期间检查事件有没有触发,一旦时间到则事件会被回调.
     */
    event_add(ev, NULL);

    /**
     * 从event_base删除事件
     *
     */
    //event_del(ev);

    //事件循环 可理解为while(1) epoll_wait()
    event_base_dispatch(base);
    //指定时间后停止
//    event_base_loopexit()
    //立即停止
//    event_base_loopbreak()

    //销毁添加的事件
    event_free(ev);
    //销毁event_base
    event_base_free(base);
    close(fd);
    return 0;
}