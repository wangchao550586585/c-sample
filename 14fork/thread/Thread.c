#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/**
 * 线程,linux下也称之为轻量级进程
 * 在内核层面看来,进程和线程是一样的，但线程PCB独立,三级页表(虚拟地址,三级为:页面-页表-页目录)相同
 * 线程可看做是寄存器和栈(用户栈,内核栈)的集合
 *
 * 多个线程也会和其他进程争抢CPU时钟.
 * 当进程创建线程后,自身也会成为线程.
 *
 * 进程分配资源的
 * 线程执行工作的
 * 进程可看做拥有一个线程的进程.
 *
 * 线程引入信号机制后,谁抢到谁处理,可通过设置线程屏蔽字指定谁处理,未决信号集共享
 *
 * @return
 */

//共享全局变量
int var = 100;

void *run(void *arg) {
    int i;
//
    var = 200;
    i = (int *) arg;
    if (i == 2) {
//        exit(1);//退出进程
//        return NULL;   //返回调用者
        //退出当前线程,返回的指针必须全局或者malloc分配的,
        // 不能再线程函数的栈上分配,因为其他线程获得指针后,线程已经退出.
        pthread_exit(NULL);
    }
    printf("pid=%d tid=%lu %d  var=%d \n", getpid(), pthread_self(), i, var);

    /*
    i = *((int *) arg);
   sleep(i);
   printf("pid=%d tid=%lu %d var=%d \n", getpid(), pthread_self(), i, var);
    */

}

void demo() {
    printf("pid=%d tid=%lu \n", getpid(), pthread_self());
    pthread_t tid;
    int ret, i;
    for (i = 0; i < 5; i++) {

//        ret = pthread_create(&tid, NULL, run, (void *) &i);     //地址传递,会共享主线程变量值
        ret = pthread_create(&tid, NULL, run, (void *) i);   //值传递
        if (ret != 0) {
            perror("error");
        }
    }
    printf("var=%d \n", var);
    //防止主线程执行结束后,清空子线程数据
//    sleep(i);

//退出当前线程,不会影响到子线程
    pthread_exit(NULL);
}

struct thre {
    int var;
    char str[256];
};

void *fun(void *args) {
    //利用形参,返回
    struct thre *t = (struct thre *) args;
    t = malloc(sizeof(t));
    t->var = 100;
    strcpy(t->str, "hello");

    //返回一个动态分配的指针
/*   struct thre *t;
    t = malloc(sizeof(t));
    t->var = 100;
    strcpy(t->str, "hello");*/

//返回一个非动态分配的指针.可以发现无法接收,因为局部地址,线程结束后,就回收了
    /*   struct thre t;
       t.var = 10;
       strcpy(t.str, "hello");
       printf("var=%d str=%s \n", t.var, t.str);
           return (void *) &t;
       */

    //返回一个整型
//    int t = 10;
    return (void *) t;
}

void demo2() {
    pthread_t *tid;
    pthread_t arg;
    int ret = pthread_create(&tid, NULL, fun, (void *) &arg);
    if (ret != 0) {
        perror("error");
        exit(1);
    }
    struct thre t2;
    struct thre *t = &t2;
    //类似waitpid,阻塞等待线程退出,回收线程
    pthread_join(tid, (void **) &t);
    printf("var=%d str=%s \n", t->var, t->str);


    //接收一个整型
/*    int t2;
    int *t = &t2;
    pthread_join(tid, (void **) &t);
    printf("var=%d  \n", t);
 */

    pthread_exit(NULL);
}

void *fun3(void *args) {
    while (1) {
        /* printf("pid=%d tid=%lu \n", getpid(), pthread_self());
         sleep(1);*/
        //添加取消点
        pthread_testcancel();
    }
    return (void *) 66;
}

void demo3() {
    pthread_t pthread;
    int ret = pthread_create(&pthread, NULL, fun3, NULL);
    if (ret != 0) {
        perror("error");
        exit(1);
    }
    printf("pid=%d tid=%lu \n", getpid(), pthread_self());
    sleep(5);
    /**终止线程
     *函数若没进入内核,则会不终止.sleep()之所以有效,是因为进入了内核.
     *可添加取消点,会进入内核检测是否取消.
     */
    ret = pthread_cancel(pthread);
    void *tret = NULL;
    pthread_join(pthread, &tret);
    printf("%d \n", tret);
    if (ret != 0) {
        perror("error");
        exit(1);
    }
    while (1);
}

void *fun4(void *args) {
//    while (1);
    return (void *) 66;
}

void demo4() {
    pthread_t pthread;
    int ret = pthread_create(&pthread, NULL, fun4, NULL);
    if (ret != 0) {
        fprintf(stderr, "error: %s \n", strerror(ret));
        exit(1);
    }
    /**
     *线程分离
     * 线程终止会自动清理。无需回收
     */
    ret = pthread_detach(pthread);
    if (ret != 0) {
        fprintf(stderr, "error: %s \n", strerror(ret));
        exit(1);
    }
    sleep(1);
    //分离后,join会抛出参数无效,这是因为线程分离后,线程直接执行完回收了.
    ret = pthread_join(pthread, NULL);
    if (ret != 0) {
        fprintf(stderr, "error: %s \n", strerror(ret));
        exit(1);
    }
    pthread_exit((void *) 0);
}

void demo5() {
    int ret;
    //初始化属性
    pthread_attr_t pa;
    ret = pthread_attr_init(&pa);
    if (ret != 0) {
        fprintf(stderr, "error: %s \n", strerror(ret));
        exit(1);
    }

    //设置分离属性
    ret = pthread_attr_setdetachstate(&pa, PTHREAD_CREATE_DETACHED);
    if (ret != 0) {
        fprintf(stderr, "error: %s \n", strerror(ret));
        exit(1);
    }

    pthread_t pthread;
    ret = pthread_create(&pthread, &pa, fun4, NULL);
    if (ret != 0) {
        fprintf(stderr, "error: %s \n", strerror(ret));
        exit(1);
    }

    //销毁属性
    ret = pthread_attr_destroy(&pa);
    if (ret != 0) {
        fprintf(stderr, "error: %s \n", strerror(ret));
        exit(1);
    }

    ret = pthread_join(pthread, NULL);
    if (ret != 0) {
        fprintf(stderr, "error: %s \n", strerror(ret));
        exit(1);
    }


    printf("pid=%d tid=%lu  \n", getpid(), pthread_self());
    pthread_exit((void *) 0);
}

int main() {
    //创建线程
//    demo();

//测试pthread_join
//    demo2();

//cancel取消线程
//    demo3();

//线程分离
//    demo4();

    //通过设置线程属性分离
//    demo5();



    return 0;
}