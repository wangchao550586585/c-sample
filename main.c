#include <stdio.h>
//类型别名,如int_32
#include <stdint.h>
//支持可移植类型
#include <inttypes.h>

//函数原型,告知编译器我要用这个函数
void butler(void);

/**
 * 数据类型
 * @return
 */
int main() {
    butler();
    float weight;
    /*
     * %f:处理float类型数据
     * %.2f:显示浮点数后2位
     * &weight:读取值写入weight内存地址
     */
/*    scanf("%f",&weight);
    printf("%.2f",weight);*/

    int x = 10;
    //10,8,16进制打印值
    printf("%d %o %x \n", x, x, x);
    //10,8,16进制打印前缀
    printf("%d %#o %#x \n", x, x, x);

    short z = 10;
    printf("%hd %ho %hx \n", z, z, z);
    //只存正整数,所有存的数字更大
    unsigned int a = 10;
    printf("%u \n", a);
    long c = 10;
    printf("%ld %lo %lx \n", c, c, c);//int和long大小一样,可直接%d表示
    unsigned long b = 10;
    printf("%lu %lo %lx \n", b, b, b);
    unsigned long long d = 10;
    printf("%llu \n", d);

    char e = 'e';
    printf("%c \n", e);


    // short(16位) ->  int(16/32位) ->
    // undigned int -> long(32位) ->
    // undigned long -> long long(64位) ->
    // undigned long long
    // long 声明的值要加L
    // undigned 则加u

    //32位有符号整数，若在int-16,long-32操作系统上,该为long的别名
    int32_t t=10;
    //若不支持精确宽度整数,则可使用最小宽度类型.
    //表示最少可容纳8位有符号整数
    int_least32_t t1=10;
    //最快最小宽度类型
    //对8位有符号值而言创建运算最快的整数类型
    int_fast8_t t3=10;
    //最大有符号整数类型
    intmax_t t4=10;
    //最大无符号整数类型
    uintmax_t t5=10;

    //针对类型别名提供了inttypes.h显示可移植类型
    printf("%" PRId32 "\n",t);

    //浮点数
    float f1=1.0;
    float f2=.0;
    float f3=1.;
    float f4=1.e-3;
    float f5=.8e-3;
    //2个浮点数运算时，默认使用double，然后结果截断成为float.也就是说会升级为64位运算.浪费内存.
    float s=1.+2.;
    //可在浮点数后面添加f,表明这是float类型,就不会成为double运算
    float s2=1.f+2.f;
    //使用l后缀,表明是long double类型
    long double s3=1.3l;
    printf("%f",f3);
    //科学计数法打印浮点数
    printf("%e",f4);
    printf("%lf le",s3,s3);
    printf("-------------------\n");

    //打印类型大小
    printf("%zd \n",sizeof(short ));
    printf("%zd \n",sizeof(int));
    printf("%zd \n",sizeof(long));
    printf("%zd \n",sizeof(long long));

    return 0;
}

void butler(void) {
    printf("hello");
}
