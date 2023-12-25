#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <cstring>
#include <csignal>

const int PORT = 9999;
const int MAXMESSAGESIZE = 1024;
const char *NICKNAMEFLAG = "/nickname";

void cleanCurLine();

int main () {
    int fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd == 0){
        perror("socket");
        exit(0);
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    inet_pton(AF_INET,"114.214.187.217",&addr.sin_addr.s_addr);

    int ret = connect(fd, reinterpret_cast<const sockaddr *>(&addr), sizeof(addr));
    if(ret == -1) {
        perror("connect");
        exit(0);
    }
    int msgfd = fileno(stdin);

    // 使用 select 进行调试
    int maxfd = fd;
    fd_set rdset;
    fd_set rdtemp;
    FD_ZERO(&rdset);
    FD_SET(fd,&rdset);
    FD_SET(msgfd,&rdset);

    std::string nickname;
    bool isNickname = false;
    static  char m[1024] = "Welcome to the chat room , please enter your nickname ,then you can enter the message! \nyour nickname :";
    write(fileno(stdout),m,sizeof(m));
//    printf("Welcome to the chat room , please enter your nickname ,then you can enter the message! \nyour nickname : ");
    char buf[MAXMESSAGESIZE];
    while(true){
        rdtemp = rdset;
        int num = select(maxfd+1,&rdtemp, nullptr, nullptr, nullptr);
        if(num == -1){
            perror("select");
            exit(0);
        }else if(num) {
            if(FD_ISSET(fd,&rdtemp)){
                memset(buf,0,sizeof(buf));
                int len = read(fd,buf,sizeof(buf));
                if(len == 0) {
                    printf("server close the connection!\n");
                    exit(0);
                }else if(len > 0){
//                    printf("%s",buf);
                    if(strstr(buf,"setNicknameFail") != nullptr){
                        char logInfo[MAXMESSAGESIZE] = "Same nickname,Please enter again! your nickname:";
                        cleanCurLine();
                        write(fileno(stdout),logInfo, strlen(logInfo));
                        isNickname = false;
                    }else if(strstr(buf,"setNicknameSuccess") == nullptr){
                        write(fileno(stdout),buf,sizeof(buf));
                    }
                }else {
                    perror("read");
                    exit(0);
                }
            }else if(FD_ISSET(msgfd,&rdtemp)){
                memset(buf,0,sizeof(buf));
                int len = read(msgfd,buf,sizeof(buf));
                char *msg = (char*)malloc(sizeof(char)*MAXMESSAGESIZE);
                int index = 0;
                for(int i = 0;i<len;i++){
                    if(buf[i]=='\n'){
                        continue;
                    }else if(buf[i]=='\r'){
                        break;
                    }else {
                        msg[index++] = buf[i];
                    }
                }
                msg[index] = '\0';
                if(!isNickname) {
                    char tmp[MAXMESSAGESIZE];
                    nickname = std::string(msg);
                    sprintf(tmp,"%s%s",NICKNAMEFLAG,msg);
                    sprintf(msg,"%s",tmp);
                    isNickname = !isNickname;
                    send(fd,msg,strlen(msg),0);
                    continue;
                }
                cleanCurLine();
                write(fileno(stdout),"you> ",5);
                write(fileno(stdout),msg,strlen(msg));
                write(fileno(stdout),"\n",1);
                send(fd,msg,strlen(msg),0);
                delete msg;
            }
        }
    }
}

void cleanCurLine()
{
    // 将光标移动到上一行
    write(fileno(stdout),"\033[F",3);
    // 将光标移动的到此行首位置
    write(fileno(stdout),"\e[2K",4);
}