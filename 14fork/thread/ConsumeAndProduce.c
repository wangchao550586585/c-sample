#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/**
 * 使用条件变量+互斥锁实现生产者消费者队列
 * @return
 */
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;
struct msg {
    struct msg *next;
    int num;
};
struct msg *head;

void *consumer(void *p) {
    struct msg *m;
    while (1) {
        pthread_mutex_lock(&lock);
        /**若修改为if会发生如下情况:
         * AB线程阻塞在这里.
         * C线程生产
         * A线程接收到信号,唤醒.
         * 执行消费.
         * 此时B线程也接收到信号,因为A线程抢到了锁,所以阻塞条件变量变成阻塞等待锁释放上
         * 当A释放锁,则B获取到锁.然后若head没值,则会异常.
         *
         */
        while (head == NULL) {
            pthread_cond_wait(&condition, &lock);
        }
        m = head;
        head = m->next;
        pthread_mutex_unlock(&lock);
        printf("consumer id=%lu,val=%d \n",pthread_self(), m->num);
        free(m);
        sleep(rand() % 3);
    }
}

void *producer(void *p) {
    struct msg *m;
    while (1) {
        m=malloc(sizeof(struct msg));
        m->num = rand() % 3;

        pthread_mutex_lock(&lock);

        m->next = head;
        head = m;
        printf("produce val=%d \n", m->num);
        pthread_mutex_unlock(&lock);

        //唤醒阻塞在条件变量上的至少一个线程
        pthread_cond_signal(&condition);
        //唤醒阻塞条件变量上的所有线程
//        pthread_cond_broadcast(&condition);

        sleep(rand() % 2);
    }
}

int main() {
    pthread_t pid, cid,cid2,cid3;

    pthread_create(&pid, NULL, producer, NULL);
    pthread_create(&cid, NULL, consumer, NULL);
    pthread_create(&cid2, NULL, consumer, NULL);
    pthread_create(&cid3, NULL, consumer, NULL);

    pthread_join(pid, NULL);
    pthread_join(cid, NULL);
    pthread_join(cid2, NULL);
    pthread_join(cid3, NULL);
    return 0;
}