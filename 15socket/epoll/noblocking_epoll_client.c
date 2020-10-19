#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include  "../warp_socket.h"

#define  PORT 8080
#define MAXLINE 10

int main() {
    int i, cfd;
    char buf[10], ch = 'a';
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    cfd = default_my_socket_ipv4_tcp();
    my_connect(cfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

    while (1) {
        //aaaa\n
        for (i = 0; i < MAXLINE / 2; i++) {
            buf[i] = ch;
        }
        buf[i - 1] = '\n';
        ch++;
        //bbbb\n
        for (; i < MAXLINE; i++) {
            buf[i] = ch;
        }
        buf[i - 1] = '\n';
        ch++;

        write(cfd, buf, sizeof(buf));
        sleep(5);
    }
    close(cfd);
    return 0;
}