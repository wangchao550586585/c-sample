#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <strings.h>
#include <arpa/inet.h>
#include  "warp_socket.h"

/**
 * 一个文件描述符指向一个套接字,该套接字内部由内核借助2个缓冲区实现
 *
 * 网络字节序
 *          小端存储法(pc本地存储):高位存高地址,低位存低地址.
 *          大端存储法(网络存储):同上相反.
 *          h表示host，n表示network，l表示32位整数,s表示16位短整数
 *          htonl：本地 => 网络(IP)      只接收32位整数,所以需要先将点分十进制形式(192.XX.XX.XX)的ip地址转成整数, INADDR_ANY获取系统有效IP地址,32整型
 *          htons：本地 => 网络(port)
 *          ntohl  网络 => 本地(IP)
 *          ntohs  网络 => 本地(port)
 *
 * IP地址转换
 *          int inet_pton(int af, const char *src, void *dst);      本地(string) =>网络
 *          af : AF_INET / AF_INET6  ipv4 /ipv6
 *          src : IP地址(点分10进制形式)
 *          dst : 传出参数,网络字节序的ip地址
 *
 *          const char *inet_ntop(int af, const void *src,char *dst, socklen_t size);  网络 => 本地(string)
 */

#define  PORT 8080

int main() {
    int lfd, cfd;
    int ret;
    char buf[BUFSIZ], cli_Ip[1024];
    struct sockaddr_in serv_addr, clit_addr;

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    /**
     * int socket(int domain, int type, int protocol);
     * domain : ip地址协议
     * type : 数据协议 流式协议,报式协议
     * protocol ： 选用协议的代表协议. 流式就是tcp,报式就是udp
     */

    lfd = default_my_socket_ipv4_tcp();

    /**
     * 设置端口复用,也就是可以重复利用同一个端口运行服务器
     */
     my_setsockopt(lfd);
    /**
     *  int bind(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
     *  struct sockaddr_in {
            sa_family_t sin_family;  IP地址协议
            in_port_t sin_port;  端口(网络字节序)
            struct in_addr sin_addr;  ip(网络字节序)
        }
        sockaddr_in主要弥补sockaddr的缺陷(目标地址和端口信息混在一起),占用字节大小都一样.
     */
    my_bind(lfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    //最多允许连接数
    my_listen(lfd, 128);

    //监听并处理连接
    socklen_t client_addr_len = sizeof(clit_addr);
    cfd = my_accept(lfd, (struct sockaddr *) &clit_addr, &client_addr_len);

    printf("client ip: %s , port: %d  \n", inet_ntop(AF_INET, &clit_addr.sin_addr.s_addr, cli_Ip, sizeof(cli_Ip)),
           ntohs(clit_addr.sin_port)
    );
    while (1) {
        ret = my_read(cfd, buf, sizeof(buf));
        my_write(STDOUT_FILENO, buf, ret);
        for (int i = 0; i < ret; ++i) {
            buf[i] = toupper(buf[i]);
        }
        my_write(cfd, buf, ret);
    }
    close(cfd);
    close(lfd);

    return 0;
}