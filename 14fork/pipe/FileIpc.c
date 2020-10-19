#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/fcntl.h>
#include <string.h>

/**
 * 文件通信
 * @return
 */
int main() {
    int te = 100;
    int i;
    pid_t pid;
    for (i = 0; i < 2; i++) {
        if ((pid = fork()) == 0)break;
    }
    if (i == 0) {
       int fd = open("aa.txt", O_RDWR | O_TRUNC | O_CREAT,0664);
        char *str = "hello \n";
        write(fd, str, strlen(str));
        sleep(4);
        lseek(fd,0,SEEK_SET);
        char buf[1024];
        int ret = read(fd, buf, sizeof(buf));
        write(STDOUT_FILENO, buf, ret);
    } else if (i == 1) {
        sleep(2);
        int fd = open("aa.txt", O_RDWR);
        char buf[1024];
        int ret = read(fd, buf, sizeof(buf));
        write(STDOUT_FILENO, buf, ret);

        char *str = "world \n";
        write(fd, str, strlen(str));

    } else {
        sleep(10);
        printf("我是父进程 %d \n", te);
    }
    return 0;
}