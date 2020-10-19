#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/wait.h>

/**
 * 进程
 *  虚拟内存通过MMU映射到物理磁盘
 *  pcb进程控制块存储在内核空间,在磁盘中共享同一片区域,方便进程通信
 *  而用户空间存储的值,在磁盘中不共享同一片区域
 *
 *  fork时，创建一个进程,这个进程和父进程的函数结构一样,但是不会执行fork之前的指令
 *  子进程执行完fork后,返回0
 *  父进程执行完fork后返回子进程id
 *
 *
 * 当执行exec时,程序重新执行.
 *
 * 孤儿进程
 *  就是父进程先于子进程结束,子进程成为孤儿进程
 *  然后子进程会被init进程领养,也就是父进程为init进程,主要负责回收孤儿进程
 *  kill就行
 *
 * 僵尸进程:
 *  子进程死了，父进程还没回收.
 *  子进程死了,用户空间数据默认操作系统回收.
 *  子进程残留PCB资源存放内核中,父进程就回收这个
 *  kill父进程就行了,
 *
 * 守护进程
 *      运行系统后台,不与用户进行直接交互,没有控制终端.
 *
 * 进程组
 *      一个或多个进程集合.
 *      父进程创建子进程,则默认属于同一组.进程组id=第一个进程id(进程组长)
 *      kill -9  -进程id   杀掉所有进程组中进程
 *      kill -9  进程id    杀掉进程
 *
 * 创建会话
 *      调用进程不能是进程组长
 *      创建会话的进程自动成为新进程组的组长进程，进程组id=会话id=组长进程id
 *      新会话丢弃控制终端.
 *      建立会话时,先fork,父进程终止,子进程调用setsid()则创建会话成功.
 */
//父子进程遵循读时共享写时复制
void forFork() {
    int te = 100;
    int i;
    pid_t pid, tempId;
    for (i = 0; i < 5; i++) {
        //创建5个进程,如果是子进程则直接退出循环
        //防止重复创建多个子进程.
        if ((pid = fork()) == 0)break;
        if (i == 2) {
            //这个是在主进程执行的,所以在主进程中te=300
            te = 300;
            tempId = pid;
        }
    }
    if (i == 5) {
//        te = 200;
        printf("我是父进程 %d %d \n", tempId, te);
    } else {
        te = 300;
        printf("我是第%d个子进程  %d\n", i + 1, te);
    }
}

void forkThread() {
    printf("hello \n");
    printf("world \n");
    pid_t pid = fork();
    //进入子进程执行
    if (pid == 0) {
        printf("当前进程id:%d 父进程id:%d fork返回值:%d\n", getpid(), getppid(), pid);
    } else if (pid > 0) {
        //进入父进程执行
        printf("当前进程id:%d 父进程id:%d fork返回值:%d\n", getpid(), getppid(), pid);
    } else {
        perror("进程创建失败");
    }

}

/**
 *  执行成功不返回,失败返回-1
 */
void exec() {
    pid_t pid = fork();
    //进入子进程执行
    if (pid == 0) {
        //通常用来调用系统程序,借助path环境变量。
        //因为是可变参数,所以NULL结尾
        //args第0个参数是文件名，所以这里再次传入java,紧接着才是-version命令
//        execlp("java", "java","-version", NULL);

        //使用路径执行
        execl("G:\\HuanJing\\JDK\\jdk1.8.0_131\\bin\\java", "java", "-version", NULL);

        perror("进程创建失败");
    } else if (pid > 0) {
        //进入父进程执行
        printf("当前进程id:%d 父进程id:%d fork返回值:%d\n", getpid(), getppid(), pid);
    } else {
        perror("进程创建失败");
        exit(1);
    }

}

void toFile() {
    int fd2 = open("../aa.txt", O_RDWR | O_WRONLY | O_CREAT | O_TRUNC);
    //复制fp -》 STDOUT_FILENO
    //将STDOUT_FILENO指向aa文件,所有输出到控制台的都输出到aa中
    dup2(fd2, STDOUT_FILENO);
    //使用路径执行
    execl("G:\\HuanJing\\JDK\\jdk1.8.0_131\\bin\\java", "java", "-version", NULL);

}

void waits() {
    pid_t pid = fork();
    //进入子进程执行
    if (pid == 0) {
        sleep(5);
        printf("当前进程id:%d \n", getpid());
    } else if (pid > 0) {
        //退出状态
        int status;
        //等待子进程退出.然后回收,就是回收PCB
        //        pid_t ip = wait(&status);

        /*
         *不等待pid子进程退出.就返回
         * pid设置-1：回收指定进程组内的任意子进程,也就是和主进程同组的子进程 ,0:回收和当前调用waitpid同组子进程,>1回收指定ID的子进程
         * 返回值>0:正常回收
         * 0:指定了WNOHANG,且没子进程结束
         * -1:失败
         *
         * WNOHANG:不阻塞,直接返回
         * 设置0:则阻塞
         */
        pid_t ip;
        //循环回收所有子进程
        while ((ip = waitpid(-1, &status, WNOHANG)) != -1) {
            printf("当前退出的子进程id:%d \n", ip);
        }


        printf("当前退出的子进程id:%d \n", ip);
        if (WIFEXITED(status)) {
            printf("子进程正常终止%d \n", WIFEXITED(status));
        }
        if (WEXITSTATUS(status)) {
            //获得exit编号
            printf("子进程正常终止%d \n", WIFEXITED(status));
        }
        if (WIFSIGNALED(status)) {
            //kill -9 9就是信号编号
            //获得信号编号
            printf("子进程信号终止%d \n", WIFSIGNALED(status));
        }
    }
}

void createPGroup() {
    pid_t pid;
    if ((pid = fork()) < 0) {
        perror("error");
        exit(1);
    }
    //关闭主进程
    if (pid > 0)exit(0);
    printf("pid: %d  pgid: %d sid: %d \n", getpid(), getpgid(0), getsid(0));

    //创建会话
    if ((pid = setsid()) == -1) {
        perror("error");
        exit(1);
    }
    printf("pid: %d  pgid: %d sid: %d \n", getpid(), getpgid(0), getsid(0));

    //改变工作目录位置,主要是为了防止执行的工作目录被卸载.通过设置目录,确保工作目录不被卸载.
    int ret = chdir("E:\\workspace\\c\\14fork");
    if (ret == -1) {
        perror("error");
    }

    //改变文件访问权限掩码
    umask(0022);

    //关闭or重定向文件描述符
    //因为现在是后台执行,没有终端.所以不需要终端的输入输出文件描述符.
    close(STDIN_FILENO);
    int fd = open("/dev/null", O_RDWR);
    if (fd == -1) {
        perror("error");
    }
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);

    while (1);


}

int main() {

    //创建进程
//    forkThread();

//循环创建进程
//    forFork();

//执行exec
//    exec();

//输出控制台数据到文件中
//toFile();

//回收子进程终止信息
//   waits();

//创建会话,守护线程
    createPGroup();

    return 0;
}

