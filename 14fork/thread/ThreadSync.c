#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/**
 * 线程同步
 * @return
 */
//定义互斥锁
pthread_mutex_t mutex;
//静态初始化
//pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

void *fun(void *arg) {
    srand(time(NULL));

    while (1) {
        //加锁,失败阻塞
        pthread_mutex_lock(&mutex);


        printf(" hello ");
        sleep(rand() % 1);
        printf(" world \n");


        sleep(rand() % 1);

        //解锁
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void lock() {
    srand(time(NULL));

    //动态初始化锁
    int ret = pthread_mutex_init(&mutex, NULL);
    if (ret != 0) {
        fprintf(stderr, "error: %s \n", strerror(ret));
        exit(1);
    }


    pthread_t tid;
    pthread_create(&tid, NULL, fun, NULL);
    while (1) {
        //加锁
        pthread_mutex_lock(&mutex);

        //再次加锁,会造成死锁
        // pthread_mutex_lock(&mutex);

        //试图加锁,失败则立即返回
//        pthread_mutex_trylock(&mutex);

        printf(" HELLO ");
        sleep(rand() % 1);
        printf(" WORLD \n");
        sleep(rand() % 1);

        //解锁
        pthread_mutex_unlock(&mutex);
    }
    pthread_join(tid, NULL);

    //销毁锁
    pthread_mutex_destroy(&mutex);

}


int main() {
    //独占锁
//  lock();

/**
 * 读写锁
 *
 *  pthread_rwlock_init();
    pthread_rwlock_wrlock();
    pthread_rwlock_trywrlock();
    pthread_rwlock_rdlock();
    pthread_rwlock_tryrdlock();
    pthread_rwlock_unlock();
    pthread_rwlock_destroy();
 */




    return 0;
}