
//避免重复导入
#ifndef LIST_H_
#define LIST_H_
#include <stdbool.h>

#define TSIZE 45
struct film {
    char title[TSIZE];
    int rating;
};
typedef struct film Item;
typedef struct node {
    Item item;
    struct node *next;
} Node;
/*typedef struct list {
    Node *head;
    int size;
} List;*/
typedef Node *List;

void init(List *plist);

bool listIsFull(List *plist);

bool AddItem(Item temp, List *plist);

bool ListIsEmpty(List *plist);

void Traverse(const List *plist, void (*pfun)(Item item));

void EmptyTheList(List *plist);


#endif