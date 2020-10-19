#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <strings.h>

#define  PORT 8080

int main() {
    char buf[BUFSIZ];
    int cfd;
    int n;
    struct sockaddr_in serv_addr;
    socklen_t serv_addr_len = sizeof(serv_addr);

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    cfd = socket(AF_INET, SOCK_DGRAM, 0);

    while (1) {
        //相当于connect+write
        n = sendto(cfd, "hello\n", 6, 0, (struct sockaddr *) &serv_addr, serv_addr_len);
        if (n == -1) {
            perror("error");
        }
        n = recvfrom(cfd, buf, BUFSIZ, 0, NULL, 0);  //不需要服务器的信息
        write(STDOUT_FILENO, buf, n);
        if (n == -1) {
            perror("error");
        }
        sleep(1);
    }
    close(cfd);
    return 0;
}