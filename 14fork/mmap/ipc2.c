#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <wait.h>

struct student {
    int age;
    char name[256];
    int count;
};

void test2() {
    struct student stu = {10, "hah"};
    //建立映射区
    int fd = open("test.txt", O_RDONLY, 0664);
    if (fd == -1) {
        perror("error");
        exit(1);
    }

    struct student *p = (struct student *) mmap(NULL, sizeof(stu), PROT_READ, MAP_SHARED, fd, 0);
    if (MAP_FAILED == p) {
        perror("error");
        exit(1);
    }
    close(fd);

    while (1) {
        sleep(1);
        printf("%d %d  %s %p \n", p->age, p->count, p->name,p);
    }
    munmap(p, sizeof(stu));
}

int main() {
    test2();
    return 0;
}