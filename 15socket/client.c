#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include  "warp_socket.h"
#define  PORT 8080

int main() {
    char buf[BUFSIZ];

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
//    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr.s_addr);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    int cfd = default_my_socket_ipv4_tcp();


    int ret = my_connect(cfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

    while (1) {
        my_write(cfd, "hello\n", 6);
        sleep(1);
        int size = my_read(cfd, buf, sizeof(buf));
        my_write(STDOUT_FILENO, buf, size);
    }
    close(cfd);
    return 0;
}