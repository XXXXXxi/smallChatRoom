//
// Created by 12135 on 2023/12/26.
//

#include "MessageQueue.h"

MessageQueue::MessageQueue() {
    pthread_mutex_init(&mutex,0);
    pthread_cond_init(&queueCond,0);
}

MessageQueue::~MessageQueue() {
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&queueCond);
}

Message MessageQueue::getMsg() {
//    printf("getMsg\n");
    pthread_mutex_lock(&mutex);
    Message msg;
    while(msgQ.size() == 0) {
        pthread_cond_wait(&queueCond,&mutex);
    }
    msg = msgQ.front();
    msgQ.pop();
    pthread_mutex_unlock(&mutex);
    return msg;
}

void MessageQueue::AddMsg(Message msg) {
//    printf("AddMsg\n");
    pthread_mutex_lock(&mutex);
    msgQ.push(msg);
    pthread_cond_signal(&queueCond);
    pthread_mutex_unlock(&mutex);
}

int MessageQueue::msgQsize() {
    int size = 0;
    pthread_mutex_lock(&mutex);
    size = msgQ.size();
    pthread_mutex_unlock(&mutex);
    return size;
}


