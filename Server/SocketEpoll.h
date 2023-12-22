//
// Created by 12135 on 2023/12/22.
//

#ifndef WEBCHATROOM_SOCKETEPOLL_H
#define WEBCHATROOM_SOCKETEPOLL_H

#include "PthreadPool.h"

const int PORT = 9999;
const int MAXSIZE = 10;

struct ConnInfo {
    PthreadPool* pool;  // 线程池
    int fd;             // 监听文件描述符
    int epfd;           // epoll监听文件描述符
};

class SocketEpoll {
private:
    int fd;     // 用于监听的文件描述符
    static void epollController(void *arg);
    static void acceptConnection(void *arg);
    static void communication(void *arg);
public:
    SocketEpoll();
};


#endif //WEBCHATROOM_SOCKETEPOLL_H
