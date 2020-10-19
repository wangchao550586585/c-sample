#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "main11_link2.h"

void showmovies(Item item);

int main() {
    List movies;
    Item temp;
    init(&movies);
    if (listIsFull(&movies)) {
        printf("满了~");
        exit(1);
    }
    for (int i = 0; i < 4; ++i) {
        char str[2];
        str[0] = '0' + i;
        str[1] = '\0';

        strcpy(temp.title, str);
        if (AddItem(temp, &movies) == false) {
            printf("添加错误");
            exit(1);
        }
    }

    if (ListIsEmpty(&movies)) {
        printf("没值");
    }
    Traverse(&movies, showmovies);

    //释放
    EmptyTheList(&movies);
    return 0;
}

void showmovies(Item item) {
    printf("%s： \n", item.title);
}


