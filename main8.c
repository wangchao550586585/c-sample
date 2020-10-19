#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN 40

/**
 * 结构和其他数据形式
 * @return
 */
#define MAXTITL 41
#define MAXAUTH 41
#define ArrSize 2

void To(char *);

void To2(void(*fp)(char *), char *str);

//定义结构体
struct book {
    char title[MAXTITL];
    char author[MAXAUTH];
    float value;
};

struct lib {
    struct book books;
    int count;
};
//定义和声明都可以一起,带book2标记的还可以在函数声明.
/*struct book2 {
    char title[MAXTITL];
    char author[MAXAUTH];
    float value;
} book2;*/
//省略了标记名的,不能再其他函数声明.
/*struct {
    char title[MAXTITL];
    char author[MAXAUTH];
    float value;
} book3;*/

//使用指针代替字符串数组
struct names {
    char *name;
};

int sum(const struct book *);

int sum2(const struct book);

//返回结构体,参数必须声明为结构体,而不是指针
struct book sum3(struct book);

//伸缩性数组
struct array {
    int i;
    //注意这里没初始化容量
    int arr[];
};
typedef unsigned char *BYTE;


int main() {
    //声明,这里才会分配空间
    struct book library;
    char *ti = library.title;
//    ti[0] = "书名";
    *ti = 'a';
    *(ti + 1) = 'b';
    ti[2] = 'c';
    ti[3] = '\0';
    *library.author = 'a';
    *(library.author + 1) = 'B';
    library.author[2] = '\0';

    library.value = 2.2f;
    printf("%s,%s,%f \n", library.title, library.author, library.value);

    struct book library2 = {
            "haha", "hehe", 1.95
    };
    struct book library3 = {
            .title="haha",
            .author="hehe", 2.36
    };

    //------------------嵌套结构
    struct lib libs[2] = {{
                                  {
                                          .title="haha",
                                          .author="hehe", 2.36
                                  },
                                  10
                          },
                          {
                                  {
                                          .title="haha",
                                          .author="hehe", 2.77
                                  },
                                  10
                          }
    };
    printf("%s \n", libs[0].books.author);

    struct lib *libpoint = &libs[0];
    printf("%p %p %p  %p\n", libs, &libs, &libs[0], &libs[1]);
    printf("%p  %p\n", libpoint, libpoint + 1);
    printf("%f %f \n", libpoint->books.value, (*libpoint).books.value);
    libpoint++;
    printf("%f  \n", libpoint->books.value);


    //和数组不同,结构名不是结构的地址
    struct book *bb = &library;
    printf("%p %p %d \n", library, &library, sum(&library));


    //当结构类型一样,可直接赋值给另外一个结构,数组不允许这样
    struct book bs = library;
    struct book bs2 = sum3(library);
    printf("%f", bs2.value);


    //--------------------------结构体采用指针代替数组
    //存在风险,指针尚未初始化,位置不定.会造成程序异常.
    //解决办法,通过malloc分配空间
    char temp[10] = "aaa";
    struct names *ptr;
/*
    ptr->name = (char *) malloc(strlen(temp) + 1);
    strcpy(ptr->name, temp);
    free(ptr->name);
*/

//----------------复合字面量和结构
    struct names {
        char *name;
    } ss = {"haha"};

//    可直接传递给方法
//传递结构体指针
    sum(&(struct book) {
            .title="haha",
            .author="hehe", 2.36
    });
    //直接传递结构体
    sum2((struct book) {
            .title="haha",
            .author="hehe", 2.36
    });

    //----------------伸缩性数组
    /*
     * 就是初始化结构体时,不设置数组容量,等到自己分配.
     * 数组必须最后一个元素
     * 不能用结构直接赋值,这样只能拷贝除数组其他内容
     * 不要参数传递.
     * 带数组的结构不要嵌套成为别的结构
     */

    //数组分配了10个长度
    struct array *arr = malloc(sizeof(struct array) + 10 * sizeof(int));
    free(arr);

    //----------------匿名结构
/*    struct names {
        char *name;
        struct {
            char *name2;
        };
    } na;
    //访问
    na.name2;*/


    //-------------------------序列化与反序列化.
    struct names namesArr[ArrSize] = {
            {
                    "haha"
            },
            {
                    "heh"
            }
    };
    FILE *file = fopen("E:\\workspace\\c\\aa.txt", "a+b");
    int count = 0;
    while (count < ArrSize) {
        fwrite(&namesArr[count], sizeof(struct names), 1, file);
        count++;
    }
    struct names namesArr2[ArrSize];
    count = 0;
    rewind(file);
    while (count < ArrSize) {
        fread(&namesArr2[count], sizeof(struct names), 1, file);
        count++;
    }
    printf("\n %s %s \n", namesArr2[0].name, namesArr2[1].name);


    //-------------------------使用联合
//    同一内存空间存储不同类型,只能存一个值
    union hold {
        int d;
        //因为double占用空间最大,所以hold8字节
        double big;
    };
    union hold val;
    val.big = 2.0;
    union hold val1 = val;//另一个联合来初始化
    union hold val2 = {22};//初始化d
    union hold val3 = {.big=22};//指定初始化器
    val3.big = 10;//清除了val3的d值
    //指针访问
    union hold *val4 = &val3;
    val4->big = 20;

    //------------------------------------枚举
    //默认赋值从0开始,也可以手动设置值
    enum color {
        red, orange, yellow = 2
    };
    char *colors[] = {"red", "orange", "yellow"};
    char ar[] = "red";
    enum color co = orange;
    int i = 0;
    for (i = red; i < yellow; i++) {
        if (strcmp(ar, colors[i]) == 0) {
            break;
        }
    }
    switch (i) {
        case red:
            printf("%d \n", i);
            break;
        case orange:
            printf("%d \n", i);
            break;
        case yellow:
            printf("%d \n", i);
            break;
    }

    //----------------------------------typedef定义类型
    BYTE x;
//与define区别:
//BYTE x,y <==> char * x,*y;
//而define 只是变成 char * x,y;

//typedef还可以简化结构
    typedef struct c {
        float x;
        float y;
    } COM;
    COM c;

    //给复杂类型命名
    //* 指针
    //() 表示函数
    //[] 表示数组
    //优先级排列
    // [] 等于 () 大于 *
    int(*uof[3])[4];//说明是一个内含3个int类型指针的数组,每个指针指向一个内含4个int的元素数组.
    char (*f[3])(int);//指向内含3个指针元素的数组,每个元素都是char函数
    char *f2(int);//变成了函数声明,意思是返回char 指针的函数

    typedef int arr5[5];
    typedef arr5 *p;
    typedef p arr10[10];
    arr5 t;//包含5个int元素的数组
    p p2; //一个指针,指向包含5个int元素的数组
    arr10 ap;//包含10个指针的数组,每个指针指向包含5个int元素的数组

//定义函数,传递函数
    /*typedef void (*PF)(char *);
    void show(PF pf1);
*/
    //------------------------------------函数和指针
    //声明数组需要告知数组类型
    //声明函数指针,则需要告知函数类型,以及函数签名(返回值,参数类型)
//    void To(char *);
//函数指针如下
    void (*pf)(char *) =To;
    (*pf)("hah");
    //因为函数名是指针,所以2者等价.
    pf("hah");

    //传递调用
    To2(pf, "hah");
    To2(To, "hah");

    return 0;
}

int sum(const struct book *su) {
    return su->value + su->value;
}

//book为传递过来的值得副本
int sum2(const struct book book) {
    return book.value + book.value;
}

struct book sum3(struct book book) {
    book.value = 10;
    return book;
}

void To(char *ch) {

}

void To2(void(*fp)(char *), char *str) {
    //调用
    (*fp)(str);

}