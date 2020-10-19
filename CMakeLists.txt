cmake_minimum_required(VERSION 3.17)
project(c C)

set(CMAKE_C_STANDARD 11)

#[[add_executable(c  15socket/warp_socket.h
        15socket/warp_socket.c
        15socket/epoll/epoll_server.c)
add_executable(c2 15socket/client.c
        15socket/warp_socket.h
        15socket/warp_socket.c)]]

#[[测试udp
add_executable(c
        15socket/udp/server.c)
add_executable(c2
        15socket/udp/client.c)]]

#测试本地套接字
#[[add_executable(c 14fork/local_socket/server.c )
add_executable(c2 14fork/local_socket/client.c)]]


#测试libevent
#[[add_executable(c  16libevent/read_fifo.c)
add_executable(c2 16libevent/write_fifo.c)]]

#测试libevent  buffer事件
add_executable(c 16libevent/bufferevent/read.c 17myhttpd0/demo/epoll_http.h 17myhttpd0/demo/epoll_http.c 17myhttpd0/demo/main.c 17myhttpd0/libevent/main.c 17myhttpd0/libevent/libevent_http.c 17myhttpd0/libevent/libevent_http.h)
add_executable(c2 16libevent/bufferevent/write.c)

# 测试线程池 add_executable(c2 14fork/thread/pool2/thread_pool.c)

#[[add_executable(c  14fork/mmap/ipc.c)
add_executable(c2  14fork/mmap/ipc2.c)]]

#add_executable(c2 14fork/ipc/namepipe2.c)
#add_executable(c main6.c main66.c)

