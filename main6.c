#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>

/**
 * 存储类别,链接,内存管理
 * 翻译单元:就是一个源文件以及包含的文件，如头文件.C预处理的时候,会把include的头文件内容替换include指令.
 * 三种链接
 * 外部链接:多文件使用,全局作用域
 * 内部链接:一个翻译单元使用,文件作用域
 * 无链接:函数作用域,函数原型作用域,块作用域(函数内部块变量)
 *
 * 四种存储期
 * 静态:程序执行期间一直存在.文件作用域变量都是
 * 线程:线程死了就没了
 * 自动:函数或块执行完就释放.
 * 动态分配:
 *
 *
 * @return
 */
//内部链接
static int g = 3;
//外部链接
//extern char arr[100];//如果arr在另外文件定义,则必须添加extern声明
char ch[100];

//外部函数链接
extern unsigned int rand0();

extern void srand0(unsigned int);

int main() {
    //强调不要修改变量存储类别.显示声明为自动变量
    auto int type;
    //存储在寄存器,编译器可能忽略
    register int quick;//可放在方法参数上
    //静态变量,存储在静态内存区.方法销毁也不会重置var的值
    //声明后自动初始化
    static int var = 10;

    //外部变量
    //不用声明大小,因为第一次声明已经提供了数组大小信息
    //声明可省略,可直接使用
    //声明后自动初始化
    extern char ch[];
    printf("%s \n", ch);

    for (int i = 0; i < 10; ++i) {
        srand0(10);
        printf("%d \n", rand0());
    }
    srand0(10);
    printf("%d \n", rand0());

    //---------------动态分配内存
    double *ptr;
    //动态分配内存,可像访问数组一样访问
    ptr = (double *) malloc(30 * sizeof(double));

    //二维数组这样声明
//   = (double (*)[m]) malloc(30*m * sizeof(double));


    //释放内存
    free(ptr);
    //表示普通的程序结束
    exit(EXIT_SUCCESS);
    //若分配不到内存
    if (ptr == NULL)
        //表示程序异常中止
        exit(EXIT_FAILURE);

    //会把块中所有位设置0
    ptr = calloc(10, sizeof(double));


//    ---------------------- volatile  restrict  const修饰符
    //--------------类型幂等
//    也就是说声明多个const,也就只保留一个
    const float f;//f不能修改,只能初始化时赋值
    const float *pf;//pt指针可以修改
    float const *pf2;//pt指向值可以修改
    const float const *pf3;//都不能修改


    //表明数据是易变的
    volatile int local;

//    restrict就是告诉编译器,指明该指针是访问对象的唯一方式.编译器就可以做合并操作指令的优化
    int *restrict res = (int *) malloc(10 * sizeof(int));
    res[0] += 2;
    res[0] += 5;
    //编译器会优化成
    res[0] += 7;

    //_Atomic原子修改,其他线程不能访问
    _Atomic int hogs;
    atomic_store(&hogs, 12);


    return 0;
}

//以前的写法
void of(int *const a1, int *restrict a2, int n);
//同上一样声明,
void of2(int a1[const], int a2[restrict], int n);

//该数组至少有10个元素
void of3(int a1[static 10]);
