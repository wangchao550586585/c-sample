#ifndef __LIBEV__
#define __LIBEV__

#include <event2/bufferevent.h>
#include <event2/listener.h>


void listencb(struct evconnlistener *listener, evutil_socket_t fd,
              struct sockaddr *cli, int len, void *ptr);

void readcb(struct bufferevent *bev, void *ctx);

void eventcb(struct bufferevent *bev, short what, void *ctx);

void signal_cb(evutil_socket_t sig, short events, void *user_data);

#endif
