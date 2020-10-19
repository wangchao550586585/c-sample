#include <stdio.h>
#include <stdlib.h>
#include "main11_link2.h"

static void initNode(Node *pnew, Item item);

void init(List *plist) {
    *plist = NULL;
}

bool listIsFull(List *plist) {
    Node *pt;
    bool full;
    pt = (Node *) malloc(sizeof(Node));
    full = pt == NULL ? true : false;
    free(pt);
    return full;
}

bool AddItem(Item item, List *plist) {
    Node *pnew;
    Node *scan = *plist;

    //分配内存
    pnew = (Node *) malloc(sizeof(Node));
    if (pnew == NULL)return false;

    //新节点赋值
    initNode(pnew, item);

    //如果是新节点,则直接设置
    if (scan == NULL) {
        *plist = pnew;
    } else {
        //添加到链表末尾
        while (scan->next != NULL) {
            scan = scan->next;
        }
        scan->next = pnew;
    }
    return true;
}

bool ListIsEmpty(List *plist) {
    return *plist == NULL ? true : false;
}

void Traverse(const List *plist, void (*pfun)(Item item)) {
    Node *pnode = *plist;
    while (pnode != NULL) {
        (*pfun)(pnode->item);
        pnode = pnode->next;
    }
}

void EmptyTheList(List *plist) {
    Node *psave;
    while (*plist != NULL) {
        psave = (*plist)->next;
        free(*plist);
        *plist = psave;
    }
}

static void initNode(Node *pnew, Item item) {
    //新节点赋值
    pnew->item = item;
    pnew->next = NULL;
}