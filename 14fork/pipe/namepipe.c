#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/fcntl.h>

/**
 * 有名管道,可不同进程通信
 * 数据只能读取一次
 * linux下mkfifo命令
 * @return
 */
int main() {
/*    int fd = mkfifo("test2", 644);  //创建一个有名管道
    printf("111111111111 %d \n", fd);*/

    char buf[1024];
    int fd = open("E:\\workspace\\c\\cmake-build-debug\\test", O_WRONLY);
    int i = 0;
    while (1) {
        sleep(3);
        sprintf(buf, "hello %d \n", i++);
        write(fd, buf, strlen(buf));
    }
    close(fd);
    return 0;
}
