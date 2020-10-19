#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <wait.h>

/**
 * mmap父子进程和非血缘关系进程通信
 * MAP_SHARED:父子进程共享映射区
 * MAP_PRIVATE:父子进程各自独占映射区
 *
 *  通过mmap可重复读
 * @return
 */
void test() {
    //建立映射区
    int fd = open("test.txt", O_RDWR | O_CREAT | O_TRUNC, 0664);
    int var = 100;
    if (fd == -1) {
        perror("error");
        exit(1);
    }
    int size = 4;
    ftruncate(fd, size);
    int *p = (int *) mmap(NULL, size, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    if (MAP_FAILED == p) {
        perror("error");
        exit(1);
    }
    //关闭文件描述符,
    close(fd);

    //建立进程通信
    pid_t pid = fork();
    if (pid == 0) {
        *p = 2000;
        var = 1000;
        printf("%d %d \n", *p, var);

    } else {
        sleep(1);
        printf("%d %d \n", *p, var);
        wait(NULL);

        int ret = munmap(p, size);
        if (ret == 1) {
            perror("error");
            exit(1);
        }
    }

}

struct student {
    int age;
    char name[256];
    int count;
};

void test2() {
    struct student stu = {10, "hah", 0};
    //建立映射区
    int fd = open("test.txt", O_RDWR | O_CREAT | O_TRUNC, 0664);

    if (fd == -1) {
        perror("error");
        exit(1);
    }
    ftruncate(fd, sizeof(stu));
    struct student *p = (struct student *) mmap(NULL, sizeof(stu), PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    if (MAP_FAILED == p) {
        perror("error");
        exit(1);
    }
    close(fd);

    while (1) {
        sleep(2);
//        strcpy(p->name, stu.name);
//同上
        char *dst = p->name;
        const char *src = stu.name;
        while ((*dst++ = *src++) != '\0');
        //同上
/*
        for (int i = 0; i < sizeof(stu.name); i++) {
            dst[i] = src[i];
        }
*/
        p->age = stu.age;
        p->count = stu.count++;
//同上2句
//memcpy(p, &stu, sizeof(stu));
        printf("%d %d  %s %p \n", p->age, p->count, p->name, p);
    }

    munmap(p, sizeof(stu));
}

int main() {
//    mmap父子进程通信
    test();

//非血缘关系
//缓冲区写入结构体
//可重复读
//    test2();

//匿名映射,只适合有血缘关系进程
    //1：可关闭文件描述符后,直接删除文件
        //    close(fd);
        //    unlink("test.txt");
    //2：直接设置MAP_ANONYMOUS即可,fd设置-1,大小可任意设置
        //   *p =  mmap(NULL,40, PROT_WRITE | PROT_READ, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    //3：若没有MAP_ANONYMOUS,则通过空文件实现。
        //    int fd = open("/dev/zero", O_RDWR | O_CREAT | O_TRUNC, 0664);
        //    int fd = open("/dev/null", O_RDWR | O_CREAT | O_TRUNC, 0664);
    return 0;
}