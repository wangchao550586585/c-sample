#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

void readAndWriteFile();

void setOffset();

void set();

/**
 * open函数
 * @returnz
 */
int main() {
    /*文件描述符
    PCB进程控制块是结构体,
    结构体有文件描述符表属性
    表中容纳1023个数字
    从0开始
    0表示STDIN_FILENO
    1表示STDOUT_FILENO
    1表示STERR_FILENO
    新打开的文件描述符会选择最小的值*/
//    readAndWriteFile();

//----------------------这里文件是阻塞的,可以在open(O_NONBLOCK)设置不阻塞读取属性.
/*    int n = 0;
    char buf[1024];
    n = read(STDIN_FILENO, buf, 10);
    write(STDOUT_FILENO, buf, n);
*/

//-----------------------------------------------设置文件属性
//    set();

//--------------------------------------------设置文件偏移
//    setOffset();
//--------------------------------------------获取文件属性
    //innode结构体,存储文件属性信息,权限，类型,大小，时间，用户 ，盘符等。
    //dentry文件名和innode结构体引用.
    //之所以有了innode为什么还要dentry,是为了共享innode,
    //也就是使用link命令建立多个连接共享innode
    //返回的就是innode
/*    struct stat sbuf;
    lstat("../aa.txt", &sbuf);
    //stat穿透连接,限制文件具体属性,lstat不会
    //字节个数
    printf("%d \n", sbuf.st_size);

    if (S_ISREG(sbuf.st_mode)) {
        printf("是普通文件");
    } else if (S_ISDIR(sbuf.st_mode)) {
        printf("是普通目录");
    }*/

    //--------------------------------传入参数,传出参数,传入传出参数
    /*传入参数:就是传入的可读参数
     *传出参数:传入参数,写入结果,然后返回
     *传入传出参数:这个参数负责读取和写入,然后返回.
     */

    //-------------------------------------删除目录项
//    link("a", "b");
    //当文件硬链接=0时,没有dentry,并不会删除文件
    //要等到所有打开该文件的进程关闭该文件
    //操作系统才会择机释放.
//    unlink("../aa.txt");


//-------------------------------------操作目录
//目录就是一个文件,记录了所有的dentry文件项
//目录读权限就是可以浏览目录
//写:创建文件,本质是在目录项中添加文件名
//运行:就是可以cd操作.

    //打开目录
    DIR *dp = opendir("E:\\workspace\\c\\");
    struct dirent *d;
    //遍历目录所有项
    while (d = readdir(dp) != NULL) {
        printf("%s \n", d->d_name);
    }
    //关闭目录
    closedir(dp);


    //------------------------重定向文件和输出
    int fd2 = open("../aa.txt", O_RDWR | O_CREAT);
    int fp = dup(fd2);
    //复制fp -》 STDOUT_FILENO
    dup2(fd2, STDOUT_FILENO);

    //模仿dup,copy文件描述符,描述符从0开始
    int fd3=fcntl(fd2,F_DUPFD,0);

    return 0;
}

void setOffset() {
    int n = 0;
    char buf[1024];

    int fd2 = open("../aa.txt", O_RDWR | O_CREAT);
    char msg[] = "hello world";
    write(fd2, msg, strlen(msg));

    //设置读取或者写入起始偏移
    //也可以扩展文件大小,必须要触发一个IO写操作,才会扩展
    //获取文件大小
//    lseek(fd2, 2, SEEK_SET);

    lseek(fd2, 110, SEEK_END);
    write(fd2, "1", 1);

    while ((n = read(fd2, buf, 1024)) != 0) {
        if (n == -1) {
            perror(" error");
            exit(1);
        }
        write(STDOUT_FILENO, buf, n);
    }
    close(fd2);
}

void set() {
    int n = 0;
    char buf[1024];
    //获取stdin属性信息
    int flags = fcntl(STDIN_FILENO, F_GETFD);
    flags |= O_NONBLOCK;
    //设置文件属性非阻塞
    int ret = fcntl(STDIN_FILENO, F_SETFL, flags);
    n = read(STDIN_FILENO, buf, 10);
    write(STDOUT_FILENO, buf, n);
}


void readAndWriteFile() {

    int fd = open("../CMakeLists2.txt", O_RDONLY);
    int fd2 = open("../aa.txt", O_RDWR | O_CREAT | O_TRUNC);
    if (fd2 == -1) {
        perror("open error");
        exit(1);
    }
    printf("%d errno=%d %s \n", fd, errno, strerror(errno));

    int n = 0;
    char buf[1024];
    while ((n = read(fd, buf, 1024)) != 0) {
        if (n == -1) {
            perror(" error");
            exit(1);
        }
        write(fd2, buf, n);
    }

    close(fd);
    close(fd2);
}