#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <assert.h>
#include <stdarg.h>

#include <limits.h>

/**
 * C预处理器和C库
 * @return
 */

#define MOD "hhaha"
//类函数宏
//宏标识符(参数)
#define SQUARE(X) X*X
#define SQUARE2(X) (X)*(X)
#define SQUARE3(X) ((X)*(X))
//#Y打印被替换的值
#define PR(Y) printf("%d --- "#Y "---- \n",Y)

//##:将2个记号组成一个记号
#define SQ(n) x ## n
#define PR2(n) printf("x" #n " = %d \n",x ## n)

#define PR3(n, ...) printf(#n " : " __VA_ARGS__)

//如果定义了xxx
#ifdef xxx
//取消在这之前声明的xxx
#undef xxx
#else
#undef xxx
#endif

#if defined(xxx)
#undef xxx
#elif defined(xxxa)
#undef xxx
#else
#undef xxx
#endif

//泛型
//x跟每个类型匹配,匹配不上则选择默认的
#define MYTYPE(X) _Generic((X),int:"int",float:"float",default:"other")
//泛型的值就是函数名,通过函数指针调用
#define MYTYPE2(X) _Generic((X),long double:sqrtl,default:sqrt)(X)
//函数调用
#define MYTYPE3(X) _Generic((X),long double:sqrtl((X)),default:sqrtl(X+1))

#define NODEBUG

inline static void ea() {
    printf("aa");
}

int comp(const void *a, const void *b);

double sum(int lim, ...);

int main() {
//双引号中的宏,不被预处理替换
    printf("MOD \n");
    printf("%s \n", MOD);
    /*
    int len = 5;
    int a[len];
    const int len2 = 5;
    static int a2[len2];
    const int a3 = len2 * 3;
*/
    PR(SQUARE(2));
    //这里被编译成2+3*2+3=11
    PR(SQUARE(2 + 3));
    //添加括号解决
    PR(SQUARE2(2 + 3));
    //被编译成100/2*2=100
    PR(100 / SQUARE2(2));
    //解决
    PR(100 / SQUARE3(2));
    //依旧存在问题
    int x = 5;
    PR(SQUARE3(++x));
    PR(x);


    //##:将2个记号组成一个记号
    int SQ(1) = 10;//变成 int x1=10;
    PR2(1);//x1 = 10

    //----------------------变参宏
    double xx = 47;
    //宏展开后变成
    //printf("1" " : " "x= %g \n", xx)
    PR3(1, "x= %g \n", xx);


    //如果没定义了xxx
#ifndef xxx
    PR3(1, "x= %g \n", xx);
#endif

    //-----------------------预定义宏

    //重置当前行号为1O00,文件名为aaa
#line 1000 "aaa"

    printf("%d \n", __LINE__);
    printf("%s \n", __DATE__);
    printf("%s \n", __TIME__);


//#error not 14


//--------------------泛型
    printf("%s \n", MYTYPE(4));

    long double x5 = 4.0;
    printf("%LF   \n", MYTYPE2(x5));
    printf("%LF  \n", MYTYPE3(x5));



    //-----------------内联函数
    //像宏函数一样,编译到main方法中
//    ea();
//-------------------------    _Noreturn 直接结束程序,不返回给main,相当于exit


    //----------------------exit退出时候调用
    atexit(ea);


    //------------------------- 排序
    int vals[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    for (int i = 0; i < 10; i++) {
        printf("%d %d \n", i, vals[i]);
    }
    printf("\n");
    qsort(vals, 10, sizeof(int), comp);
    for (int i = 0; i < 10; i++) {
        printf("%d %d \n", i, vals[i]);
    }


    //------------------------使用断言
//    assert(1 > 2);
    //#define NODEBUG 关闭断言
    //编译时断言,若错误则编译器显示该语句
    _Static_assert(3 > 2, "错误");



    //----------------------可变参数
    printf("\n");
    double s = sum(3, 1.2, 10.3, 3.3);
    printf("%g \n", s);

    return 0;

}

int comp(const void *a, const void *b) {
    const int *c = (const int *) a;
    const int *d = (const int *) b;
    if (*c < *d) {
        return 1;
    } else {
        return -1;
    }
}

double sum(int lim, ...) {
    //声明一个储存参数的对象
    va_list ap;
    va_list cp;
    double tot = 0;
    //把ap初始化为参数列表
    va_start(ap, lim);
    for (int j = 0; j < lim; j++) {
        //检索参数
        tot += va_arg(ap, double);
    }
    //释放ap后,cp依旧可以索引到
    va_copy(cp,ap);
    //释放ap
    va_end(ap);
    return tot;
}