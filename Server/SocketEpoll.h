//
// Created by 12135 on 2023/12/22.
//

#ifndef WEBCHATROOM_SOCKETEPOLL_H
#define WEBCHATROOM_SOCKETEPOLL_H

#include <unordered_map>
#include <string>
#include "PthreadPool.h"
#include "../Utils/MessageQueue.h"

const int PORT = 9999;
const int MAXMESSAGESIZE = 2048;


struct ConnInfo {
    PthreadPool* pool;  // 线程池
    MessageQueue* msgQ;  // 消息队列
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
    static void acceptMessage(void *arg);
    static void messageHandler(void *arg);
public:
    SocketEpoll();
};





#endif //WEBCHATROOM_SOCKETEPOLL_H
