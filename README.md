# 如何运行
**服务器端与客户端均运行在Linux环境下**

服务器端

````````
// 在Server文件夹中执行编译
g++ main.cpp PthreadPool.cpp TaskQueue.cpp SocketEpoll.cpp -o server -lpthread
// 在Server文件夹中执行server可执行文件
./server
````````

客户端

**运行时需更改connect时的IP地址，IP地址为服务器端运行的IP地址**
**inet_pton(AF_INET,"114.214.187.217",&addr.sin_addr.s_addr);**
**更改此处第二个参数地址，具体查看方式为ifconfig**

````````
// 在Server文件夹中执行编译
g++ main.cpp -o client
// 在Server文件夹中执行server可执行文件
./client
````````
