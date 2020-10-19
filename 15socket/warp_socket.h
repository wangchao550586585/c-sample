#ifndef _WRAP__
#define _WRAP__

#include <sys/socket.h>

int check(int ret, char *str);

int my_socket(int domain, int type, int protocol);

int default_my_socket_ipv4_tcp();

struct sockaddr_in* create_ipv4_for_localhost(struct sockaddr_in *srv_addr, uint16_t port);


int my_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

int my_listen(int sockfd, int backlog);

int my_accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

int my_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

ssize_t my_read(int fd, void *buf, size_t count);

ssize_t my_write(int fd, const void *buf, size_t count);
int my_setsockopt(int sockfd);
#endif