#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/**
 * 数据结构
 * @return
 */
#define TSIZE 45
#define FMAX 500

struct film {
    char title[TSIZE];
    int rating;
};

int main() {
    //直接声明数组容量,限制了数组大小
    struct film f[FMAX];

    //解决办法,动态分配,每个指针地址有序,可根据指针定位访问
    //缺点:浪费内存
    int n = 10;
    struct film *f2 = (struct film *) malloc(n * sizeof(struct film));

    //动态为每个结构分配一次
    //存在问题每个指针地址是乱序的.无法根据顺序访问
    //优点:因为只存储指针,大大节省内存.
    struct film *move[n];
    for (int i = 0; i < n; i++) {
        move[i] = (struct film *) malloc(sizeof(struct film));
    }



    return 0;
}