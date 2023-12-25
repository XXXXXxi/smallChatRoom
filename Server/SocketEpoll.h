//
// Created by 12135 on 2023/12/22.
//

#ifndef WEBCHATROOM_SOCKETEPOLL_H
#define WEBCHATROOM_SOCKETEPOLL_H

#include <unordered_map>
#include <string>
#include "PthreadPool.h"

const int PORT = 9999;
const int MAXMESSAGESIZE = 1024;


struct ConnInfo {
    PthreadPool* pool;  // 线程池
    int fd;             // 监听文件描述符
    int epfd;           // epoll监听文件描述符
    char msg[MAXMESSAGESIZE];
};

class SocketEpoll {
private:
    // 用于监听的文件描述符
    int fd;
    // 用户昵称索引
    static void epollController(void *arg);
    static void acceptConnection(void *arg);
    static void communication(void *arg);
    static void sendAllUsers(void *arg);
public:
    SocketEpoll();
};





#endif //WEBCHATROOM_SOCKETEPOLL_H
