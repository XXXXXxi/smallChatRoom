cmake_minimum_required(VERSION 3.22)
project(WebChatRoom)

set(CMAKE_CXX_STANDARD 17)

add_executable(WebChatRoom Server/main.cpp
        Client/main.cpp
        main.cpp
        Server/PthreadPool.h
        Server/TaskQueue.h
        Server/TaskQueue.cpp
        Server/PthreadPool.cpp
        Server/SocketEpoll.h
        Server/SocketEpoll.cpp
)
