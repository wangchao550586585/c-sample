#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <event2/event.h>

void write_cb(evutil_socket_t fd, short what, void *arg) {
    char buf[1024] = "hello";
    static int num = 0;
    sprintf(buf, "hello world %d\n", num++);
    printf("%s", buf);
    write(fd, buf, strlen(buf) + 1);
    sleep(1);
}

int main() {
    int fd = open("myfifo", O_WRONLY | O_NONBLOCK);
    //创建event_base
    struct event_base *base = event_base_new();
    //创建事件
    struct event *ev = event_new(base, fd, EV_WRITE | EV_PERSIST, write_cb, NULL);

    //添加事件
    event_add(ev, NULL);

    //事件循环
    event_base_dispatch(base);

    //销毁添加的事件
    event_free(ev);
    //销毁event_base
    event_base_free(base);
    close(fd);
    return 0;
}