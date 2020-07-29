#include "UDPLibrary.h"

UDP::UDP(){

}

bool l = true;
void server_listen(){
    while (l)
    {
        
    }
    
}

int UDP::setMode(int mode, void* function (std::string, std::string) ){
    int sockfd;
    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    struct sockaddr_in serv,client;

    serv.sin_family = AF_INET;
    serv.sin_port = htons(8080);
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");
    if(mode == 0){
        return 0;
    }
    else if(mode == 1){
        bind(sockfd, (const struct sockaddr *)&serv, sizeof(serv));
        std::thread t(server_listen);
        t.detach();
        return 1;
    }
    else{
        return -1;
    }
}

int UDP::write(std::string CID, char* buffer, int size){

}

void UDP::read(std::string CID, std::string buffer){

}

int UDP::close(std::string CID){
    l = false;
}