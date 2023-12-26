cmake_minimum_required(VERSION 3.22)
project(WebChatRoom)

include_directories("/Utils/json/include")
include_directories("/Utils/json/src")

set(CMAKE_CXX_STANDARD 17)

add_executable(server Server/main.cpp
        Server/PthreadPool.h
        Server/TaskQueue.h
        Server/TaskQueue.cpp
        Server/PthreadPool.cpp
        Server/SocketEpoll.h
        Server/SocketEpoll.cpp
        Utils/Message.h
        Utils/Message.cpp
        Utils/MessageQueue.h
        Utils/MessageQueue.cpp
)
add_executable( client
        Client/main.cpp
        Utils/Message.h
        Utils/Message.cpp
        main.cpp
        Utils/MessageQueue.h
        Utils/MessageQueue.cpp
)
#target_link_libraries()
