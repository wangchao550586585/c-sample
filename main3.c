#include <stdio.h>
#include <ctype.h>

//2者区别
//假定用户没有用函数原型声明函数,将不会检查参数
void print();

//表明函数确实没参数,编译的时候会确保确定没参数
void print(void);

//当声明原型时,参数名没用.跟方法实现的形参名不一致也没事.
void print2(int, int);
//函数声明可以放main函数内部.
//若函数实现在main前面,可以不需要函数声明.
/**
 * 无缓冲输入:常用玩游戏,按键盘指令直接写入操作系统
 * 缓冲输入
 *          完全缓冲IO:缓冲区填满则刷新缓冲区
 *          行缓冲IO:换行时刷新缓冲区
 * @return
 */
int main() {
    int i = 10;
    //获取指针,表示ptr是指针,*pi是int类型
    int *ptr = &i;
    //获取指针对应内存值,*称之为:间接运算符也叫作解引用运算符
    int val = * ptr;
    //打印指针地址值
    printf("%d %p", i, &i);
    //    char c = getchar();
    //如果是字母
    /*if (isalpha(c)) {
        putchar(c + 1);
    }*/
//    当多个if else没添加{},则if默认与最近的else匹配

/*
    while ((getchar() != 'y')) {
        printf("heh");
    }
*/
    char ch;
    int rows, clos;
    //在这里输入后会直接结束,是因为scanf可以接收换行符,他把换行符留在输入队列中,然后getchar时,读取的换行,就结束了。
/*    while ((ch = getchar()) != '\n') {
        scanf("%d %d", &rows, &clos);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < clos; j++) {
                putchar(ch);
            }
                putchar('\n');
        }
    }*/
    /*   while ((ch = getchar()) != '\n') {
           //1或者2个输入的不是整数,则结束循环
           if (scanf("%d %d", &rows, &clos) != 2)break;
           for (int i = 0; i < rows; i++) {
               for (int j = 0; j < clos; j++) {
                   putchar(ch);
               }
               putchar('\n');
           }
           //从消息队列中获取换行跳过.
           while ((ch = getchar()) != '\n') continue;
       }*/


    while (scanf("%d", &rows) != 1) {
        //读取失败，则打印所有输入的字符.
        while ((ch = getchar()) != '\n')putchar(ch);
        //也可以直接读取错误字符串,然后不处理.
        //跳过空白
        scanf("%*s");
    }


    ch = getchar();
    while ((ch < 'a' || ch > 'c') && ch != 'q') {
        //存在问题:当换行时,响应错误
        //ch = getchar();
        //获取第一个值
        ch = getchar();
        //跳过换行
        while (getchar() != '\n')continue;
    }


    return 0;


}
