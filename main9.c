#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <limits.h>

/**
 * 二进制
 * @return
 */
char *tobit(int num, char *ps);

void tobit2();

int invert_end(int, int);

void show_bstr(char *str);

int main() {
    //有符号整数
    //如何获得8位有符号整数?
    //假设第8位表示正负数,方法如下
    //用一个9位1000000000-100000000，得到128,因为第八位为1,所以为-128
    //二进制反码

    //掩码
    //通过&操作,ch&0Xff,表示只保留ch低8位

    //设置位
    //通过 flag | ch2 表示打开ch标记

    //重置位,ch对应的标志位在flag中置为0.
    //flag=flag & ~ ch

    //切换打开或关闭 flag为0的则不变,0变1,1变0
    //flag ^ ch2

    //检查位的值
    //flag==ch2  这样也会因为flag其他位修改,导致无法相同
    //flag & ch2 == ch2  这样就可以.

    //--------------------转成二进制并打印
    char bit[CHAR_BIT * sizeof(int) + 1];
    int num;
    scanf("%d", &num);
    char *ps = tobit(num, bit);
    show_bstr(ps);
    printf("\n");
//    tobit2(0xff);

//-------------------切换后4位.
    int i = invert_end(num, 4);
    show_bstr(tobit(i, bit));


    //--------------位字段
    //建立3个1位和1个7位的字段
    //prnt存储int大小内存单元中,但是只使用了其中4位
    struct {
        unsigned int a: 1;
        unsigned int b: 1;
        unsigned int c: 1;
        unsigned int d: 7;
    } prnt;
    prnt.a = 0;
    //当超过int范围,则会溢出到下个int中
    //使用对齐解决
    struct {
        unsigned int a: 1;
        unsigned int b: 1;
        unsigned int c: 1;
        //使用未命名字段填充空隙.
        unsigned int : 1;
        unsigned int d: 8;
    } prnt2;

    //-------------------------对齐
    //float类型对象的对齐要求是4
    size_t d = _Alignof(float);
    _Alignas(double) char c1;//指明c1对齐值为8
    char _Alignas(8) c2;//指明c1对齐值为8
    printf("%zd \n", _Alignof(double));

    //分配对齐的内存
//    aligned_alloc()

    return 0;
}

char *tobit(int num, char *ps) {
    int size = CHAR_BIT * sizeof(int);
    for (int i = size - 1; i >= 0; i--, num >>= 1) {
        //01是八进制掩码,获取最后一位值
        //然后+'0'是为了转成字符.
        ps[i] = (01 & num) + '0';
    }
    ps[size] = '\0';
    return ps;
}

void show_bstr(char *str) {
    int i = 0;
    while (str[i]) {
        putchar(str[i]);
        if (++i % 4 == 0 && str[i])putchar(' ');
    }
}

void tobit2(unsigned long n) {
    int r = n % 2;
    if (n >= 2)tobit2(n / 2);
    putchar(r == 0 ? '0' : '1');
    return;
}

int invert_end(int num, int bits) {
    int mask = 0;
    int bit = 1;
    while (bits-- > 0) {
        mask |= bit;
        bit <<= 1;
    }
    return num ^ mask;
}