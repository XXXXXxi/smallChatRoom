//
// Created by 12135 on 2023/12/26.
//

#ifndef WEBCHATROOM_MESSAGEQUEUE_H
#define WEBCHATROOM_MESSAGEQUEUE_H


#include <queue>
#include "Message.h"

class MessageQueue {
private:
    std::queue<Message>  msgQ;
    pthread_mutex_t mutex;
    pthread_cond_t queueCond;
public:
    MessageQueue();
    ~MessageQueue();
    void AddMsg(Message msg);
    Message getMsg();
    int msgQsize();
};


#endif //WEBCHATROOM_MESSAGEQUEUE_H
