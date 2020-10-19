#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>


/**
 * 字符串与字符串函数
 * @return
 */
/*
get:容易溢出,丢弃换行
fget:接收全部字符(包括换行符).
scanf:适合混合类型,可限制接收个数

puts:在末尾自动添加换行
fputs:不打印换行符,末尾不添加换行符

*/
#define MSG "hah"

int main() {

    //双引号中的字符,编译器会自动加入\n末尾
    char words[100] = "haha";
    char words2[100] = {'h', 'a', 'h', 'a', '\0'};
    //计算数组大小只能用在初始化数组时,
    char words3[] = "haha";
    const char *ptr = "some";
    //未被使用的元素,自动初始化\0
    putchar(words[6]);
    puts(words);
    puts(words2);
    puts(ptr);


//    words++;在这里words是地址常量,++不能用于常量修改,但是可以words+1
//也可以通过赋值指针,然后如下修改
//初始化数组把静态存储区的字符串拷贝到数组
//初始化指针只是把字符串的地址拷贝给指针.
    char *c = words;
    char *c2 = c++;
    char *c3 = ++c;
    printf("%p %p %p %p \n", c, words + 1, c++, ++c);
    printf("%c %c %c %c \n", *c, *(words + 1), *c2, *c3);

    char ar[] = MSG;
    const char *pt = MSG;
    //就ar地址不一样,这是因为数组会copy hah字符串到数组.
    //而hah字符串地址和MSG,pt地址一样,这是因为编译器把多个相同的字符串保存同一个地址.
    printf("%p %p %p %p \n", MSG, ar, pt, "hah");


    //-----------------------字符串数组
    //内含3个指针的数组,占用24字节
    //直接指向值
    const char *my[3] = {"haha", "heh", "world"};
    //内含10个数组的数组,每个数组占用80个char类型的值,一共占用240字节
    //存的是值的副本,且规则存,不足10的补\0

    //追求访问效率,就指针
    //追求修改,则数组
    const char your[3][10] = {"haha", "heh", "world"};
    for (int i = 0; i < 3; i++) {
        printf("%s %s \n", my[i], your[i]);
    }

//    分配空间
//存在不安全问题:在这里name尚未初始化,可能指向其他地方.
/*    char *name;
    printf("%s %p", name, name);
    scanf("%s", name);
    printf("%s %p", name, name);*/

//    可通过数组分配空间
    char name2[2];
    //gets 只知道数组开始,不知道数组结束,容易溢出.有些编译器溢出直接段异常.
    //gets(name2);
    //puts(name2);

    /*
     *最大可读数量,读取n-1字符,或者读到换行符为止
     *存储换行符
     *指明读入的文件.
     */
    // fgets(name2,10,stdin);
    //不打印换行符,puts会
    //返回值是name2指针,读到末尾,则返回NULL
    //fputs(name2,stdout);
    // puts(name2);

    //fget读取的所有字符都存入缓冲区,然后当按下换行,则将缓冲区数据刷入数组,
    //当数组刷到n-1时,最后一个字符填充为\0,然后输出
    //接着在将缓冲区数据刷入数组,然后直到缓冲区没数据为止

    //不打印换行符.
    /*  while (fgets(name2, 10, stdin) != NULL && name2[0] != '\n') {
          int i = 0;
          while (name2[i] != '\n' && name2[i] != '\0')i++;
          //遇到换行符,则替换空字符.
          if (name2[i] == '\n') {
              name2[i] = '\0';
          } else {
              //遇到空字符,则丢弃输入行剩余字符.
              while (getchar() != '\n')continue;
          }
          puts(name2);
  //        fputs(name2, stdout);
      }*/



    //----------------------常用方法
    //需要导入string.h头
    //strlen
    //strcat:拼接第二个字符串,返回第一个字符串地址
    //strncat:第三个参数,限制拼接字符串数量就停止拼接
    //strcmp:查找到末尾
    //strncmp:第三个参数,限定查找的长度.
    //strcpy():拷贝字符串,相当于赋值操作.返回类型为第一个参数的值
    //strncpy:第三个参数,最大拷贝数量.可能需要自己添加\0


    //stdlib.h
    //atoi:将字符串10变成数字10
    //strtol:将字符串变成long
    char *num = "1000";
    char *val;
    //num待转换指针地址
    //val标识输入数组结束字符的地址
    //10进制
    long val2 = strtol(num, &val, 10);
    //val为空字符,value为0.
    printf("%ld %s  %d \n", val2, val, *val);




    //需要导入stdio.h头
    /* char ch[100];
     //同printf,不同的是输出到数组。
     sprintf(ch, "%d %s", 10, "哈加");
     printf("%s", ch);*/


    //需要导入ctype头
    char *str = words3;
    while (*str) {
        *str = toupper(*str);
        str++;
    }
    printf("%s", words3);

    return 0;

}

//打印字符串
void put(const char *string) {
    while (*string != '\0')putchar(*string++);
}

//打印字符串,并统计个数
int put2(const char *string) {
    int count = 0;
    //指向空字符串时,值为0.比如:'\0'
    while (*string) {
        putchar(*string++);
        count++;
    }
    putchar('\n');
    return count;
}
