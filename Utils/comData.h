//
// Created by 12135 on 2023/12/24.
//

#ifndef WEBCHATROOM_COMDATA_H
#define WEBCHATROOM_COMDATA_H

const int NICKNAMESIZE = 12;
const int MESSAGESIZE = 1024;

struct comData {
    char nickname[NICKNAMESIZE];    // 用户名称
    char msg[MESSAGESIZE];          // 当前数据
    int sum;                        // 共分几次发送
    int msgSize;                    // 此次放松字符串大小
    int now;                        // 当前发送为的第几次
};


#endif //WEBCHATROOM_COMDATA_H
