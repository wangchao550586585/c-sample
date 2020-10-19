#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

static void initNode(Node *pnew, Item item);

void init(Queue *pQueue) {
    pQueue->front = pQueue->tail = NULL;
    pQueue->items = 0;
}

bool QueueIsFull(Queue *pQueue) {
    return pQueue->items == MAXQUEUE;
}

bool AddItem(Item item, Queue *pq) {
    if (QueueIsFull(pq)) { return false; }

    Node *pnew = (Node *) malloc(sizeof(Node));
    if (pnew == NULL) {
        printf("内存不足");
        exit(1);
    }

    pnew->item = item;
    pnew->next = NULL;
    if (QueueIsEmpty(pq)) {
        pq->front = pnew;
    } else {
        //头插法
        /*   node->next = p->front;
           p->front = node;*/

        //尾插法
        pq->tail->next = pnew;
    }
    pq->tail = pnew;
    pq->items++;
    return true;
}

bool QueueIsEmpty(Queue *pQueue) {
    return pQueue->items == 0;
}

bool DeItem(Item *pitem, Queue *pq) {
    if (QueueIsEmpty(pq))return false;

    *pitem = pq->front->item;

    Node *pt = pq->front;

    pq->front = pq->front->next;

    free(pt);

    pq->items--;
    if (pq->items == 0) {
        pq->tail = NULL;
    }
    return true;
}


void EmptyTheQueue(Queue *pQueue) {
    Item du;
    while (!QueueIsEmpty(pQueue))DeItem(&du, pQueue);
}

