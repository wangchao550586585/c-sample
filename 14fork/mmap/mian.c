#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/fcntl.h>

/**
 * mmap
 * @return
 */
int main() {
    int fd = open("test.txt", O_RDWR | O_CREAT | O_TRUNC, 0664);
    if (fd == -1) {
        perror("error");
        exit(1);
    }
    //文件大小扩容10
    /*write(fd, '\0', 1);
    lseek(fd, 10, SEEK_SET);*/
    //同上2个函数
    ftruncate(fd, 20);
    int len = lseek(fd, 0, SEEK_END);
    /*
     * addr:指定缓冲区地址,这里null系统分配
     * len:指定缓冲区大小,不能为0,默认<=文件占用空间大小
     * prot:共享内存映射区读写属性
     * flags:共享属性,share则直接刷盘,private则独占,不会反映磁盘上.
     * offset:开始读取偏移位置,需要4k倍数
     *
     * 创建映射区,需要文件读权限.因为默认会执行一次读取操作,将文件内容映射缓冲区中.
     * 当操作缓冲区时,MAP_SHARED需要文件写权限，MAP_private因为只操作缓冲区,所以不要也可.
     */
    char *p = mmap(NULL, len, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    if (MAP_FAILED == p) {
        perror("error");
        exit(1);
    }
    //缓冲区创建后,在操作缓冲区之前,文件描述符关闭,缓冲区也能有效操作.后续用地址读取文件
    close(fd);

//越界也能输入
//    strcpy(p + len -3, "hello2");


//当使用自增修改p值时,释放的时候失败.因为创建映射区的p和释放的p不一致.
//    strcpy(p++, "hello5");

strcpy(p, "hello5");
    int ret = munmap(p, len);
    if (ret == -1) {
        perror("error");
        exit(1);
    }
    return 0;
}