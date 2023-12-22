//
// Created by 12135 on 2023/12/22.
//
#include <iostream>
#include <arpa/inet.h>
#include <cstring>
#include <sys/epoll.h>
#include <fcntl.h>
#include <csignal>
#include "PthreadPool.h"
#include "SocketEpoll.h"

SocketEpoll::SocketEpoll() {
    // 创建套接字
    fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd == -1) {
        perror("socket");
        exit(0);
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // 绑定接口
    int ret = bind(fd,(sockaddr*)&addr,sizeof(addr));
    if(ret == -1) {
        perror("bind");
        exit(0);
    }

    // 设置监听
    ret = listen(fd,128);
    if(ret == -1) {
        perror("listen");
        exit(0);
    }

    // 初始化epoll
    int epfd = epoll_create(128);   //2.6.8内核版本后此值被忽略，大于0即可
    if(epfd == -1){
        perror("epoll_create");
        exit(0);
    }

    int flag = fcntl(fd,F_GETFL);
    flag |= O_NONBLOCK;
    fcntl(fd,F_SETFL,flag);

    // 添加epoll事件
    epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = fd;
    ret = epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&ev);
    if(ret == -1) {
        perror("epoll_ctl");
        exit(0);
    }

    // 创建线程池
    PthreadPool* pool = new PthreadPool(3,10);

    auto* info = (ConnInfo*)malloc(sizeof(ConnInfo));
    info->pool = pool;
    info->fd = fd;
    info->epfd = epfd;
    Task task;
    task.function = epollController;
    task.arg = info;

    pool->TaskAdd(task);
    pthread_exit(nullptr);
}

void SocketEpoll::epollController(void *arg) {

    auto *info = (ConnInfo*)(arg);
    int fd = info->fd;
    int epfd = info->epfd;
    PthreadPool *pool = info->pool;

    epoll_event evs[1024];
    int size = sizeof(evs) / sizeof(epoll_event);

    Task task;

    while(true) {
        int num = epoll_wait(epfd,evs,size,-1);
        for(int i = 0;i<num;++i){
            int curfd = evs[i].data.fd;
            if(curfd == fd) {
                info->fd = curfd;
                task.function = acceptConnection;
                task.arg = info;
                pool->TaskAdd(task);
            }else{
                info->fd = curfd;
                task.function = communication;
                task.arg = info;
                pool->TaskAdd(task);
            }
        }
    }
}

void SocketEpoll::acceptConnection(void *arg) {
    printf("accept\n");
    auto* info = (ConnInfo*)(arg);
    int fd = info->fd;
    int epfd = info->epfd;
    PthreadPool* pool = info->pool;

    epoll_event ev;

    sockaddr_in caddr;
    int len = sizeof(caddr);
    int cfd = accept(fd, (sockaddr*)&caddr, reinterpret_cast<socklen_t *>(&len));
    if(cfd == -1) {
        perror("accept");
        exit(0);
    }

    char IP[24] = {0};
    printf("client information : ");
    printf("    IP : %s , PORT : %d\n",
           inet_ntop(AF_INET,&caddr.sin_addr.s_addr,IP,sizeof(IP)),
           ntohs(caddr.sin_port));

    // 设置文件描述符为非阻塞模式，得到文件描述符的属性
    int flag = fcntl(cfd,F_GETFL);
    flag |= O_NONBLOCK;
    fcntl(cfd,F_SETFL,flag);
    // 新得到的文件描述符添加到epoll模型中，下一轮循环的时候可以被检测
    // 通信的文件描述符艰涩赌缓冲区数据的时候设置为边沿触发模式

    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = cfd;
    int ret = epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&ev);
    if(ret == -1){
        perror("epoll_ctl-accept");
        exit(0);
    }
}

void SocketEpoll::communication(void *arg) {
    printf("communication\n");

    auto* info = (ConnInfo*)(arg);
    int cfd = info->fd;
    int epfd = info->epfd;

    char buf[MAXSIZE];
    memset(buf,0,sizeof(buf));
    while(true){

        int len = recv(cfd,buf,sizeof(buf),0);
        if(len > 0){
            printf("client say : %s \n",buf);
            len = send(cfd,buf,strlen(buf),0);
            if(len < 0){
                perror("send");
                exit(0);
            }
        }else if(len == 0){
            printf("client close the connection!\n");
            // 将此文件描述符从epoll中删除
            epoll_ctl(epfd,EPOLL_CTL_DEL,cfd, nullptr);
            close(cfd);
            break;
        }else if(len < 0){
            if(errno == EAGAIN){
                printf("data read complete ...\n");
                break;
            }else {
                perror("recv");
                exit(0);
            }

        }
    }
}


