//
// Created by 12135 on 2023/12/18.
//

#ifndef WEBCHATROOM_PTHREADPOOL_H
#define WEBCHATROOM_PTHREADPOOL_H

#include <pthread.h>
#include <queue>
#include "TaskQueue.h"


class PthreadPool {
private:
    TaskQueue* taskQueue;           // 工作队列

    int maxNumber;                  // 最大线程数量
    int minNumber;                  // 最小线程数量
    int busyNumber;                 // 当前工作线程数量
    int aliveNumber;                // 当前存活线程
    int exitNumber;                 // 管理者线程管理需要退出的线程

    pthread_mutex_t poolMutex;      // 互斥变量
    pthread_cond_t poolCondition;   // 条件变量

    std::vector<pthread_t> workIds; // 工作线程ID
    pthread_t manageId;             // 管理者线程ID

    int shutdown ;                  // 1为关闭，0为开启

    static void * Work(void* arg);
    static void * Manage(void* arg);

public:
    PthreadPool() = default;
    PthreadPool(int min,int max);
    ~PthreadPool();
    void TaskAdd(Task task);
    int getBusyNumber() ;
    int getAliveNumber() ;
    void ExitThread();

};


#endif //WEBCHATROOM_PTHREADPOOL_H
