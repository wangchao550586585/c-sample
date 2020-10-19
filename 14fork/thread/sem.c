#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/**
 * 信号量
 * @return
 */
#define  NUM 5
int queue[NUM];
sem_t blank_number, product_number;

void *consumer(void *p) {
    int i = 0;
    while (1) {
        //初次调用,这里一直阻塞,等待生产后释放信号量.相当于独占锁
        sem_wait(&product_number);
        queue[i] = rand() % 1000 + 1;
        printf("consumer %d \n", queue[i]);
        queue[i]=0;
        sem_post(&blank_number);
        i = (i + 1) % NUM;
        sleep( rand() % 3);
    }
}

void *producer(void *p) {
    int i = 0;
    while (1) {
        //信号量数量-1,为0则阻塞
        sem_wait(&blank_number);
        queue[i] = rand() % 1000 + 1;
        printf("produce %d \n", queue[i]);
        //信号量+=1,为设定数量则阻塞
       sem_post(&product_number);
        i = (i + 1) % NUM;
        sleep( rand() % 1);
    }
}

int main() {
    pthread_t pid, cid;
    /** 信号量
     * 0:线程同步
     * 1:进程同步
     * value:指定同时访问的线程数
     *
     */
    sem_init(&blank_number, 0, NUM);
    sem_init(&product_number, 0, 1);

    pthread_create(&pid, NULL, producer, NULL);
    pthread_create(&cid, NULL, consumer, NULL);

    pthread_join(pid, NULL);
    pthread_join(cid, NULL);

    //销毁信号量
    sem_destroy(&blank_number);
    sem_destroy(&product_number);

    return 0;
}