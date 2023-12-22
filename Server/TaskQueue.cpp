//
// Created by 12135 on 2023/12/18.
//

#include "TaskQueue.h"

TaskQueue::TaskQueue() {
    pthread_mutex_init(&taskQ_mutex, nullptr);
}

TaskQueue::~TaskQueue() {
    pthread_mutex_destroy(&taskQ_mutex);
}

void TaskQueue::AddTask(Task task) {
    pthread_mutex_lock(&taskQ_mutex);
    taskQ.push(task);
    pthread_mutex_unlock(&taskQ_mutex);
}

void TaskQueue::AddTask(callback func, void *arg) {
    Task task(func,arg);
    pthread_mutex_lock(&taskQ_mutex);
    taskQ.push(task);
    pthread_mutex_unlock(&taskQ_mutex);
}

Task TaskQueue::TakeTask() {
    Task task;
    pthread_mutex_lock(&taskQ_mutex);
    task = taskQ.front();
    taskQ.pop();
    pthread_mutex_unlock(&taskQ_mutex);
    return task;
}

int TaskQueue::taskNum() {
    return taskQ.size();
}