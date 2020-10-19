#include <stdio.h>
#include <ctype.h>


/**
 * 数组与指针
 * @return
 */
int main() {
    //声明数组,但尚未初始化,调用的还是内存中的值
    int arr[4];
//构建4长度数组,并初始化前三个数组,第四个下标初始化为0
    int arr2[4] = {1, 2, 3};
    //长度3数组
    int arr3[] = {1, 2, 3};
    //初始化第五个元素
    int arr4[] = {[5] = 3, 0, [5]=7};

    for (int i = 0; i < sizeof arr / sizeof arr[0]; i++) {
        printf("%d ", arr[i]);
        printf("%d ", arr2[i]);
        printf("%d ", arr3[i]);
        printf("\n");
    }

    //声明二维数组
    int arr5[2][2] = {{1, 2},
                      {2, 3}};
    int arr6[2][2] = {1, 2, 2, 3};

    int *ptr = arr5;
    //2者内存地址一样的
    if (ptr + 1 == &arr5[0][1]) {
        printf("success \n");
    }
    //到内存的ptr位置移动1个存储单元,这里存储类型是int,int表示32位,4字节，相当于移动4字节.
    if (*(ptr + 1) == arr5[0][1]) {
        printf("%p %p %p \n", ptr, ptr + 1, &arr5[0][1]);
        printf("success \n");
    }

    sum3(arr3, 3);
    sum4(arr3, arr3 + 3);
    int *ptr2 = arr3;
    //移动4字节,因为int占用4字节
    int *ptr4 = ptr2++;
    //指针也有存储的内存地址.
    int *ptr3 = &ptr2;
    //指针存储的value
    int value = *ptr2;

    //s变成可读,但是arr3依旧可写
    const int *s = arr3;
    //可以更换s指针,但是不能修改里面的值
    s = arr4;


    const int arr7[] = {1, 2, 3};
    int *s2 = arr7;//无效,如何有效的话就能通过s2修改arr7了
    printf("%d \n", s2);

    int *const ss = arr3;
    printf("%d \n", *ss);
    //不可以更换s指针
//    ss = arr4;
    //但是可以更改值
    *ss = 10;
    printf("%d \n", *ss);

    //2个const分别标记不能修改地址,也不能修改指向地址的值
    const int *const sss = arr3;

    int zi[2][2] = {{2, 4},
                    {1, 3}};
    //三者地址一样
    printf("%p %p %p \n", zi, zi[0], &zi[0][0]);
    //zi是一个包含2个int类型的数组,zi指向8字节对象,zi[0]指向4字节.
    printf("%p %p %p \n", zi + 1, zi[0] + 1, &zi[0][0] + 1);
    printf("%d %d %d \n", **zi, *zi[0], *&zi[0][0]);
    //二维数组是由线性存储的,zi+1,先移动8字节,相当于移动到了下一个zi[1]中,然后+1,移动了4字节,移动到了zi[1][1]中.
    printf("%d %d %d \n", *(*(zi)), *(*(zi + 1)), *(*(zi + 1) + 1));

    //pz指向一个内含2个int类型值的数组.也就是int[][2]
    int(*pz)[2] = zi;
    //如下声明表示pz是一个包含2元素的数组,*表示包含的是2个指针,int表示包含的是2个int类型的指针
//    int *pz[2] =zi;
//指向指针的指针
    int **p2;
    *p2 = zi[0];
    p2 = &zi[0];

    //------------------------------变长数组,VLA
    //运行时才确定数组大小
    //必须是自动存储类别,static和extern不能用
    //不能再声明中初始化.
    //变指的是创建数组时,可以使用变量指定数组维度.但是一旦创建后,数组大小保持不变,
    int ro = 3;
    int clo = 2;
    int arrs[ro][clo];
    //函数声明,必须ro和clo在start[][]前面,方便数组引用
    //int sum6(int ro,int clo,int start[ro][clo]);
    //省略变量名,必须*代替
    //int sum6(int ,int,int start[*][*]);


    //------------------------------复合字面量
    //创建匿名数组，可省略数组大小
    (int[2]) {10, 10};
    int (*ptr77)[2];
    ptr77 = (int[3][2]) {{10, 20},
                         {20, 10},
                         {30, 10}};
    //可直接传递给方法
    sum3((int[]) {1, 2, 3}, 3);

    return 0;


}

//若arr是个数组可声明为arr数组，也可以指针表示.
int sum(int arr[], int size) {
}

int sum2(int *arr, int size) {
    for (int i = 0; i < size; ++i) {
        //这里也可以采用数组方式访问指针,前提是arr的确是个数组
        printf("%d", arr[i]);
    }
}

int sum3(int *arr, int size) {
    int total = 0;
    for (int i = 0; i < size; ++i) {
        //这里也可以采用数组方式访问指针,前提是arr的确是个数组
        total += arr[i];
    }
    printf("%d", total);
}

int sum4(int *start, int *end) {
    int total = 0;
    while (start < end) {
        total += *start;
        //指针指向下个元素
        start++;
    }
    printf("%d", total);
}

//表示接收一个int[n][2]数组
int sum5(int (*start)[2]) {

}

int sum6(int start[][2]) {

}
