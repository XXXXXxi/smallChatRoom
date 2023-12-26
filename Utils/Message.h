//
// Created by 12135 on 2023/12/25.
//

#ifndef WEBCHATROOM_MESSAGE_H
#define WEBCHATROOM_MESSAGE_H

const int setNickname = 1;
const int sendMsg = 2;
const int setNicknameSuccess = 3;
const int setNicknameFail = 4;

#include <string>
class Message {
private:
    int type;                   // 1.设置昵称  2.发送消息  3.设置昵称成功  4.设置昵称失败
    std::string from;
    std::string to;
    std::string sendTime;
    std::string msg;
public:
    std::string getTime();
    Message() = default;
    Message(int type,std::string from,std::string to,std::string msg);
    Message(int type,std::string from,std::string to,std::string sendTime,std::string msg);
    std::string serialization();
    Message deserialization(std::string str);
    std::string printMessage();
    int getType();
    std::string getFromName();
    std::string getToName();
    std::string getMsg();
    void setType(int type);

};


#endif //WEBCHATROOM_MESSAGE_H
