#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/un.h>

#define  PATH "cli.socket"
#define  SER_PATH "serv.socket"


int main() {
    int  cfd;
    int ret, len;
    char buf[BUFSIZ];
    struct sockaddr_un clit_addr, serv_addr;

    cfd = socket(AF_UNIX, SOCK_STREAM, 0);

    bzero(&clit_addr, sizeof(clit_addr));
    clit_addr.sun_family = AF_UNIX;
    strcpy(clit_addr.sun_path, PATH);
    len = offsetof(struct sockaddr_un, sun_path) + strlen(clit_addr.sun_path);

    //确保bind之前,套接字文件不存在.
    unlink(PATH);
    //这里不能依赖于隐式绑定,需要显示绑定,从而创建套接字文件
    bind(cfd, (struct sockaddr *) &clit_addr, len);

    ////////////////////////////
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, SER_PATH);
    len = offsetof(struct sockaddr_un, sun_path) + strlen(serv_addr.sun_path);
    connect(cfd, (struct sockaddr *) &serv_addr, len);

    while (1) {
        write(cfd, "hello\n", 6);
        sleep(1);
        ret = read(cfd, buf, sizeof(buf));
        write(STDOUT_FILENO, buf, ret);
    }
    close(cfd);
    return 0;
}