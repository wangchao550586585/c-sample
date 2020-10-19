#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN 40

/**
 * 文件输入/输出
 * @return
 */
static char filePath[] = "E:\\workspace\\c\\CMakeLists.txt";

void copyFile();

void demo2();

void demo3();

int main() {

    FILE *fp = fopen(filePath, "r");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }
    int ch = '\0', count = 0;
    //从fd指定文件中获取一个字符
    while ((ch = getc(fp)) != EOF) {
        //把字符ch输出到stdout指定的文件中
        putc(ch, stdout);
    }
    fclose(fp);

    //-------------------------------------拷贝文件
//    copyFile();


    //----------------------------------------fscanf  & fprintf  &rewind
//    demo2();


    //---------------------------------------fseek & ftell
    //逆序打印内容.
    //fseek只能容纳long类型数据,fgetpos/fsetpos对其补充
//    demo3();

//ungetc:把指定字符放入输入流
//fflush():刷入fd指定文件
//setvbuf():fd存入buf
//fread/fwrite:二进制方式写入读取
    FILE *source, *dest;//ab+/rb+设置二进制读取写入方式
    if (setvbuf(source, NULL, _IOFBF, 2048) != 0) {
        fputs("不能创建输入缓冲", stderr);
    }

    size_t bytes;
    static char temp[2048];
    //fread:返回读取的项数,这里是2048.
    //表示把2048个char大小的值拷贝到temp
    while ((bytes = fread(temp, sizeof(char), 2048, source)) > 0) {
        fwrite(temp, sizeof(char), bytes, dest);
    }

//feof/ferror:分别表示文件结尾错误以及读或写错误.主要为了区分fopen出现的EOF
    if (ferror(source)) {
        fprintf(stderr, "读取文件错误");
    }
    if (ferror(dest)) {
        fprintf(stderr, "写入文件错误");
    }
    fclose(source);
    fclose(dest);


    return 0;
}

void copyFile() {
    FILE *in, *out;
    int ch = '\0', count = 0;
    if ((in = fopen(filePath, "r")) == NULL) {
        fprintf(stderr, "不能打开文件");
        exit(EXIT_FAILURE);
    }

    //拷贝文件
    char arg[] = "2222211111";
    char name[LEN];
    strncpy(name, arg, LEN - 5);
    name[LEN - 5] = '\0';
    //文件名添加后缀
    strcat(name, ".red");
    if ((out = fopen(name, "w")) == NULL) {
        fprintf(stderr, "不能创建输出文件");
        exit(3);
    }
    while ((ch = getc(in)) != EOF) {
        if (count++ % 3 == 0) {
            putc(ch, out);
        }
    }
    if (fclose(in) != 0 || fclose(out) != 0) {
        fprintf(stderr, "ERROR");
    }
    printf("%d \n", count);
}

void demo2() {
    FILE *fp2 = fopen("E:\\workspace\\c\\CMakeLists2.txt", "a+");

    char words[LEN];
    fscanf(stdin, "%40s", words);
    fprintf(fp2, "%s\n", words);
    rewind(fp2);//返回文件开始处
    while (fscanf(fp2, "%s", words) == 1) {
        puts(words);
    }
}

void demo3() {
//定位文件末尾
/*
 * 0L:从起始点偏移量
 * SEEK_END:起始点为文件末尾
 */
//二进制读取,文本模式打开fseek可能会有不同处理
    FILE *fp2 = fopen("E:\\workspace\\c\\CMakeLists2.txt", "rb");
    int ch = '\0', count = 0;
    fseek(fp2, 0L, SEEK_END);
//返回当前位置.
    long last = ftell(fp2);
    for (count = 1L; count <= last; count++) {
        fseek(fp2, -count, SEEK_END);
        ch = getc(fp2);
        putchar(ch);
    }
    fclose(fp2);
}