#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>


void isFile(char string[5]);

void read_dir(char *name,void (* func)(char *));

/**
 * 递归打印文件目录
 * @returnz
 */
int main() {
    isFile("path");

    return 0;
}

void isFile(char *name) {
    struct stat sb;
    int ret = stat(name, &sb);
    if (S_ISDIR(sb.st_mode)) {
        read_dir(name,isFile);
    }
    printf("%s \n", name);
}

void read_dir(char *name,void (* func)(char *)) {
    DIR *dir = opendir(name);
    struct dirent *sdb;
    char path[256];
    while ((sdb = readdir(dir)) != NULL) {
        //拼接路径
        sprintf(path, "%s/%s", dir, sdb->d_name);
        (*func)(path);
    }
    close(dir);

}
