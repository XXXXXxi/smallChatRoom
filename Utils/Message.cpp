//
// Created by 12135 on 2023/12/25.
//

#include <iostream>
#include "Message.h"
#include <jsoncpp/json/json.h>
#include <ctime>
Message::Message(int type, std::string from, std::string to, std::string sendTime, std::string msg) {
    this->type =type;
    this->from = from;
    this->to = to;
    this->sendTime  = sendTime;
    this->msg = msg;
}

Message::Message(int type, std::string from, std::string to, std::string msg) {
    this->type =type;
    this->from = from;
    this->to = to;
    this->sendTime  = getTime();
    this->msg = msg;
}

std::string Message::serialization() {
    // 解决中文乱码问题
    Json::StreamWriterBuilder builder;
    builder["commentStyle"] = "None";
    builder["indentation"] = "";
    builder["emitUTF8"] = true;
    Json::Value root;
    root["type"] = type;
    root["from"] = from;
    root["to"] = to;
    root["time"] = sendTime;
    root["msg"] =  msg;
//    Json::FastWriter writer;
    std::string str = Json::writeString(builder, root);
    return str;
}

Message Message::deserialization(std::string str) {
//    std::cout <<  str << std::endl;
    Json::Value root;
    Json::Reader reader;
    int Type;
    std::string From;
    std::string To;
    std::string sendTime;
    std::string Msg;
    if(reader.parse(str,root)){
        Type = root["type"].asInt();
        From = root["from"].asString();
        To = root["to"].asString();
        sendTime = root["time"].asString();
        Msg = root["msg"].asString();
    }
    return Message(Type,From,To,sendTime,Msg);
}

std::string Message::printMessage() {
    std::string info =  "[" + sendTime + "]  " + from + "> " + msg ;
    return info;
}

std::string Message::getTime() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    std::string nowTime = std::to_string(1900+ltm->tm_year) + "年" +
            std::to_string(1+ltm->tm_mon) + "月" +
            std::to_string(ltm->tm_mday) + "日 " +
            std::to_string(ltm->tm_hour) + ":" +
            std::to_string(ltm->tm_min) + ":" +
            std::to_string(ltm->tm_sec);
//    std::cout << nowTime << std::endl;
    return nowTime;
}

int Message::getType() {
    return type;
}

std::string Message::getFromName() {
    return from;
}

void Message::setType(int type) {
    this->type = type;
}

std::string Message::getToName() {
    return to;
}

std::string Message::getMsg() {
    return msg;
}



