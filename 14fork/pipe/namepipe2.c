#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/fcntl.h>

/**
 *有名管道
 * @return
 */
int main() {
    char buf[1024];
    int fd = open("E:\\workspace\\c\\cmake-build-debug\\test", O_RDONLY);
    printf("11122222222222222211%d \n", fd);
    while (1) {
        sleep(3);
        int size = read(fd, buf, sizeof(buf));
        write(STDOUT_FILENO, buf, size);
    }
    close(fd);
    return 0;
}
