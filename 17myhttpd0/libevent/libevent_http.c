#include "libevent_http.h"
#include <stdio.h>
#include <event2/buffer.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

// 通过文件名获取文件的类型
const char *get_file_type(const char *name) {
    char *dot;

    // 自右向左查找‘.’字符, 如不存在返回NULL
    dot = strrchr(name, '.');
    if (dot == NULL)
        return "text/plain; charset=utf-8";
    if (strcmp(dot, ".html") == 0 || strcmp(dot, ".htm") == 0)
        return "text/html; charset=utf-8";
    if (strcmp(dot, ".jpg") == 0 || strcmp(dot, ".jpeg") == 0)
        return "image/jpeg";
    if (strcmp(dot, ".gif") == 0)
        return "image/gif";
    if (strcmp(dot, ".png") == 0)
        return "image/png";
    if (strcmp(dot, ".css") == 0)
        return "text/css";
    if (strcmp(dot, ".au") == 0)
        return "audio/basic";
    if (strcmp(dot, ".wav") == 0)
        return "audio/wav";
    if (strcmp(dot, ".avi") == 0)
        return "video/x-msvideo";
    if (strcmp(dot, ".mov") == 0 || strcmp(dot, ".qt") == 0)
        return "video/quicktime";
    if (strcmp(dot, ".mpeg") == 0 || strcmp(dot, ".mpe") == 0)
        return "video/mpeg";
    if (strcmp(dot, ".vrml") == 0 || strcmp(dot, ".wrl") == 0)
        return "model/vrml";
    if (strcmp(dot, ".midi") == 0 || strcmp(dot, ".mid") == 0)
        return "audio/midi";
    if (strcmp(dot, ".mp3") == 0)
        return "audio/mpeg";
    if (strcmp(dot, ".ogg") == 0)
        return "application/ogg";
    if (strcmp(dot, ".pac") == 0)
        return "application/x-ns-proxy-autoconfig";

    return "text/plain; charset=utf-8";
}

// 16进制数转化为10进制
int hexit(char c) {
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;

    return 0;
}

/*
 *  这里的内容是处理%20之类的东西！是"解码"过程。
 *  %20 URL编码中的‘ ’(space)
 *  %21 '!' %22 '"' %23 '#' %24 '$'
 *  %25 '%' %26 '&' %27 ''' %28 '('......
 *  相关知识html中的‘ ’(space)是&nbsp
 */
void encode_str(char *to, int tosize, const char *from) {
    int tolen;

    for (tolen = 0; *from != '\0' && tolen + 4 < tosize; ++from) {
        if (isalnum(*from) || strchr("/_.-~", *from) != (char *) 0) {
            *to = *from;
            ++to;
            ++tolen;
        } else {
            sprintf(to, "%%%02x", (int) *from & 0xff);
            to += 3;
            tolen += 3;
        }

    }
    *to = '\0';
}


void decode_str(char *to, char *from) {
    for (; *from != '\0'; ++to, ++from) {
        if (from[0] == '%' && isxdigit(from[1]) && isxdigit(from[2])) {

            *to = hexit(from[1]) * 16 + hexit(from[2]);

            from += 2;
        } else {
            *to = *from;

        }

    }
    *to = '\0';

}

void listencb(struct evconnlistener *listener, evutil_socket_t fd,
              struct sockaddr *cli, int len, void *ptr) {
    struct event_base *base = ptr;
    struct bufferevent *bev =
            bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);

    //强制刷新缓冲区
    bufferevent_flush(bev, EV_READ | EV_WRITE, BEV_NORMAL);
    //set callback
    bufferevent_setcb(bev, readcb, NULL, eventcb, NULL);
    bufferevent_enable(bev, EV_READ | EV_WRITE);

}

// 断开连接的函数
void disconnect(struct bufferevent *bev) {
    bufferevent_disable(bev, EV_READ | EV_WRITE);
}

void send_respond_head(struct bufferevent *bev, int no, const char *desp, const char *type, long len) {
    char buf[1024] = {0};
    // 状态行
    sprintf(buf, "http/1.1 %d %s\r\n", no, desp);
    bufferevent_write(bev, buf, strlen(buf));
    // 消息报头
    memset(buf, 0, sizeof buf);
    sprintf(buf, "Content-Type:%s\r\n", type);
    sprintf(buf + strlen(buf), "Content-Length:%ld\r\n", len);
    bufferevent_write(bev, buf, strlen(buf));
    // 空行
    bufferevent_write(bev, "\r\n", 2);
}

void send_file(struct bufferevent *bev, const char *path) {
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        //todo 404
        perror("open err");
        exit(1);
    }


    char buf[4096];
    int ret = 0;
    while ((ret = read(fd, buf, sizeof buf)) > 0) {
        bufferevent_write(bev, buf, ret);
    }
    if (ret == -1) {
        perror("read err");
        exit(1);
    }
    close(fd);
}


void send_dir(struct bufferevent *bev, const char *path) {
    char file[1024] = {0};
    char buf[4096] = {0};
    sprintf(buf, "<html><head><title>目录名: %s</title></head>", path);
    sprintf(buf + strlen(buf), "<body><h1>当前目录: %s</h1><table>", path);
#if 1
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("opendir error");
        exit(1);
    }
    // 读目录
    char enstr[1024] = {0};
    struct dirent *ptr = NULL;
    while ((ptr = readdir(dir)) != NULL) {
        char *name = ptr->d_name;
        // 拼接文件的完整路径
        sprintf(file, "%s/%s", path, name);

        encode_str(enstr, sizeof(enstr), name);
        struct stat st;
        int ret = stat(file, &st);
        if (ret == -1) {
            perror("stat err");
            exit(1);
        }
        // 如果是文件
        if (S_ISREG(st.st_mode)) {
            sprintf(buf + strlen(buf),
                    "<tr><td><a href=\"%s\">%s</a></td><td>%ld</td></tr>",
                    enstr, name, (long) st.st_size);
        }
            // 如果是目录
        else if (S_ISDIR(st.st_mode)) {
            sprintf(buf + strlen(buf),
                    "<tr><td><a href=\"%s/\">%s/</a></td><td>%ld</td></tr>",
                    enstr, name, (long) st.st_size);
        }
        bufferevent_write(bev, buf, strlen(buf));
        memset(buf, 0, sizeof(buf));
    }
    sprintf(buf + strlen(buf), "</table></body></html>");
    bufferevent_write(bev, buf, strlen(buf));
    closedir(dir);
#endif
}

void http_get_request(const char *line, struct bufferevent *bev) {
    char method[12] = {0};
    char res[1024] = {0};
    sscanf(line, "%[^ ] %[^ ]", method, res);

    // 转码 将不能识别的中文乱码 - > 中文
    // 解码 %23 %34 %5f
    char destr[1024];
    decode_str(destr, res);
    // 处理path  /xx
    // 去掉path中的/
    char *file = destr + 1;
    char fbuf[1024];
    if (strcmp(res, "/") == 0) {
        file = "./";
        //getcwd(fbuf, sizeof(fbuf));
        //printf("root:[%s]\n", fbuf);
        //   file = fbuf;
    }

    struct stat st;
    printf("file:[%s]\n", file);
    int ret = stat(file, &st);
    if (ret == -1) {
        //todo 404
        perror("stat error");
        exit(1);
    }
    if (S_ISDIR(st.st_mode)) {
        //dir
        // 发送头信息
        send_respond_head(bev, 200, "OK", get_file_type(".html"), 100000);
        send_dir(bev, file);
    } else if (S_ISREG(st.st_mode)) {
        //reg file
        send_respond_head(bev, 200, "OK", get_file_type(file), st.st_size);
        send_file(bev, file);
    }
}

void readcb(struct bufferevent *bev, void *ctx) {
    struct evbuffer *evbuf = bufferevent_get_input(bev);

    char *line;
    size_t len;
    line = evbuffer_readln(evbuf, &len, EVBUFFER_EOL_CRLF);
    /*
    char* tmp = line;
    while(1){
      tmp = evbuffer_readln(evbuf, &len, EVBUFFER_EOL_CRLF);
      if(tmp) break;
      printf("%s\n", tmp);
    }
    printf("line:[%s]", line);
    */
    if (strncasecmp("get", line, 3) == 0) {
        http_get_request(line, bev);
        //disconnect(bev);
    }

}

void eventcb(struct bufferevent *bev, short what, void *ctx) {
    if (what & BEV_EVENT_EOF) {
        printf("连接关闭 \n");
    } else if (what & BEV_EVENT_ERROR) {
        printf("发生一个连接错误 \n");
    }
    bufferevent_free(bev);

}

void signal_cb(evutil_socket_t sig, short events, void *user_data) {
    struct event_base *base = user_data;
    struct timeval delay = {1, 0};
    printf("延迟一秒关闭 \n");
    event_base_loopexit(base, &delay);
}