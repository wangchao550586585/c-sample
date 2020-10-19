#include <stdio.h>
#include <string.h>

#define DENSITY "1011113331"

/**
 * 字符串输入输出
 * @return
 */
int main() {
    char name[10];
    char name2[10];
    float s;
    //读取之后会在字符序列末尾添加\0
//    scanf("%s", name);
    //读取2个字符,中间必须,隔开.scanf不会跳过中间所有空白(亲测)
//返回值0表示可能出了错误.>0表示有值
//    int sd=scanf("%s,%s", name, name2);
    printf("%s \n", name);
    printf("%s \n", name2);
//    printf("%d", sd);
    printf("------------------- \n");
    char cc;
    char cc2;
   // scanf("%c", &cc);
    //出了%c,其他读取的时候都会跳过空格.
    //从第一个读取,可以是空格
  //  printf("%c \n", cc);
    //从第一个非空格读取
//    scanf("  %c", &cc2);
//    printf("%c \n", cc2);


//    scanf("%f", &s);
    int size = sizeof name;
    int size2 = sizeof DENSITY;
    //获取字符串长度
    int le = strlen(name);
    int le2 = strlen(DENSITY);
    //s说明是字符串类型
    printf("%s \n", name);
    printf("%2.2f \n", s);
    printf("%d \n", le);
    printf("%d \n", le2);
    printf("%d \n", size);
    printf("%d \n", size2);
    //输出%
    printf("%%\n");

    //测试使用修饰符和标记
    printf("%10d \n", size2);
    printf("%-10d \n", size2);
    double c = 222.22;
    //10进制打印
    printf("%f \n", c);
    //科学计数法打印
    printf("%e \n", c);
    //
    printf("%2.2f \n", c);
    //对齐10个位置.
    printf("%10.2f \n", c);
    //前面添加+符号
    printf("%+10.2f \n", c);
    //标记0填充满足打印10长度,其中小数长度为2
    printf("%010.2f \n", c);


    printf("%2s \n", DENSITY);
    //字段对齐24长度
    printf("%24s \n", DENSITY);
    //.5表示只打印5个字符
    printf("%24.5s \n", DENSITY);
    //-:表示文本左对齐
    printf("%-24.5s \n", DENSITY);

    float f = 1.3;
    double f2 = 1.3;
    long f3 = 10;
    long f4 = 10;
    printf("%ld %ld \n", f3, f4);
    int rv = printf("%ld %-ld %ld %ld \n", f, f2, f3, f4);
    printf("%d \n", rv);

    //多个字符串换行
    printf("hello"
           "world \n");
    printf("hello world \
    haha \n");

    //动态指定宽度
    printf("%*d \n",3, rv);
    printf("%*.*f \n",3,1, rv);

    //跳过前面整数
    scanf("%*d %d",&rv);
    printf("%d \n", rv);

    return 0;
}
