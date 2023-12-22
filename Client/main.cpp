#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <cstring>

const int PORT = 9999;
const int MAXMESSAGESIZE = 1024;

int main () {
    int fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd == 0){
        perror("socket");
        exit(0);
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    inet_pton(AF_INET,"222.195.85.139",&addr.sin_addr.s_addr);

    int ret = connect(fd, reinterpret_cast<const sockaddr *>(&addr), sizeof(addr));
    if(ret == -1) {
        perror("connect");
        exit(0);
    }

    while(1){
        char buf[MAXMESSAGESIZE];
        printf("Please Enter msg : ");
        scanf("%s",buf);
        send(fd,buf,strlen(buf),0);

        memset(buf,0,sizeof(buf));
        int len = recv(fd,buf,sizeof(buf),0);
        if(len > 0) {
            printf("client msg : %s\n",buf);
        }else if(len == 0){
            printf("server close the connection!\n");
            break;
        }else if(len < 0){
            perror("recv");
            exit(0);
        }
    }

    return 0;
}