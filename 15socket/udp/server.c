#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <strings.h>

/**
 * upd 通信
 * @return
 */
#define PORT 8080
int main() {
    int lfd;
    int n;
    char buf[BUFSIZ], cli_Ip[1024];
    struct sockaddr_in serv_addr, clit_addr;
    socklen_t client_addr_len = sizeof(clit_addr);

    lfd=socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(lfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

    printf("acepting...");
    while (1){
        //相当于accpet+write
        n = recvfrom(lfd, buf,BUFSIZ,0,(struct sockaddr *)&clit_addr,&client_addr_len);
        if (n==-1){
            perror("error");
        }
        write(STDOUT_FILENO, buf, n);
        printf("client ip: %s , port: %d  \n", inet_ntop(AF_INET, &clit_addr.sin_addr.s_addr, cli_Ip, sizeof(cli_Ip)),
               ntohs(clit_addr.sin_port));
        for (int i = 0; i < n; ++i) {
            buf[i] = toupper(buf[i]);
        }
        n=sendto(lfd, buf, n,0,(struct sockaddr *)&clit_addr, sizeof(clit_addr));
        if (n==-1){
            perror("error");
        }
    }
    close(lfd);
    return 0;
}