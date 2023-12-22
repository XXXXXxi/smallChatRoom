//
// Created by 12135 on 2023/12/18.
//

#ifndef WEBCHATROOM_TASKQUEUE_H
#define WEBCHATROOM_TASKQUEUE_H

#include <queue>
#include <pthread.h>

using callback = void(*)(void*);
struct Task {
    Task() {
        function = nullptr;
        arg = nullptr;
    }
    Task(callback func,void* arg){
        function = func;
        arg = arg;
    }
    callback function;
    void* arg;
};

class TaskQueue {
private:
    std::queue<Task> taskQ;
    pthread_mutex_t taskQ_mutex;
public:
    TaskQueue();
    ~TaskQueue();
    // 添加任务
    void AddTask(Task task);
    void AddTask(callback func,void* arg);
    // 取出任务
    Task TakeTask();
    // 查看任务数量
    int taskNum();
};


#endif //WEBCHATROOM_TASKQUEUE_H
