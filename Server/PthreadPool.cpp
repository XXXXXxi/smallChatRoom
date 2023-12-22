//
// Created by 12135 on 2023/12/18.
//

#include <cstdio>
#include <unistd.h>
#include "PthreadPool.h"

const int MANAGERNUMBER = 2;

PthreadPool::PthreadPool(int min, int max) {
    do {
        shutdown = 0;

        taskQueue = new TaskQueue;

        minNumber = min;
        maxNumber = max;
        busyNumber = 0;
        aliveNumber = min;
        exitNumber = 0;

        if(pthread_mutex_init(&poolMutex, nullptr) != 0 ||
                pthread_cond_init(&poolCondition, nullptr)){
            printf("pthread_mutex_t or pthread_cond_t init fail !\n");
            break;
        }

        workIds = std::vector<pthread_t>(min);
        for (int i = 0; i < min; ++i) {
            pthread_t id;
            pthread_create(&id, nullptr, Work, this);
            workIds[i] = id;
            printf("create pthread ：%ld \n",workIds[i]);
        }

        pthread_create(&manageId, nullptr,Manage, this);



    }while(0);
}

PthreadPool::~PthreadPool() {
    shutdown = 1;
    // 销毁管理者线程
    pthread_join(manageId, nullptr);
    // 唤醒消费者线程
    for(int i = 0 ; i < aliveNumber ; ++i){
        pthread_cond_signal(&poolCondition);
    }

    delete taskQueue;

    pthread_mutex_destroy(&poolMutex);
    pthread_cond_destroy(&poolCondition);
}

void * PthreadPool::Work(void *arg) {
    auto* pool = (PthreadPool*)(arg);
//    printf("\n\n%d\n\n",pool->taskQueue->taskNum());
    while(true){
        pthread_mutex_lock(&pool->poolMutex);
        while(pool->taskQueue->taskNum() == 0 && pool->shutdown == 0) {
            printf("thread %ld is waiting ... \n" ,pthread_self());
            // 被阻塞时自动解锁，被唤醒加锁
            pthread_cond_wait(&pool->poolCondition,&pool->poolMutex);
            // 被唤醒后，如果管理者进程管理需要进行退出
            if(pool->exitNumber > 0) {
                pool->exitNumber--;
                if(pool->aliveNumber > pool->minNumber){
                    pool->aliveNumber--;
                    pthread_mutex_unlock(&pool->poolMutex);
                    pool->ExitThread();
                }
            }
        }
        if(pool->shutdown){
            pthread_mutex_unlock(&pool->poolMutex);
            pool->ExitThread();
        }

        Task task = pool->taskQueue->TakeTask();
        pool->busyNumber++;
        pthread_mutex_unlock(&pool->poolMutex);
        task.function(task.arg);
        // 释放内存
        task.arg = nullptr;

        printf("thread %ld complete the task ... \n",pthread_self());

        pthread_mutex_lock(&pool->poolMutex);
        pool->busyNumber--;
        pthread_mutex_unlock(&pool->poolMutex);

    }
    return nullptr;
}

void * PthreadPool::Manage(void *arg) {
    PthreadPool* pool = (PthreadPool*)(arg);
    while(!pool->shutdown) {
//        printf("mange...\n");
        sleep(5);
        pthread_mutex_lock(&pool->poolMutex);
        int qSize = pool->taskQueue->taskNum();
        int alive = pool->aliveNumber;
        int busy = pool->busyNumber;
        pthread_mutex_unlock(&pool->poolMutex);
        // 当前线程无法处理更多任务时
        int NUMBER = MANAGERNUMBER;
        if(alive < qSize && alive < pool->maxNumber) {
            pthread_mutex_lock(&pool->poolMutex);
            while(pool->aliveNumber<pool->maxNumber && NUMBER--){
                pthread_t id = 0;
                pthread_create(&id, nullptr,Work,pool);
                pool->workIds.push_back(id);
                pool->aliveNumber++;
            }
            pthread_mutex_unlock(&pool->poolMutex);
        }

        // 当前线程过多时
        NUMBER = MANAGERNUMBER;
        if(busy*2 < qSize && alive > pool->minNumber) {
            pthread_mutex_lock(&pool->poolMutex);
            pool->exitNumber = NUMBER;
            for (int i = 0; i < NUMBER; ++i) {
                pthread_cond_signal(&pool->poolCondition);
            }
            pthread_mutex_unlock(&pool->poolMutex);
        }
    }
    return nullptr;
}

void PthreadPool::TaskAdd(Task task) {
    if(shutdown) {
        return;
    }
    taskQueue->AddTask(task);
    // 唤醒工作线程
    pthread_cond_signal(&poolCondition);
}

int PthreadPool::getBusyNumber()  {
    pthread_mutex_lock(&poolMutex);
    int busy = busyNumber;
    pthread_mutex_unlock(&poolMutex);
    return busy;
}

int PthreadPool::getAliveNumber()  {
    pthread_mutex_lock(&poolMutex);
    int alive = aliveNumber;
    pthread_mutex_unlock(&poolMutex);
    return alive;
}

void PthreadPool::ExitThread() {
    pthread_t id = pthread_self();
    for(auto it = workIds.begin() ; it != workIds.end(); it++){
        if(*it == id){
            printf("pthread %ld is exiting ! \n",id);
            workIds.erase(it);
            break;
        }
    }
    pthread_exit(nullptr);
}

