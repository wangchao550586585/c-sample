#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TSIZE 45
struct film {
    char title[TSIZE];
    int rating;
    struct film *next;
};

int main() {
    struct film *head = NULL;
    struct film *prev, *current;

    current = (struct film *) malloc(sizeof(struct film));
    strcpy(current->title, "hah");
    current->next = NULL;

    head = current;

    prev = current;

    current = (struct film *) malloc(sizeof(struct film));
    strcpy(current->title, "hah2");
    current->next = NULL;

    prev->next = current;

    current = head;
    while (current != NULL) {
        printf("%s \n", current->title);
        current = current->next;
    }

    current = head;
    while (current != NULL) {
        current = head;
        head = current->next;
        free(current);
    }
    return 0;
}


