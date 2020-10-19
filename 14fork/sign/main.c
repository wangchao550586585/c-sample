
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <sys/time.h>
#include <wait.h>

/**
 * 信号
 *      满足条件才能发送
 * 每个进程收到的所有信号,都是内核负责发送的.
 * 类似硬件中断
 *
 * 产生信号原因
 *      按键产生,如CTRL+X
 *      系统调用，如KILL,ABORT
 *      软件产生,如定时器，sleep
 *      硬件异常,1/0,内存对齐错误
 *      命令产生,kill命令
 *
 * 递达:信号到达目标进程
 * 未决:处于产生和递达中间状态,主要由于阻塞(屏蔽)导致
 * 处理
 *         1:执行默认动作
 *         2:忽略
 *         3:捕捉(调用用户处理函数)
 *
 * 阻塞信号集和未决信号集 位图形式 存储PCB中
 * 阻塞信号集:将信号加入集和,对他们设置屏蔽,除非解除屏蔽才会执行此信号
 * 未决信号集:信号产生后,未决信号集中描述该信号的位标记1,当信号处理后,重置0.
 *           在信号产生后由于某些原因(如阻塞)不能抵达,则称之为未决信号.解除屏蔽前,一直处于未决状态.
 *
 * kill -l查看信号
 * kill -信号编号  执行默认动作
 * 信号不能忽略
 *      SIGSTOP
 *      SIGKILL
 * 用户自定义信号
 *      SIGUSR1/USERR2
 * 子进程状态变化通知父进程,默认动作忽略
 *      SIGCHLD信号产生条件
 *               子进程终止时
 *               子进程收到SIGSTOP信号停止时
 *               子进程处于停止态,接收到SIGCONT后唤醒时
 * 按键产生的信号
 *      SIGINT      Ctrl+c      终止/中断
 *      SIGTSTP     Ctrl+z      暂停/停止
 *      SIGQUIT     Ctrl+\      退出
 * 硬件异常信号
 *      SIGFPE      浮点数错误,如除0
 *      SIGSEGV     段错误,非法访问内存
 *      SIGBUS      总线错误
 *
 *
 *
 * @return
 */

void killTest() {
    pid_t pid = fork();
    if (pid == 0) {
        /*
       给指定进程发送信号
            pid>0,发送指定进程
            =0,发送信号给与调用kill函数进程属于同一进程组的所有进程
            <-1,取pid发送对应进程组
            =-1,发送给进程有权限发送的系统中所有进程
        */
//        kill(getppid(), SIGCHLD);
    } else if (pid > 0) {

    }
}

void myfunc(int sign) {
    switch (sign) {
        case SIGALRM:
            printf("捕捉到了SIGALRM:%d \n", sign);
            break;
    }
}

/**
 * 设置定时
 * 以及捕捉信号,执行回调函数.
 */
void alarmTest() {
    /* 一个进程只有一个定时器,存PCB中
  *      延时多少秒发送SIGALRM信号,给当前执行进程
  *      返回上次设置的延时到如今,还剩下多少秒数
     *
     *      time 命令可查看程序运行时间
     *              实际时间=用户时间+内核时间+等待时间
  */

    alarm(1);

    //捕捉信号,以及执行方法
//    signal(SIGALRM, myfunc);
    struct sigaction act, old;
    //设置捕捉后回调方法
    act.sa_handler = myfunc;

    /** 捕捉函数执行期间,本信号自动屏蔽.
     sa_mask跟程序阻塞信号集二者不可同日而语,samask生命周期只在回调函数执行期间,执行完后.会恢复阻塞信号集.
     阻塞的常规信号不支持排队,当捕捉函数执行结束,恢复阻塞信号集后,多次执行的信号只会执行一次.(linux kill -l 手册中后32信号支持排队)*/


    //设置捕捉函数执行期间,屏蔽的信号集
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGQUIT);
    /**
     * 中断系统调用:
     *      系统调用分为2种，慢速系统和其他系统调用
     * 慢系统调用
     *      可能造成永远阻塞的情况,如:wait()或者write()read()...
     *      这个时候执行中断后,当中断回调函数执行结束,应该返回到阻塞函数.
     *      可设置flags=SA_RESTART
     *  sigaction:
     *      sa_flags:
     *        SA_RESTART:系统中断后重启
     *        SA_NODEFER:不屏蔽当前信号
     *
     *  其他系统调用：
     *      getPid,fork
     */
    //设置0,表示回调执行期间,屏蔽捕捉的当前信号
    //之所有这个参数,是为了避免,回调函数执行期间,捕捉了当前信号,然后递归.内存溢出.
    act.sa_flags = 0;


    int ret = sigaction(SIGALRM, &act, &old);
    if (ret == -1) {
        perror("error");
    }
    for (int i = 0;; ++i) {
        sleep(1);
        printf("%d \n", i);
    }
}


int setitimerTest() {
    struct itimerval it, oldit;

    signal(SIGALRM, myfunc);

    //第一次执行定时2秒钟,只有每次间隔5S执行一次定时器
    //定时时长
    it.it_value.tv_sec = 2;
    it.it_value.tv_usec = 0;
    //2次定时任务之间间隔时间
    it.it_interval.tv_sec = 5;
    it.it_interval.tv_usec = 0;

    /*
     * ITIMER_REAL      发送SIGLARM信号        计算自然时间
     * 	ITIMER_VIRTUAL	SIGVTALRM             计算进程占用CPU时间
        ITIMER_PROF     SIGPROF               计算占用CPU时间+系统调用时间

        it:设置的定时时间
        oldit:传出参数，记录上次剩余时间


     */
    if (setitimer(ITIMER_REAL, &it, &oldit) == -1) {
        perror("error");
        return -1;
    }
    while (1);
}

/**
 * 设置未决信号和阻塞信号集
 */
void sigset() {
    sigset_t set, oldset, pedset;
    //清空信号集
    sigemptyset(&set);
    //将一个信号添加到集合中
    sigaddset(&set, SIGINT);
    //设置信号屏蔽字
    /*SIG_BLOCK:设置阻塞
    /*SIG_UNBLOCK:设置不阻塞
    /*SIG_SETMASK:自定义set替换mask

     oldset:旧的mask
     */
    int ret = sigprocmask(SIG_BLOCK, &set, &oldset);
    if (ret == -1) {
        perror("error");
    }
    while (1) {
        sleep(1);
        //查看未决信号集
        ret = sigpending(&pedset);
        if (ret == -1) {
            perror("error");
        }
        //遍历打印
        for (int i = 0; i < 32; i++) {
            //判断一个信号是否在未决信号集合中
            if (sigismember(&pedset, i)) {
                putchar('1');
            } else {
                putchar('0');
            }
        }
        printf("\n");
    }
}

void myfunc2(int sign) {
    pid_t pid;
    //当同一个时间点,多个进程死了,发送信号.当回调函数执行结束后,只会执行一个.
//     pid = wait(NULL);

//每次执行回收多个子进程
    int status;
    pid_t ip;
//    while ((pid = wait(NULL)) != -1)
    while ((ip = waitpid(-1, &status, WNOHANG)) != -1) {
        if (WIFEXITED(status)) {
            printf("子进程正常终止%d  返回的值为:%d \n", ip, WEXITSTATUS(status));
        }
    }

}

int clearChild() {
    //设置阻塞
    sigset_t set, oldset;
    sigemptyset(&set);
    sigaddset(&set, SIGCHLD);
    int ret = sigprocmask(SIG_BLOCK, &set, &oldset);
    if (ret == -1) {
        perror("error");
    }


    pid_t pid;
    int i;
    for (i = 0; i < 5; i++) {
        if ((pid = fork()) == 0)break;

    }
    if (5 == i) {
        sleep(4);
        struct sigaction act;
        act.sa_handler = myfunc2;
        sigemptyset(&act.sa_mask);
        /**
         *
         */
        act.sa_flags = 0;
        sigaction(SIGCHLD, &act, NULL);

        //恢复阻塞
/* int ret = sigprocmask(SIG_SETMASK, &oldset, NULL);
        if (ret == -1) {
            perror("error");
        }*/
        sigprocmask(SIG_UNBLOCK, &set, NULL);

        printf("我是父进程 \n");
        //父进程先结束,注册的捕捉信号就清空了,这里让父进程不要结束.
        while (1);
    } else {
//        sleep(1);
        printf("我是子进程:%d \n", getpid());
        return i;
    }
}

int main() {
    //操作kill函数
    //    killTest();


/* 设置定时
 以及捕捉信号,执行回调函数.*/
//    alarmTest();
    //操作重复定时
//        setitimerTest();

    //设置未决信号和阻塞信号集
    //    sigset();



    //回收子进程
    return clearChild();
    return 0;
}