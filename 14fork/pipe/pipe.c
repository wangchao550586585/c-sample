#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

/**
 * 进程通信(IPC)
 * 因为整个操作系统中,内核只有一块,所以采用内核共享缓冲区,实现进程通信
 *
 *
 * 管道
 *  作用与父子进程
 *  本质是一个伪文件(内核缓冲区,大小4K,环形队列实现)
 *  2个文件描述符引用,一个表示读端,一个表示写端
 *
 * 双向半双工
 *  只能在有公共祖先进程间才能使用
 *
 *  读
 *      管道有数据,返回数据
 *      无数据
 *          写端全部关闭,返回0,表示读到末尾
 *          写端没关闭,则阻塞.
 *  写
 *      读端全部关闭,则进程异常终止(收到SIGPIPE信号)
 *      没关闭
 *          管道满了,则阻塞
 *          没满,写入数据
 */

void test() {
    int fd[2];
    char *str = "hello World";
    char buf[1024];
    int ret = pipe(fd);

    if (ret == 1) perror(" error");

    pid_t pid = fork();
    if (pid > 0) {
        //关闭读端
        close(fd[0]);
        write(fd[1], str, strlen(str));
        close(fd[1]);
    } else if (pid == 0) {
        //子进程关闭写端
        close(fd[1]);
        int size = read(fd[0], buf, sizeof(buf));
        write(STDOUT_FILENO, buf, size);
        close(fd[0]);

    }
}

void test1() {
    int fd[2];
    int ret = pipe(fd);
    if (ret == 1) perror(" error");
    pid_t pid = fork();
    /* 这里之所以主进程负责读,然后执行命令
     当主进程负责写,子进程负责读时
     子进程会一直阻塞等待缓冲区数据.
     这样就会造成父进程永远先于子进程结束,然后子进程会与bash进程争抢输出,从而乱序.
     这里让父进程sleep也是无效的,因为exec写入数据是新开的一个新进程,无法控制它的同步输入缓冲区.
     让父进程负责读操作就行了.*/
    if (pid > 0) {
        //关闭写端
        close(fd[1]);
        //将控制台输入数据指向缓冲区
        //也就是当执行wc时,会读取控制台输入数据,这个时候控制台指向缓冲区
        //因此读取的是缓冲区的数据
        dup2(fd[0], STDIN_FILENO);
        execlp("wc", "wc", "-l", NULL);

        //这里不再执行close,因为缓冲区交由execlp处理了,只能期待内核隐式回收.
//        close(fd[0]);
    } else if (pid == 0) {
        //子关闭读端
        close(fd[0]);
        //将控制台输出数据,写入缓冲区
        dup2(fd[1], STDOUT_FILENO);
        execlp("ls", "ls", NULL);
    }
}


void test2() {
    int fd[2];
    int ret = pipe(fd);
    if (ret == 1) perror(" error");
    int i;
    for (i = 0; i < 2; i++) {
        pid_t pid = fork();
        if (pid == 0)break;
    }
    if (i == 0) {
        //关闭写端
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        execlp("ls", "ls", NULL);
    } else if (i == 1) {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        execlp("wc", "wc", "-l", NULL);
    } else if (i == 2) {
        //只能存在一个读端和写端
        close(fd[0]);
        close(fd[1]);
        wait(NULL);
        wait(NULL);
    }
}

void test3() {
    char *str1 = "hello World1";
    char *str2 = "hello World2";
    char buf[1024];

    int fd[2];
    int ret = pipe(fd);
    if (ret == 1) perror(" error");
    int i;
    for (i = 0; i < 2; i++) {
        pid_t pid = fork();
        if (pid == 0)break;
    }
    if (i == 0) {
        close(fd[0]);
        write(fd[1], str1, strlen(str1));
        close(fd[1]);
    } else if (i == 1) {
        close(fd[0]);
        write(fd[1], str2, strlen(str2));
        close(fd[1]);
    } else if (i == 2) {
        sleep(1);
        close(fd[1]);
        int size = read(fd[0], buf, sizeof(buf));
        write(STDOUT_FILENO, buf, size);
        close(fd[0]);

        wait(NULL);
        wait(NULL);
    }

}

int main() {
/* 管道通信 */
//    test();
//父子通信执行ls | wc -l
//    test1();

//兄弟进程通信
//    test2();

//多端写入,一端读取
//    test3();
    return 0;
}

