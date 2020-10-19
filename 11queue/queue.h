
//避免重复导入
#ifndef Queue_H_
#define Queue_H_

#include <stdbool.h>

#define MAXQUEUE 10;
typedef int Item;

typedef struct node {
    Item item;
    struct node *next;
} Node;

typedef struct queue {
    Node *front;
    Node *tail;
    int items;
} Queue;

void init(Queue *pQueue);

bool QueueIsFull(Queue *pQueue);

bool QueueIsEmpty(Queue *pQueue);

void EmptyTheQueue(Queue *pQueue);

bool AddItem(Item temp, Queue *pQueue);

bool DeItem(Item *temp, Queue *pQueue);

#endif