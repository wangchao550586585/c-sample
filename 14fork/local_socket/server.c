#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/un.h>

#define  PATH "serv.socket"

/**
 * 本地套接字
 * @return
 */
int main() {
    int lfd, cfd;
    int ret, len;
    char buf[BUFSIZ];
    struct sockaddr_un serv_addr, cliaddr;
    /**
     声明本地套接字 AF_UNIX/AF_LOCAL
     */
    lfd = socket(AF_UNIX, SOCK_STREAM, 0);

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, PATH);

    //算出serv_addr的具体占用长度
    len = offsetof(struct sockaddr_un, sun_path) + strlen(serv_addr.sun_path);
    unlink(PATH);

    /**
     * 这里的长度,传递的不是结构体的大小,而是结构体内容占用空间大小.
     */
    bind(lfd, (struct sockaddr *) &serv_addr, len);
    listen(lfd, 20);
    printf("accept...\n");

    while (1) {
        //监听并处理连接
        len = sizeof(cliaddr);
        cfd = accept(lfd, (struct sockaddr *) &cliaddr, (socklen_t *) &len);

        len -= offsetof(struct sockaddr_un, sun_path);
        cliaddr.sun_path[len] = '\0';
        printf("fileName %s \n", cliaddr.sun_path);

        while ((ret = read(cfd, buf, sizeof(buf))) > 0) {
            write(STDOUT_FILENO, buf, ret);
            for (int i = 0; i < ret; ++i) {
                buf[i] = toupper(buf[i]);
            }
            write(cfd, buf, ret);
        }
        close(cfd);
    }
    close(lfd);

    return 0;
}